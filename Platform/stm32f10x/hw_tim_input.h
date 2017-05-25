#ifndef  __HW_TIM_INPUT_H
#define  __HW_TIM_INPUT_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void TIMx_Input_Init(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_INPUT_MODE Mode);
  void TIMx_Input_IRQEnable(TIM_TypeDef *TIMx, uint8_t ChMask, uint8_t isEnable);
  uint16_t TIMx_Input_GetCount(TIM_TypeDef *TIMx);
  uint16_t TIMx_Input_GetCaptureValue(TIM_TypeDef *TIMx, uint8_t ChMask);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_INPUT_H */
