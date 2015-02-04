#include "tree.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static TreeNode* findParentForUUID(TreeNode *parent, uuid_t uuid) {
  int i;

  for (i = 0; i < parent->children_count; ++i) {
    if (0 == custom_uuid_compare(uuid, parent->children[i]->uuid)) {
      return parent;
    }
  }

  for (i = 0; i < parent->children_count; ++i) {
    TreeNode *node = findParentForUUID(parent->children[i], uuid);

    if (node != NULL)
      return node;
  }

  return NULL;
}

static TreeNode *checktree (lua_State *L, int i) {
  void *ud = luaL_checkudata(L, i, "LuaBook.treenode");
  luaL_argcheck(L, ud != NULL, i, "'treenode' expected");
  return (TreeNode *)ud;
}

int setname (lua_State *L) {
  TreeNode *node = checktree(L, 1);
  const char *name = luaL_checkstring(L, 2);
  luaL_argcheck(L, name != NULL, 2, "'treenode' name shouldn't be empty");
  node->name = assignNewString(node->name, name);
  return 0;
}

int getname (lua_State *L) {
  TreeNode *node = checktree(L, 1);
  lua_pushstring(L, node->name);
  return 1;
}

int setvalue (lua_State *L) {
  TreeNode *node = checktree(L, 1);
  int valueType = lua_type(L, 2);

  if (valueType != LUA_TNIL &&
      valueType != LUA_TNUMBER &&
      valueType != LUA_TBOOLEAN &&
      valueType != LUA_TSTRING)
  {
    lua_pushstring(L, "incorrect value. supported types are int, float, boolean, string");
    lua_error(L);
  }

  if (valueType != LUA_TSTRING && node->dataType == LUA_TSTRING)
    free(node->data.charVal);

  switch (valueType) {
  case LUA_TNIL:
    node->dataType = LUA_TNIL;
    break;
  case LUA_TNUMBER:
    node->data.numberVal = lua_tonumber(L,2);
    node->dataType = LUA_TNUMBER;
    break;
  case LUA_TBOOLEAN:
    node->data.boolVal = lua_toboolean(L,2);
    node->dataType = LUA_TBOOLEAN;
    break;
  case LUA_TSTRING:
    if (node->dataType == LUA_TSTRING)
      node->data.charVal = assignNewString(node->data.charVal, lua_tostring(L,2));
    else
      node->data.charVal = strdup(lua_tostring(L,2));

    node->dataType = LUA_TSTRING;
    break;
  }

  return 0;
}

int getvalue (lua_State *L) {
  TreeNode *node = checktree(L, 1);

  switch (node->dataType) {
  case LUA_TNIL:
    lua_pushnil(L);
    break;
  case LUA_TNUMBER:
    lua_pushnumber(L, node->data.numberVal);
    break;
  case LUA_TBOOLEAN:
    lua_pushboolean(L, node->data.boolVal);
    break;
  case LUA_TSTRING:
    lua_pushstring(L, node->data.charVal);
    break;
  }

  return 1;
}

int newtree (lua_State *L) {
  const char *name = luaL_checkstring(L, 1);
  int valueType = lua_type(L, 2);

  luaL_argcheck(L, name != NULL, 1, "name for new 'treenode' expected");

  if (valueType != LUA_TNIL &&
      valueType != LUA_TNUMBER &&
      valueType != LUA_TBOOLEAN &&
      valueType != LUA_TSTRING)
  {
    lua_pushstring(L, "incorrect value. supported types are int, float, boolean, string");
    lua_error(L);
  }

  TreeNode *node = (TreeNode*)lua_newuserdata(L, sizeof(TreeNode));
  node->name = strdup(name);
  node->children_count = 0;
  node->udreflist = NULL;
  node->children = NULL;
  node->parent = NULL;
  node->dataType = valueType;
  custom_generate_uuid(&node->uuid);

  switch (valueType) {
  case LUA_TNUMBER:
    node->data.numberVal = luaL_checknumber(L,2);
    break;
  case LUA_TBOOLEAN:
    node->data.boolVal = lua_toboolean(L,2);
    break;
  case LUA_TSTRING:
    node->data.charVal = strdup(lua_tostring(L,2));
    break;
  }

  luaL_getmetatable(L, "LuaBook.treenode");
  lua_setmetatable(L, -2);
  char uuid_str[40];
  uuid_to_string(&node->uuid, uuid_str);
  printd("treenode with uuid %s created\n", uuid_str);

  return 1;
}

