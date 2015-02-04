#include <stdio.h>
#include <stdlib.h>

#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Prototype for luaL_openlibs(), */

#include "numarray.h"
#include "tree.h"

static void test_array(lua_State *L);
static void test_tree(lua_State *L);

void custom_error(lua_State *L, const char *format, const char *msg) {
#ifdef __MINGW32__
  printf(format, msg);
  exit(1);
#elif __linux__
  error(L, format, msg);
#endif
}

int main() {
  char* scriptName = "test.lua";
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  if (luaL_loadfile(L, scriptName))
    custom_error(L, "luaL_loadfile() failed: %s", lua_tostring(L, -1));

  if (lua_pcall(L, 0, 0, 0))
    custom_error(L, "lua_pcall() failed: %s", lua_tostring(L, -1));

  // open new array lib in lua space
  luaopen_array(L);
  luaopen_tree(L);

  //test_array(L);
  test_tree(L);

  // clear all
  lua_close(L);
}

static void test_tree(lua_State *L) {
  lua_getglobal(L, "treeTest");

  if (lua_pcall(L, 0, 0, 0) != 0)
    custom_error(L, "error running function 'treeTest': %s",
          lua_tostring(L, -1));
}

static void test_array(lua_State *L) {
  lua_getglobal(L, "arrayTest");

  if (lua_pcall(L, 0, 0, 0) != 0)
    custom_error(L, "error running function 'arrayTest': %s",
          lua_tostring(L, -1));
}
