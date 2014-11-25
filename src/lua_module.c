#include "lua_module.h"

void bail(lua_State *L, char *msg) {
  fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n", msg, lua_tostring(L, -1));
  exit(1);
}

void luamodule_register(lua_State *L, const char *name, lua_CFunction function) {
  lua_register(L, name, function);
}

lua_State* luamodule_init(const char* scriptName) {
  lua_State* L = luaL_newstate();						/* Create Lua state variable */
  luaL_openlibs(L);							/* Load Lua libraries */

  if (luaL_loadfile(L, scriptName))	/* Load but don't run the Lua script */
    bail(L, "luaL_loadfile() failed");		/* Error out if file can't be read */

  if (lua_pcall(L, 0, 0, 0))					/* PRIMING RUN. FORGET THIS AND YOU'RE TOAST */
    bail(L, "lua_pcall() failed");			/* Error out if Lua file has an error */

  /*
    printf("In C, calling Lua->square(6)\n");

    lua_getglobal(L, "square");					// Tell it to run callfuncscript.lua->square()
    lua_pushnumber(L, 6);						// Submit 6 as the argument to square()
    if (lua_pcall(L, 1, 1, 0))					// Run function, !!! NRETURN=1 !!!
    bail(L, "lua_pcall() failed"); 

    printf("Back in C again\n");
    int mynumber = lua_tonumber(L, -1);
    printf("Returned number=%d\n", mynumber);
  */
  return L;
}

void luamodule_close(lua_State* L) {
  lua_close(L);
}

void luamodule_callLUA(lua_State* L, char* function) {
  lua_getglobal(L, function);				/* Tell it to run callfuncscript.lua->tellme() */
  if (lua_pcall(L, 0, 0, 0))				/* Run the function */
    bail(L, "lua_pcall() failed");	
}

void luamodule_callVoidStr(lua_State* L, char* function, int argc, char *argv[]) {
  int i;
  lua_getglobal(L, function);

  for (i = 0; i < argc; ++i)
    lua_pushstring(L, argv[i]);

  if (lua_pcall(L, argc, 0, 0))				/* Run the function */
    bail(L, "lua_pcall() failed");
}

void luamodule_callVoidNumber(lua_State* L, char* function, int argc, lua_Number argv[]) {
  int i;
  lua_getglobal(L, function);

  for (i = 0; i < argc; ++i)
    lua_pushnumber(L, argv[i]);

  if (lua_pcall(L, argc, 0, 0))				/* Run the function */
    bail(L, "lua_pcall() failed");
}

int luamodule_average(lua_State *L) {
  /* get number of arguments */
  int n = lua_gettop(L);
  double sum = 0;
  int i;

  /* loop through each argument */
  for (i = 1; i <= n; i++) {
    /* total the arguments */
    sum += lua_tonumber(L, i);
  }

  /* push the average */
  lua_pushnumber(L, sum / n);

  /* push the sum */
  lua_pushnumber(L, sum);

  /* return the number of results */
  return 2;
}
