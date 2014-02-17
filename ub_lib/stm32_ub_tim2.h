//--------------------------------------------------------------
// File     : stm32_ub_tim2.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_TIM2_H
#define __STM32F4_UB_TIM2_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"




//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_TIMER2_Init(uint16_t prescaler, uint16_t periode);
void UB_TIMER2_Init_WithFrequency( int desiredFreqInHz );
void UB_TIMER2_Start(void);
void UB_TIMER2_Stop(void);
void UB_TIMER2_ISR_CallBack(void);  // Diese Funktion muss extern benutzt werden !!


//--------------------------------------------------------------
#endif // __STM32F4_UB_TIM2_H