int addnode (lua_State *L) {
  TreeNode *node = checktree(L, 1);
  TreeNode *new_node = checktree(L, 2);

  luaL_argcheck(L, new_node->parent == NULL, 2, "new 'treenode' should not have parent");
  lua_pushvalue(L, 2);  /* put new_node userdata on the stack top */

  if (node->children_count == 0) {
    node->udreflist = (int *)malloc(sizeof(int));
    node->children = (TreeNode **)malloc(sizeof(TreeNode*));
    node->children[0] = new_node;
    node->children_count = 1;
    new_node->parent = node;
    //register lua reference to TreeNode userdata
    node->udreflist[0] = luaL_ref(L, LUA_REGISTRYINDEX);
  }
  else {
    int i;
    bool tamped = false;

    for (i = 0; i < node->children_count; ++i) {
      if (node->children[i] == NULL) {
        node->children[i] = new_node;
        //register lua reference to TreeNode userdata
        node->udreflist[i] = luaL_ref(L, LUA_REGISTRYINDEX);
        tamped = true;
        break;
      }
    }

    if (!tamped) { // there is no place to put another node. Create new space then
      node->children_count++;
      node->children = (TreeNode **)realloc(node->children, node->children_count*sizeof(TreeNode*));
      node->children[node->children_count-1] = new_node;
      node->udreflist = (int *)realloc(node->udreflist, node->children_count*sizeof(int));
      //register lua reference to TreeNode userdata
      node->udreflist[node->children_count-1] = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    new_node->parent = node;
  }

  return 0;
}

int rmnode (lua_State *L) {
  TreeNode *node = checktree(L, 1);
  TreeNode *parent = node->parent;
  int i;
  char uuid_str[40];
  uuid_to_string(&node->uuid, uuid_str);
  printd("Removing node with uuid %s\n", uuid_str);

  if (parent != NULL) {
    uuid_to_string(&parent->uuid, uuid_str);
    printd("Node has parent with uuid %s. Remove reference from parent's children list.\n", uuid_str);

    for (i = 0; i < parent->children_count; ++i) {
      if (parent->children != NULL && parent->children[i] != NULL &&
          0 == custom_uuid_compare(node->uuid, parent->children[i]->uuid))
      {
        parent->children[i] = NULL;
        /* free (unref) userdata in parent node */
        luaL_unref(L, LUA_REGISTRYINDEX, parent->udreflist[i]);
        parent->udreflist[i] = LUA_REFNIL;
      }
    }
  }

  // free node strucrure
  /* free (unref) userdata in current node */
  for (i = 0; i < node->children_count; ++i) {
    if (node->udreflist != NULL && node->udreflist[i] != LUA_REFNIL) {
      luaL_unref(L, LUA_REGISTRYINDEX, node->udreflist[i]);
      node->udreflist[i] = LUA_REFNIL;
      node->children[i]->parent = NULL;
    }
  }

  if (node->dataType == LUA_TSTRING)
    free(node->data.charVal);

  if (node->name)
    free(node->name);

  if (node->children)
    free(node->children);

  if (node->udreflist)
    free(node->udreflist);

  node->udreflist = NULL;
  node->children = NULL;
  node->parent = NULL;
  node->name = NULL;

  return 0;
}

static int next_child_ref(TreeNode* parent) {
  if (parent == NULL) {
    return LUA_REFNIL;
  }

  if (parent->children_count == 0 || parent->children == NULL) {
    return LUA_REFNIL;
  }

  if (parent->children_iter < 0 || parent->children_iter >= parent->children_count) {
    return LUA_REFNIL;
  }

  while (parent->children[parent->children_iter] == NULL && parent->children_iter < parent->children_count) {
    parent->children_iter++;
  }

  if (parent->children_iter < 0 || parent->children_iter >= parent->children_count) {
    return LUA_REFNIL;
  }

  return parent->udreflist[parent->children_iter++];
}

int node_iter(lua_State *L) {
  TreeNode *node = (TreeNode*)lua_touserdata(L, lua_upvalueindex(1));
  int refn = next_child_ref(node);

  if (refn != LUA_REFNIL) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, refn);
    return 1;
  }

  return 0;
}

