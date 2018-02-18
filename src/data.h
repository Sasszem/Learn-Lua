//data.h



#define TESTER_FILE "tester.lua"
#define INSTR_FILE "instructions.txt"
#define TASKPATH "./data/tasks/"

#define SAVEPATH "./save/"


//Path of a task
typedef struct TaskPath {char *section,*name;} TaskPath;



//Theese functions should not be callded outside of 'data.h'
void _open_task(TaskPath task);
void _fill_list();



void _save_code();
void _load_code();

void _complete_task();


//Interface to 'data.h'
static struct TaskLoader {
void (*open_task)(TaskPath task);
void (*list)();
void (*save)();
void (*complete)();
void (*load_save)();
} TaskLoader = {.open_task=&_open_task,.list=&_fill_list,.save=&_save_code,.load_save=&_load_code,.complete=&_complete_task};



