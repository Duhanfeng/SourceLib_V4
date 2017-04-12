#ifndef  __HW_TIM_OUTPUT_H
#define  __HW_TIM_OUTPUT_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void     TIMx_Output_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_OUTPUT_MODE Mode);    //定时器x初始化为PWM输出模式
  void     TIMx_Output_SetPwmDutyRatio(TIM_TYPE Timer, TIMx_CHANNEL_MASK cTimChMask, float dDutyRatio);     //定时器x通道的PWM占空比设置函数
  void     TIMx_Output_SetCompareVal(TIM_TYPE Timer, TIMx_CHANNEL_MASK cTimChMask, uint16_t nCompareVal);   //定时器x通道的PWM占空比设置函数
  void     TIMx_Output_SetPwmFrq(TIM_TYPE Timer, uint32_t uiFrq);       //设置定时器xPWM波的频率(初始化中默认配置1KHz)
  void     TIMx_Output_SetAutoReloadReg(TIM_TYPE Timer, uint16_t nReloadVal);  //定时器x设置自动重加载值函数
  uint16_t TIMx_Output_GetAutoReloadReg(TIM_TYPE Timer);        //获取定时器xARR寄存器中的重加载值
  void     TIMx_Output_Enable(TIM_TYPE Timer, uint8_t cFlag);   //定时器x使能函数(cFlag 0:关闭定时器 1:开启定时器)
  void     TIMx_Output_IRQEnable(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, uint8_t isEnable);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_OUTPUT_H */
