/**
  ******************************************************************************
  * @file    hw_platform.c
  * @author  杜公子寒枫
  * @version V1.1
  * @date    2017.03.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * STM32F10X平台通用文件,包括了硬件和SourceLib的交互操作,关于Cortex-M3内核的相关操
  * 操的封装,以及GPIO,EXTI,TIM等硬件的通用配置
  * 
  * V1.1------------
  * 修改描述: 增加对SWJ功能的配置,可以实现对JTAG口和SW口的配置
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2017.04.07
  * 
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/

#include "SourceLib.h"
#include "ker_typedef.h"
#include "ker_timer.h"
#include "hw_platform.h"
#include "hw_it_handler.h"
#include "hw_tim_delay.h"
#include "hw_usart.h"
#include "stm32f10x.h"


uint32_t SystemClock = 72000000;


static void PrintByte(uint8_t Data)
{
  USARTx_SendData(USARTx_1, Data);
}


/**
  * @brief  系统参数初始化
  * @param  None
  * @retval None
  */
void System_Init(unsigned int Ticks)
{
  //配置JTAG口
  SWJ_Config(SWJ_ONLY_SW);
  
  //初始化延时接口
  TIM_TYPE DelayTimer = TIMx_1;
  TIMx_DelayInit(DelayTimer);
  SL_DelayOperation(TIMx_DelayUs, SLTimer_Delay);
  
  //重定向printf函数
  SL_PrintOperation(PrintByte);
  
  //初始化系统滴答
  SysTick_Init(Ticks);     //系统时钟滴答初始化
  SLTimer_SetTick(Ticks);  //设置内核时钟滴答
  IT_IRQ_FuncLogin(SLTimer_Update, IT_SysTick_SL); //将回调函数SLTimer_Update注册入SysTick中断中
  
}


/*----------------------------------------------------------------------------
    系统内核层/底层相关配置代码
 *----------------------------------------------------------------------------*/


/**
  * @brief  系统滴答定时中断初始化函数
  * @param  ms 定时中断时间间隔
  * @retval None
  */
void SysTick_Init(uint16_t Ms)
{
  #if 0
	/* 配置时钟源 */
  SysTick->CTRL &= ~(1<<2);  //选择9M外部时钟
  
  /* 清空计数器 */
	SysTick->VAL   = 0x00;      //清空计数器
  
  /* 配置中断间隔 */
  SysTick->LOAD  = 9000 * Ms;   //定时1ms,要计数9000次
  
  /* 开中断 */
  SysTick->CTRL |= (0X1<<1);  //开中断
  
  /* 开时钟 */
  SysTick->CTRL |= (0X1<<0);  //开系统滴答
  
  #endif
  
  SysTick_Config(SYSTEM_FCLK/1000*Ms);
  
}


/**
  * @brief  系统软件复位函数
  * @param  None
  * @retval None
  * @note   内核层软件复位,M0,M3,M4兼容
  */
void System_SoftwareReset(void)
{
  NVIC_SystemReset();

}


/**
  * @brief  设置系统时钟为56M(内部高速时钟)
  * @param  None
  * @retval None
  * @note   在无外部晶振的情况下,系统内核时钟源为内部高速晶振(HSI),此时的内核时钟为8M.
  *         为了发挥更好的性能,可以将内部高速晶振倍频.根据其时钟树的结构,内部高速晶振
  *         倍频后可达到的最高时钟频率为64M(8M/2*16=64M).除了最高性能的64M外,我们常
  *         用的频率还有56M和48M.在56M下ADC的性能最强
  */
void System_CoreClockConfigure(SYS_CORE_CLOCK CoreClock)
{
//  
  switch (CoreClock)
  {
    case SYS_CLOCK_48M: SystemClock = 48000000; break;
    case SYS_CLOCK_56M: SystemClock = 56000000; break;
    case SYS_CLOCK_64M: SystemClock = 64000000; break;
    default: break;
  }
  
  RCC->CR |= ((uint32_t)RCC_CR_HSION);                      // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                   // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                              // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);   // Wait for HSI used as system clock

  FLASH->ACR  = FLASH_ACR_PRFTBE;                           // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                          // Flash 1 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                          // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;                         // APB1 = HCLK/2
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                         // APB2 = HCLK

  RCC->CR &= ~RCC_CR_PLLON;                                 // Disable PLL

  //  PLL configuration:  = HSI/2 * 12 = 48 MHz
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | CoreClock);

  RCC->CR |= RCC_CR_PLLON;                                  // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();            // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                                // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);   // Wait till PLL is system clock src
  
  SystemCoreClockUpdate();                                  // Update system clock

}


/**
  * @brief  JTAG模式设置,用于设置JTAG的模式
  * @param  mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
  * @retval None
  */
void SWJ_Config(SWJ_CFG_MODE MODE)
{
  RCC->APB2ENR |= 1<<0;     //开启辅助时钟	   
  AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
  AFIO->MAPR |=  MODE;
  
} 


