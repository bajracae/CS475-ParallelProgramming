#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

// setting the number of threads:
#ifndef N	
#define N	4
#endif

// setting the number of threads:
#ifndef NUMT	
#define NUMT	1
#endif

// setting the number of nodes:
#ifndef NUMNODES	
#define NUMNODES	16
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	10
#endif

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

float Height( int, int );

int main( int argc, char *argv[ ] )
{
	#ifndef _OPENMP
		fprintf(stderr, "No OpenMP support!\n");
		return 1;
	#endif
	
	omp_set_num_threads(NUMT);	// set the number of threads to use in the for-loop:`

	// the area of a single full-sized tile:
	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );


	float maxPerformance = 0.0;
	float maxVolume = 0.0;
	
	// looking for the maximum performance:
	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();
		float volume = 0.0;
		#pragma omp parallel for default(none), shared(fullTileArea), reduction(+:volume)
		for( int i = 0; i < NUMNODES*NUMNODES; i++ )
		{
			int iu = i % NUMNODES;
			int iv = i / NUMNODES;

			float z = Height( iu, iv ) * 2;
			
			// Corner tile
			if((iv == 0 && iu == 0) 
				|| (iv == 0 && iu == NUMNODES-1) 
					|| (iv == NUMNODES-1 && iu == 0) 
						|| (iv == NUMNODES-1 && iu == NUMNODES-1)) {
				volume += z * fullTileArea/4.0;
			}
			else if(((iv > 0 && iv < NUMNODES-1) && (iu == 0))
				|| ((iv > 0 && iv < NUMNODES-1) && (iu == NUMNODES-1))
					|| ((iv == 0) && (iu > 0 && iu < NUMNODES-1))
						|| ((iv == NUMNODES-1) && (iu > 0 && iu < NUMNODES-1))) {
				volume += z * fullTileArea/2.0;
			}
			else{
				volume += z * fullTileArea;
			}
		}
		double time1 = omp_get_wtime();
		
		double megaHeightsPerSecond = (double)NUMNODES * NUMNODES / (time1 - time0) / 1000000.;	
		if (megaHeightsPerSecond > maxPerformance){
			maxPerformance = megaHeightsPerSecond;
		}
		if (volume > maxVolume){
			maxVolume = volume;
		}
	}	
	printf("NUMT: %d\t NUMNODES: %d\t megaHeightsPerSecond: %10.2lf\t volume: %10.2lf\t\n", 
			NUMT, NUMNODES, maxPerformance, maxVolume);
}

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r < 0. )
	        return 0.;
	float height = pow( 1. - xn - yn, 1./(float)N );
	return height;
}