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

  int i;
  for (i = 0; i < SFX_BUFFER_SIZE; ++i) {
    alutPlayersRunningFlags[i] = false;
  }
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
  if (alutPlayersRunningFlags[id])
    return;

  struct alut_player_attr *player_attr = malloc(sizeof(struct alut_player_attr));
  player_attr->id = id;
  player_attr->volume = volume;
  player_attr->loop = loop;

  printf("Starting alut_player thread: %d\n", id);

  if(pthread_create(&alutPlayers[id], NULL, alut_player, player_attr)) {
    fprintf(stderr, "Error creating alut_player thread\n");
    exit(EXIT_FAILURE);
  }
  else {
    alutPlayersRunningFlags[id] = true;
  }
}

void *alut_player(void* attr) {
  assert(attr != NULL);
  struct alut_player_attr* player_attr = (struct alut_player_attr*)attr;
  int id = player_attr->id;
  float volume = player_attr->volume;
  bool loop = player_attr->loop;
  printf("alut_player: %d | %.2f | %d\n", id, volume, loop);
  free(player_attr);
  ALuint source = sfxSources[id];
  ALenum error;
  ALint status;

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

  do
  {
    alutSleep(0.001f);
    alGetSourcei(source, AL_SOURCE_STATE, &status);
  }
  while (status == AL_PLAYING);

  alutPlayersRunningFlags[id] = false;
  pthread_exit(NULL);
}
