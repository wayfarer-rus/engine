#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <chipmunk.h>
#include <string.h>

#include "lua_module.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define STATIC_BODY_TYPE 0
#define DYNAMIC_BODY_TYPE 1

struct physicsAttr {
  int bodyType;
  cpBody *body;
  cpShape *shape;
  int inTouchWith;  // Defines id in texturesAttr with who it is in touch
};

#define TEXTURE_LIST_SIZE 101
static int allocatedTextures;
static int luaTextures;

struct textureAttr {
  bool drawIt;
  int   textureId;
  float size_x;
  float size_y;
  float x;
  float y;
  float z;
  float rot_angle;
  float rot_x;
  float rot_y;
  float rot_z;
  struct physicsAttr *physics;
};

static GLuint texturesArray[TEXTURE_LIST_SIZE];
static struct textureAttr texturesAttr[TEXTURE_LIST_SIZE];
static GLFWwindow* window;
static lua_State* L;

static bool collisionDetected;
static bool gamepad_button_pressed;
static float gamepad_axis_value;
static char* keyboard_key_pressed;

// Chipmunk
static cpSpace *space;

static void DrawGrid(void);
static void DrawTextures(void);
static void reshape(GLFWwindow* window, int w, int h);
static void gamepad_pressed_callback(int id, int gamepad_button);
static void gamepad_released_callback(int id, int gamepad_button);
static void gamepad_axis_callback(int id, int axis_id, float axis_value);
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static int begin(cpArbiter *arb, cpSpace *space, void *unused);
static void separate(cpArbiter *arb, cpSpace *space, void *data);
//static void postStepCollision(cpSpace *space, cpShape *shape, void *unused);

int __closeWindow(lua_State *L);
int __redraw(lua_State *L);
int __drawTexture(lua_State *L);
int __loadTexture(lua_State *L);
int __copyTexture(lua_State *L);
int __keyboardGetKeyPressed(lua_State *L);
int __gamepadButtonPressed(lua_State *L);
int __gamepadGetAxisValue(lua_State *L);
int __loadSoundFile(lua_State *L);
int __playSound(lua_State *L);
int __sleepMillis(lua_State *L);
int __sleepSeconds(lua_State *L);

int __applyPhysics(lua_State *L);
int __getPosition(lua_State *L);
int __setPosition(lua_State *L);
int __applyImpulse(lua_State *L);
int __drawTextureWithPhysics(lua_State *L);
int __getBodysVelocity(lua_State *L);

extern int engine_core(void);
