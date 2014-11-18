#include <stdlib.h>
#include <math.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

/* Random number generator */
#ifndef RAND_MAX
	#define RAND_MAX 4095
#endif

/* PI */
#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif

/* Vertex type */
typedef struct {float x; float y; float z;} vertex_t;

GLFWwindow* graphics_init(const int w, const int h, const char* title,  void (*reshape)(GLFWwindow*,int,int));
void graphics_close();
int graphics_redraw(GLFWwindow* window);
void graphics_setKeyCallback(GLFWwindow* window, GLFWkeyfun function);
void graphics_setFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun function);

GLfloat PerspectiveAngle(GLfloat size, GLfloat dist);
void CrossProduct(vertex_t a, vertex_t b, vertex_t c, vertex_t *n);
GLfloat TruncateDeg(GLfloat deg);

double cos_deg(double deg);
double sin_deg(double deg);
double deg2rad(double deg);

static void g_init();

