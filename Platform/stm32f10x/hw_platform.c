/**
  ******************************************************************************
  * @file    hw_platform.c
  * @author  �Ź��Ӻ���
  * @version V1.1
  * @date    2017.03.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * STM32F10Xƽ̨ͨ���ļ�,������Ӳ����SourceLib�Ľ�������,����Cortex-M3�ں˵���ز�
  * �ٵķ�װ,�Լ�GPIO,EXTI,TIM��Ӳ����ͨ������
  * 
  * V1.1------------
  * �޸�����: ���Ӷ�SWJ���ܵ�����,����ʵ�ֶ�JTAG�ں�SW�ڵ�����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.1
  * �޸�����: 2017.04.07
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

//��ʱ��ʱ������
static TIM_TypeDef *HW_DelayTimer = TIM1;


//DEBUG_PRINT�ĵײ���ýӿ�
static void PrintByte(uint8_t Data)
{
//  USARTx_SendData(USARTx_1, Data);
  
}

//΢�뼶��ʱ�����Դ���
static void My_DelayUs(uint32_t Us)
{
  TIMx_DelayUs(HW_DelayTimer, Us);
  
}

//���뼶��ʱ�����Դ���
static void My_DelayMs(uint32_t Ms)
{
  SLTimer_Delay(Ms);
  
}



/**
  * @brief  ϵͳ������ʼ��
  * @param  None
  * @retval None
  */
void System_Init(unsigned int Ticks)
{
  //����JTAG��
  SWJ_Config(SWJ_ONLY_SW);
  
  //��ʼ����ʱ�ӿ�
  TIMx_DelayInit(HW_DelayTimer);
  SL_DelayOperation(My_DelayUs, My_DelayMs);
  
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
  
  SysTick_Config(SYSTEM_FCLK/1000*Ms);
  
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
  * @brief  ����ϵͳʱ��Ϊ56M(�ڲ�����ʱ��)
  * @param  None
  * @retval None
  * @note   �����ⲿ����������,ϵͳ�ں�ʱ��ԴΪ�ڲ����پ���(HSI),��ʱ���ں�ʱ��Ϊ8M.
  *         Ϊ�˷��Ӹ��õ�����,���Խ��ڲ����پ���Ƶ.������ʱ�����Ľṹ,�ڲ����پ���
  *         ��Ƶ��ɴﵽ�����ʱ��Ƶ��Ϊ64M(8M/2*16=64M).����������ܵ�64M��,���ǳ�
  *         �õ�Ƶ�ʻ���56M��48M.��56M��ADC��������ǿ
  */
void System_CoreClockConfigure(SYS_CORE_CLOCK CoreClock)
{
   
  switch (CoreClock)
  {
    #ifdef STM32F10X_CL
    case SYS_CLOCK_16M: SystemClock = 16000000; break;
    case SYS_CLOCK_20M: SystemClock = 20000000; break;
    case SYS_CLOCK_24M: SystemClock = 24000000; break;
    case SYS_CLOCK_26M: SystemClock = 26000000; break;
    case SYS_CLOCK_28M: SystemClock = 28000000; break;
    case SYS_CLOCK_32M: SystemClock = 32000000; break;
    case SYS_CLOCK_36M: SystemClock = 36000000; break;
    #else
    case SYS_CLOCK_48M: SystemClock = 48000000; break;
    case SYS_CLOCK_56M: SystemClock = 56000000; break;
    case SYS_CLOCK_64M: SystemClock = 64000000; break;
    #endif
    
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
  * @brief  JTAGģʽ����,��������JTAG��ģʽ
  * @param  mode:jtag,swdģʽ����;00,ȫʹ��;01,ʹ��SWD;10,ȫ�ر�;
  * @retval None
  */
void SWJ_Config(SWJ_CFG_MODE MODE)
{
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //��������ʱ��	   
  AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
  AFIO->MAPR |=  MODE;
  
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
  EXTI->RTSR |=  (GET_BOOL(Trigger&0X1)<<Pin);  //������x�������ش���
  
  EXTI->FTSR &= ~(0X1<<Pin);
  EXTI->FTSR |=  (GET_BOOL(Trigger&0X2)<<Pin);  //������x���½��ش���
  
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




/**
  * @brief  ��ʱ���˿�����
  * @param  Timer ��ʱ�����
  * @param  ChMask ͨ������
  * @retval None
  */
void TIMx_PortConfig(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_PORT_DIRECTION Direction)
{
  switch ((uint32_t)TIMx)
  {
    case TIM1_BASE:
    {
      /* ����״̬���������� */
      if (ChMask & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_8, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_9, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (ChMask & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_10, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_11, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIM2_BASE:
    case TIM5_BASE:
    {
      /* ����״̬���������� */
      if (ChMask & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_0, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_1, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (ChMask & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_2, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_3, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIM3_BASE:
    {
      /* ����״̬���������� */
      if (ChMask & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_6, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PA, GPIOx_7, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (ChMask & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_0, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_1, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIM4_BASE:
    {
      /* ����״̬���������� */
      if (ChMask & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_6, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_7, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      if (ChMask & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_8, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PB, GPIOx_9, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      
      break;
    }
    case TIM8_BASE:
    {
      if (ChMask & (0X1<<0))
      {
        GPIOx_FastInit(GPIOx_PC, GPIOx_6, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<1))
      {
        GPIOx_FastInit(GPIOx_PC, GPIOx_7, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<2))
      {
        GPIOx_FastInit(GPIOx_PC, GPIOx_8, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }
      if (ChMask & (0X1<<3))
      {
        GPIOx_FastInit(GPIOx_PC, GPIOx_9, (Direction == TIMx_Port_Output) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      }  
      break;
    }
    
    default: break;
  }

}











