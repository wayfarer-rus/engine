#include "stb_image.h"
#include "graphics.h"
#include "gamepad_module.h"
#include "alut_module.h"
#include "crossplatform_utils.h"

#include "engine_core.h"

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
  for (i = 0; i < luaTextures; ++i) {
    struct textureAttr texture = texturesAttr[i];
    if (texture.drawIt) {
      glPushMatrix();
      glMatrixMode(GL_MODELVIEW);

      glTranslatef(-texture.x, -texture.y, texture.z);
      glRotatef(texture.rot_angle, texture.rot_x, texture.rot_y, texture.rot_z);

      glColor3f(1,1,1);
      glScalef(texture.size_x,texture.size_y,0);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texturesArray[texture.textureId]);

      // Draw a textured quad
      glBegin(GL_QUADS);

      glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0);	// Низ право
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, 0);	// Верх право
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, 0);	// Верх лево
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, 0);	// Низ лево

      /*
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);	// Низ право
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);	// Верх право
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);	// Верх лево
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);	// Низ лево
      */
      glEnd();

      glDisable(GL_TEXTURE_2D);
      glPopMatrix();
    }
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
  luaTextures = 0;
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
  luamodule_register(L, "__copyTexture", __copyTexture);
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
  luamodule_register(L, "__applyPhysics", __applyPhysics);
  luamodule_register(L, "__getPosition", __getPosition);
  luamodule_register(L, "__applyImpulse", __applyImpulse);
  luamodule_register(L, "__setPosition", __setPosition);
  luamodule_register(L, "__drawTextureWithPhysics", __drawTextureWithPhysics);
  luamodule_register(L, "__getBodysVelocity", __getBodysVelocity);
  // Allocate textures container
  glGenTextures(TEXTURE_LIST_SIZE, texturesArray);
}

int __loadTexture(lua_State *L) {
  if (lua_gettop(L) != 1 || !lua_isstring(L, 1)) {
    lua_pushstring(L, "incorrect argument for __loadTexture");
    lua_error(L);
  }

  if (allocatedTextures == TEXTURE_LIST_SIZE) {
    lua_pushstring(L, "Can't allocate more textures");
    lua_error(L);
  }

  char* textureName = lua_tostring(L, 1);
  GLuint textureID = texturesArray[allocatedTextures];
  int width, height, n;
  unsigned char *data = stbi_load(textureName, &width, &height, &n, 0);

  // Сделаем созданную текстуру текущий, таким образом все следующие функции будут работать именно с этой текстурой
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Передадим изображение OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  struct textureAttr newTexture;
  newTexture.textureId = allocatedTextures++;
  texturesAttr[luaTextures] = newTexture;

  lua_pushnumber(L, luaTextures++);
  return 1;
}

int __copyTexture(lua_State *L) {
  if (lua_gettop(L) != 1 || !lua_isnumber(L, 1)) {
    lua_pushstring(L, "incorrect argument for __copyTexture");
    lua_error(L);
  }

  int luaTextureId = lua_tonumber(L, 1);

  if (luaTextureId < 0 || luaTextureId >= luaTextures) {
    lua_pushstring(L, "incorrect argument for __copyTexture: no such texture");
    lua_error(L);
  }

  struct textureAttr newLuaTexture  = texturesAttr[luaTextureId];
  texturesAttr[luaTextures] = newLuaTexture;
  lua_pushnumber(L, luaTextures++);
  return 1;
}

