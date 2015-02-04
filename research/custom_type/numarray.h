#include <stdio.h>
#include <stdlib.h>

#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Prototype for luaL_openlibs(), */

#define luaL_reg luaL_Reg

typedef struct NumArray {
  int size;
  double values[1];  /* variable part */
} NumArray;

int newarray (lua_State *L);
int setarray (lua_State *L);
int getarray (lua_State *L);
int getsize (lua_State *L);

static const struct luaL_reg arraylib [] = {
  {"new", newarray},
  {"set", setarray},
  {"get", getarray},
  {"size", getsize},
  {NULL, NULL}
};

int luaopen_array (lua_State *L);
