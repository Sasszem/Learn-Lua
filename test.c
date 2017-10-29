#include <gtk/gtk.h>
GtkWidget *window;
GtkWidget *grid;
    GtkApplication *app;

apply_css (GtkWidget *widget, GtkStyleProvider *provider)
{
  gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
  if (GTK_IS_CONTAINER (widget))
    gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *view;
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  PangoFontDescription *font_desc;
  GdkRGBA rgba;
  GtkTextTag *tag;
  GtkCssProvider *provider;
  GtkStyleContext *context;

g_print("Begin activating!\n");

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Learn Lua");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  grid = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

  gtk_container_add(GTK_CONTAINER(window), grid);
  view = gtk_text_view_new();

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  gtk_text_buffer_set_text(buffer, "Hello, this is some text", -1);

  /* Change default font and color throughout the widget */
provider = gtk_css_provider_new ();
gtk_css_provider_load_from_data (provider,
                                 "textview text {"
                                 " font: 15 serif;"
                                 "  color: green;"
                                 "}",
                                 -1,
                                 NULL);
apply_css(window,provider);


  /* Change left margin throughout the widget */
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 30);

  /* Use a tag to change the color for just one part of the widget */
  tag = gtk_text_buffer_create_tag(buffer, "blue_foreground", "foreground",
                                   "blue", NULL);
  gtk_text_buffer_get_iter_at_offset(buffer, &start, 7);
  gtk_text_buffer_get_iter_at_offset(buffer, &end, 12);
  gtk_text_buffer_apply_tag(buffer, tag, &start, &end);
  gtk_grid_attach(GTK_GRID(grid), view, 1, 1, 1, 1);
    gtk_widget_show_all(window);
g_print("End!\n");
}

int main(int argc, char **argv) {
g_print("Hello!\n");
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}
