#include "wc_frontend.h"
#include "ub_lib/stm32_ub_ws2812.h"

// Note: No overlapping configuration allowed!
T_WC_CONFIGURATION WC_CONFIG[] = {
		{WC_ELEMENT_ES, 0, 2, 1},
		{WC_ELEMENT_IST, 3, 3, 1},
		{WC_ELEMENT_FUENF_MINUTE, 7, 4, 1},

		{WC_ELEMENT_ZEHN_MINUTE, 11, 4, 1},
		{WC_ELEMENT_ZWANZIG_MINUTE, 15, 7, 1},

		{WC_ELEMENT_DREI_MINUTE, 22, 4, 1},
		{WC_ELEMENT_VIERTEL_MINUTE, 26, 7, 1},

		{WC_ELEMENT_VOR, 33, 3, 1},
		{WC_ELEMENT_FUNK, 36, 4, 1},
		{WC_ELEMENT_NACH, 40, 4, 1},

		{WC_ELEMENT_HALB, 44, 4, 1},
		{WC_ELEMENT_EL_STUNDE, 49, 2, 1},
		{WC_ELEMENT_F_STUNDE, 51, 1, 1},
		{WC_ELEMENT_UENF_STUNDE, 52, 3, 1},

		{WC_ELEMENT_EIN_STUNDE, 55, 3, 1},
		{WC_ELEMENT_S_STUNDE, 58, 1, 1},
		{WC_ELEMENT_ZWEI_STUNDE, 62, 4, 1},

		{WC_ELEMENT_DREI_STUNDE, 66, 4, 1},
		{WC_ELEMENT_VIER_STUNDE, 73, 4, 1},

		{WC_ELEMENT_SECHS_STUNDE, 77, 5, 1},
		{WC_ELEMENT_ACHT_STUNDE, 84, 4, 1},

		{WC_ELEMENT_SIEBEN_STUNDE, 88, 6, 1},
		{WC_ELEMENT_ZWOELF_STUNDE, 94, 5, 1},

		{WC_ELEMENT_ZEH_STUNDE, 99, 4, 1},
		{WC_ELEMENT_N_STUNDE, 102, 1, 1},
		{WC_ELEMENT_EUN_STUNDE, 103, 3, 1},
		{WC_ELEMENT_UHR, 107, 3, 1},

		{WC_ELEMENT_MIN_upRight, 110, 1, 1},
		{WC_ELEMENT_MIN_downRight, 111, 1, 1},
		{WC_ELEMENT_MIN_downLeft, 112, 1, 1},
		{WC_ELEMENT_MIN_upLeft, 113, 1, 1},
};

WS2812_HSV_t WC_Colour = WS2812_HSV_COL_OFF;

void WC_SetElement(T_WC_ELEMENT element, int enable)
{
	int index = WC_CONFIG[element].START_INDEX;
	int length = WC_CONFIG[element].LENGTH;
	if( enable == 1 ) {
		WC_CONFIG[element].ENABLED = 1;
		while(length-- > 0 ) {
			UB_WS2812_One_Led_HSV(index++, WC_Colour, 0);
		}
	} else {
		WC_CONFIG[element].ENABLED = 0;
		while(length-- > 0 ) {
			UB_WS2812_One_Led_HSV(index++, WS2812_HSV_COL_OFF, 0);
		}
	}

	// Because the segments "ES IST" are separated by one LED, individual handling
	// is required
	if( element == WC_ELEMENT_ES ) {
		WC_SetElement(WC_ELEMENT_IST, enable);
	}
}

void WC_SetColor(WS2812_HSV_t colour)
{
	WC_Colour.h = colour.h;
	WC_Colour.s = colour.s;
	if( WC_Colour.v == 0 ) {
		WC_Colour.v = colour.v;
	}
}

WS2812_HSV_t WC_GetColor( void ){
	return WC_Colour;
}

void WC_SetBrightness(int brightness)
{
	WC_Colour.v = brightness;
}

void WC_Refresh()
{
	int i;
	for( i=0; i<T_WC_ELEMENT_COUNT; ++i ) {
		if( WC_CONFIG[i].ENABLED == 1 ) {
			WC_SetElement(WC_CONFIG[i].ELEMENT_NAME, 1);
		}
		else{
			WC_SetElement(WC_CONFIG[i].ELEMENT_NAME, 0);
		}
	}

	UB_WS2812_Refresh();
}

void WC_DisableAll()
{
	UB_WS2812_All_Led_HSV(WS2812_HSV_COL_OFF, 1);
}

void WC_DisableAllElements()
{
	// Iterate over all LEDs to set disabled
	int i;
	for( i=0; i<T_WC_ELEMENT_COUNT; ++i ) {
		WC_CONFIG[i].ENABLED = 0;
	}
}
