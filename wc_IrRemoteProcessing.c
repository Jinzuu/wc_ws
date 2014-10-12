#include "wc_IrRemoteProcessing.h"
#include "wc_frontend.h"

/*****************************************
 *  GLOBALS
 *****************************************/
int irAddressFirstSeen = -1;	// Used to take only cmds from this address (-1 = not set yet)


// Function that uses a received IR command to do stuff
void ProcessIrDataPacket( IRMP_DATA irPacket ){
	// Process packet only if it came from the right remote
	if ( irAddressFirstSeen == -1 )
		irAddressFirstSeen = irPacket.address;
	else if ( irAddressFirstSeen != irPacket.address )
		return;


	switch ( irPacket.command )
	{
		case 4:	// Color: Red
			WC_SetColor(WS2812_HSV_COL_RED);
			break;
		case 5:	// Color: Green
			WC_SetColor(WS2812_HSV_COL_GREEN);
			break;
		case 6:	// Color: Blue
			WC_SetColor(WS2812_HSV_COL_BLUE);
			break;
		case 7:	// Color: White
			WC_SetColor(WS2812_HSV_COL_WHITE);
			break;
	}
}
