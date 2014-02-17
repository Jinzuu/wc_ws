/*
//--------------------------------------------------------------
// File     : stm32_ub_tim3.c
// Datum    : 20.08.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : TIM, MISC
// Funktion : Timer-Funktionen per Timer3
//            (mit Callback-Funktion für externe ISR)
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_tim3.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_TIM3_TIMER(uint16_t prescaler, uint16_t periode);
void P_TIM3_NVIC(void);
//--------------------------------------------------------------
uint32_t tim3_enable_flag=0;



//--------------------------------------------------------------
// Init und Stop vom Timer mit Vorteiler und Counterwert
// prescaler : [0...65535]
// periode   : [0...65535]
//
// F_TIM = 84 MHz / (prescaler+1) / (periode+1)
//--------------------------------------------------------------
void UB_TIMER3_Init(uint16_t prescaler, uint16_t periode)
{
  // Timer flag löschen
  tim3_enable_flag=0;
  // Timer einstellen
  P_TIM3_TIMER(prescaler, periode);
  P_TIM3_NVIC();
}


//--------------------------------------------------------------
// Timer starten
//--------------------------------------------------------------
void UB_TIMER3_Start(void)
{
  // Timer enable
  TIM_Cmd(TIM3, ENABLE);
  // Timer flag setzen
  tim3_enable_flag=1;
}


//--------------------------------------------------------------
// Timer anhalten
//--------------------------------------------------------------
void UB_TIMER3_Stop(void)
{
  // Timer flag löschen
  tim3_enable_flag=0;
  // Timer disable
  TIM_Cmd(TIM3, DISABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Timer
//--------------------------------------------------------------
void P_TIM3_TIMER(uint16_t prescaler, uint16_t periode)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // Timer disable
  TIM_Cmd(TIM3, DISABLE);

  // Timer init
  TIM_TimeBaseStructure.TIM_Period =  periode;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  // Timer preload enable
  TIM_ARRPreloadConfig(TIM3, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Interrupt
//--------------------------------------------------------------
void P_TIM3_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // Update Interrupt enable
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



//--------------------------------------------------------------
// Timer-Interrupt
// wird beim erreichen vom Counterwert aufgerufen
// die Callback funktion muss extern benutzt werden
//--------------------------------------------------------------
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    // wenn Interrupt aufgetreten
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    // Callback Funktion aufrufen
    // Diese Funktion muss extern vom User benutzt werden !!
    if(tim3_enable_flag!=0) {
      // nur wenn Timer aktiviert ist
      UB_TIMER3_ISR_CallBack();
    }
  }
}
*/
