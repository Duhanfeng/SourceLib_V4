#ifndef  __HW_TIM_RECKON_H
#define  __HW_TIM_RECKON_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void TIM_REK_Init(TIM_TypeDef *TIMx);
  void TIMx_REK_Enable(TIM_TypeDef *TIMx, uint8_t isEnable);
  uint16_t TIMx_REK_GetCount(TIM_TypeDef *TIMx);
  void TIMx_REK_SetCount(TIM_TypeDef *TIMx, uint16_t nCount);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_RECKON_H */
