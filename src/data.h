//data.h

struct Task {
    char name[64];
    // task name
    char inst[8192];
    // task instructions
    char cases[4096];
    // task test cases generator LUA code
    int type;
    //type=0: function - 1: stdio
    // start-end pairs of CODE tags inside instructions
};


char *file;
int len;

void load_file(char* name);
void load_task();

struct Task tasks[16];
int num_tasks;
