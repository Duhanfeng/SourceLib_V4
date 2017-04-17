#ifndef  __HW_TIM_OUTPUT_H
#define  __HW_TIM_OUTPUT_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void     TIMx_Output_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_OUTPUT_MODE Mode);    //��ʱ��x��ʼ��ΪPWM���ģʽ
  void     TIMx_Output_SetPwmDutyRatio(TIM_TYPE Timer, TIMx_CHANNEL_MASK cTimChMask, float dDutyRatio);     //��ʱ��xͨ����PWMռ�ձ����ú���
  void     TIMx_Output_SetCompareVal(TIM_TYPE Timer, TIMx_CHANNEL_MASK cTimChMask, uint16_t nCompareVal);   //��ʱ��xͨ����PWMռ�ձ����ú���
  void     TIMx_Output_SetPwmFrq(TIM_TYPE Timer, uint32_t uiFrq);       //���ö�ʱ��xPWM����Ƶ��(��ʼ����Ĭ������1KHz)
  void     TIMx_Output_SetAutoReloadReg(TIM_TYPE Timer, uint16_t nReloadVal);  //��ʱ��x�����Զ��ؼ���ֵ����
  uint16_t TIMx_Output_GetAutoReloadReg(TIM_TYPE Timer);        //��ȡ��ʱ��xARR�Ĵ����е��ؼ���ֵ
  void     TIMx_Output_Enable(TIM_TYPE Timer, uint8_t cFlag);   //��ʱ��xʹ�ܺ���(cFlag 0:�رն�ʱ�� 1:������ʱ��)
  void     TIMx_Output_IRQEnable(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, uint8_t isEnable);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_OUTPUT_H */
