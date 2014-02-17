//--------------------------------------------------------------
// File     : stm32_ub_tim4.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_TIM4_H
#define __STM32F4_UB_TIM4_H


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
void UB_TIMER4_Init(uint16_t prescaler, uint16_t periode);
void UB_TIMER4_Start(void);
void UB_TIMER4_Stop(void);
void UB_TIMER4_ISR_CallBack(void);  // Diese Funktion muss extern benutzt werden !!


//--------------------------------------------------------------
#endif // __STM32F4_UB_TIM4_H
