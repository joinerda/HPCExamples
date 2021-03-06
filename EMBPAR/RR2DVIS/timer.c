#include <time.h>
#include <unistd.h>
#include "timer.h"

void TimerReset(Timer * timer) {
    timer->elapsed_time =0.0;
}

void TimerStart(Timer * timer) {
    clock_gettime(CLOCK_REALTIME,&(timer->begin));
}

void TimerStop(Timer * timer) {
    clock_gettime(CLOCK_REALTIME,&(timer->end));
    timer->elapsed_time += (double)(timer->end.tv_nsec-timer->begin.tv_nsec);
    timer->elapsed_time += (double)(timer->end.tv_sec-timer->begin.tv_sec)*1.0e9;
}

double TimerRead(Timer * timer) {
    return timer->elapsed_time*1.0e-9;
}
