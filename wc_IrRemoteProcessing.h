#ifndef __WC_IRREMOTEPROCESSING_H
#define __WC_IRREMOTEPROCESSING_H

#include "IRMP/irmp.h"
#include "wc_frontend.h"

//####################################################################################################
//######################################### VARIABLES ################################################
//####################################################################################################


//####################################################################################################
//##################################### REMOTE KEY DEFINES ###########################################
//####################################################################################################
// Defines for DX Remote ED618
#define IR_REMOTE_KEY_BRIGHTNESS_INC	0
#define IR_REMOTE_KEY_BRIGHTNESS_DEC	1
#define IR_REMOTE_KEY_OFF				2
#define IR_REMOTE_KEY_ON				3

#define IR_REMOTE_KEY_FLASH				11
#define IR_REMOTE_KEY_STROBE			15
#define IR_REMOTE_KEY_FADE				19
#define IR_REMOTE_KEY_SMOOTH			23

#define IR_REMOTE_KEY_RED				4
#define IR_REMOTE_KEY_GREEN				5
#define IR_REMOTE_KEY_BLUE				6
#define IR_REMOTE_KEY_WHITE				7

#define IR_REMOTE_KEY_ORANGE			8
#define IR_REMOTE_KEY_LIGHTORANGE		12
#define IR_REMOTE_KEY_DARKYELLOW		16
#define IR_REMOTE_KEY_YELLOW			20
#define IR_REMOTE_KEY_LIGHTGREEN		9
#define IR_REMOTE_KEY_LIGHTGREENBLUE	13
#define IR_REMOTE_KEY_CYAN				17
#define IR_REMOTE_KEY_DARKGREENBLUE		21
#define IR_REMOTE_KEY_DARKBLUE			10
#define IR_REMOTE_KEY_PURPLE			14
#define IR_REMOTE_KEY_PINK				18
#define IR_REMOTE_KEY_DARKPINK			22

//####################################################################################################
//####################################### OTHER DEFINES ##############################################
//####################################################################################################

//####################################################################################################
//########################################### GLOBALS ################################################
//####################################################################################################
WS2812_HSV_t gCurrentMatrixColor;

//####################################################################################################
//##################################### EXTERNAL FUNCTIONS ###########################################
//####################################################################################################

// Deals with e.g. this remote: http://www.dx.com/p/24-key-wireless-infrared-ir-remote-controller-for-rgb-led-light-bulb-1-cr2025-47019#.VDqdxBauQgp

// Function that uses a received IR command to do stuff
void ProcessIrDataPacket( IRMP_DATA irPacket );



#endif //__WC_IRREMOTEPROCESSING_H
