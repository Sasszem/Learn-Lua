// lua.c

#include "includes.h"

#include "data.h"

#include "lua.h"

#include "tagger.h"

#include <glib.h>
static int custom_print(lua_State *L);
static int run_user_code(lua_State *L);
void run_code(char *code);
void run_task_code(int taskno);

static void show_error(char *str) {
    g_print("[LUA-ERROR]%s\n", str);

    char *token;
    token = strsep(&str, ":");
    token = strsep(&str, ":");
    int line = g_ascii_strtoll(token, NULL, 0) - 2;
    g_print("Error in line %d\n", line + 2);
    tag_error_line(line);

    GtkWidget *dia = gtk_message_dialog_new_with_markup(
        GTK_WINDOW(gtk_builder_get_object(builder, "window")), GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "ERROR:\n%s", str, NULL);
    gtk_dialog_run(GTK_DIALOG(dia));
    gtk_widget_destroy(dia);
}

static void show_print(int line, char *str) {

    GtkWidget *dia = gtk_message_dialog_new_with_markup(
        GTK_WINDOW(gtk_builder_get_object(builder, "window")), GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", str, NULL);
    gtk_dialog_run(GTK_DIALOG(dia));
    gtk_widget_destroy(dia);
}

void run_task(int taskno) {
    init_lua();
    lua_register(L, "print", custom_print);
    g_print("Custom print() registated\n");
    lua_register(L, "run", run_user_code);
    run_task_code(taskno);

    close_lua();
}

void init_lua() {
    g_print("Initializing LUA\n");
    L = luaL_newstate();

    luaopen_base(L); /* opens the basic library */
    luaopen_table(L);

    luaopen_string(L); /* opens the string lib. */
    luaopen_math(L);
}

static int custom_print(lua_State *L) {
    g_print("Entering custom print...\n");
    int argc = lua_gettop(L);

    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "nSl", &ar);

    char str[256];

    g_print("[line %d]Print called with %d args\n", ar.currentline, argc);
    if (lua_tostring(L, -1)) {
        strcpy(str, lua_tostring(L, -1));
    } else {
        strcpy(str, "false");
    }
    lua_pop(L, 1);

    for (int i = 1; i < argc; i++) {
        strcat(str, " ");
        if (lua_tostring(L, -1)) {
            strcat(str, lua_tostring(L, -1));
        } else {
            strcat(str, "false");
        }
    }

    g_print("[line %d]Print: \n%s\n", ar.currentline, str);
    show_print(ar.currentline, str);

    return 0;
}

void close_lua() {
    g_print("Closing LUA session\n");
    lua_close(L);
}
int run_user_code(lua_State *L) {
    GtkTextIter start, end;
    char *text;
    gtk_text_buffer_get_bounds(
        GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "program_buffer")),
        &start, &end);
    text = gtk_text_buffer_get_text(
        GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "program_buffer")),
        &start, &end, 0);
    int error;
    g_print("Running user code...\n");

    error = luaL_dostring(L, text);
    if (error) {
        show_error((char *)lua_tostring(L, -1));

        lua_pop(L, 1); /* pop error message from the stack */
    }
    g_free(text);
    return lua_gettop(L);
}

void run_task_code(int taskno) { run_code(tasks[taskno].cases); }

void run_code(char *code) {
    int error;
    g_print("Running user code...\n");

    error = luaL_dostring(L, code);
    if (error) {
        show_error(lua_tostring(L, -1));

        lua_pop(L, 1); /* pop error message from the stack */
    }
}