void clear() {
  glDeleteTextures(TEXTURE_LIST_SIZE, texturesArray);
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

  int luaTextureID = lua_tonumber(L, 1);
  float size_x = lua_tonumber(L, 2);
  float size_y = lua_tonumber(L, 3);
  float x = lua_tonumber(L, 4);
  float y = lua_tonumber(L, 5);
  float z = lua_tonumber(L, 6);
  float angle = lua_tonumber(L, 7);
  float rot_x = lua_tonumber(L, 8);
  float rot_y = lua_tonumber(L, 9);
  float rot_z = lua_tonumber(L, 10);

  texturesAttr[luaTextureID].size_x = size_x;
  texturesAttr[luaTextureID].size_y = size_y;
  texturesAttr[luaTextureID].x = x;
  texturesAttr[luaTextureID].y = y;
  texturesAttr[luaTextureID].z = z;
  texturesAttr[luaTextureID].rot_angle = angle;
  texturesAttr[luaTextureID].rot_x = rot_x;
  texturesAttr[luaTextureID].rot_y = rot_y;
  texturesAttr[luaTextureID].rot_z = rot_z;
  texturesAttr[luaTextureID].rot_z = rot_z;
  texturesAttr[luaTextureID].drawIt = true;

  return 0;
}

int __redraw(lua_State *L) {
  bool res = true;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  DrawGrid();
  DrawTextures();

  int i;
  for (i = 0; i < luaTextures; ++i) {
    texturesAttr[i].drawIt = false;
  }

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
  sleepMillis(m);
  cpFloat timeStep = ((cpFloat)m)/1000.0;
  cpSpaceStep(space, timeStep);
  return 0;
}

int __sleepSeconds(lua_State *L) {
  if (lua_gettop(L) != 1) {
    lua_pushstring(L, "incorrect arguments for __sleepMillis");
    lua_error(L);
  }

  long s = (long)lua_tonumber(L, 1);
  sleepSeconds(s);
  cpFloat timeStep = (cpFloat)s;
  cpSpaceStep(space, timeStep);
  return 0;
}

int engine_core(void)
{
  init();
  // init Chipmunk2D
  cpVect gravity = cpv(0, -2000);
  space = cpSpaceNew();
  cpBody *staticBody = space->staticBody;
  cpSpaceSetGravity(space, gravity);
  // Add borders around the screen
  cpShape *wall = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-320,-240), cpv(-320,240), 0.0f));
  cpShape *wall2 = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(320,-240), cpv(320,240), 0.0f));
  cpShape *ground = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-320, -240), cpv(320, -240), 0));
  cpSpaceAddCollisionHandler(space, STATIC_BODY_TYPE, DYNAMIC_BODY_TYPE, begin, NULL, NULL, separate, NULL);

  luamodule_callLUA(L, "mainLoop");
  clear();

  // Clean up physics objects
  int i;
  for (i = 0; i < TEXTURE_LIST_SIZE; ++i) {
    if (texturesAttr[i].physics != NULL) {
      if (texturesAttr[i].physics->shape != NULL)
        cpShapeFree(texturesAttr[i].physics->shape);

      if (texturesAttr[i].physics->body != NULL)
        cpBodyFree(texturesAttr[i].physics->body);

      free(texturesAttr[i].physics);
    }
  }
  cpShapeFree(ground);
  cpShapeFree(wall);
  cpShapeFree(wall2);
  cpSpaceFree(space);

  pthread_exit(NULL);
}

int begin(cpArbiter *arb, cpSpace *space, void *unused) {
  // Get the cpShapes involved in the collision
  // The order will be the same as you defined in the handler definition
  // a->collision_type will be BULLET_TYPE and b->collision_type will be MONSTER_TYPE
  //cpShape *a, *b; cpArbiterGetShapes(arb, &a, &b);

  // Alternatively you can use the CP_ARBITER_GET_SHAPES() macro
  // It defines and sets the variables for you.
  CP_ARBITER_GET_SHAPES(arb, a, b);
  // Cycle through texturesAttr to find the dynamic bodies that collyding
  int dBodyInd;
  int sBodyInd;
  int i;

  if (a->collision_type == STATIC_BODY_TYPE && b->collision_type == DYNAMIC_BODY_TYPE) {
    for (i = 0; i < luaTextures; ++i) {
      if (texturesAttr[i].physics) {
        if (texturesAttr[i].physics->shape == b) {
          dBodyInd = i;
        }
        else if (texturesAttr[i].physics->shape == a) {
          sBodyInd = i;
        }
      }
    }
  }
  else if (b->collision_type == STATIC_BODY_TYPE && a->collision_type == DYNAMIC_BODY_TYPE) {
    for (i = 0; i < luaTextures; ++i) {
      if (texturesAttr[i].physics) {
        if (texturesAttr[i].physics->shape == a) {
          dBodyInd = i;
        }
        else if (texturesAttr[i].physics->shape == b) {
          sBodyInd = i;
        }
      }
    }
  }

  texturesAttr[dBodyInd].physics->inTouchWith = sBodyInd;
  texturesAttr[sBodyInd].physics->inTouchWith = dBodyInd;
  // Add a post step callback to safely remove the body and shape from the space.
  // Calling cpSpaceRemove*() directly from a collision handler callback can cause crashes.
  //cpSpaceAddPostStepCallback(space, (cpPostStepFunc)postStepRemove, b, NULL);

  return 1;
}

