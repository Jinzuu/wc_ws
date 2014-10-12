#ifndef __WC_IRREMOTEPROCESSING_H
#define __WC_IRREMOTEPROCESSING_H

#include "IRMP/irmp.h"
//####################################################################################################
//##################################### EXTERNAL FUNCTIONS ###########################################
//####################################################################################################

// Deals with e.g. this remote: http://www.dx.com/p/24-key-wireless-infrared-ir-remote-controller-for-rgb-led-light-bulb-1-cr2025-47019#.VDqdxBauQgp

// Function that uses a received IR command to do stuff
void ProcessIrDataPacket( IRMP_DATA irPacket );



#endif __WC_IRREMOTEPROCESSING_H
