//
// Created by kobus on 1/24/2017.
//

#ifndef EOSP_TIMECLOCK_H
#define EOSP_TIMECLOCK_H

#include <sys/time.h>

class timeClock
{
public:
    timeClock();
    static int getTime();
    static long getMs();

private:
    static struct timespec start;
};


#endif //EOSP_TIMECLOCK_H
