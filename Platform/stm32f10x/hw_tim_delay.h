#ifndef  __HW_TIM_DELAY_H
#define  __HW_TIM_DELAY_H

#include "hw_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

  void TIMx_DelayInit(TIM_TypeDef *TIMx);
  void TIMx_DelayUs(TIM_TypeDef *TIMx, uint16_t nus);
  void TIMx_DelayMs(TIM_TypeDef *TIMx, uint16_t nms);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_TIM_DELAY_H */
