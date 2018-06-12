#ifndef TIMER_H
#define TIMER_H

#include <unistd.h>
#include <time.h>

typedef struct {
    struct timespec begin;
    struct timespec end;
    double elapsed_time;
} Timer;

void TimerReset(Timer * timer);
void TimerStart(Timer * timer);
void TimerStop(Timer * timer);
double TimerRead();

#endif
