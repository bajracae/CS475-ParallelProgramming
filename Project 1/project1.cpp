#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include "helperfunctions.h"

// setting the number of threads:
#ifndef NUMT	
#define NUMT	1
#endif

// setting the number of trials in the monte carlo simulation:
#ifndef NUMTRIALS
#define NUMTRIALS	1
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	1000
#endif

// ranges for the random numbers:
const float XCMIN = -1.0;
const float XCMAX = 1.0;
const float YCMIN = 0.0;
const float YCMAX = 2.0;
const float RMIN = 0.5;
const float RMAX = 2.0;

// function prototypes:
float		Ranf(float, float);
int		Ranf(int, int);
void		TimeOfDaySeed();

// main program:
int
main(int argc, char *argv[])
{
#ifndef _OPENMP
	fprintf(stderr, "No OpenMP support!\n");
	return 1;
#endif

	float tn = tan((M_PI / 180.)*30.);
	TimeOfDaySeed();		// seed the random number generator

	omp_set_num_threads(NUMT);	// set the number of threads to use in the for-loop:`

	// better to define these here so that the rand() calls don't get into the thread timing:
	float *xcs = new float[NUMTRIALS];
	float *ycs = new float[NUMTRIALS];
	float * rs = new float[NUMTRIALS];

	// fill the random-value arrays:
	for (int a = 0; a < NUMTRIALS; a++)
	{
		xcs[a] = Ranf(XCMIN, XCMAX);
		ycs[a] = Ranf(YCMIN, YCMAX);
		rs[a] = Ranf(RMIN, RMAX);
	}

	// get ready to record the maximum performance and the probability:
	float maxPerformance = 0.;      // must be declared outside the NUMTRIES loop
	float currentProb;              // must be declared outside the NUMTRIES loop

	// looking for the maximum performance:
	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();

		int numHits = 0;
		#pragma omp parallel for default(none) shared(xcs,ycs,rs,tn) reduction(+:numHits)
		for (int n = 0; n < NUMTRIALS; n++)
		{
			// randomize the location and radius of the circle:
			float xc = xcs[n];
			float yc = ycs[n];
			float  r = rs[n];

			// solve for the intersection using the quadratic formula:
			float a = 1. + tn * tn;
			float b = -2.*(xc + yc * tn);
			float c = xc * xc + yc * yc - r * r;
			float d = b * b - 4.*a*c;

			// case A: circle is completely missed
			if (d < 0) {
				continue;
			}

			// hits the circle:
			// get the first intersection:
			d = sqrt(d);
			float t1 = (-b + d) / (2.*a);	// time to intersect the circle
			float t2 = (-b - d) / (2.*a);	// time to intersect the circle
			float tmin = t1 < t2 ? t1 : t2;		// only care about the first intersection

			// case B: circle engulfs the laser pointer
			if (tmin < 0) {
				continue;
			}

			// where does it intersect the circle?
			float xcir = tmin;
			float ycir = tmin * tn;

			// get the unitized normal vector at the point of intersection:
			float nx = xcir - xc;
			float ny = ycir - yc;
			float nxy = sqrt(nx*nx + ny * ny);
			nx /= nxy;	// unit vector
			ny /= nxy;	// unit vector

			// get the unitized incoming vector:
			float inx = xcir - 0.;
			float iny = ycir - 0.;
			float in = sqrt(inx*inx + iny * iny);
			inx /= in;	// unit vector
			iny /= in;	// unit vector

			// get the outgoing (bounced) vector:
			float dot = inx * nx + iny * ny;
			float outx = inx - 2.*nx*dot;	// angle of reflection = angle of incidence`
			float outy = iny - 2.*ny*dot;	// angle of reflection = angle of incidence`

			// find out if it hits the infinite plate:
			float tt = (0. - ycir) / outy;

			// case C: reflected beam went up instead of down
			if (tt < 0) {
				continue;
			}
			
			// case D: beam hit the infinite plate
			numHits++;
		}
		double time1 = omp_get_wtime();
		double megaTrialsPerSecond = (double)NUMTRIALS / (time1 - time0) / 1000000.;
		if (megaTrialsPerSecond > maxPerformance)
			maxPerformance = megaTrialsPerSecond;
		currentProb = (float)numHits / (float)NUMTRIALS;
	}
	printf("Number of threads: %d\n", NUMT);
	printf("Number of trials: %d\n", NUMTRIALS);
	printf("Probability: %.2lf\n", currentProb);
	printf("MegaTrialsPerSecond: %.2lf\n", maxPerformance);
}
