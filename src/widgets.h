//widgets.h


void _setup();
void _connect_signal(char *name, char *signal, void *funcs);
void _set_instructions(char *);
void _show();
void *_get_object(char *name);
void _set_code(char *);
void _set_start();
void _next_task();

static struct Widgets 
{
void (*setup)(void);
void (*connect_signal)(char*name,char*signal,void*funcs);
void (*set_instructions)(char *);
void (*show)();
void *(*get_object)(char *);
void (*set_code)(char *);
void (*postinit)();
void (*next)();
}
Widgets =
{
.setup=&_setup,
.connect_signal=&_connect_signal,
.set_instructions=&_set_instructions,
.show=&_show,
.get_object=&_get_object,
.set_code=&_set_code,
.postinit=&_set_start,
.next=&_next_task
};
