#include "alut_module.h"

void alutmodule_init() {
  if (!alutInit (NULL, NULL)) {
    ALenum error = alutGetError ();
    fprintf (stderr, "%s\n", alutGetErrorString (error));
    exit (EXIT_FAILURE);
  }

  alGenBuffers(SFX_BUFFER_SIZE, sfxFiles);
  alGenSources(SFX_BUFFER_SIZE, sfxSources);
  sfxIterator = 0;
}

void alutmodule_exit() {
  alDeleteBuffers(SFX_BUFFER_SIZE, sfxFiles);
  alDeleteSources(SFX_BUFFER_SIZE, sfxSources);

  if (!alutExit ()) {
    ALenum error = alutGetError ();
    fprintf (stderr, "%s\n", alutGetErrorString (error));
    exit (EXIT_FAILURE);
  }
}

int alutmodule_loadFile(const char *fileName) {
  ALenum error;

  /* Create an AL buffer from the given sound file. */
  ALuint buffer = alutCreateBufferFromFile (fileName);
  if (buffer == AL_NONE) {
    error = alutGetError ();
    fprintf (stderr, "Error loading file: '%s'\n",
           alutGetErrorString (error));
    alutExit ();
    exit (EXIT_FAILURE);
  }

  sfxFiles[sfxIterator] = buffer;
  alSourcei(sfxSources[sfxIterator], AL_BUFFER, sfxFiles[sfxIterator]);
  return sfxIterator++;
}

void alutmodule_playFile (const int id, const float volume, const bool loop) {
  ALuint source = sfxSources[id];
  ALint status;
  alGetSourcei(source, AL_SOURCE_STATE, &status);

  if ((status == AL_PLAYING))
    return;

  printf("alut_player: %d | %.2f | %d\n", id, volume, loop);
  ALenum error;

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
