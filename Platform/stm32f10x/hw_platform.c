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
#include "hw_usart.h"
#include "stm32f10x.h"


static void PrintByte(uint8_t Data)
{
  USARTx_SendData(USARTx_1, Data);
}



/**
  * @brief  ϵͳ������ʼ��
  * @param  None
  * @retval None
  */
void System_Init(unsigned int Ticks)
{
  //��ʼ����ʱ�ӿ�
  TIM_TYPE DelayTimer = TIMx_6;
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
  #if 0
	/* ����ʱ��Դ */
  SysTick->CTRL &= ~(1<<2);  //ѡ��9M�ⲿʱ��
  
  /* ��ռ����� */
	SysTick->VAL   = 0x00;      //��ռ�����
  
  /* �����жϼ�� */
  SysTick->LOAD  = 9000 * Ms;   //��ʱ1ms,Ҫ����9000��
  
  /* ���ж� */
  SysTick->CTRL |= (0X1<<1);  //���ж�
  
  /* ��ʱ�� */
  SysTick->CTRL |= (0X1<<0);  //��ϵͳ�δ�
  
  #endif
  
  SysTick_Config(72000*Ms);
  
}


/**
  * @brief  ϵͳ������λ����
  * @param  None
  * @retval None
  * @note   �ں˲�������λ,M0,M3,M4����
  */
void System_SoftwareReset(void)
{
  NVIC_SystemReset();

}


/**
  * @brief  ����ϵͳʱ��Ϊ56M(�ڲ�����ʱ��)
  * @param  None
  * @retval None
  * @note   �����ⲿ����������,ϵͳ��8M��ϵͳʱ������, ������Ҫ�޸�ʱ����,
  *         ��ϵͳ��ʱ�ӱ�Ƶ. �����ڲ�����ʱ������Ƶ��ʱ��������64(8/2*16)M, 
  *         ����64M��ADCʱ��ֻ�ܴﵽ10.67M,��56M��ʱ����ADC����������,�ʶ�Ĭ
  *         ��Ϊ56M.����USB������Ҫ������48M USBʱ����,���������Ҫʹ��USB,
  *         ���齫ʱ�ӱ�ƵΪ48M.
  *         ֵ��ע�����,�ڵ��ⲿ����ʱ�������������,��ֹ��main������ִ�д�
  *         ����,��Ϊ���ⲿ����ʱ�������������,����ִ�е�main����ʱ,ϵͳPLL
  *         �Ѿ�����,�޷��ٶ����໷�������úͲ���,����ADC��USBʱ�ӵĲ�������
  *         ԭ72M��������,����ADC��USB��ʱ�ӳ�����������������(ADCʱ������Ϊ
  *         14M,USBʱ�ӹ̶�Ϊ48M).������������,�����޸�system_stm32f10x.c
  *         �ļ�,������ִ�е�SetSysClockTo72()����ʱ,HSE,PLL��δ����,����ɾ
  *         ��SetSysClockTo72()ԭ�е�����,ִ�б���������ϵͳʱ������.
  *         �����Ҫʵ�ֹ���:���ⲿ����ʱ������ʱִ�г����ʱ������(72M),����
  *         ��ʱ��ʱЧʱ�Զ����ó�56M,ֻ��Ҫ�ѱ���������SetSysClockTo72()����
  *         ĩβ�Ĵ���������δ�����.������HSEʧ��,���Զ����ô˺���
  * 
  */
