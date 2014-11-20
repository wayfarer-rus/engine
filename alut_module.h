#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <AL/alut.h>

#define SFX_BUFFER_SIZE 100

static int sfxIterator;
static ALuint sfxFiles[SFX_BUFFER_SIZE];
static ALuint sfxSources[SFX_BUFFER_SIZE];
static pthread_t alutPlayers[SFX_BUFFER_SIZE];
static bool alutPlayersRunningFlags[SFX_BUFFER_SIZE];

struct alut_player_attr {
  int id;
  float volume;
  bool loop;
};

static void *alut_player(void* attr);

void alutmodule_init();
void alutmodule_exit();

int alutmodule_loadFile(const char *fileName);
void alutmodule_playFile(const int id, const float volume, const bool loop);
