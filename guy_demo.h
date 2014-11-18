#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

typedef enum {MOVE_UP, MOVE_DOWN, STOP} jump_sequence_t;

static const float view_scene_dist = 520.f;
static GLuint textureID;
static int jump_base;
static float guy_zoom;
static float guy_x;
static float guy_y;
static jump_sequence_t jump_sequence;

static const int jump_max_value = 100;

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void display();
static void reshape(GLFWwindow* window, int w, int h);
static void DrawGrid(void);
static void DrawGuy();

static void gamepad_pressed_callback(int id, int gamepad_button);
static void gamepad_axis_callback(int id, int axis_id, float axis_value);

extern int guy_demo(void);
