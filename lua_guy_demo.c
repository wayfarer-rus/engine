#include "stb_image.h"
#include "graphics.h"
#include "gamepad_module.h"
#include "luamodule.h"
#include "alut_module.h"

#include "lua_guy_demo.h"

void DrawGrid(void) {
  int row, col;
  const float grid_zize = 640;
  const int rowTotal = 12;					/* must be divisible by 2 */
  const int colTotal = rowTotal;				/* must be same as rowTotal */
  const GLfloat widthLine = 2.0;				/* should be divisible by 2 */
  const GLfloat sizeCell = grid_zize/rowTotal;
  const GLfloat z_offset = -40.0;
  GLfloat xl, xr, yt, yb;

  glPushMatrix();
  glDisable(GL_CULL_FACE);

  /*
   * Another relative Z translation to separate objects.
   */
  glTranslatef(0, 0, 100);

  /*
   * Draw vertical lines (as skinny 3D rectangles).
   */
  for (col = 0; col <= colTotal; col++) {
    /*
     * Compute co-ords of line.
     */
    xl = -grid_zize / 2 + col * sizeCell;
    xr = xl + widthLine;

    yt =  grid_zize / 2;
    yb = -grid_zize / 2 - widthLine;

    glBegin(GL_POLYGON);

    glColor3f(0.6f, 0.1f, 0.6f);               /* purple */

    glVertex3f(xr, yt, z_offset);       /* NE */
    glVertex3f(xl, yt, z_offset);       /* NW */
    glVertex3f(xl, yb, z_offset);       /* SW */
    glVertex3f(xr, yb, z_offset);       /* SE */

    glEnd();
  }

  /*
   * Draw horizontal lines (as skinny 3D rectangles).
   */
  for (row = 0; row <= rowTotal; row++) {
    /*
     * Compute co-ords of line.
     */
    yt = grid_zize / 2 - row * sizeCell;
    yb = yt - widthLine;

    xl = -grid_zize / 2;
    xr =  grid_zize / 2 + widthLine;

    glBegin(GL_POLYGON);

    glColor3f(0.6f, 0.1f, 0.6f);		/* purple */

    glVertex3f(xr, yt, z_offset);		/* NE */
    glVertex3f(xl, yt, z_offset);		/* NW */
    glVertex3f(xl, yb, z_offset);		/* SW */
    glVertex3f(xr, yb, z_offset);		/* SE */

    glEnd();
  }

  glPopMatrix();

  return;
}

void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

void reshape(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(PerspectiveAngle(150, 200),
                 (GLfloat)w / (GLfloat)h,
                 1.0,
                 640);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0.0, 0.0, 640,/* eye */
            0.0, 0.0, 0.0,            /* center of vision */
            0.0, -1.0, 0.0);          /* up vector */
}

void DrawTextures() {
  int i;
  for (i = 0; i < allocatedTextures; ++i) {
    struct textureAttr texture = texturesArray[i];
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(texture.x, texture.y, texture.z);
    glRotatef(texture.rot_angle, texture.rot_x, texture.rot_y, texture.rot_z);

    glColor3f(1,1,1);
    glScalef(texture.size_x,texture.size_y,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // Draw a textured quad
    glBegin(GL_QUADS);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Низ лево

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
  }
}

void gamepad_pressed_callback(int id, int gamepad_button) {
  gamepad_button_pressed = true;
}

void gamepad_released_callback(int id, int gamepad_button) {
  gamepad_button_pressed = false;
}

void gamepad_axis_callback(int id, int axis_id, float axis_value) {
  if (axis_id == LEFT_RIGHT_AXIS)
    gamepad_axis_value = axis_value;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    keyboard_key_pressed = "ESCAPE";
  }
  else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    keyboard_key_pressed = "RIGHT";
  }
  else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    keyboard_key_pressed = "LEFT";
  }
  else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    keyboard_key_pressed = "SPACE";
  }
  else
    keyboard_key_pressed = "";
}

void init() {
  gamepad_button_pressed = false;
  gamepad_axis_value = 0.0;
  keyboard_key_pressed = "";
  allocatedTextures = 0;
  alutmodule_init();
  window = graphics_init(640, 480, "LUA Guy demo", &reshape);
  glfwSetKeyCallback(window, key_callback);

  if (gamepadmodule_checkGamepad(0)) {
    gamepadmodule_initGamepadListener(0);
    gamepadmodule_setGamepadButtonPressedCallback(&gamepad_pressed_callback);
    gamepadmodule_setGamepadButtonReleasedCallback(&gamepad_released_callback);
    gamepadmodule_setGamepadAxisCallback(&gamepad_axis_callback);
  }

  L = luamodule_init("lua_guy_demo.lua");
  luamodule_register(L, "__loadTexture", __loadTexture);
  luamodule_register(L, "__redraw", __redraw);
  luamodule_register(L, "__drawTexture", __drawTexture);
  luamodule_register(L, "__closeWindow", __closeWindow);
  luamodule_register(L, "__gamepadGetAxisValue", __gamepadGetAxisValue);
  luamodule_register(L, "__gamepadButtonPressed", __gamepadButtonPressed);
  luamodule_register(L, "__keyboardGetKeyPressed", __keyboardGetKeyPressed);
  luamodule_register(L, "__playSound", __playSound);
  luamodule_register(L, "__sleepMillis", __sleepMillis);
  luamodule_register(L, "__sleepSeconds", __sleepSeconds);
  luamodule_register(L, "__loadSoundFile", __loadSoundFile);
}

