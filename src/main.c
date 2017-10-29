
//#include <glib.h>
#include "data.h"
#include "includes.h"
#include "tagger.h"
const char tagged[4] = "end";
#line 7
char *contents;
int len;
GError *error;

void apply_css (GtkWidget *widget, GtkCssProvider *provider)
{
  gtk_style_context_add_provider (gtk_widget_get_style_context (widget), GTK_STYLE_PROVIDER(provider), G_MAXUINT);
  if (GTK_IS_CONTAINER (widget))
    gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
}


static void retag(GtkWidget *w, gpointer data) {
    tag_keywords();
    // tag_kw_group(FLOW,tag);
}

void setup_widgets() {
    g_print("Start widget setup, line: %d\n",__LINE__);




    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Learn Lua");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);



    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);


    
    GtkWidget *program_scroll,*inst_scroll;


    program_view = gtk_text_view_new();
    gtk_widget_set_name(program_view,"prg_view");
    program_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(program_view));

    g_signal_connect(program_buffer, "changed", G_CALLBACK(retag), NULL);
gtk_widget_set_size_request(program_view,400,400);

        program_scroll=gtk_scrolled_window_new(NULL,NULL);
//gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(program_scroll),program_view);
    gtk_grid_attach(GTK_GRID(grid),program_scroll,2,1,1,1);

    // tag_keywords();

    inst_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(inst_view),GTK_WRAP_WORD);
    inst_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(inst_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(inst_view),FALSE);


inst_scroll=gtk_scrolled_window_new(NULL,NULL);
gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(inst_scroll),400);
gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(inst_scroll),400);
    gtk_container_add(GTK_CONTAINER(inst_scroll),inst_view);

    gtk_grid_attach(GTK_GRID(grid),inst_scroll,1,1,1,1);

    toolbar=gtk_action_bar_new();

        gtk_grid_attach(GTK_GRID(grid),toolbar,1,2,2,1);

    run_button = gtk_button_new_from_icon_name("media-playback-start",3);
    gtk_widget_set_name(run_button,"btn");
    g_signal_connect_swapped(run_button, "clicked", G_CALLBACK(gtk_widget_destroy),
                             window);

    gtk_action_bar_pack_start(GTK_ACTION_BAR(toolbar),run_button);
    gtk_container_add(GTK_CONTAINER(window),grid);
    gtk_widget_show_all(window);
}

void setup_css() {

    provider = gtk_css_provider_new();

    g_print("path: %s\n",GTK_CSS_FILE_NAME);
    gtk_css_provider_load_from_path (provider,GTK_CSS_FILE_NAME,NULL);
    char *str;
    str=gtk_css_provider_to_string(provider);
    g_print("provider2str: %s\n",str);
    

    GtkStyleContext *cont;
    cont=gtk_widget_get_style_context(program_view);

    g_free(str);
    apply_css(window,provider);
}

static void activate(GtkApplication *app, gpointer user_data) {
    setup_widgets();
    setup_css();
    make_tags();
    load_file("./data/Tutorial");
    load_task();
    GtkTextIter *pos;
    g_print("&buffer=%d",inst_buffer);

    

    char * cont;
    g_file_get_contents("test.lua",&cont,NULL,NULL);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(program_buffer),cont,-1);
    g_free(cont);


GtkTextIter start;
///    gtk_text_buffer_set_text(inst_buffer," ",-1);
    gtk_text_buffer_get_start_iter(inst_buffer,&start);
    gtk_text_buffer_insert_markup(GTK_TEXT_BUFFER(inst_buffer), &start,tasks[0].inst, -1);
    

}

int main(int argc, char **argv) {

    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
