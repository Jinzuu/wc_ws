#include "wc_LightDependentBrightness.h"

#include "CommonMath.h"

/*****************************************
 *  GLOBALS
 *****************************************/
// Ambient brightness support points in ADC values (need to be in ascending order)
// Ambient brightness tests: bright light leads to about 60 as min ADC value, darkness to about 4095 (max ADC value)
float ambientBrightnessPoints[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = 			{32, 512, 1500, 2500, 3300, 3700, 	3900, 4095};

// LED dimming factors for above ambient brightness points (needs to be monotonically decreasing for AdjustBrightnessArray)
float ambientBrightnessLedDimmingFactors[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = {1.0, 0.8, 0.5, 0.3, 0.09, 	0.05, 	0.03, 0.01};

// current ambient brightness in ADC reads
float ambientBrightnessCurrent = 0;

/*****************************************
 *  FUNCTIONS
 *****************************************/
// Get the light attenuation factor based on a configured array/lookup table and the currently measured brightness in ADC counts
float GetBrightnessFactor( float* brightnessArray, float* factorArray, float currentBrightness ){
	return LinearApproximation( brightnessArray, factorArray, N_AMBIENT_BRIGHTNESS_CONVERSION_PTS, currentBrightness );
}


// De- or Increases (inc if increaseBrightness != 0) the brightness stored in the ambient brightness
// multiplier array depending on current ambient brightness
// Note: The brightnessMultiplier array needs to be monotonically decreasing
void AdjustBrightnessArray( int increaseBrightness, float currentBrightness, float* brightnessMultiplierArray )
{
	// First: Find first index in ambientBrighntess array that holds a smaller brightness than the measured one
	int firstSmallerAmbientBrightnessIdx;
	// Note: Iteration starts at -2, as the highest idx, that is 1, will always be higher (and it ensures that no out of bounds access occurs later on)
	for ( firstSmallerAmbientBrightnessIdx=N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-2; firstSmallerAmbientBrightnessIdx >= 0; --firstSmallerAmbientBrightnessIdx )
		if ( ambientBrightnessPoints[firstSmallerAmbientBrightnessIdx] < currentBrightness )
			break;

	// Second: Calculate relative difference to next higher brightness point. This indicates how much effect
	// the brightness change has to the indices with brightnesses in between the measured brightness
	//	Note: Here it is necessary, that no two brightness points are the same
	float adjustmentFactorHigherIdx = ( currentBrightness - ambientBrightnessPoints[firstSmallerAmbientBrightnessIdx] ) /
			( ambientBrightnessPoints[firstSmallerAmbientBrightnessIdx + 1 ] - ambientBrightnessPoints[firstSmallerAmbientBrightnessIdx] );

	// Calculate effect on brightnesses and set them
	// Also, limit brightnesses and adjust brightness levels beneath to ensure strict monotony
	if ( increaseBrightness )
	{
		brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx] += ( 1.0 - adjustmentFactorHigherIdx ) * ( BRIGHTNESS_DIM_FACTOR - 1.0 )* brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx];
		brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx+1] += adjustmentFactorHigherIdx * ( BRIGHTNESS_DIM_FACTOR - 1.0 ) * brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx+1];

		// limit all touched and increase if necessary higher untouched brightness levels to ensure a monotonic function
		int i = firstSmallerAmbientBrightnessIdx+1;	// Start is at highest modified idx
		// to reduce the complexity of the following loop, the highest index is dealt with alone
		brightnessMultiplierArray[i] = GetMinimum( brightnessMultiplierArray[i], OVERALL_BRIGHTNESS_MAXIMUM - i*MIN_BRIGHTNESS_MULTI_DIFF );
		// Iterate over the rest by decreasing in idx
		for( --i; i>=0; --i ){
			float maximumForPt = OVERALL_BRIGHTNESS_MAXIMUM - i*MIN_BRIGHTNESS_MULTI_DIFF;
			float minimumForPt = GetMaximum( brightnessMultiplierArray[i+1] + MIN_BRIGHTNESS_MULTI_DIFF, brightnessMultiplierArray[i] );

			brightnessMultiplierArray[i] = GetMinimum( maximumForPt, minimumForPt );
		}
	}
	else
	{
		brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx] -= ( 1.0 - adjustmentFactorHigherIdx ) * ( 1.0 - 1.0 / BRIGHTNESS_DIM_FACTOR ) * brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx];
		brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx+1] -= adjustmentFactorHigherIdx * ( 1.0 - 1.0 / BRIGHTNESS_DIM_FACTOR ) * brightnessMultiplierArray[firstSmallerAmbientBrightnessIdx+1];

		// limit all touched and decrease if necessary lower untouched brightness levels to ensure a monotonic function
		int i = firstSmallerAmbientBrightnessIdx;	// Start is at lowest modified idx
		// to reduce the complexity of the following loop, the lowest index is dealt with alone
		brightnessMultiplierArray[i] = GetMaximum( brightnessMultiplierArray[i], OVERALL_BRIGHTNESS_MINIMUM + (N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-1-i) * MIN_BRIGHTNESS_MULTI_DIFF );
		// Iterate over the rest by increasing in idx
		for( ++i; i < N_AMBIENT_BRIGHTNESS_CONVERSION_PTS; ++i ){
			float minimumForPt = OVERALL_BRIGHTNESS_MINIMUM + (N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-1-i) * MIN_BRIGHTNESS_MULTI_DIFF;
			float maximumForPt = GetMinimum( brightnessMultiplierArray[i-1] - MIN_BRIGHTNESS_MULTI_DIFF, brightnessMultiplierArray[i] );

			brightnessMultiplierArray[i] = GetMaximum( maximumForPt, minimumForPt );
		}
	}
}

