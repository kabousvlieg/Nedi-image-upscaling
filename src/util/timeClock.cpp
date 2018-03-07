//
// Created by kobus on 1/24/2017.
//

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "timeClock.h"

struct timespec timeClock::start;

timeClock::timeClock()
{
    clock_gettime(CLOCK_REALTIME, &start);
}

int timeClock::getTime()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec - start.tv_sec;
}

long int timeClock::getMs()
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

    long int ms;
	ms = (now.tv_sec - start.tv_sec) * 1000;
	ms += (now.tv_nsec - start.tv_nsec) / 1.0e6;
	return ms;
}


