/**
  ******************************************************************************
  * @file    hw_it_handler.c
  * @author  杜公子寒枫
  * @version V1.0 寄存器版本
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

/* ---相关中断回调函数指针定义--- */
static IT_CALLBACK_FUNC SL_ItrEnter;
static IT_CALLBACK_FUNC SL_ItrLeave;


//系统滴答中断
static IT_CALLBACK_FUNC pSysTick_SlTick_CallBackFunc;
static IT_CALLBACK_FUNC pSysTick_OsTick_CallBackFunc;
static IT_CALLBACK_FUNC pSysTick_Task_CallBackFunc;

//外部中断
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

//定时器中断
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

//串口中断
static IT_CALLBACK_FUNC pUsart1_Idle_CallBackFunc;
static IT_CALLBACK_FUNC pUsart2_Idle_CallBackFunc;
static IT_CALLBACK_FUNC pUsart3_Idle_CallBackFunc;
static IT_CALLBACK_FUNC pUsart4_Idle_CallBackFunc;

static IT_CALLBACK_FUNC pUsart1_CallBackFunc;
static IT_CALLBACK_FUNC pUsart2_CallBackFunc;
static IT_CALLBACK_FUNC pUsart3_CallBackFunc;
static IT_CALLBACK_FUNC pUsart4_CallBackFunc;

//DMA通道
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
    中断回调函数注册函数
 *----------------------------------------------------------------------------*/