int __loadTexture(lua_State *L) {
  if (lua_gettop(L) != 1 || !lua_isstring(L, 1)) {
    lua_pushstring(L, "incorrect argument for __loadTexture");
    lua_error(L);
  }

  char* textureName = lua_tostring(L, 1);

  GLuint textureID;
  int width=512,height=512,n=32;
  unsigned char *data = stbi_load(textureName, &width, &height, &n, 0);

  // Создадим одну текстуру OpenGL
  glGenTextures(1, &textureID);

  // Сделаем созданную текстуру текущий, таким образом все следующие функции будут работать именно с этой текстурой
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Передадим изображение OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (allocatedTextures == TEXTURE_LIST_SIZE) {
    fprintf(stderr, "Can't allocate more textures");
    exit(EXIT_FAILURE);
  }

  struct textureAttr newTexture;
  newTexture.id = textureID;
  texturesArray[allocatedTextures++] = newTexture;

  lua_pushnumber(L, textureID);
  return 1;
}

void clear() {
  gamepadmodule_close();
  graphics_close();
  alutmodule_exit();
  luamodule_close(L);
}

int __drawTexture(lua_State *L) {
  if (lua_gettop(L) != 10) {
    lua_pushstring(L, "incorrect arguments for __drawTexture");
    lua_error(L);
  }

  GLuint textureID = lua_tonumber(L, 1);
  float size_x = lua_tonumber(L, 2);
  float size_y = lua_tonumber(L, 3);
  float x = lua_tonumber(L, 4);
  float y = lua_tonumber(L, 5);
  float z = lua_tonumber(L, 6);
  float angle = lua_tonumber(L, 7);
  float rot_x = lua_tonumber(L, 8);
  float rot_y = lua_tonumber(L, 9);
  float rot_z = lua_tonumber(L, 10);

  int i;

  for (i = 0; i < allocatedTextures; ++i) {
    if (texturesArray[i].id == textureID) {
      texturesArray[i].size_x = size_x;
      texturesArray[i].size_y = size_y;
      texturesArray[i].x = x;
      texturesArray[i].y = y;
      texturesArray[i].z = z;
      texturesArray[i].rot_angle = angle;
      texturesArray[i].rot_x = rot_x;
      texturesArray[i].rot_y = rot_y;
      texturesArray[i].rot_z = rot_z;
      break;
    }
  }

  return 0;
}

int __redraw(lua_State *L) {
  bool res = true;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  DrawGrid();
  DrawTextures();

  glPopMatrix();
  glFlush();

  if (graphics_redraw(window))
    res = false;

  lua_pushboolean(L,res);
  return 1;
}

int __closeWindow(lua_State *L) {
  glfwSetWindowShouldClose(window, GL_TRUE);
  return 0;
}

int __keyboardGetKeyPressed(lua_State *L) {
  lua_pushstring(L, keyboard_key_pressed);
  return 1;
}
int __gamepadButtonPressed(lua_State *L) {
  lua_pushboolean(L,gamepad_button_pressed);
  return 1;
}

int __gamepadGetAxisValue(lua_State *L) {
  lua_pushnumber(L,gamepad_axis_value);
  return 1;
}

int __loadSoundFile(lua_State *L) {
  if (lua_gettop(L) != 1) {
    lua_pushstring(L, "incorrect arguments for __loadSoundFile");
    lua_error(L);
  }

  char *fileName = lua_tostring(L,1);
  int id = alutmodule_loadFile(fileName);
  lua_pushnumber(L,id);
  return 1;
}

int __playSound(lua_State *L) {
  if (lua_gettop(L) != 2) {
    lua_pushstring(L, "incorrect arguments for __playSound");
    lua_error(L);
  }

  int fileId = (int)lua_tonumber(L, 1);
  float volume = lua_tonumber(L, 2);
  alutmodule_playFile(fileId, volume, false);
  return 0;
}

int __sleepMillis(lua_State *L) {
  if (lua_gettop(L) != 1) {
    lua_pushstring(L, "incorrect arguments for __sleepMillis");
    lua_error(L);
  }

  long m = (long)lua_tonumber(L, 1);
  struct timespec sleepTime;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = m*1000;
  nanosleep(&sleepTime, NULL);
  return 0;
}

int __sleepSeconds(lua_State *L) {
  if (lua_gettop(L) != 1) {
    lua_pushstring(L, "incorrect arguments for __sleepMillis");
    lua_error(L);
  }

  long s = (long)lua_tonumber(L, 1);
  struct timespec sleepTime;
  sleepTime.tv_sec = s;
  sleepTime.tv_nsec = 0;
  nanosleep(&sleepTime, NULL);
  return 0;
}

int lua_guy_demo(void)
{
  init();
  luamodule_callLUA(L, "mainLoop");
  clear();
  exit(EXIT_SUCCESS);
}
