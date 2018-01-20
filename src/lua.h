//lua.h
#pragma once


void _run_task();
void _set_tester(char *tester);



static struct Lua {
void (*run)();
void (*set_tester)(char*);
} Lua =
{
.run=&_run_task,
.set_tester=_set_tester
};


