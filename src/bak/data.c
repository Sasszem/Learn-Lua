// data.c

#include "includes.h"

#include "globals.h"

#include "widgets.h"

#include "lua.h"

#include <glib.h>
#include <glib/gstdio.h>

#include "data.h"

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

    GtkTreeIter section, task;

    g_print("[Data]Begin listing...\n");

    for (char *p = strtok(sections_list, "\n"); p != NULL;
         p = strtok(NULL, "\n")) { // iterate over file contents line by line

        char *tasks_list;
        // list file contetns inside current task root
        char *tasks_list_path =
            g_build_filename(TASKPATH, p, LIST_FILE_NAME, NULL); // path

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

int save_code(char *name, char *code) {
    if (g_utf8_strlen(code, -1) > 2048) {
        return -1;
    }
    char *path = g_build_filename(SAVE_PATH, name, NULL);
    g_print("[Data]File path: %s\n", path);
    g_file_set_contents(path, code, -1, NULL);
    g_free(path);
    return 0;
}
// stub, do not use it yet...
char *load_code(char *name) {
    char *buff;
    char *path = g_build_filename(SAVE_PATH, name, NULL);
    g_file_get_contents(path, &buff, NULL, NULL);
    g_free(path);
    return buff;
}
