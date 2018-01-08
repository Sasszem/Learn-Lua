// data.c

#include "includes.h"

#include "globals.h"

#include "data.h"
/* void free_task(struct Task task) {
     g_free(task.name);
     g_free(task.inst);
     g_free(task.cases);
 }*/
void load_file(char *name) {
    g_file_get_contents(name, &file, NULL, NULL);

    // load_task();
    num_tasks = 0;
}

int cmpstr(char *str, char *pattern, int len) {
    if (len == -1)
        len = strlen(pattern);
    for (int i = 0; i < len; i++) {
        if (str[i] != pattern[i])
            return 0;
    }
    return 1;
}

void null_str(char *str, int len) {
    for (; len != 0; len--) {
        str[len - 1] = 0;
    }
}


char *
strtok_single (char * str, char const * delims)
{
  static char  * src = NULL;
  char  *  p,  * ret = 0;

  if (str != NULL)
    src = str;

  if (src == NULL)
    return NULL;

  if ((p = strpbrk (src, delims)) != NULL) {
    *p  = 0;
    ret = src;
    src = ++p;

  } else if (*src) {
    ret = src;
    src = NULL;
  }

  return ret;
}

void load_task() {

    struct Task task;
    char state = 'N';
    char *ptr;
    // States: Nothing, Task, Instructions,  caSes
    g_print("Begin loading task...\n");
    for (char *p = strtok_single(file, "\n"); p != NULL; p = strtok_single(NULL, "\n")) {
        //g_print("[%c]Line: %s\n", state, p);
        switch (state) {

        case 'N':
            //g_print("State %c\n", state);
            if (cmpstr(p, "#task", -1)) {

                null_str(task.name, 64);
                null_str(task.inst, 8192);
                null_str(task.cases, 4096);
                stpcpy(task.name, p + 6);
                state = 'T';
            }
            break;
        case 'T':
            if (cmpstr(p, "#inst", -1)) {
                state = 'I';
                ptr = task.inst;
            }
            if (cmpstr(p, "#cases", -1)) {
                state = 'C';
                ptr = task.cases;
            }
            if (cmpstr(p, "#!task", -1)) {

                //g_print("Task name: %s\nText:%s\nCode:%s\n", task.name,
                //        task.inst, task.cases);
                tasks[num_tasks] = task;
                num_tasks++;

                state = 'N';
            }
            break;
        case 'I':
            if (cmpstr(p, "#!inst", -1)) {
                state = 'T';
            } else {
                //g_print("Offset: %d\n",ptr-task.inst);

                ptr = g_utf8_offset_to_pointer(
                    g_utf8_strncpy(ptr, p, g_utf8_strlen(p, -1)),
                    g_utf8_strlen(p, -1));
                ptr[0] = '\n';
                ptr = g_utf8_next_char(ptr);

                //g_print("Text(I): \n%s\n",task.inst);
            }
            break;

        case 'C':
            if (cmpstr(p, "#!cases", -1)) {
                state = 'T';
            } else {
                ptr = g_utf8_offset_to_pointer(
                    g_utf8_strncpy(ptr, p, g_utf8_strlen(p, -1)),
                    g_utf8_strlen(p, -1));
                ptr[0] = '\n';
                ptr = g_utf8_next_char(ptr);
            }
            break;

        }
    }
}
