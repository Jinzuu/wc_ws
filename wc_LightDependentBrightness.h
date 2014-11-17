#ifndef __WC_LIGHTDEPENDENTBRIGHTNESS_H
#define __WC_LIGHTDEPENDENTBRIGHTNESS_H

/*****************************************
 *  DEFINES
 *****************************************/
// Due to exponential dimming, the current brightness the following factor is multiplied by this
// factor for higher brightness and divided by it for lower brightness.
#define BRIGHTNESS_DIM_FACTOR	1.25
#define MIN_BRIGHTNESS_MULTI_DIFF	0.0001	// Minimal brightness multiplication difference allowed between indices in the multiplication arrays
#define N_AMBIENT_BRIGHTNESS_CONVERSION_PTS	8 // Number of points with conversion factors from ambient to led brightness

/*****************************************
 *  GLOBALS
 *****************************************/
// Sampling points of the ambient brightess
float ambientBrightnessPoints[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS];
// LED dimming factors for the above sampling points
float ambientBrightnessLedDimmingFactors[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS];

/*****************************************
 *  CALLABLE FUNCTIONS
 *****************************************/

// Get the light attenuation factor based on a configured array/lookup table
float GetBrightnessFactor( float* brightnessArray, float* factorArray, float brightness );

// Chooses the ambient brightness depending on ambient to led brightness conversion array
void AdjustBrightnessArray( int increaseBrightness, float currentBrightness, float* brightnessMultiplierArray );



#endif __WC_LIGHTDEPENDENTBRIGHTNESS_H
