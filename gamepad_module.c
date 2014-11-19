#include "gamepad_module.h"

bool gamepadmodule_checkGamepad(const int id) {
  if (glfwJoystickPresent(id)) {
    printf("Gamepad exits\n");
    printf("Gamepad's name: %s\n", glfwGetJoystickName(id));
    return true;
  }

  return false;
}

void gamepadmodule_initGamepadListener(const int id) {
  if (!glfwJoystickPresent(id)) {
    printf("Gamepad can't be initialized. Sorry.\n");
    return;
  }

  listenGamepad = true;
  gamepadButtonPressedCallbackFunction = NULL;
  gamepadButtonReleasedCallbackFunction = NULL;
  gamepadAxisCallbackFunction = NULL;
  int _id = id;

  if(pthread_create(&gamepadButtonListener, NULL, glfw_GamepadListener, &_id)) {
    fprintf(stderr, "Error creating thread\n");
    exit(EXIT_FAILURE);
  }
}

void *glfw_GamepadListener(void* gamepad_id_ptr) {
  int *id = (int *)gamepad_id_ptr;
  const unsigned char* buttonsArray;
  int buttonsArraySize;
  const float* axisArray;
  int axisArraySize;
  int i;
  struct timespec remainingSleepTime;
  struct timespec sleepTime;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = 50*1000;

  while (listenGamepad) {
    buttonsArray = glfwGetJoystickButtons(*id, &buttonsArraySize);
    axisArray = glfwGetJoystickAxes(*id, &axisArraySize);

    for (i = 0; i < buttonsArraySize; ++i) {
      //printf("Gamepad button status: %d = %d\n", i, buttonsArray[i]);
      if (buttonsArray[i] != 0 && gamepadButtonsPressedArray[i] == 0) {
        if (gamepadButtonPressedCallbackFunction != NULL) {
          (*gamepadButtonPressedCallbackFunction)(*id, i);
        }

        gamepadButtonsPressedArray[i] = 1;
      } else if (buttonsArray[i] == 0 && gamepadButtonsPressedArray[i] != 0) {
        if (gamepadButtonReleasedCallbackFunction != NULL) {
          (*gamepadButtonReleasedCallbackFunction)(*id, i);
        }

        gamepadButtonsPressedArray[i] = 0;
      }
    }


    for (i = 0; i < axisArraySize; ++i) {
      if (gamepadAxisCallbackFunction != NULL) {
        (*gamepadAxisCallbackFunction)(*id, i, axisArray[i]);
      }
    }

    nanosleep(&sleepTime, &remainingSleepTime);
  }
}

void gamepadmodule_close() {
  listenGamepad = false;
}

void gamepadmodule_setGamepadButtonPressedCallback(void (*function)(int,int)) {
  gamepadButtonPressedCallbackFunction = function;
}

void gamepadmodule_setGamepadButtonReleasedCallback(void (*function)(int,int)) {
  gamepadButtonReleasedCallbackFunction = function;
}

void gamepadmodule_setGamepadAxisCallback(void (*function)(int,int,float)) {
  gamepadAxisCallbackFunction = function;
}
