#include "stb_image.h"
#include "graphics.h"
#include "gamepad_module.h"
#include "guy_demo.h"

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

void DrawGuy() {
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);

  /* Set guy position */
  glTranslatef(-guy_x, -guy_y, 0.0);

  glColor3f(1,1,1);
  glScalef(guy_zoom/2,guy_zoom,0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID);

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

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  DrawGrid();
  DrawGuy();

  glPopMatrix();
  glFlush();
}

void gamepad_pressed_callback(int id, int gamepad_button) {
  if (jump_sequence == STOP)
    jump_sequence = MOVE_UP;
}

void gamepad_axis_callback(int id, int axis_id, float axis_value) {
  if (axis_id == LEFT_RIGHT_AXIS) {
    guy_x += 0.05*axis_value;
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  cpVect pos = cpBodyGetPos(guyBody);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    cpBodyApplyImpulse(guyBody,cpv(1,0), cpv(0,0));
  //    cpBodySetPos(guyBody, cpv(pos.x+15/100.0, pos.y));
  else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    cpBodyApplyImpulse(guyBody,cpv(-1,0), cpv(0,0));
  //    cpBodySetPos(guyBody, cpv(pos.x-15/100.0, pos.y));
  else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    if (jump_sequence == STOP)
      jump_sequence = MOVE_UP;
  }
}

int guy_demo(void)
{
  int width,height,n;
  unsigned char *data = stbi_load("resources/textures/some_guy_2.png", &width, &height, &n, 0);
  // stbi_image_free(data);

  GLFWwindow* window = graphics_init(640, 480, "Guy demo", &reshape);
  glfwSetKeyCallback(window, key_callback);

  if (gamepadmodule_checkGamepad(0)) {
    gamepadmodule_initGamepadListener(0);
    gamepadmodule_setGamepadButtonPressedCallback(&gamepad_pressed_callback);
    gamepadmodule_setGamepadAxisCallback(&gamepad_axis_callback);
  }

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

  // init Chipmunk2D
  cpVect gravity = cpv(0, -5);
  cpSpace *space = cpSpaceNew();
  cpBody *staticBody = space->staticBody;
  cpSpaceSetGravity(space, gravity);
  cpShape *wall = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(-3.2,-2.4), cpv(-3.2,2.4), 0.0f));
  wall = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(3.2,-2.4), cpv(3.2,2.4), 0.0f));
  cpShape *ground = cpSegmentShapeNew(staticBody, cpv(-320.0/100.0, -1.2), cpv(3.2, -1.2), 0);
  ground->collision_type = FLOR_TYPE;
  cpShapeSetFriction(ground, 1);
  cpSpaceAddShape(space, ground);
  cpFloat w = 70.0/2.0/100.0, h=70.0/100.0;
  cpFloat mass = 1;
  cpFloat moment = INFINITY;//cpMomentForBox(1,w,h);
  guyBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
  cpBodySetPos(guyBody, cpv(0, 5));
  cpShape *guyShape = cpSpaceAddShape(space, cpBoxShapeNew(guyBody, w, h));
  guyShape->collision_type = GUY_TYPE;
  cpShapeSetFriction(guyShape, 2);
  cpFloat timeStep = 1.0/60.0; // delay in seconds
  struct timespec sleepTime;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = (long)(timeStep*1000.0*1000.0); //delay in nanoseconds

  jump_base = 0;
  jump_sequence = STOP;
  guy_zoom = 70;

  cpSpaceAddCollisionHandler(space, GUY_TYPE, FLOR_TYPE, begin, NULL, NULL, NULL, NULL);

  while (1)	{
    guy_x = cpBodyGetPos(guyBody).x*100.0;
    guy_y = cpBodyGetPos(guyBody).y*100.0;
    //printf("Guy's position (%.2f; %.2f)\n", guy_x, guy_y);

    if (jump_sequence == MOVE_UP) {
      cpBodyApplyImpulse(guyBody,cpv(0,4.5), cpv(0,0));
      jump_sequence = MOVE_DOWN;
    }

    display();
    if (graphics_redraw(window))
      break;

    cpSpaceStep(space, timeStep);
    nanosleep(&sleepTime, NULL);
  }

  // Clean up our objects and exit!
  cpShapeFree(guyShape);
  cpBodyFree(guyBody);
  cpShapeFree(ground);
  cpSpaceFree(space);

  gamepadmodule_close();
  graphics_close();
  exit(EXIT_SUCCESS);
}

int begin(cpArbiter *arb, cpSpace *space, void *unused)
{
  // Get the cpShapes involved in the collision
  // The order will be the same as you defined in the handler definition
  // a->collision_type will be BULLET_TYPE and b->collision_type will be MONSTER_TYPE
  cpShape *a, *b; cpArbiterGetShapes(arb, &a, &b);

  // Alternatively you can use the CP_ARBITER_GET_SHAPES() macro
  // It defines and sets the variables for you.
  //CP_ARBITER_GET_SHAPES(arb, a, b);

  // Add a post step callback to safely remove the body and shape from the space.
  // Calling cpSpaceRemove*() directly from a collision handler callback can cause crashes.
  cpSpaceAddPostStepCallback(space, (cpPostStepFunc)postStepRemove, b, NULL);

  // The object is dead, don’t process the collision further
  return 1;
}

void postStepRemove(cpSpace *space, cpShape *shape, void *unused)
{
  jump_sequence = STOP;
}
