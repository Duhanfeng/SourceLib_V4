#ifndef  __HW_TIM_ITR_H
#define  __HW_TIM_ITR_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void TIMx_Itr_Init(TIM_TypeDef *TIMx, uint16_t nms, uint8_t isOnePulse);
  void TIMx_Itr_Enable(TIM_TypeDef *TIMx, uint8_t isEnable);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_TIM_ITR_H */
