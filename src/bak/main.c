
//#include <glib.h>
#include "data.h"
#include "includes.h"
#include "lua.h"
#include "tagger.h"
#include "widgets.h"

const char tagged[4] = "end";
#line 7
char *contents;
int len;
GError *error;


void activate(GtkApplication *app, gpointer user_data) {
    //setup_widgets();

    //setup_css();
    Widgets.setup();

    //init_tagger();
    Tagger.init();
    TaskPath task;
    task.section="Tutorial";
    task.name="Introduction";
    TaskLoader.open_task(task);
    
    
    GtkTextIter *pos;
    //g_print("Selected task %s\n", tasks[tasknum].name);
    //gtk_label_set_markup(
    //    GTK_LABEL(gtk_builder_get_object(builder, "inst_label")),
    //    tasks[tasknum].inst);

    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "window")));
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
