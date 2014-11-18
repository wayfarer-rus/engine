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

void DrawGuy() {
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);

  // printf("%i %f\n", jump_base, delta);
  glTranslatef(0, 100, 0);

  /* Set guy position */
  glTranslatef(-guy_x, -guy_y, 0.0);

  glColor3f(1,1,1);
  glScalef(guy_zoom,guy_zoom,0);
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
    if (axis_value < 0) {
      guy_x -= 0.05*axis_value;
    }
    else if (axis_value > 0) {
      guy_x += 0.05*axis_value;
    }
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, GL_TRUE);
  else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    guy_x += 5;
  else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    guy_x -= 5;
  else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    if (jump_sequence == STOP)
      jump_sequence = MOVE_UP;
  }
}

int guy_demo(void)
{
  int width=512,height=512,n=32;
  unsigned char *data = stbi_load("resources/some_guy.png", &width, &height, &n, 0);
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

  jump_base = 0;
  jump_sequence = STOP;
  guy_zoom = 50;
  guy_x = 0;
  guy_y = 0;

  while (1)	{
    if (jump_sequence == MOVE_UP) {
      guy_y += 3.0f;
    }
    else if (jump_sequence == MOVE_DOWN) {
      guy_y -= 3.0f;
    }

    if (guy_y <= 0.0) {
      jump_sequence = STOP;
      guy_y = 0.0;
    }
    else if (guy_y >= jump_max_value && jump_sequence != STOP) {
      jump_sequence = MOVE_DOWN;
    }

    display();
    if (graphics_redraw(window))
      break;
  }

  gamepadmodule_close();
  graphics_close();
  exit(EXIT_SUCCESS);
}
