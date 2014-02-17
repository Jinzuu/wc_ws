//--------------------------------------------------------------
// File     : stm32_ub_tim4.c
// Datum    : 20.08.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : TIM, MISC
// Funktion : Timer-Funktionen per Timer4
//            (mit Callback-Funktion für externe ISR)
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_tim4.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_TIM4_TIMER(uint16_t prescaler, uint16_t periode);
void P_TIM4_NVIC(void);
//--------------------------------------------------------------
uint32_t tim4_enable_flag=0;



//--------------------------------------------------------------
// Init und Stop vom Timer mit Vorteiler und Counterwert
// prescaler : [0...65535]
// periode   : [0...65535]
//
// F_TIM = 84 MHz / (prescaler+1) / (periode+1)
//--------------------------------------------------------------
void UB_TIMER4_Init(uint16_t prescaler, uint16_t periode)
{
  // Timer flag löschen
  tim4_enable_flag=0;
  // Timer einstellen
  P_TIM4_TIMER(prescaler, periode);
  P_TIM4_NVIC();
}


//--------------------------------------------------------------
// Timer starten
//--------------------------------------------------------------
void UB_TIMER4_Start(void)
{
  // Timer enable
  TIM_Cmd(TIM4, ENABLE);
  // Timer flag setzen
  tim4_enable_flag=1;
}


//--------------------------------------------------------------
// Timer anhalten
//--------------------------------------------------------------
void UB_TIMER4_Stop(void)
{
  // Timer flag löschen
  tim4_enable_flag=0;
  // Timer disable
  TIM_Cmd(TIM4, DISABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Timer
//--------------------------------------------------------------
void P_TIM4_TIMER(uint16_t prescaler, uint16_t periode)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  // Timer disable
  TIM_Cmd(TIM4, DISABLE);

  // Timer init
  TIM_TimeBaseStructure.TIM_Period =  periode;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  // Timer preload enable
  TIM_ARRPreloadConfig(TIM4, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Interrupt
//--------------------------------------------------------------
void P_TIM4_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // Update Interrupt enable
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
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
void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
    // wenn Interrupt aufgetreten
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    // Callback Funktion aufrufen
    // Diese Funktion muss extern vom User benutzt werden !!
    if(tim4_enable_flag!=0) {
      // nur wenn Timer aktiviert ist
      UB_TIMER4_ISR_CallBack();
    }
  }
}
