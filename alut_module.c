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

void alutmodule_playFile (const char *fileName, const float volume, const bool loop) {
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
  alSourcef (source, AL_GAIN, volume/100.0);
  alSourcei (source, AL_LOOPING, loop);
  alSourcePlay (source);

  /* Normally nothing should go wrong above, but one never knows... */
  error = alGetError ();
  if (error != ALUT_ERROR_NO_ERROR) {
    fprintf (stderr, "%s\n", alGetString (error));
    alutExit ();
    exit (EXIT_FAILURE);
  }
}
