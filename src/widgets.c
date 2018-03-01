// widgets.c

#include "includes.h"

#include "data.h"
#include "globals.h"

#include "lua.h"
#include "tagger.h"

#include <glib-object.h>

// the builder
static GtkBuilder *builder;

static GtkCssProvider *provider;

static GtkTreeIter current_task_iter;

static GClosure *swtab[3];
static int pages[3] = {0, 1, 2};

static GtkAccelGroup *accgroup;
void _w_open_task();

int next_task();


//-----------------------
//  WRAPPERS START HERE
//-----------------------

void _show() {
    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "window")));
}

// wrapper aroung g_signal_connect
void _connect_signal(char *name, char *signal, void *funcs) {
    g_signal_connect(gtk_builder_get_object(builder, name), signal,
                     G_CALLBACK(funcs), NULL);
}

// weraper aroung gtk_builder_get_object
void *_get_object(char *name) { return gtk_builder_get_object(builder, name); }

void _set_code(char *code) {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(_get_object("program_buffer")),
                             code, -1);
}

//-------------------------
//  CALLBACKS START HERE
//-------------------------

// Callback for pressing the run button
static void btn_run(GtkApplication *app, gpointer user_data) { Lua.run(); }

// callback for editing the code
static void retag(GtkWidget *w, gpointer data) { Tagger.tag(); }

// callback for save button
static void _signal_save(GtkWidget *w, gpointer data) {
    char *code;
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(_get_object("program_buffer")),
                               &start, &end);
    code = gtk_text_buffer_get_text(
        GTK_TEXT_BUFFER(_get_object("program_buffer")), &start, &end, FALSE);
    TaskLoader.save(code);
    g_free(code);
}

// callback for double-clicking a row in the tasks view
static void _signal_task_selected(GtkWidget *w, gpointer data) {
    GtkTreeSelection *select;
    select =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(_get_object("tasks_view")));

    GtkTreeIter sectioni;
    GtkTreeModel *model = GTK_TREE_MODEL(_get_object("tasks_tree"));
    gtk_tree_selection_get_selected(select, &model, &current_task_iter);

    _w_open_task();
}

// setter for the markup of the instructions label
void _set_instructions(char *inst) {

    gtk_label_set_markup(
        GTK_LABEL(gtk_builder_get_object(builder, "inst_label")), inst);
}


//------------------------
//  FUNCTIONS START HERE
//------------------------

static void switch_tab(int tab) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(_get_object("notebook")), tab);
}

static void tab0(gpointer d) { switch_tab(0); }

static void tab1(gpointer d) { switch_tab(1); }

static void tab2(gpointer d) { switch_tab(2); }

// recursive function stolen from a demo
// as I remember it was part of one of the demos in gtk-demo
static void apply_css(GtkWidget *widget, GtkCssProvider *provider) {
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider), G_MAXUINT);
    if (GTK_IS_CONTAINER(widget))
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css,
                             provider);
}

// setup the builder, the widgets and all callbacks
static void setup_widgets() {
    builder = gtk_builder_new_from_file(GTK_GUI_FILE_NAME);
    gtk_application_add_window(
        app, GTK_WINDOW(gtk_builder_get_object(builder, "window")));

    accgroup = gtk_accel_group_new();

    gtk_window_add_accel_group(
        GTK_WINDOW(gtk_builder_get_object(builder, "window")), accgroup);

    swtab[0] = g_cclosure_new(G_CALLBACK(tab0), NULL, NULL);
    swtab[1] = g_cclosure_new(G_CALLBACK(tab1), NULL, NULL);
    swtab[2] = g_cclosure_new(G_CALLBACK(tab2), NULL, NULL);

    GdkModifierType mod;
    unsigned int key = 0;

    gtk_accelerator_parse("F1", &key, &mod);
    gtk_accel_group_connect(accgroup, key, mod, GTK_ACCEL_VISIBLE, swtab[0]);

    gtk_accelerator_parse("F2", &key, &mod);
    gtk_accel_group_connect(accgroup, key, mod, GTK_ACCEL_VISIBLE, swtab[1]);

    gtk_accelerator_parse("F3", &key, &mod);
    gtk_accel_group_connect(accgroup, key, mod, GTK_ACCEL_VISIBLE, swtab[2]);

    gtk_window_maximize(GTK_WINDOW(gtk_builder_get_object(builder, "window")));
    //    connect_signal("run_btn", "clicked", btn_run);
    _connect_signal("run_btn", "clicked", btn_run);
    _connect_signal("program_buffer", "changed", retag);
    _connect_signal("tasks_view", "row-activated", _signal_task_selected);
    _connect_signal("save_btn", "clicked", _signal_save);
    _connect_signal("window", "destroy", _signal_save);
    //_connect_signal("next_btn", "clicked", _signal_next);
    GtkTreeSelection *select;
    select =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(_get_object("tasks_view")));
    gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
}