void separate(cpArbiter *arb, cpSpace *space, void *data) {
  CP_ARBITER_GET_SHAPES(arb, a, b);
  // Cycle through texturesAttr to find the dynamic bodies that collyding
  int dBodyInd;
  int sBodyInd;
  int i;

  if (a->collision_type == STATIC_BODY_TYPE && b->collision_type == DYNAMIC_BODY_TYPE) {
    for (i = 0; i < luaTextures; ++i) {
      if (texturesAttr[i].physics) {
        if (texturesAttr[i].physics->shape == b) {
          dBodyInd = i;
        }
        else if (texturesAttr[i].physics->shape == a) {
          sBodyInd = i;
        }
      }
    }
  }
  else if (b->collision_type == STATIC_BODY_TYPE && a->collision_type == DYNAMIC_BODY_TYPE) {
    for (i = 0; i < luaTextures; ++i) {
      if (texturesAttr[i].physics) {
        if (texturesAttr[i].physics->shape == a) {
          dBodyInd = i;
        }
        else if (texturesAttr[i].physics->shape == b) {
          sBodyInd = i;
        }
      }
    }
  }

  texturesAttr[dBodyInd].physics->inTouchWith = -1;
  texturesAttr[sBodyInd].physics->inTouchWith = -1;
}

int __getBodysVelocity(lua_State *L) {
  if (lua_gettop(L) != 1) {
    lua_pushstring(L, "incorrect arguments for __getBodysVelocity");
    lua_error(L);
  }

  int textureId = (int)lua_tonumber(L,1);

  if (texturesAttr[textureId].physics != NULL) {
    cpVect vel = cpBodyGetVel(texturesAttr[textureId].physics->body);
    lua_pushnumber(L,vel.x);
    lua_pushnumber(L,vel.y);
  }
  else {
    lua_pushstring(L, "error in __setPosition: no physics applyed to this texture");
    lua_error(L);
  }

  return 2;
}

int __applyPhysics(lua_State *L) {
  if (lua_gettop(L) != 6) {
    lua_pushstring(L, "incorrect arguments for __applyPhysics");
    lua_error(L);
  }

  int textureId = (int)lua_tonumber(L,1);
  cpFloat mass = (cpFloat)lua_tonumber(L,2);
  cpFloat friction = (cpFloat)lua_tonumber(L,3);
  cpFloat width = (cpFloat)lua_tonumber(L,4);
  cpFloat height = (cpFloat)lua_tonumber(L,5);
  char *bodyType = lua_tostring(L,6);
  cpFloat moment = INFINITY;

  struct physicsAttr *physics = (struct physicsAttr*)malloc(sizeof(struct physicsAttr));

  if (bodyType != NULL && strcmp(bodyType, "dynamic") == 0) {
    physics->bodyType = DYNAMIC_BODY_TYPE;
    physics->body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    physics->shape = cpSpaceAddShape(space, cpBoxShapeNew(physics->body, width, height));
    physics->shape->collision_type = DYNAMIC_BODY_TYPE;
  }
  else if (bodyType != NULL && strcmp(bodyType, "static") == 0) {
    physics->bodyType = STATIC_BODY_TYPE;
    physics->body = cpBodyNew(INFINITY, INFINITY);
    physics->shape = cpSpaceAddShape(space, cpBoxShapeNew(physics->body, width, height));
    physics->shape->collision_type = STATIC_BODY_TYPE;
  }
  else {
    lua_pushstring(L, "incorrect arguments for __applyPhysics: bodyType should be static or dynamic");
    lua_error(L);
  }

  cpShapeSetFriction(physics->shape, friction);
  texturesAttr[textureId].physics = physics;
  return 0;
}