void System_SetSysClockTo56(void)
{
  #if 1
  /* �����ⲿ����ʱʹ���ڲ�ʱ��HSIʱ������,����STM32F10x����8Mʱ������ */

  /* Enable Prefetch Buffer and set Flash Latency */
  FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

  /* HCLK = SYSCLK */
  RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;   //SYSCLK ����Ƶ��ΪAHB���ߵ�ʱ��

  /* PCLK1 = HCLK/2 */
  RCC->CFGR &= (uint32_t)((uint32_t)~RCC_CFGR_PPRE1);
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;   //APB1����2��Ƶ

  /* PCLK2 = HCLK */
  RCC->CFGR &= (uint32_t)((uint32_t)~RCC_CFGR_PPRE2);
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;   //APB2���߲���Ƶ

  /* USBPRE = PLL */
  RCC->CFGR |= (uint32_t)(RCC_CFGR_USBPRE);     //USBʱ��ΪPLL

  /* ADC = PCLK2/4 = 14MHz */
  RCC->CFGR &= (uint32_t)((uint32_t)~RCC_CFGR_ADCPRE);
  RCC->CFGR |= (uint32_t)RCC_CFGR_ADCPRE_DIV4;  //APB2����4��Ƶ����ΪADC������ʱ��(������14M)

  /* PLL configuration = HSI/2 * 14 = 56 MHz */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));  //HSI/2����ΪPLLSRC����
  RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLMULL14 );                              //PLL = PLLSRC *12 =48MHz    

  /* Enable PLL */
  RCC->CR |= RCC_CR_PLLON;

  /* Wait till PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0);

  /* Select PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;            //SWS = PLL

  /* Wait till PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
  #endif
}


/**
  * @brief  �ⲿ�жϳ�ʼ������
  * @param  IRQn �жϺ�
  * @param  PreemptPriority  ��ռ���ȼ�
  * @param  SubPriority  ��Ӧ���ȼ�
  * @retval None
  */
void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);  
  NVIC_SetPriority((IRQn), NVIC_EncodePriority (NVIC_PRIORITY_GROUP, PreemptPriority, SubPriority));  
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

/**
  * @brief  �ⲿ�жϳ�ʼ������
  * @param  Port PA~PG,Ҫ��ʼ�����ж϶˿�
  * @param  Pin  0~15, �ж�����
  * @param  Mode IO����/���ģʽ
  * @retval None
  */
void GPIOx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, GPIOx_MODE Mode)
{
  GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
  
  /* ��ʱ�� */
  RCC->APB2ENR |= (0x1<<(2+Port));
  
  /* ���ù���ģʽ */
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
  const IRQn_Type EXTIx_IRQn[7] = { EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, \
                                    EXTI3_IRQn, EXTI4_IRQn, EXTI9_5_IRQn, EXTI15_10_IRQn };
  
  /* ��ʱ�� */
  RCC->APB2ENR |= (0X1<<0);  //AFIO
  
  /* �����ⲿ�ж�Դ */
  AFIO->EXTICR[Pin/4] &= ~(0XF<<((Pin%4)*4));  
  AFIO->EXTICR[Pin/4] |=  (Port<<((Pin%4)*4));  //�ж���xӳ�䵽Port��
  
  /* ���ñ��ؼ�� */
  EXTI->RTSR &= ~(0X1<<Pin);
  EXTI->RTSR |=  (getBOOL(Trigger&0X1)<<Pin);  //������x�������ش���
  
  EXTI->FTSR &= ~(0X1<<Pin);
  EXTI->FTSR |=  (getBOOL(Trigger&0X2)<<Pin);  //������x���½��ش���
  
  /* �����¼�/�ж� */
  EXTI->IMR |=  (0X1<<Pin);    //������x�ϵ��ж�����
  EXTI->EMR &= ~(0X1<<Pin);    //������x�ϵ��¼�����
  
  /* ���ú˼��ж� */
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
  
  /* �����ⲿ�ж� */
  EXTI->PR  |=  (0X1<<Pin);    //���־λ
  EXTI->IMR |=  (0X1<<Pin);    
  
}



/*----------------------------------------------------------------------------
    ��ʱ���������
 *----------------------------------------------------------------------------*/

TIM_TypeDef * const STM32_TIMER[8] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8};
IRQn_Type const TIMx_IRQn[8] = {TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn};


/**
  * @brief  ��ʱ��ʱ��ʹ��
  * @param  Timer ��ʱ�����
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
  * @brief  ��ʱ���˿�����
  * @param  Timer ��ʱ�����
  * @param  Channel ͨ������
  * @retval None
  */
void TIMx_PortConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_PORT_DIRECTION Direction)
{
  switch (Timer)
  {
    case TIMx_1:
    {
            /* ����״̬���������� */
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
      /* ����״̬���������� */
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
      /* ����״̬���������� */
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
      /* ����״̬���������� */
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
      NVIC_Enable(TIMx_IRQn[Timer], 2, 2); //���ں��ж�
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

