//--------------------------------------------------------------
// File     : stm32_ub_adc2_single.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_ADC2_SINGLE_H
#define __STM32F4_UB_ADC2_SINGLE_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"


//--------------------------------------------------------------
// Liste aller ADC-Kanäle
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum {
  ADC_PA3 = 0,  // PA3
  ADC_PC4 = 1,  // PC4
  ADC_PC5 = 2   // PC5
}ADC2s_NAME_t;

#define  ADC2s_ANZ   3 // Anzahl von ADC2s_NAME_t

//--------------------------------------------------------------
// Anzahl der Mittelwerte
//--------------------------------------------------------------
typedef enum {
  MW_NONE =0,  // keine Mittelwerte
  MW_2,        // 2 Mittelwerte
  MW_4,        // 4 Mittelwerte
  MW_8,        // 8 Mittelwerte
  MW_16,       // 16 Mittelwerte
  MW_32,       // 32 Mittelwerte
  MW_64,       // 64 Mittelwerte
  MW_128       // 128 Mittelwerte
}ADC2s_MW_t;


//--------------------------------------------------------------
// ADC-Clock
// Max-ADC-Frq = 36MHz
// Grundfrequenz = APB2 (APB2=84MHz)
// Moegliche Vorteiler = 2,4,6,8
//--------------------------------------------------------------

//#define ADC2s_VORTEILER     ADC_Prescaler_Div2 // Frq = 42 MHz
#define ADC2s_VORTEILER     ADC_Prescaler_Div4   // Frq = 21 MHz
//#define ADC2s_VORTEILER     ADC_Prescaler_Div6 // Frq = 14 MHz
//#define ADC2s_VORTEILER     ADC_Prescaler_Div8 // Frq = 10.5 MHz



//--------------------------------------------------------------
// Struktur eines ADC Kanals
//--------------------------------------------------------------
typedef struct {
  ADC2s_NAME_t ADC_NAME;  // Name
  GPIO_TypeDef* ADC_PORT; // Port
  const uint16_t ADC_PIN; // Pin
  const uint32_t ADC_CLK; // Clock
  const uint8_t ADC_CH;   // ADC-Kanal
  ADC2s_MW_t ADC_MW;      // Mittelwerte
}ADC2s_t;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_ADC2_SINGLE_Init(void);
uint16_t UB_ADC2_SINGLE_Read(ADC2s_NAME_t adc_name);
uint16_t UB_ADC2_SINGLE_Read_MW(ADC2s_NAME_t adc_name);


//--------------------------------------------------------------
#endif // __STM32F4_UB_ADC2_SINGLE_H
