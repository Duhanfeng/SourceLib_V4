#ifndef  __HW_TIM_DELAY_H
#define  __HW_TIM_DELAY_H

#include "hw_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

  void TIMx_DelayInit(TIM_TYPE Timer);
  void TIMx_DelayUs(uint32_t nus);
  void TIMx_DelayMs(uint32_t nus);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_DELAY_H */
