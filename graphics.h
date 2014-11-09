#include <stdlib.h>
#include <math.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define BOING_DEBUG 0

#define RADIUS           70.f
#define STEP_LONGITUDE   22.5f                   /* 22.5 makes 8 bands like original Boing */
#define STEP_LATITUDE    22.5f

#define DIST_BALL       (RADIUS * 2.f + RADIUS * 0.1f)

#define VIEW_SCENE_DIST (DIST_BALL * 3.f + 200.f)/* distance from viewer to middle of boing area */
#define GRID_SIZE       (RADIUS * 4.5f)          /* length (width) of grid */
#define BOUNCE_HEIGHT   (RADIUS * 2.1f)
#define BOUNCE_WIDTH    (RADIUS * 2.1f)

#define SHADOW_OFFSET_X -20.f
#define SHADOW_OFFSET_Y  10.f
#define SHADOW_OFFSET_Z   0.f

#define WALL_L_OFFSET   0.f
#define WALL_R_OFFSET   5.f

/* Animation speed (50.0 mimics the original GLUT demo speed) */
#define ANIMATION_SPEED 50.f

/* Maximum allowed delta time per physics iteration */
#define MAX_DELTA_T 0.02f

/* Draw ball, or its shadow */
typedef enum { DRAW_BALL, DRAW_BALL_SHADOW } DRAW_BALL_ENUM;

/* Vertex type */
typedef struct {float x; float y; float z;} vertex_t;

/* Global vars */
static GLfloat deg_rot_y       = 0.f;
static GLfloat deg_rot_y_inc   = 2.f;
static GLfloat ball_x          = -RADIUS;
static GLfloat ball_y          = -RADIUS;
static GLfloat ball_x_inc      = 1.f;
static GLfloat ball_y_inc      = 2.f;
static DRAW_BALL_ENUM drawBallHow;

static double  t;
static double  t_old = 0.f;
static double  dt;

/* Random number generator */
#ifndef RAND_MAX
	#define RAND_MAX 4095
#endif

/* PI */
#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif

GLFWwindow* graphics_init();
void graphics_close();
int graphics_redraw(GLFWwindow* window);
void graphics_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void g_init();
static GLfloat PerspectiveAngle(GLfloat size, GLfloat dist);
static void CrossProduct(vertex_t a, vertex_t b, vertex_t c, vertex_t *n);
static GLfloat TruncateDeg(GLfloat deg);
static void DrawGrid(void);
static void DrawBoingBallBand(GLfloat long_lo, GLfloat long_hi);
static void BounceBall(double delta_t);
static void DrawBoingBall(void);
static void reshape(GLFWwindow* window, int w, int h);

static double cos_deg(double deg);
static double sin_deg(double deg);
static double deg2rad(double deg);

extern void graphics_display();
