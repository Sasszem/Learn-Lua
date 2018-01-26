// data.c

#include "includes.h"

#include "globals.h"

#include "widgets.h"

#include "lua.h"

#include <glib.h>
#include <glib/gstdio.h>

//#include <zip.h>

#include "data.h"
/* void free_task(struct Task task) {
     g_free(task.name);
     g_free(task.inst);
     g_free(task.cases);
 }*/

// char currFile[64];

/*
void load_file(char *name) {
    g_file_get_contents(name, &file, NULL, NULL);

    // load_task();
    num_tasks = 0;
    _load_tasks_from_file();
}

*/

int cmpstr(char *str, char *pattern, int len) {
    if (len == -1)
        len = strlen(pattern);
    for (int i = 0; i < len; i++) {
        if (str[i] != pattern[i])
            return 0;
    }
    return 1;
}

/*
void _load_task_file(TaskPath path)
{
if (cmpstr(path.file,currFile,-1)==0)
{
load_file(path.file);
}
set_instructions(tasks[path.id].inst);
set_tester_program(tasks[path.id].tester);
}

*/

void null_str(char *str, int len) {
    for (; len != 0; len--) {
        str[len - 1] = 0;
    }
}

char *strtok_single(char *str, char const *delims) {
    static char *src = NULL;
    char *p, *ret = 0;

    if (str != NULL)
        src = str;

    if (src == NULL)
        return NULL;

    if ((p = strpbrk(src, delims)) != NULL) {
        *p = 0;
        ret = src;
        src = ++p;

    } else if (*src) {
        ret = src;
        src = NULL;
    }

    return ret;
}
/*
void _load_tasks_from_file() {

    struct Task task;
    char state = 'N';
    char *ptr;
    // States: Nothing, Task, Instructions,  tester(c)
    g_print("Begin loading task...\n");
    for (char *p = strtok_single(file, "\n"); p != NULL;
         p = strtok_single(NULL, "\n")) {
        // g_print("[%c]Line: %s\n", state, p);
        switch (state) {

        case 'N':
            // g_print("State %c\n", state);
            if (cmpstr(p, "#task", -1)) {

                null_str(task.name, 64);
                null_str(task.inst, 8192);
                null_str(task.tester, 4096);
                stpcpy(task.name, p + 6);
                state = 'T';
            }
            break;
        case 'T':
            if (cmpstr(p, "#inst", -1)) {
                state = 'I';
                ptr = task.inst;
            }
            if (cmpstr(p, "#tester", -1)) {
                state = 'C';
                ptr = task.tester;
            }
            if (cmpstr(p, "#!task", -1)) {


                tasks[num_tasks] = task;
                num_tasks++;

                state = 'N';
            }
            break;
        case 'I':
            if (cmpstr(p, "#!inst", -1)) {
                state = 'T';
            } else {
                // g_print("Offset: %d\n",ptr-task.inst);

                ptr = g_utf8_offset_to_pointer(
                    g_utf8_strncpy(ptr, p, g_utf8_strlen(p, -1)),
                    g_utf8_strlen(p, -1));
                ptr[0] = '\n';
                ptr = g_utf8_next_char(ptr);

                // g_print("Text(I): \n%s\n",task.inst);
            }
            break;

        case 'C':
            if (cmpstr(p, "#!tester", -1)) {
                state = 'T';
            } else {
                ptr = g_utf8_offset_to_pointer(
                    g_utf8_strncpy(ptr, p, g_utf8_strlen(p, -1)),
                    g_utf8_strlen(p, -1));
                ptr[0] = '\n';
                ptr = g_utf8_next_char(ptr);
            }
            break;
        }
    }
}
*/

