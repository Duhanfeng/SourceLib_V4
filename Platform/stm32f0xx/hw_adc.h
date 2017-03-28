#ifndef  __HW_ADC_H
#define  __HW_ADC_H

#include "stm32f0xx.h"

 

#ifdef __cplusplus
extern "C" { 
#endif

void     ADC_Init(void);
void     ADC_StartConversion(void);
uint16_t ADC_GetConversionVal(void);
float    ADC_GetTemperature(void);

#ifdef __cplusplus
}
#endif


#endif /* __HW_ADC_H */
