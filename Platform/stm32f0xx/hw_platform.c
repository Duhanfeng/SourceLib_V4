/**
  ******************************************************************************
  * @file    hw_platform.c
  * @author  �Ź��Ӻ���
  * @version V1.0
  * @date    2017.03.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * STM32F10Xƽ̨ͨ���ļ�,������Ӳ����SourceLib�Ľ�������,����Cortex-M3�ں˵���ز�
  * �ٵķ�װ,�Լ�GPIO,EXTI,TIM��Ӳ����ͨ������
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
  * @brief  ϵͳ������ʼ��
  * @param  None
  * @retval None
  */
void System_Init(unsigned int Ticks)
{
  //��ʼ����ʱ�ӿ�
  TIM_TYPE DelayTimer = TIMx_17;
  TIMx_DelayInit(DelayTimer);
  SL_DelayOperation(TIMx_DelayUs, SLTimer_Delay);
  
  //�ض���printf����
  SL_PrintOperation(PrintByte);
  
  //��ʼ��ϵͳ�δ�
  SysTick_Init(Ticks);     //ϵͳʱ�ӵδ��ʼ��
  SLTimer_SetTick(Ticks);  //�����ں�ʱ�ӵδ�
  IT_IRQ_FuncLogin(SLTimer_Update, IT_SysTick_SL); //���ص�����SLTimer_Updateע����SysTick�ж���
  
}



/*----------------------------------------------------------------------------
    ϵͳ�ں˲�/�ײ�������ô���
 *----------------------------------------------------------------------------*/


/**
  * @brief  ϵͳ�δ�ʱ�жϳ�ʼ������
  * @param  ms ��ʱ�ж�ʱ����
  * @retval None
  */
void SysTick_Init(uint16_t Ms)
{
  SysTick_Config(48000*Ms);
  
}


/**
  * @brief  ϵͳ�����λ����
  * @param  None
  * @retval None
  * @note   �ں˲������λ,M0,M3,M4����
  */
void System_SoftwareReset(void)
{
  NVIC_SystemReset();

}




/**
  * @brief  �ⲿ�жϳ�ʼ������
  * @param  IRQn �жϺ�
  * @param  PreemptPriority  ��ռ���ȼ�
  * @param  SubPriority  ��Ӧ���ȼ�
  * @retval None
  */
void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority)
{
//  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);  
//  NVIC_SetPriority((IRQn), NVIC_EncodePriority (NVIC_PRIORITY_GROUP, PreemptPriority, SubPriority));  
  NVIC_EnableIRQ(IRQn); 
  
}


/**
  * @brief  ��ֹ��Ӧ���ں��ж�
  * @param  IRQn �жϺ�
  * @retval None
  */
void NVIC_Disable(IRQn_Type IRQn)
{ 
  NVIC_DisableIRQ((IRQn));
  
}




/*----------------------------------------------------------------------------
    GPIO�������ú���
 *----------------------------------------------------------------------------*/
typedef struct
{
  unsigned char OpSpeed    : 2; //[1:0]
  unsigned char PullUp     : 2; //[3:2]
  unsigned char OpType     : 2; //[5:4]
  unsigned char Moder      : 2; //[7:6]
}GPIO_REG_CODE;


/**
  * @brief  ͨ��IO��ʼ������
  * @param  Port PA~PG,Ҫ��ʼ�����ж϶˿�
  * @param  Pin  0~15, �ж�����
  * @param  Mode IO����/���ģʽ
  * @retval None
  */
void GPIOx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, GPIOx_MODE Mode)
{
  GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};
  GPIO_REG_CODE  *GpioRegCode = (GPIO_REG_CODE *)&Mode;
  
  // ��ʱ��
  RCC->AHBENR |= (0x1<<(17+Port));
  
  // ����ģʽ
  GPIOx[Port]->MODER   &= ~(0x3<<Pin*2);
  GPIOx[Port]->MODER   |=  (GpioRegCode->Moder<<Pin*2);
  
  // �����������
  GPIOx[Port]->OTYPER  &= ~(0x1<<Pin*1);
  GPIOx[Port]->OTYPER  |=  (GpioRegCode->OpType<<Pin*1);
  
  // ����������
  GPIOx[Port]->PUPDR   &= ~(0x3<<Pin*2);
  GPIOx[Port]->PUPDR   |=  (GpioRegCode->PullUp<<Pin*2);
  
  // ��������ٶ�
  GPIOx[Port]->OSPEEDR &= ~(0x3<<Pin*2);
  GPIOx[Port]->OSPEEDR |=  (GpioRegCode->OpSpeed<<Pin*2);
  
}


/*----------------------------------------------------------------------------
    EXTI�������ú���
 *----------------------------------------------------------------------------*/

/**
  * @brief  �ⲿ�жϳ�ʼ������
  * @param  Port PA~PG,Ҫ��ʼ�����ж϶˿�
  * @param  Pin  0~15, �ж�����
  * @param  Trigger ������/�½��ش�������
  * @retval None
  */
void EXTIx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, EXTIx_TRIGGER Trigger)
{
  


}



/*----------------------------------------------------------------------------
    ��ʱ���������
 *----------------------------------------------------------------------------*/
//ֵ��ע�����,TIM6��TIM15��STM32F030x8�����ϲ��е�
//TIM7��STM32F030xB����
TIM_TypeDef * const STM32_TIMER[8] = {TIM1, TIM3, TIM6, TIM7, TIM14, TIM15, TIM16, TIM17};


IRQn_Type const TIMx_IRQn[8] = {TIM1_BRK_UP_TRG_COM_IRQn, TIM3_IRQn, (IRQn_Type)0, (IRQn_Type)0, TIM14_IRQn, TIM15_IRQn, TIM16_IRQn, TIM17_IRQn};


/**
  * @brief  ��ʱ��ʱ��ʹ��
  * @param  Timer ��ʱ�����
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
  * @brief  ��ʱ���˿�����
  * @param  Timer ��ʱ�����
  * @param  Channel ͨ������
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
  * @brief  ��ʱ��ʱ��ʹ��
  * @param  Timer ��ʱ�����
  * @retval None
  */
void TIMx_IQR_Enable(TIM_TYPE Timer, TIMx_IRQ_CODE NvicCode, uint8_t isEnable)
{

  if (isEnable)
  {
    TIM[Timer]->DIER |= NvicCode;  //�������ж�
    
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
      NVIC_Enable(TIMx_IRQn[Timer], 2); //���ں��ж�
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


