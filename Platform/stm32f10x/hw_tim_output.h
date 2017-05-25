#ifndef  __HW_TIM_OUTPUT_H
#define  __HW_TIM_OUTPUT_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void     TIMx_Output_Init(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_OUTPUT_MODE Mode);    //��ʱ��x��ʼ��ΪPWM���ģʽ
  void     TIMx_Output_SetPwmDutyRatio(TIM_TypeDef *TIMx, uint8_t cTimChMask, float dDutyRatio);     //��ʱ��xͨ����PWMռ�ձ����ú���
  void     TIMx_Output_SetCompareVal(TIM_TypeDef *TIMx, uint8_t cTimChMask, uint16_t nCompareVal);   //��ʱ��xͨ����PWMռ�ձ����ú���
  void     TIMx_Output_SetPwmFrq(TIM_TypeDef *TIMx, uint32_t uiFrq);       //���ö�ʱ��xPWM����Ƶ��(��ʼ����Ĭ������1KHz)
  void     TIMx_Output_SetAutoReloadReg(TIM_TypeDef *TIMx, uint16_t nReloadVal);  //��ʱ��x�����Զ��ؼ���ֵ����
  uint16_t TIMx_Output_GetAutoReloadReg(TIM_TypeDef *TIMx);        //��ȡ��ʱ��xARR�Ĵ����е��ؼ���ֵ
  void     TIMx_Output_Enable(TIM_TypeDef *TIMx, uint8_t cFlag);   //��ʱ��xʹ�ܺ���(cFlag 0:�رն�ʱ�� 1:������ʱ��)
  void     TIMx_Output_IRQEnable(TIM_TypeDef *TIMx, uint8_t ChMask, uint8_t isEnable);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_TIM_OUTPUT_H */
