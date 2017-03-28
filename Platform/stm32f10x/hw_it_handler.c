/**
  ******************************************************************************
  * @file    hw_it_handler.c
  * @author  �Ź��Ӻ���
  * @version V1.0 �Ĵ����汾
  * @date    2017.02.10
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "hw_it_handler.h"
#include "stm32f10x.h"

#define USING_OS  (1)

/* ---����жϻص�����ָ�붨��--- */
static IT_CALLBACK_FUNC SL_ItrEnter;
static IT_CALLBACK_FUNC SL_ItrLeave;


//ϵͳ�δ��ж�
static IT_CALLBACK_FUNC pSysTick_SlTick_CallBackFunc;
static IT_CALLBACK_FUNC pSysTick_OsTick_CallBackFunc;
static IT_CALLBACK_FUNC pSysTick_Task_CallBackFunc;

//�ⲿ�ж�
static IT_CALLBACK_FUNC pExti0_CallBackFunc;
static IT_CALLBACK_FUNC pExti1_CallBackFunc;
static IT_CALLBACK_FUNC pExti2_CallBackFunc;
static IT_CALLBACK_FUNC pExti3_CallBackFunc;
static IT_CALLBACK_FUNC pExti4_CallBackFunc;
static IT_CALLBACK_FUNC pExti5_CallBackFunc;
static IT_CALLBACK_FUNC pExti6_CallBackFunc;
static IT_CALLBACK_FUNC pExti7_CallBackFunc;
static IT_CALLBACK_FUNC pExti8_CallBackFunc;
static IT_CALLBACK_FUNC pExti9_CallBackFunc;
static IT_CALLBACK_FUNC pExti10_CallBackFunc;
static IT_CALLBACK_FUNC pExti11_CallBackFunc;
static IT_CALLBACK_FUNC pExti12_CallBackFunc;
static IT_CALLBACK_FUNC pExti13_CallBackFunc;
static IT_CALLBACK_FUNC pExti14_CallBackFunc;
static IT_CALLBACK_FUNC pExti15_CallBackFunc;

//��ʱ���ж�
static IT_CALLBACK_FUNC pTim1_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim1_Cc1if_CallBackFunc;
static IT_CALLBACK_FUNC pTim1_Cc2if_CallBackFunc;
static IT_CALLBACK_FUNC pTim1_Cc3if_CallBackFunc;
static IT_CALLBACK_FUNC pTim1_Cc4if_CallBackFunc;
static IT_CALLBACK_FUNC pTim1_Tif_CallBackFunc;
static IT_CALLBACK_FUNC pTim1_Bif_CallBackFunc;

static IT_CALLBACK_FUNC pTim8_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim8_Cc1if_CallBackFunc;
static IT_CALLBACK_FUNC pTim8_Cc2if_CallBackFunc;
static IT_CALLBACK_FUNC pTim8_Cc3if_CallBackFunc;
static IT_CALLBACK_FUNC pTim8_Cc4if_CallBackFunc;
static IT_CALLBACK_FUNC pTim8_Tif_CallBackFunc;
static IT_CALLBACK_FUNC pTim8_Bif_CallBackFunc;

static IT_CALLBACK_FUNC pTim2_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim2_Cc1if_CallBackFunc;
static IT_CALLBACK_FUNC pTim2_Cc2if_CallBackFunc;
static IT_CALLBACK_FUNC pTim2_Cc3if_CallBackFunc;
static IT_CALLBACK_FUNC pTim2_Cc4if_CallBackFunc;

static IT_CALLBACK_FUNC pTim3_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim3_Cc1if_CallBackFunc;
static IT_CALLBACK_FUNC pTim3_Cc2if_CallBackFunc;
static IT_CALLBACK_FUNC pTim3_Cc3if_CallBackFunc;
static IT_CALLBACK_FUNC pTim3_Cc4if_CallBackFunc;

static IT_CALLBACK_FUNC pTim4_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim4_Cc1if_CallBackFunc;
static IT_CALLBACK_FUNC pTim4_Cc2if_CallBackFunc;
static IT_CALLBACK_FUNC pTim4_Cc3if_CallBackFunc;
static IT_CALLBACK_FUNC pTim4_Cc4if_CallBackFunc;

