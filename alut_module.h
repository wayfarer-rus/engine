#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <AL/alut.h>

void alutmodule_init();
void alutmodule_exit();

void alutmodule_playFile (const char *fileName, const float volume, const bool loop);
