#include "graphics.h"
#include "luamodule.h"

int main(void) {
  GLFWwindow* window = graphics_init();
  lua_State* L = luamodule_init();

  for (;;) {
    luamodule_callLUA(L, "tellme");
    if (graphics_redraw(window))
      break;
  }

  luamodule_close(L);								/* Clean up, free the Lua state var */
  graphics_close();
  exit(EXIT_SUCCESS);
}