static IT_CALLBACK_FUNC pTim5_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim5_Cc1if_CallBackFunc;
static IT_CALLBACK_FUNC pTim5_Cc2if_CallBackFunc;
static IT_CALLBACK_FUNC pTim5_Cc3if_CallBackFunc;
static IT_CALLBACK_FUNC pTim5_Cc4if_CallBackFunc;

static IT_CALLBACK_FUNC pTim6_Uif_CallBackFunc;
static IT_CALLBACK_FUNC pTim7_Uif_CallBackFunc;

//�����ж�
static IT_CALLBACK_FUNC pUsart1_Idle_CallBackFunc;
static IT_CALLBACK_FUNC pUsart2_Idle_CallBackFunc;
static IT_CALLBACK_FUNC pUsart3_Idle_CallBackFunc;
static IT_CALLBACK_FUNC pUsart4_Idle_CallBackFunc;

static IT_CALLBACK_FUNC pUsart1_CallBackFunc;
static IT_CALLBACK_FUNC pUsart2_CallBackFunc;
static IT_CALLBACK_FUNC pUsart3_CallBackFunc;
static IT_CALLBACK_FUNC pUsart4_CallBackFunc;

//DMAͨ��
static IT_CALLBACK_FUNC pDma1_CH1_CallBackFunc;
static IT_CALLBACK_FUNC pDma1_CH2_CallBackFunc;
static IT_CALLBACK_FUNC pDma1_CH3_CallBackFunc;
static IT_CALLBACK_FUNC pDma1_CH4_CallBackFunc;
static IT_CALLBACK_FUNC pDma1_CH5_CallBackFunc;
static IT_CALLBACK_FUNC pDma1_CH6_CallBackFunc;
static IT_CALLBACK_FUNC pDma1_CH7_CallBackFunc;
static IT_CALLBACK_FUNC pDma2_CH1_CallBackFunc;
static IT_CALLBACK_FUNC pDma2_CH2_CallBackFunc;
static IT_CALLBACK_FUNC pDma2_CH3_CallBackFunc;
static IT_CALLBACK_FUNC pDma2_CH4_CallBackFunc;
static IT_CALLBACK_FUNC pDma2_CH5_CallBackFunc;

/*----------------------------------------------------------------------------
    �жϻص�����ע�ắ��
 *----------------------------------------------------------------------------*/

