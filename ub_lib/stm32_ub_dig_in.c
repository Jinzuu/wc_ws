//--------------------------------------------------------------
// File     : stm32_ub_dig_in.c
// Datum    : 07.10.2013
// Version  : 1.2
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : GPIO, (TIM, MISC)
// Funktion : Digital_In Funktionen
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_dig_in.h"


//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
#if DIN_USE_TIMER==1
  void P_DIN_InitTIM(void);
  void P_DIN_InitNVIC(void);
#endif




//--------------------------------------------------------------
// Definition aller Digital-In Pins
// Reihenfolge wie bei DIN_NAME_t
//
// Widerstand : [GPIO_PuPd_NOPULL,GPIO_PuPd_UP,GPIO_PuPd_DOWN]
//--------------------------------------------------------------
DIN_PIN_t DIN_PIN[] = {
  // Name    ,PORT , PIN       , CLOCK              ,Widerstand , Status
  {DIN_PA15	,GPIOA,GPIO_Pin_15,RCC_AHB1Periph_GPIOA,GPIO_PuPd_UP,Bit_RESET},
  {DIN_PC8  ,GPIOC,GPIO_Pin_8 ,RCC_AHB1Periph_GPIOC,GPIO_PuPd_UP,Bit_RESET},
  {DIN_PD11 ,GPIOD,GPIO_Pin_11,RCC_AHB1Periph_GPIOD,GPIO_PuPd_UP,Bit_RESET},
  {DIN_PE4  ,GPIOE,GPIO_Pin_4 ,RCC_AHB1Periph_GPIOE,GPIO_PuPd_UP,Bit_RESET},
};


//--------------------------------------------------------------
// Init aller Digital-In Pins
//--------------------------------------------------------------
void UB_DigIn_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  DIN_NAME_t dig_pin;
  
  for(dig_pin=0;dig_pin<DIN_ANZ;dig_pin++) {
    // Clock Enable
    RCC_AHB1PeriphClockCmd(DIN_PIN[dig_pin].GPIO_CLK, ENABLE);  
  
    // Config als Digital-Eingang
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = DIN_PIN[dig_pin].GPIO_R;
    GPIO_InitStructure.GPIO_Pin = DIN_PIN[dig_pin].GPIO_PIN;
    GPIO_Init(DIN_PIN[dig_pin].GPIO_PORT, &GPIO_InitStructure);   
  }

  #if DIN_USE_TIMER==1
    // Init vom Timer
    P_DIN_InitTIM();
    // Init vom NVIC
    P_DIN_InitNVIC();
  #endif
}


//--------------------------------------------------------------
// Pegel von Digital-In Pin auslesen (nicht entprellt)
// Return Wert :
//  -> bei Lo-Pegel = Bit_RESET
//  -> bei Hi-Pegel = Bit_SET
//--------------------------------------------------------------
BitAction UB_DigIn_Read(DIN_NAME_t dig_pin)
{
  BitAction wert;

  wert=GPIO_ReadInputDataBit(DIN_PIN[dig_pin].GPIO_PORT, DIN_PIN[dig_pin].GPIO_PIN);
  return(wert);
} 


#if DIN_USE_TIMER==1
//--------------------------------------------------------------
// Digital-In OnHiLevel Auswertung (entprellt)
// ret_wert, ist solange true wie der GPIO-Pin Hi-Pegel hat
//--------------------------------------------------------------
bool UB_DigIn_OnHiLevel(DIN_NAME_t dig_pin)
{
  uint8_t wert;

  wert=DIN_PIN[dig_pin].GPIO_AKT;

  if(wert==Bit_RESET) {
    return(false);
  }
  else {
    return(true);
  }
}
#endif


#if DIN_USE_TIMER==1
//--------------------------------------------------------------
// Digital-In OnHiEdge Auswertung (entprellt)
// ret_wert, ist nur einmal true nach Hi-Flanke am GPIO-Pin
//--------------------------------------------------------------
bool UB_DigIn_OnHiEdge(DIN_NAME_t dig_pin)
{
  uint8_t wert,old;
  static uint8_t old_wert[DIN_ANZ];

  wert=DIN_PIN[dig_pin].GPIO_AKT;
  old=old_wert[dig_pin];
  old_wert[dig_pin]=wert;

  if(wert==Bit_RESET) {
    return(false);
  }
  else if(old!=Bit_RESET) {
    return(false);
  }
  else {
    return(true);
  }

}
#endif


#if DIN_USE_TIMER==1
//--------------------------------------------------------------
// Digital-In OnLoEdge Auswertung (entprellt)
// ret_wert, ist nur einmal true nach Lo-Flanke am GPIO-Pin
//--------------------------------------------------------------
bool UB_DigIn_OnLoEdge(DIN_NAME_t dig_pin)
{
  uint8_t wert,old;
  static uint8_t old_wert[DIN_ANZ];

  wert=DIN_PIN[dig_pin].GPIO_AKT;
  old=old_wert[dig_pin];
  old_wert[dig_pin]=wert;

  if(wert!=Bit_RESET) {
    return(false);
  }
  else if(old==Bit_RESET) {
    return(false);
  }
  else {
    return(true);
  }

}
#endif


#if DIN_USE_TIMER==1
//--------------------------------------------------------------
// interne Funktion
// init vom Timer
//--------------------------------------------------------------
void P_DIN_InitTIM(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(UB_DIN_TIM_CLK, ENABLE);

  // Timer init
  TIM_TimeBaseStructure.TIM_Period =  UB_DIN_TIM_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = UB_DIN_TIM_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(UB_DIN_TIM, &TIM_TimeBaseStructure);

  // Timer enable
  TIM_ARRPreloadConfig(UB_DIN_TIM, ENABLE);
  TIM_Cmd(UB_DIN_TIM, ENABLE);
}
#endif


#if DIN_USE_TIMER==1
//--------------------------------------------------------------
// interne Funktion
// init vom NVIC
//--------------------------------------------------------------
void P_DIN_InitNVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  //---------------------------------------------
  // init vom Timer Interrupt
  //---------------------------------------------
  TIM_ITConfig(UB_DIN_TIM,TIM_IT_Update,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = UB_DIN_TIM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif


#if DIN_USE_TIMER==1
//--------------------------------------------------------------
// ISR vom Timer
//--------------------------------------------------------------
void UB_DIN_TIM_ISR_HANDLER(void)
{
  DIN_NAME_t dig_pin;
  uint8_t wert;

  // es gibt hier nur eine Interrupt Quelle
  TIM_ClearITPendingBit(UB_DIN_TIM, TIM_IT_Update);

  // alle Eingaenge einlesen
  for(dig_pin=0;dig_pin<DIN_ANZ;dig_pin++) {
    wert=GPIO_ReadInputDataBit(DIN_PIN[dig_pin].GPIO_PORT, DIN_PIN[dig_pin].GPIO_PIN);
    DIN_PIN[dig_pin].GPIO_AKT=wert;
  }
}
#endif

