//globals.h

GtkApplication *app;

GtkWidget *window;
GtkWidget *program_view, *inst_view;
GtkTextBuffer *program_buffer, *inst_buffer;

GtkWidget *grid;
GtkWidget *toolbar;
GtkWidget *run_button;
GIcon *run_icon;


#define GTK_CSS_FILE_NAME "./style.css"

GtkCssProvider *provider;
