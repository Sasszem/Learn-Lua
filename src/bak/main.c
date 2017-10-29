
//#include <glib.h>
#include "data.h"
#include "includes.h"
#include "tagger.h"
const char tagged[4] = "end";

char *contents;
int len;
GError *error;

static void retag(GtkWidget *w, gpointer data) {
    tag_keywords();
    // tag_kw_group(FLOW,tag);
}

void setup_widgets()
{
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Learn Lua");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    gtk_container_add(GTK_CONTAINER(window), grid);

    program_view = gtk_text_view_new();

    program_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(program_view));


    g_signal_connect(program_buffer, "changed", G_CALLBACK(retag), NULL);

    gtk_grid_attach(GTK_GRID(grid), program_view, 2, 1, 1, 1);
    //tag_keywords();

    inst_view = gtk_text_view_new();

    inst_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(inst_view));
    gtk_widget_set_sensitive(inst_view,FALSE);

    gtk_grid_attach(GTK_GRID(grid), inst_view, 1, 1, 1, 1);


    /*button = gtk_button_new_with_label("Hello World");

    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy),
                             window);

    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);*/

    gtk_widget_show_all(window);

}

void setup_css()
{



provider= gtk_css_provider_new ();
char *dir = g_get_current_dir();
g_print("Dir_ %s\n",dir);
char *path= g_build_filename(dir,GTK_CSS_FILE_NAME);
g_print("Path: %s\n",path);


GResource * res = g_resource_load(path,NULL);
g_print("res loaded!\n");

g_resources_register (res);
g_print("Res registrated\n");

gtk_css_provider_load_from_resource(provider,path);
g_print("provider loaded\n").
g_free(dir);
g_free(path);
g_print("Memory deallocated\n");
//g_free(res);
gtk_style_context_add_provider (gtk_widget_get_style_context (program_view),
                                GTK_STYLE_PROVIDER (provider),
                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}





static void activate(GtkApplication *app, gpointer user_data) {
    setup_widgets();
    setup_css();
    make_tags();
    load_file("./data/Tutorial");
    load_task();
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(inst_buffer), tasks[1].inst, -1);
}

int main(int argc, char **argv) {

    


    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
