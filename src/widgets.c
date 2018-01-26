// widgets.c

#include "includes.h"

#include "data.h"
#include "globals.h"
#include "lua.h"
#include "tagger.h"
static GtkBuilder *builder;

static GtkCssProvider *provider;

void _show() {
    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "window")));
}

void *_get_object(char *name) { return gtk_builder_get_object(builder, name); }

static void apply_css(GtkWidget *widget, GtkCssProvider *provider) {
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider), G_MAXUINT);
    if (GTK_IS_CONTAINER(widget))
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css,
                             provider);
}

static void btn_run(GtkApplication *app, gpointer user_data) {
    g_print("Button pressed!!\n");
    Lua.run();
}

static void save(GtkApplication *app, gpointer user_data) {

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(
        GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "program_buffer")),
        &start, &end);

    // char *code = gtk_text_buffer_get_text(
    //    GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "program_buffer")),
    //    &start, &end, FALSE);

    // save_code("code2.lua", code);

    // g_free(code);
}
static void retag(GtkWidget *w, gpointer data) { Tagger.tag(); }

void _connect_signal(char *name, char *signal, void *funcs) {
    g_signal_connect(gtk_builder_get_object(builder, name), signal,
                     G_CALLBACK(funcs), NULL);
}

void _signal_task_selected(GtkWidget *w, gpointer data) {

    GtkTreeSelection *select;
    select =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(_get_object("tasks_view")));

    GtkTreeIter taski, sectioni;
    GtkTreeModel *model = GTK_TREE_MODEL(_get_object("tasks_tree"));
    gtk_tree_selection_get_selected(select, &model, &taski);

    char *section = NULL, *task = NULL;
    gtk_tree_model_get(model, &taski, 0, &task, -1);
    if (gtk_tree_model_iter_parent(model, &sectioni, &taski)) {
        gtk_tree_model_get(model, &sectioni, 0, &section, -1);
        g_print("Activated: %s/%s\n", section, task);

        TaskPath path;
        path.section = section;
        path.name = task;
        TaskLoader.open_task(path);
        gtk_notebook_set_current_page(GTK_NOTEBOOK(_get_object("notebook")), 0);
    }

    g_free(task);
    g_free(section);
}

static void setup_widgets() {
    builder = gtk_builder_new_from_file(GTK_GUI_FILE_NAME);
    gtk_application_add_window(
        app, GTK_WINDOW(gtk_builder_get_object(builder, "window")));
    gtk_window_maximize(GTK_WINDOW(gtk_builder_get_object(builder, "window")));
    //    connect_signal("run_btn", "clicked", btn_run);
    _connect_signal("run_btn", "clicked", btn_run);
    _connect_signal("program_buffer", "changed", retag);
    _connect_signal("tasks_view", "row-activated", _signal_task_selected);

    GtkTreeSelection *select;
    select =
        gtk_tree_view_get_selection(GTK_TREE_VIEW(_get_object("tasks_view")));
    gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
}

static void setup_css() {

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, GTK_CSS_FILE_NAME, NULL);
    apply_css(GTK_WIDGET(gtk_builder_get_object(builder, "window")), provider);

    g_print("CSS loaded!\n");
}

void _set_instructions(char *inst)

{

    gtk_label_set_markup(
        GTK_LABEL(gtk_builder_get_object(builder, "inst_label")), inst);
}

void _setup() {
    setup_widgets();
    setup_css();
    g_print("Widgets setup completed!\n");
}
