//--------------------------------------------------------------
// File     : stm32_ub_tim2.c
// Datum    : 20.08.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : TIM, MISC
// Funktion : Timer-Funktionen per Timer2
//            (mit Callback-Funktion für externe ISR)
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_tim2.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_TIM2_TIMER(uint16_t prescaler, uint16_t periode);
void P_TIM2_NVIC(void);
//--------------------------------------------------------------
uint32_t tim2_enable_flag=0;



//--------------------------------------------------------------
// Init und Stop vom Timer mit Vorteiler und Counterwert
// prescaler : [0...65535]
// periode   : [0...65535]
//
// F_TIM = 84 MHz / (prescaler+1) / (periode+1)
//--------------------------------------------------------------
void UB_TIMER2_Init(uint16_t prescaler, uint16_t periode)
{
  // Timer flag löschen
  tim2_enable_flag=0;
  // Timer einstellen
  P_TIM2_TIMER(prescaler, periode);
  P_TIM2_NVIC();
}

//--------------------------------------------------------------
// Init und Stop vom Timer mit Vorgabe in Hertz
//--------------------------------------------------------------
void UB_TIMER2_Init_WithFrequency( int desiredFreqInHz )
{
	// Hole Taktfrequenz
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    int inputClkFreq = RCC_Clocks.PCLK2_Frequency;

    // Berechne prescaler und periode
    int frequencyDivNecessary = (float) inputClkFreq / desiredFreqInHz;
    frequencyDivNecessary = frequencyDivNecessary >> 16;	// Schiebe 16 Bit nach links, da 16 Bit Periode moeglich
    int timerMaxCountValue = ( inputClkFreq / (frequencyDivNecessary + 1) ) / desiredFreqInHz;

	// Setze Timer auf
	UB_TIMER2_Init(frequencyDivNecessary, timerMaxCountValue);		// Todo: Eigentlich wäre 32 bit periode möglich
    //UB_TIMER2_Init(0, 1000 );
}

//--------------------------------------------------------------
// Timer starten
//--------------------------------------------------------------
void UB_TIMER2_Start(void)
{
  // Timer enable
  TIM_Cmd(TIM2, ENABLE);
  // Timer flag setzen
  tim2_enable_flag=1;
}


//--------------------------------------------------------------
// Timer anhalten
//--------------------------------------------------------------
void UB_TIMER2_Stop(void)
{
  // Timer flag löschen
  tim2_enable_flag=0;
  // Timer disable
  TIM_Cmd(TIM2, DISABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Timer
//--------------------------------------------------------------
void P_TIM2_TIMER(uint16_t prescaler, uint16_t periode)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // Timer disable
  TIM_Cmd(TIM2, DISABLE);

  // Timer init
  TIM_TimeBaseStructure.TIM_Period =  periode;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  // Timer preload enable
  TIM_ARRPreloadConfig(TIM2, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Interrupt
//--------------------------------------------------------------
void P_TIM2_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // Update Interrupt enable
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
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
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    // wenn Interrupt aufgetreten
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    // Callback Funktion aufrufen
    // Diese Funktion muss extern vom User benutzt werden !!
    if(tim2_enable_flag!=0) {
      // nur wenn Timer aktiviert ist
      UB_TIMER2_ISR_CallBack();
    }
  }
}