int __setPosition(lua_State *L) {
  if (lua_gettop(L) != 3) {
    lua_pushstring(L, "incorrect arguments for __setPosition");
    lua_error(L);
  }

  int textureId = (int)lua_tonumber(L,1);
  cpVect pos = cpv(lua_tonumber(L,2), lua_tonumber(L,3));

  if (texturesAttr[textureId].physics != NULL) {
    cpBodySetPos(texturesAttr[textureId].physics->body, pos);
    cpSpaceReindexShapesForBody(space, texturesAttr[textureId].physics->body);
  }
  else {
    lua_pushstring(L, "error in __setPosition: no physics applyed to this texture");
    lua_error(L);
  }

  return 0;
}

int __getPosition(lua_State *L) {
  if (lua_gettop(L) != 1) {
    lua_pushstring(L, "incorrect arguments for __getPosition");
    lua_error(L);
  }

  int textureId = (int)lua_tonumber(L,1);
  cpVect pos;

  if (texturesAttr[textureId].physics != NULL) {
    pos = cpBodyGetPos(texturesAttr[textureId].physics->body);
  }
  else {
    lua_pushstring(L, "error in __getPosition: no physics applyed to this texture");
    lua_error(L);
  }

  lua_pushnumber(L,pos.x);
  lua_pushnumber(L,pos.y);
  return 2;
}

int __applyImpulse(lua_State *L) {
  if (lua_gettop(L) != 3) {
    lua_pushstring(L, "incorrect arguments for __applyImpulse");
    lua_error(L);
  }

  int textureId = (int)lua_tonumber(L,1);
  cpVect impulse = cpv(lua_tonumber(L,2), lua_tonumber(L,3));

  if (texturesAttr[textureId].physics != NULL) {
    cpBodyApplyImpulse(texturesAttr[textureId].physics->body,impulse, cpv(0,0));
  }
  else {
    lua_pushstring(L, "error in __getPosition: no physics applyed to this texture");
    lua_error(L);
  }

  return 0;
}

int __drawTextureWithPhysics(lua_State *L) {
  if (lua_gettop(L) != 7) {
    lua_pushstring(L, "incorrect arguments for __drawTextureWithPhysics");
    lua_error(L);
  }

  int textureId = lua_tonumber(L, 1);
  float size_x = lua_tonumber(L, 2);
  float size_y = lua_tonumber(L, 3);
  float angle = lua_tonumber(L, 4);
  float rot_x = lua_tonumber(L, 5);
  float rot_y = lua_tonumber(L, 6);
  float rot_z = lua_tonumber(L, 7);
  cpVect pos;

  if (texturesAttr[textureId].physics != NULL) {
    pos = cpBodyGetPos(texturesAttr[textureId].physics->body);
  }
  else {
    lua_pushstring(L, "error in __drawTextureWithPhysics: no physics applyed to this texture");
    lua_error(L);
  }

  float x = pos.x;
  float y = pos.y;
  float z = 0;

  texturesAttr[textureId].size_x = size_x;
  texturesAttr[textureId].size_y = size_y;
  texturesAttr[textureId].x = x;
  texturesAttr[textureId].y = y;
  texturesAttr[textureId].z = z;
  texturesAttr[textureId].rot_angle = angle;
  texturesAttr[textureId].rot_x = rot_x;
  texturesAttr[textureId].rot_y = rot_y;
  texturesAttr[textureId].rot_z = rot_z;
  texturesAttr[textureId].rot_z = rot_z;
  texturesAttr[textureId].drawIt = true;

  return 0;
}
