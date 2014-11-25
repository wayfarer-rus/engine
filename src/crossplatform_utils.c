#include "crossplatform_utils.h"

void sleepMillis(const long m) {
#ifdef __MINGW32__
  Sleep(m);
#elif __linux__
  struct timespec sleepTime;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = m*1000*1000; // convert to nanoseconds
  nanosleep(&sleepTime, NULL);
#endif
}

void sleepSeconds(const long s) {
#ifdef __MINGW32__
  long m = s*1000; // convert to millis
  Sleep(m);
#elif __linux__
  struct timespec sleepTime;
  sleepTime.tv_sec = s;
  sleepTime.tv_nsec = 0;
  nanosleep(&sleepTime, NULL);
#endif
}
