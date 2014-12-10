#ifndef __WC_LIGHTDEPENDENTBRIGHTNESS_H
#define __WC_LIGHTDEPENDENTBRIGHTNESS_H

/*****************************************
 *  DEFINES
 *****************************************/
// Due to exponential dimming, the current brightness the following factor is multiplied by this
// factor for higher brightness and divided by it for lower brightness.
#define BRIGHTNESS_DIM_FACTOR	1.25
#define MIN_BRIGHTNESS_MULTI_DIFF	0.0001	// Minimal brightness multiplication difference allowed between indices in the multiplication arrays
#define OVERALL_BRIGHTNESS_MINIMUM	0.0001
#define OVERALL_BRIGHTNESS_MAXIMUM	1.0
#define N_AMBIENT_BRIGHTNESS_CONVERSION_PTS	8 // Number of points with conversion factors from ambient to led brightness

/*****************************************
 *  GLOBALS
 *****************************************/
// Ambient brightness support points in ADC values (need to be in ascending order)
// Ambient brightness tests: bright light leads to about 60 as min ADC value, darkness to about 4095 (max ADC value)
extern float ambientBrightnessPoints[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS];

// LED dimming factors for above ambient brightness points
extern float ambientBrightnessLedDimmingFactors[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS];

// current ambient brightness in ADC reads
extern float ambientBrightnessCurrent;

/*****************************************
 *  CALLABLE FUNCTIONS
 *****************************************/

// Get the light attenuation factor based on a configured array/lookup table and the currently measured brightness in ADC counts
float GetBrightnessFactor( float* brightnessArray, float* factorArray, float currentBrightness );

// Chooses the ambient brightness depending on ambient to led brightness conversion array
void AdjustBrightnessArray( int increaseBrightness, float currentBrightness, float* brightnessMultiplierArray );



#endif __WC_LIGHTDEPENDENTBRIGHTNESS_H
