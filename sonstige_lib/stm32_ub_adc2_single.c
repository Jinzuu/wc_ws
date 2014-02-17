//--------------------------------------------------------------
// File     : stm32_ub_adc2_single.c
// Datum    : 16.02.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO,ADC
// Funktion : AD-Wandler (ADC2 im Single-Conversion-Mode)
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_adc2_single.h"


//--------------------------------------------------------------
// Definition der benutzten ADC Pins
// Reihenfolge wie bei ADC2s_NAME_t
//--------------------------------------------------------------
ADC2s_t ADC2s[] = {
  //NAME  ,PORT , PIN      , CLOCK              , Kanal        , Mittelwerte
  {ADC_PA3,GPIOA,GPIO_Pin_3,RCC_AHB1Periph_GPIOA,ADC_Channel_3 ,MW_32},   // ADC an PA3 = ADC123_IN3
  {ADC_PC4,GPIOC,GPIO_Pin_4,RCC_AHB1Periph_GPIOC,ADC_Channel_14,MW_64},   // ADC an PC4 = ADC12_IN14
  {ADC_PC5,GPIOC,GPIO_Pin_5,RCC_AHB1Periph_GPIOC,ADC_Channel_15,MW_NONE}, // ADC an PC5 = ADC12_IN15
};



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_ADC2s_InitIO(void);
void P_ADC2s_InitADC(void);


//--------------------------------------------------------------
// init vom ADC2 im Single-Conversion-Mode
//--------------------------------------------------------------
void UB_ADC2_SINGLE_Init(void)
{
  P_ADC2s_InitIO();
  P_ADC2s_InitADC();
}

//--------------------------------------------------------------
// starten einer Messung
// und auslesen des Messwertes
//--------------------------------------------------------------
uint16_t UB_ADC2_SINGLE_Read(ADC2s_NAME_t adc_name)
{
  uint16_t messwert=0;

  // Messkanal einrichten
  ADC_RegularChannelConfig(ADC2,ADC2s[adc_name].ADC_CH,1,ADC_SampleTime_3Cycles);
  // Messung starten
  ADC_SoftwareStartConv(ADC2);
  // warte bis Messung fertig ist
  while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==RESET);
  // Messwert auslesen
  messwert=ADC_GetConversionValue(ADC2);

  return(messwert);
}


//--------------------------------------------------------------
// starten von n-Messungen
// und auslesen vom Mittelwert
//--------------------------------------------------------------
uint16_t UB_ADC2_SINGLE_Read_MW(ADC2s_NAME_t adc_name)
{
  uint32_t mittelwert=0;
  uint16_t messwert,n;
  uint16_t anz_mw=1,anz_bit=0;

  if(ADC2s[adc_name].ADC_MW==MW_NONE) {
    anz_mw=1;anz_bit=0;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_2) {
    anz_mw=2;anz_bit=1;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_4) {
    anz_mw=4;anz_bit=2;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_8) {
    anz_mw=8;anz_bit=3;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_16) {
    anz_mw=16;anz_bit=4;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_32) {
    anz_mw=32;anz_bit=5;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_64) {
    anz_mw=64;anz_bit=6;
  }
  else if(ADC2s[adc_name].ADC_MW==MW_128) {
    anz_mw=128;anz_bit=7;
  }

  for(n=0;n<anz_mw;n++) {
    messwert=UB_ADC2_SINGLE_Read(adc_name);
    mittelwert+=messwert;
  }

  messwert=(mittelwert >> anz_bit);

  return(messwert);
}

//--------------------------------------------------------------
// interne Funktion
// Init aller IO-Pins
//--------------------------------------------------------------
void P_ADC2s_InitIO(void) {
  GPIO_InitTypeDef      GPIO_InitStructure;
  ADC2s_NAME_t adc_name;

  for(adc_name=0;adc_name<ADC2s_ANZ;adc_name++) {
    // Clock Enable
    RCC_AHB1PeriphClockCmd(ADC2s[adc_name].ADC_CLK, ENABLE);

    // Config des Pins als Analog-Eingang
    GPIO_InitStructure.GPIO_Pin = ADC2s[adc_name].ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(ADC2s[adc_name].ADC_PORT, &GPIO_InitStructure);
  }
}

//--------------------------------------------------------------
// interne Funktion
// Init von ADC Nr.2
//--------------------------------------------------------------
void P_ADC2s_InitADC(void)
{
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;

  // Clock Enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

  // ADC-Config
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC2s_VORTEILER;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  // ADC-Enable
  ADC_Cmd(ADC2, ENABLE);
}



