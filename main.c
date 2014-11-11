#include "graphics.h"
#include "luamodule.h"

#include <AL/alut.h>


int main() {

	GLFWwindow* window = graphics_init();
	lua_State* L = luamodule_init();

	if (!alutInit (NULL, NULL)) {
		ALenum error = alutGetError ();
		fprintf (stderr, "%s\n", alutGetErrorString (error));
		exit (EXIT_FAILURE);
	}

	for (;;) {
		luamodule_callLUA(L, "tellme");
		if (graphics_redraw(window))
			break;
	}

	if (!alutExit ()) {
		ALenum error = alutGetError ();
		fprintf (stderr, "%s\n", alutGetErrorString (error));
		exit (EXIT_FAILURE);
	}

	luamodule_close(L);								/* Clean up, free the Lua state var */
	graphics_close();
	exit(EXIT_SUCCESS);
}