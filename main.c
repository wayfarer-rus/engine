#include "graphics.h"
#include "luamodule.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <AL/alut.h>

void playFile (const char *fileName) {
	ALuint buffer;
	ALuint source;
	ALenum error;
	ALint status;

	/* Create an AL buffer from the given sound file. */
	buffer = alutCreateBufferFromFile (fileName);
	if (buffer == AL_NONE) {
		error = alutGetError ();
		fprintf (stderr, "Error loading file: '%s'\n",
					 alutGetErrorString (error));
		alutExit ();
		exit (EXIT_FAILURE);
	}

	/* Generate a single source, attach the buffer to it and start playing. */
	alGenSources (1, &source);
	alSourcei (source, AL_BUFFER, buffer);
	//  alSourcei (source, AL_LOOPING, AL_TRUE);
	alSourcePlay (source);
	printf("!!!!!!!!!!");

	/* Normally nothing should go wrong above, but one never knows... */
	error = alGetError ();
	if (error != ALUT_ERROR_NO_ERROR) {
		fprintf (stderr, "%s\n", alGetString (error));
		alutExit ();
		exit (EXIT_FAILURE);
	}

	/* Check every 0.1 seconds if the sound is still playing. */
	do {
	alutSleep (0.1f);
	alGetSourcei (source, AL_SOURCE_STATE, &status);
	printf("!!!!!!!!!!");
	}
	while (status == AL_PLAYING);
}

int main(int argc, char **argv) {

	/* Initialise ALUT and eat any ALUT-specific commandline flags. */
	if (!alutInit (&argc, argv)) {
		ALenum error = alutGetError ();
		fprintf (stderr, "%s\n", alutGetErrorString (error));
		exit (EXIT_FAILURE);
	}

	/* Check for correct usage. */
	// if (argc != 2) {
	// 	fprintf (stderr, "usage: playfile <fileName>\n");
	// 	alutExit ();
	// 	exit (EXIT_FAILURE);
	// }

	/* If everything is OK, play the sound file and exit when finished. */
	playFile ("1.wav");

	if (!alutExit ()) {
		ALenum error = alutGetError ();
		fprintf (stderr, "%s\n", alutGetErrorString (error));
		exit (EXIT_FAILURE);
	}

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