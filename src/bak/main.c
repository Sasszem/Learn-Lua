
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
static void show_dia(Task *task, short id);
void apply_css(GtkWidget *widget, GtkCssProvider *provider) {
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider), G_MAXUINT);
    if (GTK_IS_CONTAINER(widget))
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css,
                             provider);
}

static void btn_run(GtkApplication *app, gpointer user_data) {
    run_task(1);

}

static void retag(GtkWidget *w, gpointer data) {
    tag_keywords();
    // tag_kw_group(FLOW,tag);
}
/*
void setup_widgets() {
    g_print("Start widget setup, line: %d\n", __LINE__);

    window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(window), "Learn Lua");
    // gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    
    grid = gtk_grid_new();
    gtk_widget_set_name(grid,"grid");
    //gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    
    GtkWidget *program_scroll, *inst_scroll, *console_scroll;

    program_view = gtk_text_view_new();
    gtk_widget_set_name(program_view, "prg_view");
    program_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(program_view));

    g_signal_connect(program_buffer, "changed", G_CALLBACK(retag), NULL);
    // gtk_widget_set_size_request(program_view, 400, 400);

    program_scroll = gtk_scrolled_window_new(NULL, NULL);
    // gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(program_scroll,TRUE);
    gtk_widget_set_vexpand(program_scroll,TRUE);

    gtk_container_add(GTK_CONTAINER(program_scroll), program_view);
    gtk_grid_attach(GTK_GRID(grid), program_scroll, 2, 1, 1, 1);

    // tag_keywords();

    inst_view = gtk_text_view_new();
    gtk_widget_set_name(inst_view, "inst_view");
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(inst_view), GTK_WRAP_WORD);
    inst_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(inst_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(inst_view), FALSE);
    
    inst_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(inst_scroll,TRUE);
        gtk_widget_set_hexpand(inst_scroll,TRUE);
    // gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(inst_scroll),
    //    400);
    gtk_container_add(GTK_CONTAINER(inst_scroll), inst_view);

    gtk_grid_attach(GTK_GRID(grid), inst_scroll, 1, 1, 1, 1);

   
    toolbar = gtk_action_bar_new();

    gtk_grid_attach(GTK_GRID(grid), toolbar, 1, 2, 3, 1);

    run_button = gtk_button_new_from_icon_name("media-playback-start", 3);
    gtk_widget_set_name(run_button, "btn");
    // g_signal_connect_swapped(run_button, "clicked",
    // G_CALLBACK(gtk_widget_destroy),
    //                         window);
    g_signal_connect(run_button, "clicked", G_CALLBACK(btn_run), NULL);
    gtk_action_bar_pack_start(GTK_ACTION_BAR(toolbar), run_button);
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_widget_show_all(window);

    int w, h;
    gtk_window_get_size(GTK_WINDOW(window), &w, &h);
    g_print("Win w=%d, h=%d\n", w, h);
    //gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(inst_scroll),
    //                                           870);
}*/

void connect_signal(char *name,char* signal, void* funcs)
{
g_signal_connect(gtk_builder_get_object(builder,name), signal, G_CALLBACK(funcs), NULL);
}

void setup_widgets()
{
builder=gtk_builder_new_from_file(GTK_GUI_FILE_NAME);
gtk_application_add_window(app,GTK_WINDOW(gtk_builder_get_object(builder,"window")));
    gtk_window_maximize(GTK_WINDOW(gtk_builder_get_object(builder,"window")));
connect_signal("run_btn","clicked",btn_run);
        connect_signal("program_buffer", "changed",retag);
}




void setup_css() {

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, GTK_CSS_FILE_NAME, NULL);
    apply_css(GTK_WIDGET(gtk_builder_get_object(builder,"window")), provider);

    g_print("CSS loaded!\n");
}

/*static void show_dia(Task *task, short id) {

    GtkWidget *dialog, *scrolled, *text, *content_area, *label;
    GtkDialogFlags flags;

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Message", gtk_builder_get_object(builder,"window"), flags, "OK",
                                         GTK_RESPONSE_NONE, NULL);
    gtk_window_set_title(GTK_WINDOW(dialog), task->popup_titles[id]);
    gtk_widget_set_name(dialog, "popup");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 150, 75);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_icon_name(GTK_WINDOW(dialog), "task-due");

    label = gtk_label_new(task->popup_texts[id]);
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_widget_set_name(label, "popup_label");
    gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled),
                                               150);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled),
                                              400);

    g_object_set(scrolled, "margin-left", 30, "margin-right", 30, "margin-top",
                 10, NULL);

    gtk_container_add(GTK_CONTAINER(scrolled), label);

    // Ensure that the dialog box is destroyed when the user responds

    g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy),
                             dialog);

    // Add the label, and show everything we’ve added

    gtk_container_add(GTK_CONTAINER(content_area), scrolled);
    apply_css(dialog, provider);
    gtk_window_set_position(dialog, GTK_WIN_POS_CENTER);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
}*/

static void activate(GtkApplication *app, gpointer user_data) {
    setup_widgets();

    //g_print("Win:%d\n",gtk_builder_get_object(builder,"window"));

    setup_css();
    init_tagger();
    load_file("./data/Tutorial");
    load_task();
    GtkTextIter *pos;


    char *cont;
    g_file_get_contents("test.lua", &cont, NULL, NULL);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"program_buffer")), cont, -1);
    g_free(cont);


    ///    gtk_text_buffer_set_text(inst_buffer," ",-1);

    gtk_label_set_markup(GTK_LABEL(gtk_builder_get_object(builder,"inst_label")),tasks[tasknum].inst);
    //Task *task = &tasks[TASKNUM];
gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder,"window")));

    /*for (short id = 0; id < task->popup_count; id++) {
        show_dia(task, id);
    }*/
}



#define TASKNUM 0
int main(int argc, char **argv) {
    tasknum=TASKNUM;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
