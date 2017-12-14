//lua.h
#pragma once
lua_State *S, *L; //The lua sandbox used to run the user's code

void init_lua();
void run_task(int taskno);
void close_lua();
