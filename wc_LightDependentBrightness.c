#include "wc_LightDependentBrightness.h"

#include "CommonMath.h"

/*****************************************
 *  GLOBALS
 *****************************************/
float ambientBrightnessPoints[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = {0.0, 0.01, 0.05, 0.1, 	0.2, 	0.33, 	0.5, 1.0};
// Ambient brightness tests: bright light leads to about 60 as min ADC value, darkness to about 4095 (max ADC value)
float ambientMultiplierPointsClock[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = {32, 512, 1500, 2500, 3300, 	3700, 	3900, 4095};


/*****************************************
 *  FUNCTIONS
 *****************************************/
// Get the light attenuation factor based on a configured array/lookup table
float GetBrightnessFactor( float* brightnessArray, float* factorArray, float brightness ){
	LinearApproximation( brightnessArray, factorArray, N_AMBIENT_BRIGHTNESS_CONVERSION_PTS, brightness );
}


// Chooses the ambient brightness depending on ambient to led brightness conversion array
void AdjustBrightnessArray( int increaseBrightness, float currentBrightness, float* brightnessMultiplierArray )
{
	// First: Find first index in ambientBrighntess array that holds a smaller brightness than the measured one
	int closestAmbientBrightnessIdx;
	// Note: Iteration starts at -2, as the highest idx, that is 1, will always be higher (and it ensures that no out of bounds access occurs later on)
	for ( closestAmbientBrightnessIdx=N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-2; closestAmbientBrightnessIdx >= 0; --closestAmbientBrightnessIdx )
		if ( ambientBrightnessPoints[closestAmbientBrightnessIdx] < currentBrightness )
			break;

	// Second: Calculate relative difference to next higher brightness point. This indicates how much effect
	// the brightness change has to the indicies with brightnesses inbetween the measured brightness
	//	Note: Here it is necessary, that no two brightness points are the same
	float adjustmentFactorHigherIdx = ( currentBrightness - ambientBrightnessPoints[closestAmbientBrightnessIdx] ) /
			( ambientBrightnessPoints[closestAmbientBrightnessIdx + 1 ] - ambientBrightnessPoints[closestAmbientBrightnessIdx] );

	// Calculate effect on brightnesses and set them
	// Also, limit brightnesses and adjust brightness levels beneath to ensure strict monotony
	if ( increaseBrightness )
	{
		brightnessMultiplierArray[closestAmbientBrightnessIdx] += ( 1.0 - adjustmentFactorHigherIdx ) * ( BRIGHTNESS_DIM_FACTOR - 1.0 )* brightnessMultiplierArray[closestAmbientBrightnessIdx];
		brightnessMultiplierArray[closestAmbientBrightnessIdx+1] += adjustmentFactorHigherIdx * ( BRIGHTNESS_DIM_FACTOR - 1.0 ) * brightnessMultiplierArray[closestAmbientBrightnessIdx+1];

		// adjust higher brightlevels
		// to reduce the complexity of the following loop, index zero is dealt with alone
		brightnessMultiplierArray[0] = MINIMUM( brightnessMultiplierArray[0], 0.9 );
		int i;
		for( i=1; i<N_AMBIENT_BRIGHTNESS_CONVERSION_PTS; ++i )
			brightnessMultiplierArray[i] = MINIMUM( 1.0 - ( N_AMBIENT_BRIGHTNESS_CONVERSION_PTS - closestAmbientBrightnessIdx ) * MIN_BRIGHTNESS_MULTI_DIFF, //upper limit
					MAXIMUM ( brightnessMultiplierArray[i-1] + MIN_BRIGHTNESS_MULTI_DIFF, brightnessMultiplierArray[i] ) );	// lower limit
	}
	else
	{
		brightnessMultiplierArray[closestAmbientBrightnessIdx] -= ( 1.0 - adjustmentFactorHigherIdx ) * ( 1.0 - 1.0 / BRIGHTNESS_DIM_FACTOR ) * brightnessMultiplierArray[closestAmbientBrightnessIdx];
		brightnessMultiplierArray[closestAmbientBrightnessIdx+1] -= adjustmentFactorHigherIdx * ( 1.0 - 1.0 / BRIGHTNESS_DIM_FACTOR ) * brightnessMultiplierArray[closestAmbientBrightnessIdx+1];

		// adjust lower brightlevels
		// to reduce the complexity of the following loop, the highest index is dealt with alone
		brightnessMultiplierArray[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-1] = MAXIMUM( brightnessMultiplierArray[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-1], 0.0 );
		int i;
		for( i=N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-2; i>=0; --i )
			brightnessMultiplierArray[i] = MAXIMUM( i * MIN_BRIGHTNESS_MULTI_DIFF, // lower limit
					MINIMUM ( brightnessMultiplierArray[i+1] - MIN_BRIGHTNESS_MULTI_DIFF, brightnessMultiplierArray[i] ) );	// upper limit
	}
}

