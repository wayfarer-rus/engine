#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

pthread_mutex_t lua_mutex;

#define TEXTURE_LIST_SIZE 101
static int allocatedTextures;

struct textureAttr {
  GLuint id;
  float size_x;
  float size_y;
  float x;
  float y;
  float z;
  float rot_angle;
  float rot_x;
  float rot_y;
  float rot_z;
};

static struct textureAttr texturesArray[TEXTURE_LIST_SIZE];
static GLFWwindow* window;
static lua_State* L;

static bool gamepad_button_pressed;
static float gamepad_axis_value;
static char* keyboard_key_pressed;

static void DrawGrid(void);
static void DrawTextures(void);
static void reshape(GLFWwindow* window, int w, int h);
static void gamepad_pressed_callback(int id, int gamepad_button);
static void gamepad_released_callback(int id, int gamepad_button);
static void gamepad_axis_callback(int id, int axis_id, float axis_value);
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int __closeWindow(lua_State *L);
int __redraw(lua_State *L);
int __drawTexture(lua_State *L);
int __loadTexture(lua_State *L);
int __keyboardGetKeyPressed(lua_State *L);
int __gamepadButtonPressed(lua_State *L);
int __gamepadGetAxisValue(lua_State *L);
int __loadSoundFile(lua_State *L);
int __playSound(lua_State *L);
int __sleepMillis(lua_State *L);
int __sleepSeconds(lua_State *L);

extern int lua_guy_demo(void);
