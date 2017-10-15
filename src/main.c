
//#include <glib.h>
#pragma once
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
static void activate(GtkApplication *app, gpointer user_data) {

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Learn Lua");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    gtk_container_add(GTK_CONTAINER(window), grid);

    view = gtk_text_view_new();

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    make_tags();
    
    g_file_get_contents("./test.lua",&contents,&len,&error);
    g_print("File contetnts(%d bytes): \n%s",len,contents);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer),contents,-1);
    g_free(contents);
    g_signal_connect(buffer, "changed", G_CALLBACK(retag), NULL);

gtk_grid_attach(GTK_GRID(grid), view, 2, 1, 1, 1);
tag_keywords();

    button = gtk_button_new_with_label("Hello World");

    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy),
                             window);

    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
