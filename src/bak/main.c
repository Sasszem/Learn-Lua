
//#include <glib.h>
#pragma once
#include "includes.h"
#include "tagger.h"
const char tagged[4] = "end";

/*static void retag() {
    g_print("Starting retag\n");
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    int offset = -3;
    GtkTextIter tags, tage;
    gtk_text_buffer_remove_tag(buffer, tag, &start, &end);
    while (1) {
        char *pos = g_strstr_len(text+offset+3, -1,tagged);
        if (pos == NULL) {
            g_print("substring not found :(\n");
            g_free(pos);
            break;
        } else {
            g_print("Found substr\n");
            offset = pos - text;
            g_print("offset: %d\n",offset);

            gtk_text_buffer_get_iter_at_offset(buffer, &tags, offset);
            gtk_text_buffer_get_iter_at_offset(buffer, &tage,
                                               offset + sizeof(tagged));
            gtk_text_buffer_apply_tag(buffer, tag, &tags, &tage);
        }

    }
    g_free(text);
}*/

static void retag(GtkWidget *w, gpointer data) {
    g_print("Starting retag\n");
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gtk_text_buffer_remove_tag(buffer, tag, &start, &end);
    // tag_kw("local",kw_tags[0]);
    tag_keywords();
    // tag_kw_group(FLOW,tag);
}

static void print_hello(GtkWidget *widget, gpointer data) {
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    g_print("%s\n", str);
    g_free(str);
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
    gtk_text_buffer_set_text(buffer, "Hello, this is some text", -1);
    tag = gtk_text_buffer_create_tag(buffer, "blue_foreground", "foreground",
                                     "#0000FF", "weight", 800, NULL);
    gtk_text_buffer_get_iter_at_offset(buffer, &start, 7);
    gtk_text_buffer_get_iter_at_offset(buffer, &end, 12);
    gtk_text_buffer_apply_tag(buffer, tag, &start, &end);
    g_signal_connect(buffer, "changed", G_CALLBACK(retag), NULL);

    gtk_grid_attach(GTK_GRID(grid), view, 2, 1, 1, 1);
    button = gtk_button_new_with_label("Hello World");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

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
