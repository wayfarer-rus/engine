#include <stdio.h>
#include <stdlib.h>

#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Prototype for luaL_openlibs(), */

lua_State* luamodule_init(const char* scriptName);
void luamodule_close(lua_State* L);
int luamodule_average(lua_State *L);
void luamodule_register(lua_State *L, const char* name, lua_CFunction function);

void luamodule_callLUA(lua_State* L, char* function);
void luamodule_callVoidStr(lua_State* L, char* function, int argc, char *argv[]);
void luamodule_callVoidNumber(lua_State* L, char* function, int argc, lua_Number argv[]);

static void bail(lua_State *L, char *msg);
