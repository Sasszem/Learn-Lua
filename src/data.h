//data.h

typedef struct Task {
    char name[64];
    // task name
    char inst[8192];
    // task instructions
    char cases[4096];
    // task test cases generator LUA code
    int type;
    //type=0: function - 1: stdio
/*
    
    char popup_titles[8][16];
    char popup_texts[8][1024];   
    short popup_count;
*/
} Task;


char *file;
int len;

void load_file(char* name);
void load_task();

struct Task tasks[64];
int num_tasks;