int getnodes (lua_State *L) {
  TreeNode *node = checktree(L, 1);
  node->children_iter = 0;
  /* creates and returns the iterator function
     (its sole upvalue, the directory userdatum,
     is already on the stack top */
  lua_pushcclosure(L, node_iter, 1);
  return 1;
}

int treenode2string (lua_State *L) {
  TreeNode *node = checktree(L, 1);

  switch (node->dataType) {
  case LUA_TNIL:
    lua_pushfstring(L, "treenode: name = '%s'; value = 'NIL'", node->name);
    break;
  case LUA_TNUMBER:
    lua_pushfstring(L, "treenode: name = '%s'; value = '%f'", node->name, node->data.numberVal);
    break;
  case LUA_TBOOLEAN:
    lua_pushfstring(L, "treenode: name = '%s'; value = '%d'", node->name, node->data.boolVal);
    break;
  case LUA_TSTRING:
    lua_pushfstring(L, "treenode: name = '%s'; value = '%s'", node->name, node->data.charVal);
    break;
  }

  return 1;
}

int luaopen_tree (lua_State *L) {
  luaL_newmetatable(L, "LuaBook.treenode");

  // some staff that I do not fully understand =(
  lua_pushstring(L, "__index");
  lua_pushvalue(L, -2);  /* pushes the metatable */
  lua_settable(L, -3);  /* metatable.__index = metatable */

  /* register methods */
  luaL_openlib (L, NULL, tree_meths, 0);

  /* register functions (only lxp.new) */
  luaL_openlib (L, "treenode", tree_funcs, 0);
  return 1;
}

static int printd(const char *format, ...) {
  if (!DEBUG) {
    return 0;
  }

  va_list arg;
  int done;

  va_start (arg, format);
  done = vprintf(format, arg);
  va_end (arg);

  return done;
}

static int custom_uuid_compare(uuid_t uuid1, uuid_t uuid2) {
#ifdef __MINGW32__
  RPC_STATUS status;
  return UuidCompare(&uuid1, &uuid2, &status);
#elif __linux__
  return uuid_compare(uuid1, uuid2);
#endif
}

static void uuid_to_string(uuid_t *uuid, char *uuid_str) {
#ifdef __MINGW32__
  RPC_CSTR tmp;
  UuidToString(uuid, &tmp);
  strcpy(uuid_str, tmp);
  RpcStringFree(&tmp);
#elif __linux__
  uuid_unparse_lower(*uuid, uuid_str);
#endif
}

static void custom_generate_uuid(uuid_t *uuid) {
#ifdef __MINGW32__
  RPC_STATUS retval = UuidCreateSequential(uuid);
  switch (retval) {
  case RPC_S_OK:
    printd("uuid created successfully\n");
    break;
  case RPC_S_UUID_LOCAL_ONLY:
    printd("uuid created computer unique\n");
    break;
  case RPC_S_UUID_NO_ADDRESS:
    printd("uuid not created\n");
    break;
  }
#elif __linux__
  uuid_generate(*uuid);
#endif
}

static char* assignNewString(char *str_to, const char *str_from) {
  if (strlen(str_to) < (strlen(str_from)+1)) {
    str_to = (char*)realloc(str_to, (strlen(str_from)+1)*sizeof(char));
  }

  return strcpy(str_to, str_from);
}