void _set_start() {
    g_print("[Widgets]Post-init...\n");
    GtkTreeIter parent;

    gtk_tree_model_iter_children(GTK_TREE_MODEL(_get_object("tasks_tree")),
                                 &parent, NULL);

    gtk_tree_model_iter_children(GTK_TREE_MODEL(_get_object("tasks_tree")),
                                 &current_task_iter, &parent);

    GtkTreeSelection *select;
    select =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(_get_object("tasks_view")));
    gtk_tree_selection_select_iter(select,&current_task_iter);
    _w_open_task();
}

void _w_open_task()
{

    GtkTreeIter sectioni;
    GtkTreeModel *model = GTK_TREE_MODEL(_get_object("tasks_tree"));

    char *section = NULL, *task = NULL;
    gtk_tree_model_get(model, &current_task_iter, 0, &task, -1);
    if (gtk_tree_model_iter_parent(model, &sectioni, &current_task_iter)) {
        gtk_tree_model_get(model, &sectioni, 0, &section, -1);
        g_print("[Widgets]Activated: %s/%s\n", section, task);

        TaskPath path;
        path.section = section;
        path.name = task;
        TaskLoader.open_task(path);
        gtk_notebook_set_current_page(GTK_NOTEBOOK(_get_object("notebook")), 0);
    }

    g_free(task);
    g_free(section);
}

void _next_task() {
    TaskLoader.save();
    gtk_tree_store_set(GTK_TREE_STORE(_get_object("tasks_tree")),&current_task_iter,1,1,-1);
    TaskLoader.complete();
    if (next_task() < 0) {
        return;
    }

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(_get_object("program_buffer")),
                             "", -1);

    GtkTreeSelection *select;
    select =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(_get_object("tasks_view")));
    gtk_tree_selection_select_iter(select,&current_task_iter);


    _w_open_task();
}

int next_task() {

    GtkTreeIter i;
    /*
        if (gtk_tree_model_iter_n_children(
                GTK_TREE_MODEL(_get_object("tasks_tree")), NULL) != 0 &&
                gtk_tree_model_iter_children(
                    GTK_TREE_MODEL(_get_object("tasks_tree")), &i, NULL);) {
        }

    */
    GtkTreeIter tmp = current_task_iter;
    if (gtk_tree_model_iter_next(GTK_TREE_MODEL(_get_object("tasks_tree")),
                                 &tmp)) {
        current_task_iter = tmp;
        return 1;
    }
    GtkTreeIter parent = current_task_iter;

    if (!gtk_tree_model_iter_parent(GTK_TREE_MODEL(_get_object("tasks_tree")),
                                    &parent, &current_task_iter)) {
        return -1;
    }
    if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(_get_object("tasks_tree")),
                                  &parent)) {
        return -1;
    }
    if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(_get_object("tasks_tree")),
                                      &current_task_iter, &parent)) {
        return -1;
    }
    return 1;
}

// load and apply CSS
static void setup_css() {

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, GTK_CSS_FILE_NAME, NULL);
    apply_css(GTK_WIDGET(gtk_builder_get_object(builder, "window")), provider);

    g_print("[Widgets]CSS loaded!\n");
}

// call everything
void _setup() {
    setup_widgets();
    setup_css();
    g_print("[Widgets]Widgets setup completed!\n");
}
