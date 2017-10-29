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
    g_file_get_contents(name, &file, &len, NULL);
    g_print("File: %i\n",g_utf8_validate (file, len, NULL));


    //load_task();
    num_tasks=0;
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

void null_str(char * str, int len)
{
for(;len!=0;len--)
{
str[len-1]=0;
}
}

void load_task() {

    struct Task task;
    task.l_tags=0;
    char state = 'N';
    char *ptr;
    // States: Nothing, Task, Instructions, Code, caSes
    g_print("Begin loading task...\n");
    for (char *p = strtok(file, "\n"); p != NULL; p = strtok(NULL, "\n")) {
        //g_print("Line: %s\n",p);
        switch (state) {

        case 'N':
                    g_print("State %c\n",state);
            if (cmpstr(p, "#task", -1)) {

                null_str(task.name,64);
                null_str(task.inst,8192);
                null_str(task.cases,4096);

                stpcpy(task.name, p+6);
                state = 'T';
            }
            break;
        case 'T':
            //g_print("State %c\n",state);
            if (cmpstr(p, "#inst", -1)) {
                state = 'I';
                ptr = task.inst;
            }
            if (cmpstr(p, "#cases", -1)) {
                state = 'C';
                ptr = task.cases;
            }
            if (cmpstr(p, "#!task", -1)) {
                

                g_print("Task name: %s\nText:%s\nCode:%s\n", task.name,
                        task.inst, task.cases);
                        //g_print("INST: %d\n",g_utf8_validate (task.inst, -1, NULL));
    tasks[num_tasks]=task;
    num_tasks++;

                state='N';
            }
            break;
        case 'I':
            //g_print("State %c\n",state);
            if (cmpstr(p, "#!inst",-1)) {
                state = 'T';
            } 
            else if (cmpstr(p, "#code",-1))
            {
                state ='H';
                                    //g_print("State changed to code\n");
                task.tags[task.l_tags]=ptr;
                task.l_tags++;

            }
            else {
                //g_print("Offset: %d\n",ptr-task.inst);
                
                ptr = g_utf8_offset_to_pointer (g_utf8_strncpy(ptr, p,g_utf8_strlen (p,-1)),g_utf8_strlen (p,-1));
                ptr[0]='\n';
                                ptr=g_utf8_next_char(ptr);

                //g_print("Text(I): \n%s\n",task.inst);
            }
            break;
        case 'H':
                   //g_print("State %c\n",state);
            if (cmpstr(p, "#!code",-1))
            {
                state='I';
                task.tags[task.l_tags]=ptr;
                task.l_tags++;
               // g_print("State back from code...");
            }
            else
            {
                ptr = g_utf8_offset_to_pointer (g_utf8_strncpy(ptr, p,g_utf8_strlen (p,-1)),g_utf8_strlen (p,-1));
                ptr[0]='\n';
                                ptr=g_utf8_next_char(ptr);
                //g_print("Text(C): \n%s\n",task.inst);
            }
            break;
       case 'C':
            //g_print("State %c\n",state);
            if (cmpstr(p, "#!cases",-1))
            {
                state='T';
            }
            else
            {
ptr = g_utf8_offset_to_pointer (g_utf8_strncpy(ptr, p,g_utf8_strlen (p,-1)),g_utf8_strlen (p,-1));
                ptr[0]='\n';
                                ptr=g_utf8_next_char(ptr);
            }
        
        }
        
    }
    
}
