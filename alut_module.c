#include "alut_module.h"

void alutmodule_init() {
  if (!alutInit (NULL, NULL)) {
    ALenum error = alutGetError ();
    fprintf (stderr, "%s\n", alutGetErrorString (error));
    exit (EXIT_FAILURE);
  }
}

void alutmodule_exit() {
  if (!alutExit ()) {
    ALenum error = alutGetError ();
    fprintf (stderr, "%s\n", alutGetErrorString (error));
    exit (EXIT_FAILURE);
  }
}

void alutmodule_playFile (const char *fileName) {
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
}
