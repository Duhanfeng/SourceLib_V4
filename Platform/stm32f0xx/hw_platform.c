/**
  ******************************************************************************
  * @file    hw_platform.c
  * @author  杜公子寒枫
  * @version V1.0
  * @date    2017.03.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * STM32F10X平台通用文件,包括了硬件和SourceLib的交互操作,关于Cortex-M3内核的相关操
  * 操的封装,以及GPIO,EXTI,TIM等硬件的通用配置
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
//#include "hw_usart.h"
#include "stm32f0xx.h"



GPIOx_DR_CTRL *GPIOA_IN_CTRL  = (GPIOx_DR_CTRL *)(GPIOA_BASE + 0x10);
GPIOx_DR_CTRL *GPIOB_IN_CTRL  = (GPIOx_DR_CTRL *)(GPIOB_BASE + 0x10);
GPIOx_DR_CTRL *GPIOC_IN_CTRL  = (GPIOx_DR_CTRL *)(GPIOC_BASE + 0x10);
GPIOx_DR_CTRL *GPIOD_IN_CTRL  = (GPIOx_DR_CTRL *)(GPIOD_BASE + 0x10);
GPIOx_DR_CTRL *GPIOE_IN_CTRL  = (GPIOx_DR_CTRL *)(GPIOE_BASE + 0x10);
GPIOx_DR_CTRL *GPIOF_IN_CTRL  = (GPIOx_DR_CTRL *)(GPIOF_BASE + 0x10);

GPIOx_DR_CTRL *GPIOA_OUT_CTRL = (GPIOx_DR_CTRL *)(GPIOA_BASE + 0x14);
GPIOx_DR_CTRL *GPIOB_OUT_CTRL = (GPIOx_DR_CTRL *)(GPIOB_BASE + 0x14);
GPIOx_DR_CTRL *GPIOC_OUT_CTRL = (GPIOx_DR_CTRL *)(GPIOC_BASE + 0x14);
GPIOx_DR_CTRL *GPIOD_OUT_CTRL = (GPIOx_DR_CTRL *)(GPIOD_BASE + 0x14);
GPIOx_DR_CTRL *GPIOE_OUT_CTRL = (GPIOx_DR_CTRL *)(GPIOE_BASE + 0x14);
GPIOx_DR_CTRL *GPIOF_OUT_CTRL = (GPIOx_DR_CTRL *)(GPIOF_BASE + 0x14);





static void PrintByte(uint8_t Data)
{
//  USARTx_SendData(USARTx_1, Data);
}



/**
  * @brief  系统参数初始化
  * @param  None
  * @retval None
  */
void System_Init(unsigned int Ticks)
{
  //初始化延时接口
  TIM_TYPE DelayTimer = TIMx_17;
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
  SysTick_Config(48000*Ms);
  
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
  * @brief  外部中断初始化函数
  * @param  IRQn 中断号
  * @param  PreemptPriority  抢占优先级
  * @param  SubPriority  响应优先级
  * @retval None
  */
void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority)
{
//  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);  
//  NVIC_SetPriority((IRQn), NVIC_EncodePriority (NVIC_PRIORITY_GROUP, PreemptPriority, SubPriority));  
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
typedef struct
{
  unsigned char OpSpeed    : 2; //[1:0]
  unsigned char PullUp     : 2; //[3:2]
  unsigned char OpType     : 2; //[5:4]
  unsigned char Moder      : 2; //[7:6]
}GPIO_REG_CODE;


/**
  * @brief  通用IO初始化函数
  * @param  Port PA~PG,要初始化的中断端口
  * @param  Pin  0~15, 中断引脚
  * @param  Mode IO输入/输出模式
  * @retval None
  */
void GPIOx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, GPIOx_MODE Mode)
{
  GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};
  GPIO_REG_CODE  *GpioRegCode = (GPIO_REG_CODE *)&Mode;
  
  // 开时钟
  RCC->AHBENR |= (0x1<<(17+Port));
  
  // 配置模式
  GPIOx[Port]->MODER   &= ~(0x3<<Pin*2);
  GPIOx[Port]->MODER   |=  (GpioRegCode->Moder<<Pin*2);
  
  // 配置输出类型
  GPIOx[Port]->OTYPER  &= ~(0x1<<Pin*1);
  GPIOx[Port]->OTYPER  |=  (GpioRegCode->OpType<<Pin*1);
  
  // 配置上下拉
  GPIOx[Port]->PUPDR   &= ~(0x3<<Pin*2);
  GPIOx[Port]->PUPDR   |=  (GpioRegCode->PullUp<<Pin*2);
  
  // 配置输出速度
  GPIOx[Port]->OSPEEDR &= ~(0x3<<Pin*2);
  GPIOx[Port]->OSPEEDR |=  (GpioRegCode->OpSpeed<<Pin*2);
  
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
  


}



/*----------------------------------------------------------------------------
    定时器相关配置
 *----------------------------------------------------------------------------*/
//值得注意的是,TIM6和TIM15是STM32F030x8及以上才有的
//TIM7是STM32F030xB才有
TIM_TypeDef * const STM32_TIMER[8] = {TIM1, TIM3, TIM6, TIM7, TIM14, TIM15, TIM16, TIM17};


IRQn_Type const TIMx_IRQn[8] = {TIM1_BRK_UP_TRG_COM_IRQn, TIM3_IRQn, (IRQn_Type)0, (IRQn_Type)0, TIM14_IRQn, TIM15_IRQn, TIM16_IRQn, TIM17_IRQn};


/**
  * @brief  定时器时钟使能
  * @param  Timer 定时器标号
  * @retval None
  */
void TIMx_EnableClock(TIM_TYPE Timer)
{
  switch (Timer)
  {
    case TIMx_1:  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; break;
    case TIMx_3:  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  break;
    case TIMx_6:  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;  break;
    case TIMx_7:  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;  break;
    case TIMx_14: RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; break;
    case TIMx_15: RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; break;
    case TIMx_16: RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; break;
    case TIMx_17: RCC->APB2ENR |= RCC_APB2ENR_TIM17EN; break;

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
        
        case TIMx_IRQ_CC1IF: 
        case TIMx_IRQ_CC2IF: 
        case TIMx_IRQ_CC3IF: 
        case TIMx_IRQ_CC4IF:  NVIC_Enable(TIM1_CC_IRQn, 2);      break;
        case TIMx_IRQ_UIF:
        case TIMx_IRQ_TIF:
        case TIMx_IRQ_GIF:    NVIC_Enable(TIM1_BRK_UP_TRG_COM_IRQn, 2);     break;
        
        default: break;
      }
    }
    else 
    {
      NVIC_Enable(TIMx_IRQn[Timer], 2); //开内核中断
    }
    
  }
  else 
  {
    TIM[Timer]->DIER &= ~NvicCode;
    
    if ((TIM[Timer]->DIER & 0x1F) == 0)
    {
      if (Timer == TIMx_1)
      {
        NVIC_Disable(TIM1_BRK_UP_TRG_COM_IRQn);
        NVIC_Disable(TIM1_CC_IRQn);
      }
      else 
      {
        NVIC_Disable(TIMx_IRQn[Timer]);
      }
    }
  }

}


