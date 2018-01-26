

#include "data.h"
#include "includes.h"
#include "lua.h"
#include "tagger.h"
#include "widgets.h"

void activate(GtkApplication *app, gpointer user_data) {
    Widgets.setup();
    Tagger.init();

    TaskPath task;
    task.section = "Tutorial";
    task.name = "Introduction";

    TaskLoader.open_task(task);
    TaskLoader.list();

    g_print("[Main]Setup done\n");
    Widgets.show();
}

int main(int argc, char **argv) {

    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
