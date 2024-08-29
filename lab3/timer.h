#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>

#define GET_TIME(now)                             \
  {                                               \
    struct timeval time;                          \
    gettimeofday(&time, NULL);                    \
    now = time.tv_sec + time.tv_usec / 1000000.0; \
  }
#endif
