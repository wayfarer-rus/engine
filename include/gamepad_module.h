#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <GLFW/glfw3.h>

#define LEFT_RIGHT_AXIS 4
#define UP_DOWN_AXIS 5

static pthread_t gamepadButtonListener;
static bool listenGamepad;
static void (*gamepadButtonPressedCallbackFunction)(int,int);
static void (*gamepadButtonReleasedCallbackFunction)(int,int);
static void (*gamepadAxisCallbackFunction)(int,int,float);
static int gamepadButtonsPressedArray[GLFW_JOYSTICK_LAST];

bool gamepadmodule_checkGamepad(const int id);
void gamepadmodule_initGamepadListener(const int id);
void gamepadmodule_setGamepadButtonPressedCallback(void (*function)(int,int));
void gamepadmodule_setGamepadButtonReleasedCallback(void (*function)(int,int));
void gamepadmodule_setGamepadAxisCallback(void (*function)(int,int,float));
void gamepadmodule_close();

static void *glfw_GamepadListener(void* gamepad_id_ptr);
