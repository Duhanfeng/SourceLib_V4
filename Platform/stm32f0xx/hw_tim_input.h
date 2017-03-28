#ifndef  __HW_TIM_INPUT_H
#define  __HW_TIM_INPUT_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void TIMx_Input_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK ChannelMask, TIMx_INPUT_MODE Mode);
  uint16_t TIMx_Input_GetCount(TIM_TYPE Timer);
  uint16_t TIMx_Input_GetCaptureValue(TIM_TYPE Timer, TIMx_CHANNEL_MASK cChannel);

#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_INPUT_H */