void IT_IRQ_FuncLogin(IT_CALLBACK_FUNC ptr, IT_LOGIN_CODE Code)
{
  switch (Code)
  {
    //����/�˳��ж�ʱ�ص�,������ʹ��OS�ĳ���
    case IT_OS_ITR_ENTER: SL_ItrEnter = ptr;
    case IT_OS_ITR_LEAVE: SL_ItrLeave = ptr;
    
    //ϵͳ�δ��жϻص�����
    case IT_SysTick_SL:   pSysTick_SlTick_CallBackFunc = ptr; break;
    case IT_SysTick_OS:   pSysTick_OsTick_CallBackFunc = ptr; break;
    case IT_SysTick_TASK: pSysTick_Task_CallBackFunc = ptr; break;
    
    //�ⲿ�жϻص�����
    case IT_EXTI_0:  pExti0_CallBackFunc   = ptr;    break;
    case IT_EXTI_1:  pExti1_CallBackFunc   = ptr;    break;
    case IT_EXTI_2:  pExti2_CallBackFunc   = ptr;    break;
    case IT_EXTI_3:  pExti3_CallBackFunc   = ptr;    break;
    case IT_EXTI_4:  pExti4_CallBackFunc   = ptr;    break;
    case IT_EXTI_5:  pExti5_CallBackFunc   = ptr;    break;
    case IT_EXTI_6:  pExti6_CallBackFunc   = ptr;    break;
    case IT_EXTI_7:  pExti7_CallBackFunc   = ptr;    break;
    case IT_EXTI_8:  pExti8_CallBackFunc   = ptr;    break;
    case IT_EXTI_9:  pExti9_CallBackFunc   = ptr;    break;
    case IT_EXTI_10: pExti10_CallBackFunc  = ptr;    break;
    case IT_EXTI_11: pExti11_CallBackFunc  = ptr;    break;
    case IT_EXTI_12: pExti12_CallBackFunc  = ptr;    break;
    case IT_EXTI_13: pExti13_CallBackFunc  = ptr;    break;
    case IT_EXTI_14: pExti14_CallBackFunc  = ptr;    break;
    case IT_EXTI_15: pExti15_CallBackFunc  = ptr;    break;
    
    //��ʱ���жϻص�����
    case IT_TIM1_UIF:     pTim1_Uif_CallBackFunc   = ptr; break;
    case IT_TIM1_CC1IF:   pTim1_Cc1if_CallBackFunc = ptr; break;
    case IT_TIM1_CC2IF:   pTim1_Cc2if_CallBackFunc = ptr; break;
    case IT_TIM1_CC3IF:   pTim1_Cc3if_CallBackFunc = ptr; break;
    case IT_TIM1_CC4IF:   pTim1_Cc4if_CallBackFunc = ptr; break;
    case IT_TIM1_TIF:     pTim1_Tif_CallBackFunc   = ptr; break;
    case IT_TIM1_BIF:     pTim1_Bif_CallBackFunc   = ptr; break;
    
    case IT_TIM8_UIF:     pTim8_Uif_CallBackFunc   = ptr; break;
    case IT_TIM8_CC1IF:   pTim8_Cc1if_CallBackFunc = ptr; break;
    case IT_TIM8_CC2IF:   pTim8_Cc2if_CallBackFunc = ptr; break;
    case IT_TIM8_CC3IF:   pTim8_Cc3if_CallBackFunc = ptr; break;
    case IT_TIM8_CC4IF:   pTim8_Cc4if_CallBackFunc = ptr; break;
    case IT_TIM8_TIF:     pTim8_Tif_CallBackFunc   = ptr; break;
    case IT_TIM8_BIF:     pTim8_Bif_CallBackFunc   = ptr; break;
    
    case IT_TIM2_UIF  :   pTim2_Uif_CallBackFunc   = ptr; break;
    case IT_TIM2_CC1IF:   pTim2_Cc1if_CallBackFunc = ptr; break;
    case IT_TIM2_CC2IF:   pTim2_Cc2if_CallBackFunc = ptr; break;
    case IT_TIM2_CC3IF:   pTim2_Cc3if_CallBackFunc = ptr; break;
    case IT_TIM2_CC4IF:   pTim2_Cc4if_CallBackFunc = ptr; break;
    
    case IT_TIM3_UIF  :   pTim3_Uif_CallBackFunc   = ptr; break;
    case IT_TIM3_CC1IF:   pTim3_Cc1if_CallBackFunc = ptr; break;
    case IT_TIM3_CC2IF:   pTim3_Cc2if_CallBackFunc = ptr; break;
    case IT_TIM3_CC3IF:   pTim3_Cc3if_CallBackFunc = ptr; break;
    case IT_TIM3_CC4IF:   pTim3_Cc4if_CallBackFunc = ptr; break;
    
    case IT_TIM4_UIF  :   pTim4_Uif_CallBackFunc   = ptr; break;
    case IT_TIM4_CC1IF:   pTim4_Cc1if_CallBackFunc = ptr; break;
    case IT_TIM4_CC2IF:   pTim4_Cc2if_CallBackFunc = ptr; break;
    case IT_TIM4_CC3IF:   pTim4_Cc3if_CallBackFunc = ptr; break;
    case IT_TIM4_CC4IF:   pTim4_Cc4if_CallBackFunc = ptr; break;
    
    case IT_TIM5_UIF  :   pTim5_Uif_CallBackFunc   = ptr; break;
    case IT_TIM5_CC1IF:   pTim5_Cc1if_CallBackFunc = ptr; break;
    case IT_TIM5_CC2IF:   pTim5_Cc2if_CallBackFunc = ptr; break;
    case IT_TIM5_CC3IF:   pTim5_Cc3if_CallBackFunc = ptr; break;
    case IT_TIM5_CC4IF:   pTim5_Cc4if_CallBackFunc = ptr; break;
    
    case IT_TIM6_UIF  :   pTim6_Uif_CallBackFunc   = ptr; break;
    case IT_TIM7_UIF  :   pTim7_Uif_CallBackFunc   = ptr; break;
    
    //�����жϻص�����
    case IT_USART1:       pUsart1_CallBackFunc = ptr; break;
    case IT_USART2:       pUsart2_CallBackFunc = ptr; break;
    case IT_USART3:       pUsart3_CallBackFunc = ptr; break;
    case IT_USART4:       pUsart4_CallBackFunc = ptr; break;
    
    case IT_USART1_IDLE:  pUsart1_Idle_CallBackFunc = ptr; break;
    case IT_USART2_IDLE:  pUsart2_Idle_CallBackFunc = ptr; break;
    case IT_USART3_IDLE:  pUsart3_Idle_CallBackFunc = ptr; break;
    case IT_USART4_IDLE:  pUsart4_Idle_CallBackFunc = ptr; break;
    
    //DMAͨ���жϻص�����
    case IT_DMA1_CH1: pDma1_CH1_CallBackFunc = ptr; break;
    case IT_DMA1_CH2: pDma1_CH2_CallBackFunc = ptr; break;
    case IT_DMA1_CH3: pDma1_CH3_CallBackFunc = ptr; break;
    case IT_DMA1_CH4: pDma1_CH4_CallBackFunc = ptr; break;
    case IT_DMA1_CH5: pDma1_CH5_CallBackFunc = ptr; break;
    case IT_DMA1_CH6: pDma1_CH6_CallBackFunc = ptr; break;
    case IT_DMA1_CH7: pDma1_CH7_CallBackFunc = ptr; break;
    case IT_DMA2_CH1: pDma2_CH1_CallBackFunc = ptr; break;
    case IT_DMA2_CH2: pDma2_CH2_CallBackFunc = ptr; break;
    case IT_DMA2_CH3: pDma2_CH3_CallBackFunc = ptr; break;
    case IT_DMA2_CH4: pDma2_CH4_CallBackFunc = ptr; break;
    case IT_DMA2_CH5: pDma2_CH5_CallBackFunc = ptr; break;
    
    default: break;
  }
  
}


