#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "helperfunctions.h"

float
Ranf(float low, float high)
{
	float r = (float)rand();               // 0 - RAND_MAX
	float t = r / (float)RAND_MAX;       // 0. - 1.

	return   low + t * (high - low);
}

int
Ranf(int ilow, int ihigh)
{
	float low = (float)ilow;
	float high = ceil((float)ihigh);

	return (int)Ranf(low, high);
}

void
TimeOfDaySeed()
{
	struct tm y2k = { 0 };
	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  timer;
	time(&timer);
	double seconds = difftime(timer, mktime(&y2k));
	unsigned int seed = (unsigned int)(1000.*seconds);    // milliseconds
	srand(seed);
}