// loads a task from path relative to TASKPATH
void _open_task(TaskPath task) {
    // first check if the directory exists
    char *dir = g_build_filename(TASKPATH, task.section, task.name, NULL);
    if (!(g_file_test(dir, G_FILE_TEST_EXISTS) &&
          g_file_test(dir, G_FILE_TEST_IS_DIR))) {
        g_print("Error, dir \"%s\" dosn't exists!\n", dir);
        return; // Error.NO_SUCH_FILE;
    }
    // than check if the files exists
    char *tester_path, *instr_path;
    tester_path = g_build_filename(dir, TESTER_FILE, NULL);
    instr_path = g_build_filename(dir, INSTR_FILE, NULL);
    if (!(g_file_test(tester_path, G_FILE_TEST_EXISTS) &&
          g_file_test(tester_path, G_FILE_TEST_IS_REGULAR))) {
        g_print("Error, file \"%s\" dosn't exists!\n", tester_path);
        return; // Error.NO_SUCH_FILE;
    }
    if (!(g_file_test(instr_path, G_FILE_TEST_EXISTS) &&
          g_file_test(instr_path, G_FILE_TEST_IS_REGULAR))) {
        g_print("Error, file \"%s\" dosn't exists!\n", instr_path);
        return; // Error.NO_SUCH_FILE;
    }
    g_print("[Loading]All required files exists...\n");

    char *buffer;

    g_file_get_contents(instr_path, &buffer, NULL, NULL);
    Widgets.set_instructions(buffer);
    g_free(buffer);

    g_file_get_contents(tester_path, &buffer, NULL, NULL);
    Lua.set_tester(buffer);

    g_print("[Loading]Complete, now freeing memory...\n");
    g_free(buffer);

    // last, free the allocated buffers
    g_free(dir);
    g_free(tester_path);
    g_free(instr_path);
}

void _fill_list() {
    char *sections_list;
    char *sections_list_path = g_build_filename(TASKPATH, LIST_FILE_NAME, NULL);
    gboolean success =
        g_file_get_contents(sections_list_path, &sections_list, NULL, NULL);
    g_free(sections_list_path);
    if (!success) {
        return;
    }
    GtkTreeStore *store = GTK_TREE_STORE(Widgets.get_object("tasks_tree"));
    GtkTreeIter section, task;
    g_print("Begin listing...\n");
    for (char *p = strtok(sections_list, "\n"); p != NULL;
         p = strtok(NULL, "\n")) {
        // g_print("Starting section %s\n", p);
        char *tasks_list;
        char *tasks_list_path =
            g_build_filename(TASKPATH, p, LIST_FILE_NAME, NULL);
        // g_print("Path: %s\n",tasks_list_path);
        gboolean success =
            g_file_get_contents(tasks_list_path, &tasks_list, NULL, NULL);
        g_free(tasks_list_path);
        if (!success) {
            g_print("Skipping section %s\n", p);
            continue;
        }
        gtk_tree_store_append(store, &section, NULL);
        gtk_tree_store_set(store, &section, 0, p, -1);
        char *state;
        for (char *l = strtok_r(tasks_list, "\n", &state); l != NULL;
             l = strtok_r(NULL, "\n", &state)) {
            gtk_tree_store_append(store, &task, &section);
            gtk_tree_store_set(store, &task, 0, l, -1);
        }
        g_free(tasks_list);
    }

    gtk_tree_view_expand_all(GTK_TREE_VIEW(Widgets.get_object("tasks_view")));
    g_free(sections_list);

    g_print("Listing done...\n");
}

int save_code(char *name, char *code) {
    if (g_utf8_strlen(code, -1) > 2048) {
        return -1;
    }
    char *path = g_build_filename(SAVE_PATH, name, NULL);
    g_print("File path: %s\n", path);
    g_file_set_contents(path, code, -1, NULL);
    g_free(path);
    return 0;
}
char *load_code(char *name) {
    char *buff;
    char *path = g_build_filename(SAVE_PATH, name, NULL);
    g_file_get_contents(path, &buff, NULL, NULL);
    g_free(path);
    return buff;
}
