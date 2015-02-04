#include <stdbool.h>
#ifdef __MINGW32__
#include <Rpc.h>
typedef UUID uuid_t;
#elif __linux__
#include <uuid/uuid.h>
#endif

#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Prototype for luaL_openlibs(), */

#define luaL_reg luaL_Reg
#define DEBUG 0

typedef struct TreeNode {
  uuid_t uuid;
  int children_iter;
  int children_count;
  int *udreflist; // lua references for TreeNode userdata
  struct TreeNode **children; // local references for better use in C program
  struct TreeNode *parent;
  char *name;
  unsigned int dataType;
  union {
    double numberVal;
    char *charVal;
    bool boolVal;
  } data;
} TreeNode;

static void custom_generate_uuid(uuid_t *uuid);
static void uuid_to_string(uuid_t *uuid, char *uuid_str);
static int custom_uuid_compare(uuid_t uuid1, uuid_t uuid2);
static int printd(const char *format, ...);
static char* assignNewString(char *str_to, const char *str_from);
static int next_child_ref(TreeNode* parent);

int node_iter(lua_State *L);
int newtree (lua_State *L);
int addnode (lua_State *L);
int rmnode (lua_State *L);
int setname (lua_State *L);
int getname (lua_State *L);
int setvalue (lua_State *L);
int getvalue (lua_State *L);
int getnodes (lua_State *L);
int treenode2string (lua_State *L);

static const struct luaL_reg tree_meths [] = {
  {"add", addnode},
  {"rm", rmnode},
  {"children", getnodes},
  {"get", getvalue},
  {"set", setvalue},
  {"getvalue", getvalue},
  {"setvalue", setvalue},
  {"getname", getname},
  {"setname", setname},
  {"__gc", rmnode},
  {"__tostring", treenode2string},
  {NULL, NULL}
};

static const struct luaL_reg tree_funcs [] = {
  {"new", newtree},
  {NULL, NULL}
};

int luaopen_tree (lua_State *L);