/*----------------------------------------------------------------------------
    �жϷ�����
 *----------------------------------------------------------------------------*/

#if 1
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //���ûص�����
  if (pSysTick_SlTick_CallBackFunc)
  {
    pSysTick_SlTick_CallBackFunc();
  }
  
  if (pSysTick_OsTick_CallBackFunc)
  {
    pSysTick_OsTick_CallBackFunc();
  }
  
  if (pSysTick_Task_CallBackFunc)
  {
    pSysTick_Task_CallBackFunc();
  }
  
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}
#endif


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif  
  
  if (EXTI->PR & (0X1<<0))      //�ж��Ƿ�������0���������ж�
  {
    EXTI->PR  |=  (0X1<<0);     //���־λ
    
    //���ûص�����
    if (pExti0_CallBackFunc)
    {
      pExti0_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (EXTI->PR & (0X1<<1))      //�ж��Ƿ�������1���������ж�
  {
    EXTI->PR  |=  (0X1<<1);     //���־λ
    
    //���ûص�����
    if (pExti1_CallBackFunc)
    {
      pExti1_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (EXTI->PR & (0X1<<2))      //�ж��Ƿ�������2���������ж�
  {
    EXTI->PR  |=  (0X1<<2);     //���־λ
    
    //���ûص�����
    if (pExti2_CallBackFunc)
    {
      pExti2_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (EXTI->PR & (0X1<<3))      //�ж��Ƿ�������3���������ж�
  {
    EXTI->PR  |=  (0X1<<3);     //���־λ
    
    //���ûص�����
    if (pExti3_CallBackFunc)
    {
      pExti3_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (EXTI->PR & (0X1<<4))      //�ж��Ƿ�������4���������ж�
  {
    EXTI->PR  |=  (0X1<<4);     //���־λ
    
    //���ûص�����
    if (pExti4_CallBackFunc)
    {
      pExti4_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (EXTI->PR & (0X1<<5))      //�ж��Ƿ�������5���������ж�
  {
    EXTI->PR  |=  (0X1<<5);     //���־λ
    
    //���ûص�����
    if (pExti5_CallBackFunc)
    {
      pExti5_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<6))      //�ж��Ƿ�������6���������ж�
  {
    EXTI->PR  |=  (0X1<<6);     //���־λ
    
    //���ûص�����
    if (pExti6_CallBackFunc)
    {
      pExti6_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<7))      //�ж��Ƿ�������7���������ж�
  {
    EXTI->PR  |=  (0X1<<7);     //���־λ
    
    //���ûص�����
    if (pExti7_CallBackFunc)
    {
      pExti7_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<8))      //�ж��Ƿ�������8���������ж�
  {
    EXTI->PR  |=  (0X1<<8);     //���־λ
    
    //���ûص�����
    if (pExti8_CallBackFunc)
    {
      pExti8_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<9))      //�ж��Ƿ�������9���������ж�
  {
    EXTI->PR  |=  (0X1<<9);     //���־λ
    
    //���ûص�����
    if (pExti9_CallBackFunc)
    {
      pExti9_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Exti Handler.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (EXTI->PR & (0X1<<10))      //�ж��Ƿ�������10���������ж�
  {
    EXTI->PR  |=  (0X1<<10);     //���־λ
    
    //���ûص�����
    if (pExti10_CallBackFunc)
    {
      pExti10_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<11))      //�ж��Ƿ�������11���������ж�
  {
    EXTI->PR  |=  (0X1<<11);     //���־λ
    
    //���ûص�����
    if (pExti11_CallBackFunc)
    {
      pExti11_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<12))      //�ж��Ƿ�������12���������ж�
  {
    EXTI->PR  |=  (0X1<<12);     //���־λ
    
    //���ûص�����
    if (pExti12_CallBackFunc)
    {
      pExti12_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<13))      //�ж��Ƿ�������13���������ж�
  {
    EXTI->PR  |=  (0X1<<13);     //���־λ
    
    //���ûص�����
    if (pExti13_CallBackFunc)
    {
      pExti13_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<14))      //�ж��Ƿ�������14���������ж�
  {
    EXTI->PR  |=  (0X1<<14);     //���־λ
    
    //���ûص�����
    if (pExti14_CallBackFunc)
    {
      pExti14_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<15))      //�ж��Ƿ�������15���������ж�
  {
    EXTI->PR  |=  (0X1<<15);     //���־λ
    
    //���ûص�����
    if (pExti15_CallBackFunc)
    {
      pExti15_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM1 Update Handler.
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (TIM1->SR & (0x1<<0))
  {
    TIM1->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim1_Uif_CallBackFunc)
    {
      pTim1_Uif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM1 Capture Compare Handler.
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //CC1IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM1->SR & (0x1<<1)) && (TIM1->DIER & (0x1<<1))) 
  {
    TIM1->SR &= ~(0x1<<1);  //���־λ
    
    if (pTim1_Cc1if_CallBackFunc)
    {
      pTim1_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM1->SR & (0x1<<2)) && (TIM1->DIER & (0x1<<2)))
  {
    TIM1->SR &= ~(0x1<<2);  //���־λ
    
    if (pTim1_Cc2if_CallBackFunc)
    {
      pTim1_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM1->SR & (0x1<<3)) && (TIM1->DIER & (0x1<<3)))
  {
    TIM1->SR &= ~(0x1<<3);  //���־λ
    
    if (pTim1_Cc3if_CallBackFunc)
    {
      pTim1_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM1->SR & (0x1<<4)) && (TIM1->DIER & (0x1<<4)))
  {
    TIM1->SR &= ~(0x1<<4);  //���־λ
    
    if (pTim1_Cc4if_CallBackFunc)
    {
      pTim1_Cc4if_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM1 Trigger and Commutation Handler.
  * @param  None
  * @retval None
  */
void TIM1_TRG_COM_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (TIM1->SR & (0x1<<6))
  {
    TIM1->SR &= ~(0x1<<6);  //���־λ
    
    if (pTim1_Tif_CallBackFunc)
    {
      pTim1_Tif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM1 Break Handler.
  * @param  None
  * @retval None
  */
void TIM1_BRK_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (TIM1->SR & (0x1<<7))
  {
    TIM1->SR &= ~(0x1<<7);  //���־λ
    
    if (pTim1_Bif_CallBackFunc)
    {
      pTim1_Bif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM8 Update Handler.
  * @param  None
  * @retval None
  */
void TIM8_UP_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (TIM8->SR & (0x1<<0))
  {
    TIM8->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim8_Uif_CallBackFunc)
    {
      pTim8_Uif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM8 Capture Compare Handler.
  * @param  None
  * @retval None
  */
void TIM8_CC_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //CC1IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM8->SR & (0x1<<1)) && (TIM8->DIER & (0x1<<1))) 
  {
    TIM8->SR &= ~(0x1<<1);  //���־λ
    
    if (pTim8_Cc1if_CallBackFunc)
    {
      pTim8_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM8->SR & (0x1<<2)) && (TIM8->DIER & (0x1<<2)))
  {
    TIM8->SR &= ~(0x1<<2);  //���־λ
    
    if (pTim8_Cc2if_CallBackFunc)
    {
      pTim8_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM8->SR & (0x1<<3)) && (TIM8->DIER & (0x1<<3)))
  {
    TIM8->SR &= ~(0x1<<3);  //���־λ
    
    if (pTim8_Cc3if_CallBackFunc)
    {
      pTim8_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM8->SR & (0x1<<4)) && (TIM8->DIER & (0x1<<4)))
  {
    TIM8->SR &= ~(0x1<<4);  //���־λ
    
    if (pTim8_Cc4if_CallBackFunc)
    {
      pTim8_Cc4if_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM8 Trigger and Commutation Handler.
  * @param  None
  * @retval None
  */
void TIM8_TRG_COM_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (TIM8->SR & (0x1<<6))
  {
    TIM8->SR &= ~(0x1<<6);  //���־λ
    
    if (pTim8_Tif_CallBackFunc)
    {
      pTim8_Tif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles TIM8 Break Handler.
  * @param  None
  * @retval None
  */
void TIM8_BRK_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (TIM8->SR & (0x1<<7))
  {
    TIM8->SR &= ~(0x1<<7);  //���־λ
    
    if (pTim8_Bif_CallBackFunc)
    {
      pTim8_Bif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Tim Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //UIF �ж�
  if ((TIM2->SR & (0x1<<0)) && (TIM2->DIER & (0x1<<0))) 
  {
    TIM2->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim2_Uif_CallBackFunc)
    {
      pTim2_Uif_CallBackFunc();
    }
  }
  
  //CC1IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM2->SR & (0x1<<1)) && (TIM2->DIER & (0x1<<1))) 
  {
    TIM2->SR &= ~(0x1<<1);  //���־λ
    
    if (pTim2_Cc1if_CallBackFunc)
    {
      pTim2_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM2->SR & (0x1<<2)) && (TIM2->DIER & (0x1<<2)))
  {
    TIM2->SR &= ~(0x1<<2);  //���־λ
    
    if (pTim2_Cc2if_CallBackFunc)
    {
      pTim2_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM2->SR & (0x1<<3)) && (TIM2->DIER & (0x1<<3)))
  {
    TIM2->SR &= ~(0x1<<3);  //���־λ
    
    if (pTim2_Cc3if_CallBackFunc)
    {
      pTim2_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM2->SR & (0x1<<4)) && (TIM2->DIER & (0x1<<4)))
  {
    TIM2->SR &= ~(0x1<<4);  //���־λ
    
    if (pTim2_Cc4if_CallBackFunc)
    {
      pTim2_Cc4if_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Tim Handler.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //UIF �ж�
  if ((TIM3->SR & (0x1<<0)) && (TIM3->DIER & (0x1<<0))) 
  {
    TIM3->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim3_Uif_CallBackFunc)
    {
      pTim3_Uif_CallBackFunc();
    }
  }
  
  //CC1IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM3->SR & (0x1<<1)) && (TIM3->DIER & (0x1<<1))) 
  {
    TIM3->SR &= ~(0x1<<1);  //���־λ
    
    if (pTim3_Cc1if_CallBackFunc)
    {
      pTim3_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM3->SR & (0x1<<2)) && (TIM3->DIER & (0x1<<2)))
  {
    TIM3->SR &= ~(0x1<<2);  //���־λ
    
    if (pTim3_Cc2if_CallBackFunc)
    {
      pTim3_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM3->SR & (0x1<<3)) && (TIM3->DIER & (0x1<<3)))
  {
    TIM3->SR &= ~(0x1<<3);  //���־λ
    
    if (pTim3_Cc3if_CallBackFunc)
    {
      pTim3_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM3->SR & (0x1<<4)) && (TIM3->DIER & (0x1<<4)))
  {
    TIM3->SR &= ~(0x1<<4);  //���־λ
    
    if (pTim3_Cc4if_CallBackFunc)
    {
      pTim3_Cc4if_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Tim Handler.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //UIF �ж�
  if ((TIM4->SR & (0x1<<0)) && (TIM4->DIER & (0x1<<0))) 
  {
    TIM4->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim4_Uif_CallBackFunc)
    {
      pTim4_Uif_CallBackFunc();
    }
  }
  
  //CC1IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM4->SR & (0x1<<1)) && (TIM4->DIER & (0x1<<1))) 
  {
    TIM4->SR &= ~(0x1<<1);  //���־λ
    
    if (pTim4_Cc1if_CallBackFunc)
    {
      pTim4_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM4->SR & (0x1<<2)) && (TIM4->DIER & (0x1<<2)))
  {
    TIM4->SR &= ~(0x1<<2);  //���־λ
    
    if (pTim4_Cc2if_CallBackFunc)
    {
      pTim4_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM4->SR & (0x1<<3)) && (TIM4->DIER & (0x1<<3)))
  {
    TIM4->SR &= ~(0x1<<3);  //���־λ
    
    if (pTim4_Cc3if_CallBackFunc)
    {
      pTim4_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM4->SR & (0x1<<4)) && (TIM4->DIER & (0x1<<4)))
  {
    TIM4->SR &= ~(0x1<<4);  //���־λ
    
    if (pTim4_Cc4if_CallBackFunc)
    {
      pTim4_Cc4if_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Tim Handler.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //UIF �ж�
  if ((TIM5->SR & (0x1<<0)) && (TIM5->DIER & (0x1<<0))) 
  {
    TIM5->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim5_Uif_CallBackFunc)
    {
      pTim5_Uif_CallBackFunc();
    }
  }
  
  //CC1IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM5->SR & (0x1<<1)) && (TIM5->DIER & (0x1<<1))) 
  {
    TIM5->SR &= ~(0x1<<1);  //���־λ
    
    if (pTim5_Cc1if_CallBackFunc)
    {
      pTim5_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM5->SR & (0x1<<2)) && (TIM5->DIER & (0x1<<2)))
  {
    TIM5->SR &= ~(0x1<<2);  //���־λ
    
    if (pTim5_Cc2if_CallBackFunc)
    {
      pTim5_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM5->SR & (0x1<<3)) && (TIM5->DIER & (0x1<<3)))
  {
    TIM5->SR &= ~(0x1<<3);  //���־λ
    
    if (pTim5_Cc3if_CallBackFunc)
    {
      pTim5_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF �ж�
  //��״̬λ��λ��ʹ�����жϲ�ִ����Ӧ�Ļص�����
  if ((TIM5->SR & (0x1<<4)) && (TIM5->DIER & (0x1<<4)))
  {
    TIM5->SR &= ~(0x1<<4);  //���־λ
    
    if (pTim5_Cc4if_CallBackFunc)
    {
      pTim5_Cc4if_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Tim Handler.
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //UIF �ж�
  if (TIM6->SR & (0x1<<0))
  {
    TIM6->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim6_Uif_CallBackFunc)
    {
      pTim6_Uif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Tim Handler.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  //UIF �ж�
  if (TIM7->SR & (0x1<<0))
  {
    TIM7->SR &= ~(0x1<<0);  //���־λ
    
    if (pTim7_Uif_CallBackFunc)
    {
      pTim7_Uif_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}

#if 0
/**
  * @brief  This function handles Usart Handler.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)  
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  /* �ж�����ж� */
  if (USART1->SR & (0X1<<4))  //����֡�ж����
  {
    /* ���־λ */
    USART1->DR;    //���߿����жϱ�־λ������������:�ȶ�SR,�ٶ�DR
    
    if (pUsart1_Idle_CallBackFunc)
    {
      pUsart1_Idle_CallBackFunc();
    }
  }
  
  if (pUsart1_CallBackFunc)
  {
    pUsart1_CallBackFunc();
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}
#endif


/**
  * @brief  This function handles Usart Handler.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)  
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  /* �ж�����ж� */
  if (USART2->SR & (0X1<<4))  //����֡�ж����
  {
    /* ���־λ */
    USART2->DR;    //���߿����жϱ�־λ������������:�ȶ�SR,�ٶ�DR
    
    if (pUsart2_Idle_CallBackFunc)
    {
      pUsart2_Idle_CallBackFunc();
    }
  }
  
  if (pUsart2_CallBackFunc)
  {
    pUsart2_CallBackFunc();
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Usart Handler.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)  
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  /* �ж�����ж� */
  if (USART3->SR & (0X1<<4))  //����֡�ж����
  {
    /* ���־λ */
    USART3->DR;    //���߿����жϱ�־λ������������:�ȶ�SR,�ٶ�DR
    
    if (pUsart3_Idle_CallBackFunc)
    {
      pUsart3_Idle_CallBackFunc();
    }
  }
  
  if (pUsart3_CallBackFunc)
  {
    pUsart3_CallBackFunc();
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Usart Handler.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)  
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  /* �ж�����ж� */
  if (UART4->SR & (0X1<<4))  //����֡�ж����
  {
    /* ���־λ */
    UART4->DR;    //���߿����жϱ�־λ������������:�ȶ�SR,�ٶ�DR
    
    if (pUsart4_Idle_CallBackFunc)
    {
      pUsart4_Idle_CallBackFunc();
    }
  }
  
  if (pUsart4_CallBackFunc)
  {
    pUsart4_CallBackFunc();
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<1))
  {
    DMA1->IFCR |= (0x1<<0);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH1_CallBackFunc)
    {
      pDma1_CH1_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<5))
  {
    DMA1->IFCR |= (0x1<<4);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH2_CallBackFunc)
    {
      pDma1_CH2_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel3_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<9))
  {
    DMA1->IFCR |= (0x1<<8);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH3_CallBackFunc)
    {
      pDma1_CH3_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel4_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<13))
  {
    DMA1->IFCR |= (0x1<<12);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH4_CallBackFunc)
    {
      pDma1_CH4_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}

#if 0
/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel5_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<17))
  {
    DMA1->IFCR |= (0x1<<16);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH5_CallBackFunc)
    {
      pDma1_CH5_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}
#endif

/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel6_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<21))
  {
    DMA1->IFCR |= (0x1<<20);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH6_CallBackFunc)
    {
      pDma1_CH6_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA1->ISR & (0x1<<25))
  {
    DMA1->IFCR |= (0x1<<24);  //��ȫ�ֱ�־λ
    
    if (pDma1_CH7_CallBackFunc)
    {
      pDma1_CH7_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel1_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA2->ISR & (0x1<<1))
  {
    DMA2->IFCR |= (0x1<<0);  //��ȫ�ֱ�־λ
    
    if (pDma2_CH1_CallBackFunc)
    {
      pDma2_CH1_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel2_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA2->ISR & (0x1<<5))
  {
    DMA2->IFCR |= (0x1<<4);  //��ȫ�ֱ�־λ
    
    if (pDma2_CH2_CallBackFunc)
    {
      pDma2_CH2_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel3_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  if (DMA2->ISR & (0x1<<9))
  {
    DMA2->IFCR |= (0x1<<8);  //��ȫ�ֱ�־λ
    
    if (pDma2_CH3_CallBackFunc)
    {
      pDma2_CH3_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}


/**
  * @brief  This function handles Dma Handler.
  * @param  None
  * @retval None
  */
void DMA2_Channel4_5_IRQHandler(void)
{
  #if USING_OS
  if (SL_ItrEnter)  SL_ItrEnter();
  #endif
  
  // Channel4
  if (DMA2->ISR & (0x1<<13))
  {
    DMA2->IFCR |= (0x1<<12);  //��ȫ�ֱ�־λ
    
    if (pDma2_CH4_CallBackFunc)
    {
      pDma2_CH4_CallBackFunc();
    }
  }
  
  // Channel5
  if (DMA2->ISR & (0x1<<17))
  {
    DMA2->IFCR |= (0x1<<16);  //��ȫ�ֱ�־λ
    
    if (pDma2_CH5_CallBackFunc)
    {
      pDma2_CH5_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}