/**
  * @brief  外部中断初始化函数
  * @param  IRQn 中断号
  * @param  PreemptPriority  抢占优先级
  * @param  SubPriority  响应优先级
  * @retval None
  */
void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);  
  NVIC_SetPriority((IRQn), NVIC_EncodePriority (NVIC_PRIORITY_GROUP, PreemptPriority, SubPriority));  
  NVIC_EnableIRQ(IRQn); 
  
}



/**
  * @brief  禁止对应的内核中断
  * @param  IRQn 中断号
  * @retval None
  */
void NVIC_Disable(IRQn_Type IRQn)
{ 
  NVIC_DisableIRQ((IRQn));
  
}


/*----------------------------------------------------------------------------
    GPIO快速配置函数
 *----------------------------------------------------------------------------*/

/**
  * @brief  外部中断初始化函数
  * @param  Port PA~PG,要初始化的中断端口
  * @param  Pin  0~15, 中断引脚
  * @param  Mode IO输入/输出模式
  * @retval None
  */
void GPIOx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, GPIOx_MODE Mode)
{
  GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
  
  /* 开时钟 */
  RCC->APB2ENR |= (0x1<<(2+Port));
  
  /* 配置工作模式 */
  if (Pin < 8)
  {
    GPIOx[Port]->CRL &= ~(0XFU<<Pin*4);
    
    if (Mode == GPIOx_PUSH_UP)
    {
      GPIOx[Port]->CRL |=  (GPIOx_PU_PD<<Pin*4);
      GPIOx[Port]->ODR |=  (0x1U<<Pin);
    }
    else if (Mode == GPIOx_PUSH_DOWM)
    {
      GPIOx[Port]->CRL |=  (GPIOx_PU_PD<<Pin*4);
      GPIOx[Port]->ODR &= ~(0x1U<<Pin);
    }
    else 
    {
      GPIOx[Port]->CRL |=  (Mode<<Pin*4);
    }
  }
  else
  {
    GPIOx[Port]->CRH &= ~(0XFU<<(Pin-8)*4);
    
    if (Mode == GPIOx_PUSH_UP)
    {
      GPIOx[Port]->CRH |=  (GPIOx_PU_PD<<(Pin-8)*4);
      GPIOx[Port]->ODR |=  (0x1U<<Pin);
    }
    else if (Mode == GPIOx_PUSH_DOWM)
    {
      GPIOx[Port]->CRH |=  (GPIOx_PU_PD<<(Pin-8)*4);
      GPIOx[Port]->ODR &= ~(0x1U<<Pin);
    }
    else 
    {
      GPIOx[Port]->CRH |=  (Mode<<(Pin-8)*4);
    }
  }
  
}


/*----------------------------------------------------------------------------
    EXTI快速配置函数
 *----------------------------------------------------------------------------*/

/**
  * @brief  外部中断初始化函数
  * @param  Port PA~PG,要初始化的中断端口
  * @param  Pin  0~15, 中断引脚
  * @param  Trigger 上升沿/下降沿触发条件
  * @retval None
  */
void EXTIx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, EXTIx_TRIGGER Trigger)
{
  const IRQn_Type EXTIx_IRQn[7] = { EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, \
                                    EXTI3_IRQn, EXTI4_IRQn, EXTI9_5_IRQn, EXTI15_10_IRQn };
  
  /* 开时钟 */
  RCC->APB2ENR |= (0X1<<0);  //AFIO
  
  /* 配置外部中断源 */
  AFIO->EXTICR[Pin/4] &= ~(0XF<<((Pin%4)*4));  
  AFIO->EXTICR[Pin/4] |=  (Port<<((Pin%4)*4));  //中断线x映射到Port中
  
  /* 配置边沿检测 */
  EXTI->RTSR &= ~(0X1<<Pin);
  EXTI->RTSR |=  (getBOOL(Trigger&0X1)<<Pin);  //配置线x的上升沿触发
  
  EXTI->FTSR &= ~(0X1<<Pin);
  EXTI->FTSR |=  (getBOOL(Trigger&0X2)<<Pin);  //配置线x的下降沿触发
  
  /* 配置事件/中断 */
  EXTI->IMR |=  (0X1<<Pin);    //开放线x上的中断请求
  EXTI->EMR &= ~(0X1<<Pin);    //屏蔽线x上的事件请求
  
  /* 配置核级中断 */
  if (Pin >= GPIOx_10)
  {
    NVIC_Enable(EXTIx_IRQn[6], 2, 3);
  }
  else if (Pin >= GPIOx_5)
  {
    NVIC_Enable(EXTIx_IRQn[5], 2, 3);
  }
  else 
  {
    NVIC_Enable(EXTIx_IRQn[Pin], 2, 3);
  }
  
  /* 开启外部中断 */
  EXTI->PR  |=  (0X1<<Pin);    //清标志位
  EXTI->IMR |=  (0X1<<Pin);    
  
}



