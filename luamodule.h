#include <stdio.h>
#include <stdlib.h>

#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Prototype for luaL_openlibs(), */

lua_State* luamodule_init();
void luamodule_close(lua_State* L);
void luamodule_callLUA(lua_State* L, char* function);
int luamodule_average(lua_State *L);
void luamodule_register(lua_State *L, const char* name, lua_CFunction function);

static void bail(lua_State *L, char *msg);
