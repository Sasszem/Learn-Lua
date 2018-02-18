// data.c

#include "includes.h"

#include "widgets.h"

#include "lua.h"

#include <glib.h>
#include <glib/gstdio.h>

#include "data.h"

#include "globals.h"

TaskPath current = {.section = NULL, .name = NULL};

// loads a task from path relative to TASKPATH
void _open_task(TaskPath task) {
    // first build the path of the dir
    char *dir = g_build_filename(TASKPATH, task.section, task.name, NULL);

    // Then we test if it exists
    if (!(g_file_test(dir, G_FILE_TEST_EXISTS) &&
          g_file_test(dir, G_FILE_TEST_IS_DIR))) {
        g_print("[Data][Loading]Error, dir \"%s\" dosn't exists!\n", dir);
        return; // If not then tell it and quit
    }

    // than check if the files inside exists
    char *tester_path, *instr_path;
    tester_path = g_build_filename(dir, TESTER_FILE, NULL);
    instr_path = g_build_filename(dir, INSTR_FILE, NULL);
    if (!(g_file_test(tester_path, G_FILE_TEST_EXISTS) &&
          g_file_test(tester_path, G_FILE_TEST_IS_REGULAR))) {
        g_print("[Data][Loading]Error, file \"%s\" dosn't exists!\n",
                tester_path);
        return; // If not then tell it and quit
    }
    if (!(g_file_test(instr_path, G_FILE_TEST_EXISTS) &&
          g_file_test(instr_path, G_FILE_TEST_IS_REGULAR))) {
        g_print("[Data][Loading]Error, file \"%s\" dosn't exists!\n",
                instr_path);
        return; // If not then tell it and quit
    }

    // Little debug print...
    g_print("[Data][Loading]All required files exists...\n");

    // buffer will be used to hold the contents of a file temporarly
    char *buffer;

    // Load the first file, use it and the free the buffer
    g_file_get_contents(instr_path, &buffer, NULL, NULL);
    Widgets.set_instructions(buffer);
    g_free(buffer);

    // Same with the 2nd
    g_file_get_contents(tester_path, &buffer, NULL, NULL);
    Lua.set_tester(buffer);

    // We're done, tell it
    g_print("[Data][Loading]Complete, now freeing memory...\n");

    // last, free the allocated buffers
    g_free(dir);

    g_free(tester_path);
    g_free(instr_path);

    g_free(buffer);

    g_free(current.section);
    g_free(current.name);

    current.section = g_strdup(task.section);
    current.name = g_strdup(task.name);

    _load_code();
}

