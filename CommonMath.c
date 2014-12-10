#include "CommonMath.h"


// Minimum and maximum functions
// ------------------------
float GetMinimum( float a, float b ){
	if ( a < b )
		return a;
	return b;
}
float GetMaximum( float a, float b ){
	if ( a > b )
		return a;
	return b;
}


// Sliding Average function
volatile static float slidingAvg_values[N_SLIDING_AVERAGING_POINTS] = {0.0};
volatile static int slidingAvg_InsertIdx = 0;
float SlidingAverageOnLastValues( float value )
{
	// Update array position and insert
	slidingAvg_InsertIdx = ( ++slidingAvg_InsertIdx ) % N_SLIDING_AVERAGING_POINTS;
	slidingAvg_values[slidingAvg_InsertIdx] = value;

	float average = 0.0;
	int i;
	for ( i=0; i<N_SLIDING_AVERAGING_POINTS; ++i )
		average += slidingAvg_values[i];

	return average / (float) N_SLIDING_AVERAGING_POINTS;
}



float LinearApproximation( const float* xArray, const float* fxArray, const int arrayLength, const float x )
{
	// First: Find first index in xArray that holds a smaller value that x is
	uint_fast16_t xArrayIdxSmaller = arrayLength-2;
	// Note: Iteration starts at -2, as the highest idx should always have a higher value than X
	for ( ; xArrayIdxSmaller >= 0; --xArrayIdxSmaller )
		if ( xArray[xArrayIdxSmaller] <= x )
			break;

	// Second: Calculate relative difference to higher xArray point
	//	Note: Here it is necessary, that no two xArray points are the same
	float linearApproxFactor = ( x - xArray[xArrayIdxSmaller] ) /
			( xArray[xArrayIdxSmaller + 1 ] - xArray[xArrayIdxSmaller] );

	return fxArray[xArrayIdxSmaller] + linearApproxFactor * ( fxArray[xArrayIdxSmaller + 1 ] - fxArray[xArrayIdxSmaller] );
}
