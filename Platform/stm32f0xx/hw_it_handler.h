#ifndef  __HW_IT_HANDLER_H
#define  __HW_IT_HANDLER_H


typedef void (*IT_CALLBACK_FUNC)(void);


typedef enum
{
  //IT_IRQ
  IT_OS_ITR_ENTER,
  IT_OS_ITR_LEAVE,
  
  //ϵͳ�δ�
  IT_SysTick_SL,
  IT_SysTick_OS,
  IT_SysTick_TASK,
  
  //�ⲿ�ж�
  IT_EXTI_0,
  IT_EXTI_1,
  IT_EXTI_2,
  IT_EXTI_3,
  IT_EXTI_4,
  IT_EXTI_5,
  IT_EXTI_6,
  IT_EXTI_7,
  IT_EXTI_8,
  IT_EXTI_9,
  IT_EXTI_10,
  IT_EXTI_11,
  IT_EXTI_12,
  IT_EXTI_13,
  IT_EXTI_14,
  IT_EXTI_15,
  
  //�߼���ʱ��1
  IT_TIM1_UIF,
  IT_TIM1_CC1IF,
  IT_TIM1_CC2IF,
  IT_TIM1_CC3IF,
  IT_TIM1_CC4IF,
  IT_TIM1_TIF,
  IT_TIM1_BIF,
  
  //�߼���ʱ��8
  IT_TIM8_UIF,
  IT_TIM8_CC1IF,
  IT_TIM8_CC2IF,
  IT_TIM8_CC3IF,
  IT_TIM8_CC4IF,
  IT_TIM8_TIF,
  IT_TIM8_BIF,
  
  //ͨ�ö�ʱ��2
  IT_TIM2_UIF,
  IT_TIM2_CC1IF,
  IT_TIM2_CC2IF,
  IT_TIM2_CC3IF,
  IT_TIM2_CC4IF,
  
  //ͨ�ö�ʱ��3
  IT_TIM3_UIF,
  IT_TIM3_CC1IF,
  IT_TIM3_CC2IF,
  IT_TIM3_CC3IF,
  IT_TIM3_CC4IF,
  
  //ͨ�ö�ʱ��4
  IT_TIM4_UIF,
  IT_TIM4_CC1IF,
  IT_TIM4_CC2IF,
  IT_TIM4_CC3IF,
  IT_TIM4_CC4IF,
  
  //ͨ�ö�ʱ��5
  IT_TIM5_UIF,
  IT_TIM5_CC1IF,
  IT_TIM5_CC2IF,
  IT_TIM5_CC3IF,
  IT_TIM5_CC4IF,
  
  //������ʱ��6,7
  IT_TIM6_UIF,
  IT_TIM7_UIF,
  
  //����
  IT_USART1,
  IT_USART2,
  IT_USART3,
  IT_USART4,
  
  IT_USART1_IDLE,
  IT_USART2_IDLE,
  IT_USART3_IDLE,
  IT_USART4_IDLE,
  
  //DMAͨ��
  IT_DMA1_CH1,
  IT_DMA1_CH2,
  IT_DMA1_CH3,
  IT_DMA1_CH4,
  IT_DMA1_CH5,
  IT_DMA1_CH6,
  IT_DMA1_CH7,
  IT_DMA2_CH1,
  IT_DMA2_CH2,
  IT_DMA2_CH3,
  IT_DMA2_CH4,
  IT_DMA2_CH5,
  
}IT_LOGIN_CODE;


#ifdef __cplusplus
extern "C" {
#endif
  
  void IT_IRQ_FuncLogin(IT_CALLBACK_FUNC ptr, IT_LOGIN_CODE Code);


#ifdef __cplusplus
}
#endif


#endif /* __HW_IT_HANDLER_H */