// Lists all the sections and tasks, filling the treeview rows
void _fill_list() {

    char *sections_list; // holds the contents of the 'list' file in the
                         // sections root dir

    char *sections_list_path = g_build_filename(TASKPATH, LIST_FILE_NAME,
                                                NULL); // the path of that file
    gboolean success =
        g_file_get_contents(sections_list_path, &sections_list, NULL,
                            NULL); // load the file and save success
    g_free(sections_list_path);    // We free the memory anyway
    if (!success) {
        return; // But if we couldn't load then quit
    }

    GtkTreeStore *store =
        GTK_TREE_STORE(Widgets.get_object("tasks_tree")); // get the tree store

    gtk_tree_store_clear(GTK_TREE_STORE(Widgets.get_object("tasks_tree")));

    GtkTreeIter section, task;

    g_print("[Data]Begin listing...\n");

    for (char *p = strtok(sections_list, "\n"); p != NULL;
         p = strtok(NULL, "\n")) { // iterate over file contents line by line

        char *tasks_list;
        char *completed;
        char *save_file;

        // list file contetns inside current task root
        char *tasks_list_path =
            g_build_filename(TASKPATH, p, LIST_FILE_NAME, NULL); // path

        char *save_path = g_build_filename(SAVEPATH, p, COMP_FILE_NAME, NULL);
        g_print("\"completed\" file path: %s\n", save_path);
        gboolean save = g_file_get_contents(save_path, &completed, NULL, NULL);

        g_free(save_path);
        if (!save) {
            g_print("[Data]No \"completed\" entry for %s\n", p);
        }

        // same
        gboolean success =
            g_file_get_contents(tasks_list_path, &tasks_list, NULL, NULL);
        g_free(tasks_list_path);
        if (!success) {
            g_print("[Data]Skipping section %s\n", p);
            continue; // Section missing list file, skipping it
        }

        // create a new parent
        gtk_tree_store_append(store, &section, NULL);
        gtk_tree_store_set(store, &section, 0, p, -1); // set its name

        // state required for 'strtok_r', which we need cuz normal 'strtok' is
        // not nestable and already used for the outer loop
        char *state;

        for (char *l = strtok_r(tasks_list, "\n", &state); l != NULL;
             l = strtok_r(
                 NULL, "\n",
                 &state)) { // iterate over 2nd 'list' file line by line...

            gtk_tree_store_append(store, &task, &section); // new child node
            gtk_tree_store_set(store, &task, 0, l, -1);    // set its name
            if (save) {
                char *a = g_strstr_len(completed, -1, l);
                if (a != NULL) {
                    g_print("Task %s is completed...\n", l);
                    gtk_tree_store_set(store, &task, 1, TRUE, -1);
                }
            }
        }
        // don't forget to save buffers
        g_free(tasks_list);
    }

    // Expand the tree as it will look nicer
    gtk_tree_view_expand_all(GTK_TREE_VIEW(Widgets.get_object("tasks_view")));

    // Free buffers
    g_free(sections_list);

    g_print("[Data]Listing done...\n");
}

void _save_code() {

    GtkTextIter start, end;
    char *text;
    gtk_text_buffer_get_bounds(
        GTK_TEXT_BUFFER(Widgets.get_object("program_buffer")), &start, &end);
    text = gtk_text_buffer_get_text(
        GTK_TEXT_BUFFER(Widgets.get_object("program_buffer")), &start, &end, 0);

    char *dirpath = g_build_filename(SAVE_PATH, current.section, NULL);
    char *path = g_build_filename(dirpath, current.name, NULL);
    g_print("[Data]File path: %s\n", path);
    g_mkdir_with_parents(
        dirpath,
        510); // RWX permission for user and group, RW for all - octal 0776
    GError *err = NULL;
    if (!g_file_set_contents(path, text, -1, &err)) {
        g_print("[Data]An error occured: \n   %s\n", err->message);
    }
    g_free(text);
    g_free(path);
    g_free(dirpath);
}
// stub, do not use it yet...
void _load_code() {
    char *buff;
    char *path =
        g_build_filename(SAVE_PATH, current.section, current.name, NULL);
    gboolean succ = g_file_get_contents(path, &buff, NULL, NULL);
    g_free(path);
    if (succ) {
        Widgets.set_code(buff);
    }
}

void _complete_task() {

    char *path =
        g_build_filename(SAVEPATH, current.section, COMP_FILE_NAME, NULL);

    g_print("completed file path: %s\n", path);
    char *contents;

    gboolean succ = g_file_get_contents(path, &contents, NULL, NULL);

    if (!succ) {
        g_print(".completed file does not exisit...\n");
        contents = "";
    }

    char *a = g_strstr_len(contents, -1, current.name);
    if (a != NULL) {
        return;
    }

    GError *err = NULL;

    g_print(".completed contents: \n%s", contents);
    g_print("current.name=%s\n", current.name);
    char *updated = g_strjoin("\n", contents, current.name, NULL);
    g_print("Updated string: %s\n", updated);
    if (!g_file_set_contents(path, updated, -1, &err)) {
        g_print("File write error: %s\n", err->message);
    }
    g_print("file written, freeing...\n");
    g_free(updated);
    g_free(path);
    if (succ) {
        g_free(contents);
    }
    g_print("freeing complete!\n");
}
