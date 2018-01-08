
//#include <glib.h>
#include "data.h"
#include "includes.h"
#include "lua.h"
#include "tagger.h"
const char tagged[4] = "end";
#line 7
char *contents;
int len;
GError *error;

void apply_css(GtkWidget *widget, GtkCssProvider *provider) {
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider), G_MAXUINT);
    if (GTK_IS_CONTAINER(widget))
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css,
                             provider);
}

static void btn_run(GtkApplication *app, gpointer user_data) { run_task(1); }

static void retag(GtkWidget *w, gpointer data) { tag_keywords(); }

void connect_signal(char *name, char *signal, void *funcs) {
    g_signal_connect(gtk_builder_get_object(builder, name), signal,
                     G_CALLBACK(funcs), NULL);
}

void setup_widgets() {
    builder = gtk_builder_new_from_file(GTK_GUI_FILE_NAME);
    gtk_application_add_window(
        app, GTK_WINDOW(gtk_builder_get_object(builder, "window")));
    gtk_window_maximize(GTK_WINDOW(gtk_builder_get_object(builder, "window")));
    connect_signal("run_btn", "clicked", btn_run);
    connect_signal("program_buffer", "changed", retag);
}

void setup_css() {

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, GTK_CSS_FILE_NAME, NULL);
    apply_css(GTK_WIDGET(gtk_builder_get_object(builder, "window")), provider);

    g_print("CSS loaded!\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
    setup_widgets();

    setup_css();
    init_tagger();
    load_file("./data/Tutorial");
    load_task();
    GtkTextIter *pos;


        gtk_label_set_markup(
            GTK_LABEL(gtk_builder_get_object(builder, "inst_label")),
            tasks[tasknum].inst);

        gtk_widget_show_all(
            GTK_WIDGET(gtk_builder_get_object(builder, "window")));
}

#define TASKNUM 0
int main(int argc, char **argv) {
    tasknum = TASKNUM;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
