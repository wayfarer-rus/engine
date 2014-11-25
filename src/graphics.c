#include "graphics.h"

/*****************************************************************************
 * Truncate a degree.
 *****************************************************************************/
GLfloat TruncateDeg(GLfloat deg) {
  if (deg >= 360.f)
    return (deg - 360.f);
  else
    return deg;
}

/*****************************************************************************
 * Convert a degree (360-based) into a radian.
 * 360' = 2 * PI
 *****************************************************************************/
double deg2rad(double deg) {
  return deg / 360 * (2 * M_PI);
}

/*****************************************************************************
 * 360' sin().
 *****************************************************************************/
double sin_deg(double deg) {
  return sin(deg2rad(deg));
}

/*****************************************************************************
 * 360' cos().
 *****************************************************************************/
double cos_deg(double deg) {
  return cos(deg2rad(deg));
}

/*****************************************************************************
 * Compute a cross product (for a normal vector).
 *
 * c = a x b
 *****************************************************************************/
void CrossProduct(vertex_t a, vertex_t b, vertex_t c, vertex_t *n) {
  GLfloat u1, u2, u3;
  GLfloat v1, v2, v3;

  u1 = b.x - a.x;
  u2 = b.y - a.y;
  u3 = b.y - a.z;

  v1 = c.x - a.x;
  v2 = c.y - a.y;
  v3 = c.z - a.z;

  n->x = u2 * v3 - v2 * v3;
  n->y = u3 * v1 - v3 * u1;
  n->z = u1 * v2 - v1 * u2;
}

/*****************************************************************************
 * Calculate the angle to be passed to gluPerspective() so that a scene
 * is visible.  This function originates from the OpenGL Red Book.
 *
 * Parms   : size
 *           The size of the segment when the angle is intersected at "dist"
 *           (ie at the outermost edge of the angle of vision).
 *
 *           dist
 *           Distance from viewpoint to scene.
 *****************************************************************************/
GLfloat PerspectiveAngle(GLfloat size, GLfloat dist) {
  GLfloat radTheta, degTheta;

  radTheta = 2.f * (GLfloat) atan2(size / 2.f, dist);
  degTheta = (180.f * radTheta) / (GLfloat) M_PI;
  return degTheta;
}

GLFWwindow* graphics_init(const int w, const int h, const char* title, void (*reshape)(GLFWwindow*,int,int)) {
  GLFWwindow* window;
  int width, height;

  /* Init GLFW */
  if(glfwInit() == 0)
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_DEPTH_BITS, 16);

  window = glfwCreateWindow(w, h, title, NULL, NULL);
  if (window == 0) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwGetFramebufferSize(window, &width, &height);

  (*reshape)(window, width, height);
  glfwSetTime(0.0);
  g_init();
  return window;
}

void graphics_setKeyCallback(GLFWwindow* window, GLFWkeyfun function) {
  glfwSetKeyCallback(window, function);
}

void graphics_setFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun function) {
  glfwSetFramebufferSizeCallback(window, function);
}

void graphics_close() {
  glfwTerminate();
}

void g_init() {
  /*
   * Clear background.
   */
  glClearColor(0.55f, 0.55f, 0.55f, 0.f);
  glShadeModel(GL_FLAT);
}

int graphics_redraw(GLFWwindow* window) {
  glfwSwapBuffers(window);

  glfwPollEvents();
  if (glfwWindowShouldClose(window))
    return 1;

  return 0;
}