void IT_IRQ_FuncLogin(IT_CALLBACK_FUNC ptr, IT_LOGIN_CODE Code)
{
  switch (Code)
  {
    //进入/退出中断时回调,适用于使用OS的场合
    case IT_OS_ITR_ENTER: SL_ItrEnter = ptr;
    case IT_OS_ITR_LEAVE: SL_ItrLeave = ptr;
    
    //系统滴答中断回调函数
    case IT_SysTick_SL:   pSysTick_SlTick_CallBackFunc = ptr; break;
    case IT_SysTick_OS:   pSysTick_OsTick_CallBackFunc = ptr; break;
    case IT_SysTick_TASK: pSysTick_Task_CallBackFunc = ptr; break;
    
    //外部中断回调函数
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
    
    //定时器中断回调函数
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
    
    //串口中断回调函数
    case IT_USART1:       pUsart1_CallBackFunc = ptr; break;
    case IT_USART2:       pUsart2_CallBackFunc = ptr; break;
    case IT_USART3:       pUsart3_CallBackFunc = ptr; break;
    case IT_USART4:       pUsart4_CallBackFunc = ptr; break;
    
    case IT_USART1_IDLE:  pUsart1_Idle_CallBackFunc = ptr; break;
    case IT_USART2_IDLE:  pUsart2_Idle_CallBackFunc = ptr; break;
    case IT_USART3_IDLE:  pUsart3_Idle_CallBackFunc = ptr; break;
    case IT_USART4_IDLE:  pUsart4_Idle_CallBackFunc = ptr; break;
    
    //DMA通道中断回调函数
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
    中断服务函数
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
  
  //调用回调函数
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
  
  if (EXTI->PR & (0X1<<0))      //判断是否是由线0所触发的中断
  {
    EXTI->PR  |=  (0X1<<0);     //清标志位
    
    //调用回调函数
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
  
  if (EXTI->PR & (0X1<<1))      //判断是否是由线1所触发的中断
  {
    EXTI->PR  |=  (0X1<<1);     //清标志位
    
    //调用回调函数
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
  
  if (EXTI->PR & (0X1<<2))      //判断是否是由线2所触发的中断
  {
    EXTI->PR  |=  (0X1<<2);     //清标志位
    
    //调用回调函数
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
  
  if (EXTI->PR & (0X1<<3))      //判断是否是由线3所触发的中断
  {
    EXTI->PR  |=  (0X1<<3);     //清标志位
    
    //调用回调函数
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
  
  if (EXTI->PR & (0X1<<4))      //判断是否是由线4所触发的中断
  {
    EXTI->PR  |=  (0X1<<4);     //清标志位
    
    //调用回调函数
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
  
  if (EXTI->PR & (0X1<<5))      //判断是否是由线5所触发的中断
  {
    EXTI->PR  |=  (0X1<<5);     //清标志位
    
    //调用回调函数
    if (pExti5_CallBackFunc)
    {
      pExti5_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<6))      //判断是否是由线6所触发的中断
  {
    EXTI->PR  |=  (0X1<<6);     //清标志位
    
    //调用回调函数
    if (pExti6_CallBackFunc)
    {
      pExti6_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<7))      //判断是否是由线7所触发的中断
  {
    EXTI->PR  |=  (0X1<<7);     //清标志位
    
    //调用回调函数
    if (pExti7_CallBackFunc)
    {
      pExti7_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<8))      //判断是否是由线8所触发的中断
  {
    EXTI->PR  |=  (0X1<<8);     //清标志位
    
    //调用回调函数
    if (pExti8_CallBackFunc)
    {
      pExti8_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<9))      //判断是否是由线9所触发的中断
  {
    EXTI->PR  |=  (0X1<<9);     //清标志位
    
    //调用回调函数
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
  
  if (EXTI->PR & (0X1<<10))      //判断是否是由线10所触发的中断
  {
    EXTI->PR  |=  (0X1<<10);     //清标志位
    
    //调用回调函数
    if (pExti10_CallBackFunc)
    {
      pExti10_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<11))      //判断是否是由线11所触发的中断
  {
    EXTI->PR  |=  (0X1<<11);     //清标志位
    
    //调用回调函数
    if (pExti11_CallBackFunc)
    {
      pExti11_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<12))      //判断是否是由线12所触发的中断
  {
    EXTI->PR  |=  (0X1<<12);     //清标志位
    
    //调用回调函数
    if (pExti12_CallBackFunc)
    {
      pExti12_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<13))      //判断是否是由线13所触发的中断
  {
    EXTI->PR  |=  (0X1<<13);     //清标志位
    
    //调用回调函数
    if (pExti13_CallBackFunc)
    {
      pExti13_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<14))      //判断是否是由线14所触发的中断
  {
    EXTI->PR  |=  (0X1<<14);     //清标志位
    
    //调用回调函数
    if (pExti14_CallBackFunc)
    {
      pExti14_CallBackFunc();
    }
  }
  
  if (EXTI->PR & (0X1<<15))      //判断是否是由线15所触发的中断
  {
    EXTI->PR  |=  (0X1<<15);     //清标志位
    
    //调用回调函数
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
    TIM1->SR &= ~(0x1<<0);  //清标志位
    
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
  
  //CC1IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM1->SR & (0x1<<1)) && (TIM1->DIER & (0x1<<1))) 
  {
    TIM1->SR &= ~(0x1<<1);  //清标志位
    
    if (pTim1_Cc1if_CallBackFunc)
    {
      pTim1_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM1->SR & (0x1<<2)) && (TIM1->DIER & (0x1<<2)))
  {
    TIM1->SR &= ~(0x1<<2);  //清标志位
    
    if (pTim1_Cc2if_CallBackFunc)
    {
      pTim1_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM1->SR & (0x1<<3)) && (TIM1->DIER & (0x1<<3)))
  {
    TIM1->SR &= ~(0x1<<3);  //清标志位
    
    if (pTim1_Cc3if_CallBackFunc)
    {
      pTim1_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM1->SR & (0x1<<4)) && (TIM1->DIER & (0x1<<4)))
  {
    TIM1->SR &= ~(0x1<<4);  //清标志位
    
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
    TIM1->SR &= ~(0x1<<6);  //清标志位
    
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
    TIM1->SR &= ~(0x1<<7);  //清标志位
    
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
    TIM8->SR &= ~(0x1<<0);  //清标志位
    
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
  
  //CC1IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM8->SR & (0x1<<1)) && (TIM8->DIER & (0x1<<1))) 
  {
    TIM8->SR &= ~(0x1<<1);  //清标志位
    
    if (pTim8_Cc1if_CallBackFunc)
    {
      pTim8_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM8->SR & (0x1<<2)) && (TIM8->DIER & (0x1<<2)))
  {
    TIM8->SR &= ~(0x1<<2);  //清标志位
    
    if (pTim8_Cc2if_CallBackFunc)
    {
      pTim8_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM8->SR & (0x1<<3)) && (TIM8->DIER & (0x1<<3)))
  {
    TIM8->SR &= ~(0x1<<3);  //清标志位
    
    if (pTim8_Cc3if_CallBackFunc)
    {
      pTim8_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM8->SR & (0x1<<4)) && (TIM8->DIER & (0x1<<4)))
  {
    TIM8->SR &= ~(0x1<<4);  //清标志位
    
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
    TIM8->SR &= ~(0x1<<6);  //清标志位
    
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
    TIM8->SR &= ~(0x1<<7);  //清标志位
    
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
  
  //UIF 中断
  if ((TIM2->SR & (0x1<<0)) && (TIM2->DIER & (0x1<<0))) 
  {
    TIM2->SR &= ~(0x1<<0);  //清标志位
    
    if (pTim2_Uif_CallBackFunc)
    {
      pTim2_Uif_CallBackFunc();
    }
  }
  
  //CC1IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM2->SR & (0x1<<1)) && (TIM2->DIER & (0x1<<1))) 
  {
    TIM2->SR &= ~(0x1<<1);  //清标志位
    
    if (pTim2_Cc1if_CallBackFunc)
    {
      pTim2_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM2->SR & (0x1<<2)) && (TIM2->DIER & (0x1<<2)))
  {
    TIM2->SR &= ~(0x1<<2);  //清标志位
    
    if (pTim2_Cc2if_CallBackFunc)
    {
      pTim2_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM2->SR & (0x1<<3)) && (TIM2->DIER & (0x1<<3)))
  {
    TIM2->SR &= ~(0x1<<3);  //清标志位
    
    if (pTim2_Cc3if_CallBackFunc)
    {
      pTim2_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM2->SR & (0x1<<4)) && (TIM2->DIER & (0x1<<4)))
  {
    TIM2->SR &= ~(0x1<<4);  //清标志位
    
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
  
  //UIF 中断
  if ((TIM3->SR & (0x1<<0)) && (TIM3->DIER & (0x1<<0))) 
  {
    TIM3->SR &= ~(0x1<<0);  //清标志位
    
    if (pTim3_Uif_CallBackFunc)
    {
      pTim3_Uif_CallBackFunc();
    }
  }
  
  //CC1IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM3->SR & (0x1<<1)) && (TIM3->DIER & (0x1<<1))) 
  {
    TIM3->SR &= ~(0x1<<1);  //清标志位
    
    if (pTim3_Cc1if_CallBackFunc)
    {
      pTim3_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM3->SR & (0x1<<2)) && (TIM3->DIER & (0x1<<2)))
  {
    TIM3->SR &= ~(0x1<<2);  //清标志位
    
    if (pTim3_Cc2if_CallBackFunc)
    {
      pTim3_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM3->SR & (0x1<<3)) && (TIM3->DIER & (0x1<<3)))
  {
    TIM3->SR &= ~(0x1<<3);  //清标志位
    
    if (pTim3_Cc3if_CallBackFunc)
    {
      pTim3_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM3->SR & (0x1<<4)) && (TIM3->DIER & (0x1<<4)))
  {
    TIM3->SR &= ~(0x1<<4);  //清标志位
    
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
  
  //UIF 中断
  if ((TIM4->SR & (0x1<<0)) && (TIM4->DIER & (0x1<<0))) 
  {
    TIM4->SR &= ~(0x1<<0);  //清标志位
    
    if (pTim4_Uif_CallBackFunc)
    {
      pTim4_Uif_CallBackFunc();
    }
  }
  
  //CC1IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM4->SR & (0x1<<1)) && (TIM4->DIER & (0x1<<1))) 
  {
    TIM4->SR &= ~(0x1<<1);  //清标志位
    
    if (pTim4_Cc1if_CallBackFunc)
    {
      pTim4_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM4->SR & (0x1<<2)) && (TIM4->DIER & (0x1<<2)))
  {
    TIM4->SR &= ~(0x1<<2);  //清标志位
    
    if (pTim4_Cc2if_CallBackFunc)
    {
      pTim4_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM4->SR & (0x1<<3)) && (TIM4->DIER & (0x1<<3)))
  {
    TIM4->SR &= ~(0x1<<3);  //清标志位
    
    if (pTim4_Cc3if_CallBackFunc)
    {
      pTim4_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM4->SR & (0x1<<4)) && (TIM4->DIER & (0x1<<4)))
  {
    TIM4->SR &= ~(0x1<<4);  //清标志位
    
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
  
  //UIF 中断
  if ((TIM5->SR & (0x1<<0)) && (TIM5->DIER & (0x1<<0))) 
  {
    TIM5->SR &= ~(0x1<<0);  //清标志位
    
    if (pTim5_Uif_CallBackFunc)
    {
      pTim5_Uif_CallBackFunc();
    }
  }
  
  //CC1IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM5->SR & (0x1<<1)) && (TIM5->DIER & (0x1<<1))) 
  {
    TIM5->SR &= ~(0x1<<1);  //清标志位
    
    if (pTim5_Cc1if_CallBackFunc)
    {
      pTim5_Cc1if_CallBackFunc();
    }
  }
  
  //CC2IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM5->SR & (0x1<<2)) && (TIM5->DIER & (0x1<<2)))
  {
    TIM5->SR &= ~(0x1<<2);  //清标志位
    
    if (pTim5_Cc2if_CallBackFunc)
    {
      pTim5_Cc2if_CallBackFunc();
    }
  }
  
  //CC3IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM5->SR & (0x1<<3)) && (TIM5->DIER & (0x1<<3)))
  {
    TIM5->SR &= ~(0x1<<3);  //清标志位
    
    if (pTim5_Cc3if_CallBackFunc)
    {
      pTim5_Cc3if_CallBackFunc();
    }
  }
  
  //CC4IF 中断
  //当状态位置位并使能了中断才执行相应的回调函数
  if ((TIM5->SR & (0x1<<4)) && (TIM5->DIER & (0x1<<4)))
  {
    TIM5->SR &= ~(0x1<<4);  //清标志位
    
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
  
  //UIF 中断
  if (TIM6->SR & (0x1<<0))
  {
    TIM6->SR &= ~(0x1<<0);  //清标志位
    
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
  
  //UIF 中断
  if (TIM7->SR & (0x1<<0))
  {
    TIM7->SR &= ~(0x1<<0);  //清标志位
    
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
  
  /* 中断入口判断 */
  if (USART1->SR & (0X1<<4))  //空闲帧中断入口
  {
    /* 清标志位 */
    USART1->DR;    //总线空闲中断标志位由软件序列清除:先读SR,再读DR
    
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
  
  /* 中断入口判断 */
  if (USART2->SR & (0X1<<4))  //空闲帧中断入口
  {
    /* 清标志位 */
    USART2->DR;    //总线空闲中断标志位由软件序列清除:先读SR,再读DR
    
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
  
  /* 中断入口判断 */
  if (USART3->SR & (0X1<<4))  //空闲帧中断入口
  {
    /* 清标志位 */
    USART3->DR;    //总线空闲中断标志位由软件序列清除:先读SR,再读DR
    
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
  
  /* 中断入口判断 */
  if (UART4->SR & (0X1<<4))  //空闲帧中断入口
  {
    /* 清标志位 */
    UART4->DR;    //总线空闲中断标志位由软件序列清除:先读SR,再读DR
    
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
    DMA1->IFCR |= (0x1<<0);  //清全局标志位
    
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
    DMA1->IFCR |= (0x1<<4);  //清全局标志位
    
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
    DMA1->IFCR |= (0x1<<8);  //清全局标志位
    
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
    DMA1->IFCR |= (0x1<<12);  //清全局标志位
    
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
    DMA1->IFCR |= (0x1<<16);  //清全局标志位
    
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
    DMA1->IFCR |= (0x1<<20);  //清全局标志位
    
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
    DMA1->IFCR |= (0x1<<24);  //清全局标志位
    
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
    DMA2->IFCR |= (0x1<<0);  //清全局标志位
    
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
    DMA2->IFCR |= (0x1<<4);  //清全局标志位
    
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
    DMA2->IFCR |= (0x1<<8);  //清全局标志位
    
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
    DMA2->IFCR |= (0x1<<12);  //清全局标志位
    
    if (pDma2_CH4_CallBackFunc)
    {
      pDma2_CH4_CallBackFunc();
    }
  }
  
  // Channel5
  if (DMA2->ISR & (0x1<<17))
  {
    DMA2->IFCR |= (0x1<<16);  //清全局标志位
    
    if (pDma2_CH5_CallBackFunc)
    {
      pDma2_CH5_CallBackFunc();
    }
  }
  
  #if USING_OS
  if (SL_ItrLeave)  SL_ItrLeave();
  #endif
}



