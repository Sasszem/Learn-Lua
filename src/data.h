//data.h



#define TESTER_FILE "tester.lua"
#define INSTR_FILE "instructions.txt"
#define TASKPATH "./data/tasks/"


//static struct DataError {int NO_SUTCH_FILE;} DataError = {.NO_SUTCH_FILE=-1};


/*
typedef struct Task {
    char name[64];
    // task name
    char inst[8192];
    // task instructions
    char tester[4096];
    // task tester LUA code
} Task;
*/
//Path of a task
typedef struct TaskPath {char *section,*name;} TaskPath;

//Task tasks[64]
//int num_tasks;
void _open_task(TaskPath task);
void _fill_list();


static struct TaskLoader {
void (*open_task)(TaskPath task);
void (*list)();
} TaskLoader = {.open_task=&_open_task,.list=&_fill_list};

static struct SaveManager {

} SaveManager = 
{

};


int save_task(TaskPath *task, char* code);
char* load_task(TaskPath *task);
