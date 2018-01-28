// lua.c

#include "includes.h"

#include "data.h"

#include "lua.h"

#include "tagger.h"

#include "widgets.h"

//#include <glib.h>

#include "globals.h"

lua_State *L; // The lua sandbox used to run the user's code

static char *tester = NULL; // the current code

void _set_tester(char *code) {
    g_free(tester); // free old tester
    //'g_free' dosn't cause segfault when pointer is 'NULL'
    tester = g_strdup(code);
}

static int custom_print(lua_State *L);
static int run_user_code(lua_State *L);
static void run_code(char *code);
static void run_task_code();

static void
show_error(char *str) { // shows a nice popup pointing to the error...
    g_print("[LUA-ERROR]%s\n", str);

    char *token;
    token = strsep(&str, ":"); // separate at ':'s
    token = strsep(&str, ":"); // and discard the first token
    int line = g_ascii_strtoll(token, NULL, 0) - 2; // Convert the second to int
    g_print("Error in line %d\n", line + 2);
    Tagger.tag_error(line); // mark line with red
    GtkWidget *dia = gtk_message_dialog_new_with_markup(
        GTK_WINDOW(Widgets.get_object("window")), GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "ERROR:\n%s", str,
        NULL);                       // Make popup
    gtk_dialog_run(GTK_DIALOG(dia)); // show it
    gtk_widget_destroy(dia);         // then free it
}

static void show_print(int line,
                       char *str) { // displays a popup for lua 'print' function

    GtkWidget *dia = gtk_message_dialog_new_with_markup(
        GTK_WINDOW(Widgets.get_object("window")), GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", str, NULL);
    gtk_dialog_run(GTK_DIALOG(dia));
    gtk_widget_destroy(dia);
}

static void init_lua() { // Sets up a new sandbox
    // Lua libraries: base, table stirng and math. This way it should be
    // (relativly) safe
    g_print("[Lua]Initializing LUA\n");
    L = luaL_newstate();

    luaopen_base(L); /* opens the basic library */
    luaopen_table(L);

    luaopen_string(L); /* opens the string lib. */
    luaopen_math(L);
}

static void close_lua() {
    g_print("[Lua]Closing LUA session\n");
    lua_close(L);
}

void _run_task() {                          // run current tester code
    init_lua();                             // initialize LUA sandbox
    lua_register(L, "print", custom_print); // register custom print function
    g_print("[Lua]Custom print() registated\n");
    lua_register(L, "run",
                 run_user_code); // register function to run the users code
    run_task_code();             // run tester

    close_lua(); // cleanup
}

static int custom_print(lua_State *L) { // a wrapper around 'show_print' for
                                        // luas
    g_print("[Lua]Entering custom print...\n");

    int argc = lua_gettop(L); // argument count

    if (argc < 1) {
        return 0;
    }

    lua_Debug ar; // lua_Debug used to get current line
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "nSl", &ar);

    char str[256]; // error message buffer

    g_print("[Lua][line %d]Print called with %d args\n", ar.currentline, argc);

    // Copy the 1st param
    if (lua_tostring(L, -1)) {
        strcpy(str, lua_tostring(L, -1));
    } else {
        strcpy(str, "false");
    }
    lua_pop(L, 1);

    // Then cahin the others via spaces
    for (int i = 1; i < argc; i++) {
        strcat(str, " ");
        if (lua_tostring(L, -1)) {
            strcat(str, lua_tostring(L, -1));
        } else {
            strcat(str, "false");
        }
    }

    g_print("[Lua][line %d]Print: \n%s\n", ar.currentline, str);
    show_print(ar.currentline, str); // show it

    return 0;
}

static int run_user_code(lua_State *L) {
    // runs code from the editor widget

    // get the code
    GtkTextIter start, end;
    char *text;
    gtk_text_buffer_get_bounds(
        GTK_TEXT_BUFFER(Widgets.get_object("program_buffer")), &start, &end);
    text = gtk_text_buffer_get_text(
        GTK_TEXT_BUFFER(Widgets.get_object("program_buffer")), &start, &end, 0);

    // then run it
    run_code(text);

    // don't forget to free the buffer!
    g_free(text);

    return lua_gettop(L);
}

// run the tester
static void run_task_code() { run_code(tester); }

// run any code
static void run_code(char *code) {
    int error; // stores error code
    g_print("[Lua]Running code...\n");

    // run the given code
    error = luaL_dostring(L, code);
    if (error) {
        show_error((char *)lua_tostring(L, -1));
        // show errors
        lua_pop(L, 1); /* pop error message from the stack */
    }
}
