#include "wc_IrRemoteProcessing.h"
#include "wc_frontend.h"

/*****************************************
 *  GLOBALS
 *****************************************/
int irAddressFirstSeen = -1;	// Used to take only cmds from this address (-1 = not set yet)
WS2812_HSV_t irLastUsedColor = {0,  0, 100};

//####################################################################################################
//################################## FUNCTIONS IMPLEMENTATION ########################################
//####################################################################################################
// Function that uses a received IR command to do stuff
void ProcessIrDataPacket( IRMP_DATA irPacket ){
	// Process packet only if it came from the right remote
	if ( irAddressFirstSeen == -1 )
		irAddressFirstSeen = irPacket.address;
	else if ( irAddressFirstSeen != irPacket.address )
		return;

	switch ( irPacket.command )
	{
		// Commands
//		case IR_REMOTE_KEY_BRIGHTNESS_INC:
//
//			break;
//		case IR_REMOTE_KEY_BRIGHTNESS_DEC:
//
//			break;
		case IR_REMOTE_KEY_OFF:
			irLastUsedColor = WC_GetColor();
			WC_SetColor(WS2812_HSV_COL_OFF);
			break;
		case IR_REMOTE_KEY_ON:
			WC_SetColor(irLastUsedColor);
			break;
//		case IR_REMOTE_KEY_FLASH:
//
//			break;
//		case IR_REMOTE_KEY_STROBE:
//
//			break;
//		case IR_REMOTE_KEY_FADE:
//
//			break;
//		case IR_REMOTE_KEY_SMOOTH:
//
//			break;

		// Basic colors
		case IR_REMOTE_KEY_RED:
			WC_SetColor(WS2812_HSV_COL_RED);
			break;
		case IR_REMOTE_KEY_GREEN:
			WC_SetColor(WS2812_HSV_COL_GREEN);
			break;
		case IR_REMOTE_KEY_BLUE:
			WC_SetColor(WS2812_HSV_COL_BLUE);
			break;
		case IR_REMOTE_KEY_WHITE:
			WC_SetColor(WS2812_HSV_COL_WHITE);
			break;

		// Other colors
		case IR_REMOTE_KEY_ORANGE:
			WC_SetColor(WS2812_HSV_COL_ORANGE);
			break;
		case IR_REMOTE_KEY_LIGHTORANGE:
			WC_SetColor(WS2812_HSV_COL_LIGHTORANGE);
			break;
		case IR_REMOTE_KEY_DARKYELLOW:
			WC_SetColor(WS2812_HSV_COL_DARKYELLLOW);
			break;
		case IR_REMOTE_KEY_YELLOW:
			WC_SetColor(WS2812_HSV_COL_YELLOW);
			break;
		case IR_REMOTE_KEY_LIGHTGREEN:
			WC_SetColor(WS2812_HSV_COL_LIGHTGREEN);
			break;
		case IR_REMOTE_KEY_LIGHTGREENBLUE:
			WC_SetColor(WS2812_HSV_COL_LIGHTGREENBLUE);
			break;
		case IR_REMOTE_KEY_CYAN:
			WC_SetColor(WS2812_HSV_COL_CYAN);
			break;
		case IR_REMOTE_KEY_DARKGREENBLUE:
			WC_SetColor(WS2812_HSV_COL_DARKGREENBLUE);
			break;
		case IR_REMOTE_KEY_DARKBLUE:
			WC_SetColor(WS2812_HSV_COL_DARKBLUE);
			break;
		case IR_REMOTE_KEY_PURPLE:
			WC_SetColor(WS2812_HSV_COL_PURPLE);
			break;
		case IR_REMOTE_KEY_PINK:
			WC_SetColor(WS2812_HSV_COL_PINK);
			break;
		case IR_REMOTE_KEY_DARKPINK:
			WC_SetColor(WS2812_HSV_COL_DARKPINK);
			break;
	}
}