/*----------------------------------------------------------------------------
    定时器相关配置
 *----------------------------------------------------------------------------*/

TIM_TypeDef * const STM32_TIMER[8] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8};
IRQn_Type const TIMx_IRQn[8] = {TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn};


/**
  * @brief  定时器时钟使能
  * @param  Timer 定时器标号
  * @retval None
  */
void TIMx_EnableClock(TIM_TYPE Timer)
{
  switch (Timer)
  {
    case TIMx_1: RCC->APB2ENR |= (0x1<<11); break;
    case TIMx_2: RCC->APB1ENR |= (0x1<<0);  break;
    case TIMx_3: RCC->APB1ENR |= (0x1<<1);  break;
    case TIMx_4: RCC->APB1ENR |= (0x1<<2);  break;
    case TIMx_5: RCC->APB1ENR |= (0x1<<3);  break;
    case TIMx_6: RCC->APB1ENR |= (0x1<<4);  break;
    case TIMx_7: RCC->APB1ENR |= (0x1<<5);  break;
    case TIMx_8: RCC->APB2ENR |= (0x1<<13); break;
    
    default: break;
  }
  
}


/**
  * @brief  定时器端口配置
  * @param  Timer 定时器标号
  * @param  Channel 通道掩码
  * @retval None
  */
void TIMx_PortConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_PORT_DIRECTION Direction)
{
  switch (Timer)
  {
    case TIMx_1:
    {
            /* 根据状态字配置引脚 */
      if (Channel & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_8, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_9, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (Channel & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_10, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_11, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIMx_2:
    case TIMx_5:
    {
      /* 根据状态字配置引脚 */
      if (Channel & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_0, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_1, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (Channel & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_2, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_3, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIMx_3:
    {
      /* 根据状态字配置引脚 */
      if (Channel & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_6, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_7, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (Channel & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_0, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_1, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIMx_4:
    {
      /* 根据状态字配置引脚 */
      if (Channel & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_6, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_7, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (Channel & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_8, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_9, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIMx_8:
    {
      if (Channel & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PC, GPIOx_6, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (Channel & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PC, GPIOx_7, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      break;
    }
    
    default: break;
  }

}



/**
  * @brief  定时器时钟使能
  * @param  Timer 定时器标号
  * @retval None
  */
void TIMx_IQR_Enable(TIM_TYPE Timer, TIMx_IRQ_CODE NvicCode, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->DIER |= NvicCode;  //开外设中断
    
    if (Timer == TIMx_1)
    {
      switch (NvicCode)
      {
        case TIMx_IRQ_UIF:    NVIC_Enable(TIM1_UP_IRQn, 2, 2);      break;
        case TIMx_IRQ_CC1IF: 
        case TIMx_IRQ_CC2IF: 
        case TIMx_IRQ_CC3IF: 
        case TIMx_IRQ_CC4IF:  NVIC_Enable(TIM1_CC_IRQn, 2, 2);      break;
        case TIMx_IRQ_TIF:    NVIC_Enable(TIM1_TRG_COM_IRQn, 2, 2); break;
        case TIMx_IRQ_GIF:    NVIC_Enable(TIM1_BRK_IRQn, 2, 2);     break;
        
        default: break;
      }
    }
    else if (Timer == TIMx_8)
    {
      switch (NvicCode)
      {
        case TIMx_IRQ_UIF:    NVIC_Enable(TIM8_UP_IRQn, 2, 2);      break;
        case TIMx_IRQ_CC1IF: 
        case TIMx_IRQ_CC2IF: 
        case TIMx_IRQ_CC3IF: 
        case TIMx_IRQ_CC4IF:  NVIC_Enable(TIM8_CC_IRQn, 2, 2);      break;
        case TIMx_IRQ_TIF:    NVIC_Enable(TIM8_TRG_COM_IRQn, 2, 2); break;
        case TIMx_IRQ_GIF:    NVIC_Enable(TIM8_BRK_IRQn, 2, 2);     break;
        
        default: break;
      }
    }
    else 
    {
      NVIC_Enable(TIMx_IRQn[Timer], 2, 2); //开内核中断
    }
    
  }
  else 
  {
    TIM[Timer]->DIER &= ~NvicCode;
    
    if ((TIM[Timer]->DIER & 0x1F) == 0)
    {
      if (Timer == TIMx_1)
      {
        NVIC_Disable(TIM1_UP_IRQn);
        NVIC_Disable(TIM1_CC_IRQn);
        NVIC_Disable(TIM1_TRG_COM_IRQn);
        NVIC_Disable(TIM1_BRK_IRQn);
      }
      else if (Timer == TIMx_8)
      {
        NVIC_Disable(TIM8_UP_IRQn);
        NVIC_Disable(TIM8_CC_IRQn);
        NVIC_Disable(TIM8_TRG_COM_IRQn);
        NVIC_Disable(TIM8_BRK_IRQn);
      }
      else 
      {
        NVIC_Disable(TIMx_IRQn[Timer]);
      }
    }
  }
  
}


