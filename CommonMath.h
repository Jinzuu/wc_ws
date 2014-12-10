 /***********************************************************************
 * Functions for general applications
 * Use at own risk
 * Customizable behaviour sometimes through defines
 *
 * Jan Berg, 2011
 ***********************************************************************/
#ifndef COMMON_MATH_H_
#define COMMON_MATH_H_


#include <stdint.h>


// Minimum and maximum functions
// ------------------------
float GetMinimum( float a, float b );
float GetMaximum( float a, float b );

// Sliding Average Function
// ------------------------
// The function averages over the last values that were put in and returns the average.
// Customize behaviour through defines below
#define N_SLIDING_AVERAGING_POINTS 32
float SlidingAverageOnLastValues( float value );



// Linear Approximation
// ------------------------
// Given two arrays, one with x and one with the respective f(x)'s of the function to be
// reproduced, the f(x) value of the provided x is linearly approximated
// Note: The xArray needs to be in ascending order and strictly monotonic, e.g. xArray[i] < xArray[i+1]
// Note: In order to work properly, x needs to be higher/lower than the maximum/minimum value in the xArray
float LinearApproximation( const float* xArray, const float* fxArray, const int arrayLength, const float x );

#endif /* COMMON_MATH_H_ */
