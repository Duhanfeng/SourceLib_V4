/**
  ******************************************************************************
  * @file    Usart.h
  * @author  �Ź��Ӻ���
  * @version V3.0 �Ĵ����汾
  * @date    2016.11.18
  * @brief   USART1~4 ����ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * USART1: TX --- PA9  ��������,50M
  *         RX --- PA10 ��������
  *
  * USART2: TX --- PA2  ��������,50M
  *         RX --- PA3  ��������
  *
  * USART3: TX --- PB10 ��������,50M
  *         RX --- PB11 ��������
  *
  * USART4: TX --- PC10 ��������,50M
  *         RX --- PC11 ��������
  *
  * V1.1------------
  * �޸�����: �޸����������ò�׼ȷ�Ĵ���
  * ����ԭ��: �����ʷ�Ƶ���Ӽ��㹫ʽ(��)��,Ĭ�϶���Ϊ����,����С�����ֶ�ʧ
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.1
  * �޸�����: 2015.12.13
  * 
  * V1.2------------
  * �޸�����: ���ӵ����Ľ����жϿ�����رսӿں���
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.2
  * �޸�����: 2015.12.21
  * 
  * V1.3------------
  * �޸�����: �޸��ļ��ܹ�,����UART+DMA����,�޸�H�ļ�����,�����ļ�ע��ͷ
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.3
  * �޸�����: 2016.02.18
  * 
  * V1.4------------
  * �޸�����: 1.�޸��ڳ�ʼ���׶�Tx�Ų���һ��������Ĵ���(�˸�����ᱻʶ��Ϊһ���ַ�)
  *           2.�����ڽ����޸�Ϊ�����ж�ģʽ
  * ����ԭ��: ������IO��Ϊ����ģʽ��,���ڴ�ʱUART4��ʱ����δ����,���Ե�ƽ������.
  *           ������,����UART4��ʱ�Ӳ���ʹ�ܷ��͹��ܺ�,IOͨ��,�ָ��ߵ�ƽ,�ʶ�����Ϊ
  *           һ��������,���ɱ�ʶ��Ϊһ���ַ�(0X80,0XFF,0XFE��).
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.4
  * �޸�����: 2016.02.20
  *
  * V1.5------------
  * �޸�����: 1.�����˵�ַ��־ģʽ,�������ڶദ����ģʽ�µ�Ӳ��ʶ��(һ���Ǵӻ���)
  *           2.ɾ�����ֲ���Ҫ�ĺ���,����ܹ�
  * ��ַ���: ��Ϊ��������һ��������ַ(4λ),�������������뾲Ĭ״̬.��ʱ,�������Ὣ��
  *           ��λΪ'1'������ʶ��Ϊ"��ַ",���λΪ'0'��ʶ��Ϊ����.�����������յ�һ��
  *           "��ַ"ʱ,��Դ�"��ַ"�����ж�:��4λ�Ƿ���"������ַƥ��",��ƥ��,����ս�
  *           ������"����",ֱ����һ��"��ַ"�ĵ���,��ƥ��,���������,����ƥ��,��
  *           ���½��뾲Ĭ״̬.
  *           ��ַ���ģʽ��ʹ���ڶദ����ͨ���Ҵ�����������,������Ӳ����ɸѡ����CPU
  *           ������. ���RM0008 P805  Multiprocessor communication
  * ��֪ȱ��: 1.��ַ���ģʽ��,������������ɺ�,����֡�ĵ��������ᴥ��"����֡"�ж�,��
  *             ����ʹUART�Ŀ����Խ���
  *           2.��ַ���ģʽ��,�����Ƕ����λ���ж����ݵķ���(��ַ/����),�������λ��
  *             ʹ���ܵ�������.
  * ���ⷽ��: ����"���ջ������ǿ��ж�","��ַ"�ַ�ƥ��֮��ᴥ�����ж�,�ڴ��ж�����ʱ��
  *           �ս�������Ĭ״̬,�ڴ�������֡�жϺ��ٿ�����Ĭ״̬.
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.5
  * �޸�����: 2016.03.15
  *
  * V2.0------------
  * �޸�����: 1.�Ƴ���ַ��־ģʽ
  *           2.����һ������DMA���͵�BUG
  * ��ַ��־: ��ַ��־ģʽ������ͬһ������������һ����ӵ����,ʹ���Ͻ�Ϊ����,��
  *           �����ڳ����ʹ�ó���,���ҵ�ַ��־ģʽ��Ӵ������ĸ��ӳ̶�,��֪��ȱ
  *           �ݽ��ѱ���,Ŀǰû���ҵ��Ƚϼ򵥷����ʹ�÷���,�ʶ��Ƴ���ַ��־ģʽ.
  * DMA ����: ����DMA����һ������,������"DMA�������(TCIF)"������رմ��ڷ��͹���,
  *           ʹ��ʾ�������ִ���ֻ������һ���ֵ�����.���� STM32�ο��ֲ�(Rev 16)
  *           818ҳ Figure 296 ��֪,DMA������ɼ�����"TCIF"��־,����ʱ������δ��
  *           �����,�رմ��ڻ�����ڷ��͵�������ɽض�.��Ҫ�����BUG,���԰�DMA��
  *           �͵���ɱ�־�ж���"DMA_TCIF"�ĳ�"USART_TC".
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.05.16
  *
  * V3.0------------
  * �޸�����: 1.������1~4���ϵ�ͬһ���ļ���,ͬһ���������Ա��������ڽ��е���,ͨ
  *             ���ṹ��ָ������ķ�ʽӳ�䵽ÿһ���Ĵ�����֮��,�±���ʵ�ʵ�ֻ�
  *             �Ʊ���������������ж�.
  *           2.����STM32F10xȫϵ�е�оƬ,���밲ȫ����,������ͬоƬ�Ĳ����Ի�
  *             ��������ǶԷǷ���ַ�ķ���.
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V3.0
  * �޸�����: 2016.08.09
  * 
  * V3.1------------
  * �޸�����: �޸�DMA���ͻ���
  * �޸�ԭ��: �����ⲿ�������ô�DMA���ͺ���,��εĻ�������ջ�ϵľֲ�����,��ô��
  *           �ⲿ����������ջ�ϵĿռ䱻����(��ʱUSART���ڷ�����).�����ʱ�ڱ��
  *           �������޸���ջ�ϵ�����,���ڵķ��ͱ���Ч.�ʶ��޸��䷢�ͻ���,�ڷ���
  *           ������,��ջ����ʱ�洢�����ݸ��Ƶ����ϵĿռ���,������ķ�ʽ���Ӷ��
  *           ���ͻ�����,��DMA������ɺ�(DMA��������ж�),�ͷ�������Ŀռ�.ֻҪ
  *           ���Ͽ��õĿռ��㹻,���ͼ�ʮK������,CPU��ռ��Ҳ������2MS
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V3.1
  * �޸�����: 2016.11.18
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_usart.h"
#include "SourceLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void USART1_IDLE_IRQCallBack(void); 
void USART2_IDLE_IRQCallBack(void); 
void USART3_IDLE_IRQCallBack(void); 
void USART4_IDLE_IRQCallBack(void);

void DMA1_Channe4_IRQCallBack(void);
void DMA1_Channe7_IRQCallBack(void);
void DMA1_Channe2_IRQCallBack(void);
void DMA2_Channe5_IRQCallBack(void);


/* ---���ڷ�Ƶϵ�������--- */
#define USARTx_GET_BRR(Fpclk,BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))
  

/*----------------------------------------------------------------------------
    ���ݻ���������
 *----------------------------------------------------------------------------*/
 
 /* ---�������ݻ����С--- */
#define RBUF_SIZE   256      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

/* ---������ջ�����--- */
uint8_t Uart1_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart2_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart3_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart4_RecvBuff[RBUF_SIZE] = {0};

/*----------------------------------------------------------------------------
    ��ؼĴ���ӳ��
 *----------------------------------------------------------------------------*/

/* ---ӳ������Ĵ���--- */
#if(defined(STM32F10X_HD)||defined(STM32F10X_HD_VL)||defined(STM32F10X_XL)||defined(STM32F10X_CL))
static USART_TypeDef * const USART[USARTx_COUNT] = {USART1,USART2,USART3,UART4};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[USARTx_COUNT] = {DMA1_Channel5,DMA1_Channel6,DMA1_Channel3,DMA2_Channel3};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[USARTx_COUNT] = {DMA1_Channel4,DMA1_Channel7,DMA1_Channel2,DMA2_Channel5};
static const IRQn_Type USARTx_IRQn[USARTx_COUNT] = {USART1_IRQn,USART2_IRQn,USART3_IRQn,UART4_IRQn};
static const IRQn_Type USARTx_DMA_IRQn[USARTx_COUNT] = {DMA1_Channel4_IRQn, DMA1_Channel7_IRQn, DMA1_Channel2_IRQn, DMA2_Channel4_5_IRQn};

#elif(defined(STM32F10X_MD)||defined(STM32F10X_MD_VL))
static USART_TypeDef * const USART[USARTx_COUNT] = {USART1,USART2,USART3};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[USARTx_COUNT] = {DMA1_Channel5,DMA1_Channel6,DMA1_Channel3};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[USARTx_COUNT] = {DMA1_Channel4,DMA1_Channel7,DMA1_Channel2};
static const IRQn_Type USARTx_IRQn[USARTx_COUNT] = {USART1_IRQn,USART2_IRQn,USART3_IRQn};

#elif(defined(STM32F10X_LD)||defined(STM32F10X_LD_VL))
static USART_TypeDef * const USART[USARTx_COUNT] = {USART1,USART2};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[USARTx_COUNT] = {DMA1_Channel5,DMA1_Channel6};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[USARTx_COUNT] = {DMA1_Channel4,DMA1_Channel7};
static const IRQn_Type USARTx_IRQn[USARTx_COUNT] = {USART1_IRQn,USART2_IRQn};

#else
#error Undefine STM32F10x Flash Size

#endif 



/* �ڲ�����-------------------------------------------------------------- */

//IO����
static void USARTx_IOConfig(USART_TYPE Port)
{
  switch (Port)
  {
    case USARTx_1: 
    {
      GPIOx_FastInit(GPIOx_PA, GPIOx_10, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PA, GPIOx_9, GPIOx_AF_PP_50M);
      
      break;
    }
    case USARTx_2: 
    {
      GPIOx_FastInit(GPIOx_PA, GPIOx_2, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PA, GPIOx_3, GPIOx_PUSH_UP);
      
      break;
    }
    case USARTx_3: 
    {
      GPIOx_FastInit(GPIOx_PB, GPIOx_10, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_11, GPIOx_PUSH_UP);
      
      break;
    }
    case USARTx_4: 
    {
      GPIOx_FastInit(GPIOx_PC, GPIOx_10, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PC, GPIOx_11, GPIOx_PUSH_UP);
      
      break;
    }
    default: break;
  }
  
}
  


//����ģʽ����
static void USARTx_ModeConfig(USART_TYPE Port, uint32_t iBaudRate)
{
  /* ��ʱ�� */
  if (Port == USARTx_1)
  {
    RCC->APB2ENR |=  (0X1<<14);
  }
  else 
  {
    RCC->APB1ENR |=  (0X1<<(16+Port));
  }
  
  /* ���ù���ģʽ */
  USART[Port]->CR1 &= ~(0X1<<12);  //8λ�ֳ�
  USART[Port]->CR1 &= ~(0X1<<10);  //��ʹ��У��λ
  USART[Port]->CR1 |=  (0X1<<3);   //ʹ�ܷ���
  USART[Port]->CR1 |=  (0X1<<2);   //ʹ�ý���
  USART[Port]->CR2 &= ~(0X3<<12);  //1λֹͣλ
  USART[Port]->CR3 |=  (0X1<<7);   //DMA����ʹ��
  USART[Port]->CR3 |=  (0X1<<6);   //DMA����ʹ��
  
  /* �����ж� */
  USART[Port]->CR1 &= ~(0X1<<6);   //�ط�������ж�
  USART[Port]->CR1 &= ~(0X1<<5);   //�ؽ����ж�
  USART[Port]->CR1 |=  (0X1<<4);   //�������ж�
  
  NVIC_Enable(USARTx_IRQn[Port],2,2); //���ں��ж�
  
  /* ���ò����� */
  USART[Port]->BRR = USARTx_GET_BRR((Port == USARTx_1 )? APB2_FCLK : APB1_FCLK,iBaudRate);
  
  /* ������ */
  USART[Port]->CR1 |=  (0x1<<13);
  
}



//UARTx_Tx DMAͨ������
static void USARTx_TxDMAConfig(USART_TYPE Port)
{
  /* ��ʱ�� */
  if (Port != USARTx_4)
  {
    RCC->AHBENR |= (0X1<<0);  //DMA1ʱ��ʹ�� 
  }
  else
  {
    RCC->AHBENR |= (0X1<<1);  //DMA2ʱ��ʹ��
  }
  
  /* ���ù���ģʽ */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<14);  //���ڴ�-�ڴ�ģʽ
  USART_TxDmaChannel[Port]->CCR &= ~(0X3<<12);
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<12);  //�е����ȼ�
  
  USART_TxDmaChannel[Port]->CCR &= ~(0X3<<10);
  USART_TxDmaChannel[Port]->CCR |=  (0X0<<10);  //�ڴ�:8λ����֡��ʽ
  USART_TxDmaChannel[Port]->CCR &= ~(0X3<<8);
  USART_TxDmaChannel[Port]->CCR |=  (0X0<<8);   //����:8λ����֡��ʽ
  
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<7);   //�ڴ�����ģʽ
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<6);   //���������ģʽ
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<5);   //��ѭ��ģʽ
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<4);   //���ڴ��ж�
  
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<3);   //�رմ����ж�
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<2);   //�رհ봫���ж�
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<1);   //����������ж�
  
  NVIC_Enable(USARTx_DMA_IRQn[Port],2,2); //���ں��ж�
  
  /* ���������ַ */
  USART_TxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->DR);
  
  /* �ر�ͨ�� */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);   //�ر�ͨ��,�ڿ�������/����ʱ������
  
}
  

//UARTx_Rx DMAͨ������
static void USARTx_RxDMAConfig(USART_TYPE Port)
{
  const uint8_t * const arBuffAddr[4] = { Uart1_RecvBuff,Uart2_RecvBuff,
                                          Uart3_RecvBuff,Uart4_RecvBuff };
  
  /* ��ʱ�� */
  if (Port != USARTx_4)
  {
    RCC->AHBENR |= (0X1<<0);  //DMA1ʱ��ʹ�� 
  }
  else
  {
    RCC->AHBENR |= (0X1<<1);  //DMA2ʱ��ʹ��
  }
  
  /* ���ù���ģʽ */
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<14);  //���ڴ�-�ڴ�ģʽ
  USART_RxDmaChannel[Port]->CCR &= ~(0X3<<12);
  USART_RxDmaChannel[Port]->CCR |=  (0X1<<12);  //�е����ȼ�

  USART_RxDmaChannel[Port]->CCR &= ~(0X3<<10);
  USART_RxDmaChannel[Port]->CCR |=  (0X0<<10);  //�ڴ�:8λ����֡��ʽ
  USART_RxDmaChannel[Port]->CCR &= ~(0X3<<8);
  USART_RxDmaChannel[Port]->CCR |=  (0X0<<8);   //����:8λ����֡��ʽ

  USART_RxDmaChannel[Port]->CCR |=  (0X1<<7);   //�ڴ�����ģʽ
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<6);   //���������ģʽ
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<5);   //��ѭ��ģʽ
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<4);   //�������ж�

  /* ���������ж� */
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<3);   //�رմ����ж�
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<2);   //�رհ봫���ж�
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<1);   //�رմ�������ж�
  
  /* ���������ַ */
  USART_RxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->DR);
  
  /* �����ڴ��ַ */
  USART_RxDmaChannel[Port]->CMAR = (uint32_t)arBuffAddr[Port];
  
  /* ���ô������� */
  USART_RxDmaChannel[Port]->CNDTR = RBUF_SIZE;
  
  /* ����ͨ�� */
  USART_RxDmaChannel[Port]->CCR |=  (0X1<<0);   //�ر�ͨ��,�ڿ�������/����ʱ������
  
}


//UART1_Rx DMAͨ����λ
void USARTx_RxDMAReset(USART_TYPE Port)
{
  /* �ر�ͨ�� */
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* ���÷������� */
  USART_RxDmaChannel[Port]->CNDTR = RBUF_SIZE;
    
  /* ����ͨ�� */
  USART_RxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}



//ע�ᴮ�ڿ����жϵĻص�����
static void USARTx_LoginIRQFunc(USART_TYPE Port)
{
  switch (Port)
  {
    case USARTx_1: 
    {
      IT_IRQ_FuncLogin(USART1_IDLE_IRQCallBack, IT_USART1_IDLE); 
      IT_IRQ_FuncLogin(DMA1_Channe4_IRQCallBack, IT_DMA1_CH4);  
      break;
    }
    case USARTx_2: 
    {
      IT_IRQ_FuncLogin(USART2_IDLE_IRQCallBack, IT_USART2_IDLE); 
      IT_IRQ_FuncLogin(DMA1_Channe7_IRQCallBack, IT_DMA1_CH7); 
      break;
    }
    case USARTx_3: 
    {
      IT_IRQ_FuncLogin(USART3_IDLE_IRQCallBack, IT_USART3_IDLE); 
      IT_IRQ_FuncLogin(DMA1_Channe2_IRQCallBack, IT_DMA1_CH2);  
      break;
    }
    case USARTx_4: 
    {
      IT_IRQ_FuncLogin(USART4_IDLE_IRQCallBack, IT_USART4_IDLE); 
      IT_IRQ_FuncLogin(DMA2_Channe5_IRQCallBack, IT_DMA2_CH5);  
      break;
    }
    
    default: break;
  }
}


/* �ӿں���-------------------------------------------------------------- */

/**
  * @brief  UART1��ʼ������
  * @note   Ӧ�������ù���ģʽ������IO,����������IOʱ����һ��������
  * @param  Port ѡ��Ҫ��ʼ���Ĵ���
  * @retval None
  */
void USARTx_Init(USART_TYPE Port, uint32_t iBaudRate)
{
  /* �ж���� */
  if (Port >= USARTx_COUNT)
  {
    return;
  }
  
  /* ���ù���ģʽ */
  USARTx_ModeConfig(Port, iBaudRate);
  
  /* ����DMA */
  USARTx_TxDMAConfig(Port);
  USARTx_RxDMAConfig(Port);
  
  /* �������� */
  USARTx_IOConfig(Port);
  
  /* �����жϻص����� */
  USARTx_LoginIRQFunc(Port);
  
}



/**
  * @brief  �����ֽڷ��ͺ���,��Ϊprintf�����ĵײ���������
  * @param  Port Ҫ���͵Ĵ��ڶ˺�
  * @param  cSendData Ҫ���͵�����(1���ֽ�)
  * @retval None
  */
void USARTx_SendData(USART_TYPE Port, uint8_t cSendData)
{
  //�ȴ��������(���ͻ�������)
  while (!(USART[Port]->SR & (1<<6)));
  
  //��Ҫ���͵�����д�뵽������������
  USART[Port]->DR = cSendData;
  
}
  


/**
  * @brief  �����ַ������ͺ���
  * @param  Port Ҫ���͵Ĵ��ڶ˺�
  * @param  pSendBuff ���ݷ��ͻ������ĵ�ַ
  * @retval ��
  */
void USARTx_SendStr(USART_TYPE Port, char *pSendBuff)
{
  uint16_t i = 0;
  
  while(pSendBuff[i] != '\0')
  {
    USARTx_SendData(Port, pSendBuff[i]);  //���ֽڷ��ͺ���
    i++;
  }
  
}



/**
  * @brief  ���ڶ��ֽڷ���,�������ڵ��ֽڷ��ͺ���
  * @param  Port Ҫ���͵Ĵ��ڶ˺�
  * @param  pSendBuff ���ݷ��ͻ������ĵ�ַ
  * @param  iSize Ҫ���͵���������
  * @retval ��
  */
void USARTx_SendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint32_t iSize)
{
  uint32_t i = 0;
  
  for (i = 0; i < iSize; i++)
  {
    USARTx_SendData(Port, pSendBuff[i]);
  }
  
}


#if 0

/**
  * @brief  ����DMA ���ݷ���
  * @param  pSendBuff Ҫ���͵����ݵĻ�������ַ
  * @param  nSize ���͵�����
  * @retval None
  */
void USARTx_DMASendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint16_t nSize)
{
  /* �жϲ��� */
  if (nSize == 0)
  {
    return;
  }
  
  /* �ȴ��������ַ������ */
  while (!(USART[Port]->SR & (1<<6)));
  
  /* �ر�ͨ�� */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* �����ڴ��ַ */
  USART_TxDmaChannel[Port]->CMAR = (uint32_t)pSendBuff;
  
  /* ���÷������� */
  USART_TxDmaChannel[Port]->CNDTR = nSize;
  
  /* �������� */
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}



/**
  * @brief  ����DMA ���ݷ���
  * @param  pSendBuff Ҫ���͵����ݵĻ�������ַ(ĩβ������'\0')
  * @retval None
  */
void USARTx_DMASendStr(USART_TYPE Port, uint8_t *pSendBuff)
{
  uint16_t i = 0;
  
  /* �жϲ������� */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  /* �ȴ��������ַ������ */
  while (!(USART[Port]->SR & (1<<6)));
  
  /* �ر�ͨ�� */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* �����ڴ��ַ */
  USART_TxDmaChannel[Port]->CMAR = (uint32_t)pSendBuff;
  
  /* ���÷������� */
  USART_TxDmaChannel[Port]->CNDTR = i;
  
  /* �������� */
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}

#endif


/* DMA������----------------------------------------------------------- */


typedef struct Usart_Tx_Node
{
  struct Usart_Tx_Node *pNext;  //��һ���ڵ�ĵ�ַ(��Ϊ�ڵ�β,����ΪNULL)
  unsigned short int Len; //���ݳ���
  unsigned char Buff[];   //������
}USART_TX_NODE;


USART_TX_NODE *g_HeadTxNodePtr[4] = {0};  //�׽ڵ�ָ��



//���ڵ�Ƕ������β��
static void LinkedList_SinkNode(USART_TX_NODE *pHeadNode, USART_TX_NODE *pTailNode)
{
  //ָ��ƫ�Ƶ�����β��
  while (pHeadNode->pNext != NULL)
  {
    pHeadNode = pHeadNode->pNext;
  }

  //Ƕ��β��
  pHeadNode->pNext = pTailNode;
  
}


//���͵�ǰ�����׽ڵ������
static void USARTx_SendHeadNodeBuff(USART_TYPE Port)
{
  if (g_HeadTxNodePtr[Port] == NULL)
  {
    return;
  }
  
  /* �ȴ��������ַ������ */
  while (!(USART[Port]->SR & (1<<6)));
  
  /* �ر�ͨ�� */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* �����ڴ��ַ */
  USART_TxDmaChannel[Port]->CMAR = (uint32_t)g_HeadTxNodePtr[Port]->Buff;
  
  /* ���÷������� */
  USART_TxDmaChannel[Port]->CNDTR = g_HeadTxNodePtr[Port]->Len;
  
  /* �������� */
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}


//�ͷ��׽ڵ�
static void USARTx_FreeHeadNode(USART_TYPE Port)
{
  USART_TX_NODE *pTemp = g_HeadTxNodePtr[Port];

  if (g_HeadTxNodePtr[Port] == NULL)
  {
    return;
  }

  g_HeadTxNodePtr[Port] = g_HeadTxNodePtr[Port]->pNext; //��ת����һ���ڵ�

  free(pTemp);  //�ͷŽڵ�
}




/**
  * @brief  ����DMA ���ݷ���(�������ʽ)
  * @param  pSendBuff Ҫ���͵����ݵĻ�������ַ
  * @param  nSize ���͵�����
  * @retval None
  */
void USARTx_DMASendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint16_t nSize)
{
  USART_TX_NODE *pTxNode = NULL;

  //�жϲ���
  SL_ASSERT(nSize);
  
  //����ռ�(�����䲻�ɹ�,���ظ�����,�Ա�֤�����ܷ��ͳ�ȥ)
  while ( (pTxNode = (USART_TX_NODE *)malloc(sizeof(USART_TX_NODE) + nSize*sizeof(unsigned char))) == NULL );

  //��ջ�ϵ����ݴ洢���ڵ�ռ���
  memcpy(pTxNode->Buff, pSendBuff, nSize);
  pTxNode->pNext = NULL;
  pTxNode->Len = nSize;

  //Ƕ������
  if (g_HeadTxNodePtr[Port] != NULL)  //���統ǰ����ǿ�(������)
  {
    LinkedList_SinkNode(g_HeadTxNodePtr[Port], pTxNode);  //���ڵ�Ƕ������
  }
  else //���統ǰ����Ϊ��(���ڿ���),��������
  {
    g_HeadTxNodePtr[Port] = pTxNode;  //����ǰ�ڵ�����Ϊ����ͷ
    USARTx_SendHeadNodeBuff(Port); //��������ͷ�ڵ������
  }

}



/**
  * @brief  ����DMA ���ݷ���
  * @param  pSendBuff Ҫ���͵����ݵĻ�������ַ(ĩβ������'\0')
  * @retval None
  */
void USARTx_DMASendStr(USART_TYPE Port, char *pSendBuff)
{
  uint16_t i = 0;
  
  /* �жϲ������� */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  USARTx_DMASendBuff(Port, (uint8_t *)pSendBuff, i);
  
}



void USARTx_ReadRxBuff(USART_TYPE Port, uint8_t *pReadBuff, uint16_t nSize)
{
  switch (Port)
  {
    case USARTx_1: memcpy(pReadBuff, Uart1_RecvBuff, nSize);break;
    case USARTx_2: memcpy(pReadBuff, Uart2_RecvBuff, nSize);break;
    case USARTx_3: memcpy(pReadBuff, Uart3_RecvBuff, nSize);break;
    case USARTx_4: memcpy(pReadBuff, Uart4_RecvBuff, nSize);break;
    
    default: break;
    
  }
  
}


/* �жϻص�����-------------------------------------------------------------- */


/**
  * @brief  DMA1_CH4 �ж�,ΪUSART1��TXͨ��
  * @note   ��ǰģʽΪDMA��������ж�
  * @param  None
  * @retval None
  */
void DMA1_Channe4_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_1);
  
  if (g_HeadTxNodePtr[USARTx_1] != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USARTx_1);
  }
  
}



/**
  * @brief  DMA1_CH7 �ж�,ΪUSART2��TXͨ��
  * @note   ��ǰģʽΪDMA��������ж�
  * @param  None
  * @retval None
  */
void DMA1_Channe7_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_2);
  
  if (g_HeadTxNodePtr[USARTx_2] != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USARTx_2);
  }
  
}



/**
  * @brief  DMA1_CH2 �ж�,ΪUSART3��TXͨ��
  * @note   ��ǰģʽΪDMA��������ж�
  * @param  None
  * @retval None
  */
void DMA1_Channe2_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_3);
  
  if (g_HeadTxNodePtr[USARTx_3] != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USARTx_3);
  }

}



/**
  * @brief  DMA2_CH5 �ж�,ΪUSART4��TXͨ��
  * @note   ��ǰģʽΪDMA��������ж�
  * @param  None
  * @retval None
  */
void DMA2_Channe5_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_4);
  
  if (g_HeadTxNodePtr[USARTx_4] != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USARTx_4);
  }

}


extern void USART1_IdleTask(void *pBuff, uint16_t nCount);
/**
  * @brief  �����ж�ģʽ,������һ֡���ݺ�������״̬ʱ,�Զ�������ж�
  * @note   ��ǰģʽΪUART+DMA+�����ж�
  * @param  None
  * @retval None
  */
void USART1_IDLE_IRQCallBack(void)  
{
  uint16_t nUart1RecvCnt = 0;  //�洢��ǰ���յ������ݵĻ���������
  
  /* ������յ���֡�� */
  nUart1RecvCnt = RBUF_SIZE - DMA1_Channel5->CNDTR;
  
  /* ��λDMA */
  USARTx_RxDMAReset(USARTx_1);
  
  /* ����֡���� */
  (void)nUart1RecvCnt;

}



/**
  * @brief  �����ж�ģʽ,������һ֡���ݺ�������״̬ʱ,�Զ�������ж�
  * @note   ��ǰģʽΪUART+DMA+�����ж�
  * @param  None
  * @retval None
  */
void USART2_IDLE_IRQCallBack(void)  
{
  uint16_t nUart2RecvCnt = 0;  //�洢��ǰ���յ������ݵĻ���������

  /* ������յ���֡�� */
  nUart2RecvCnt = RBUF_SIZE - DMA1_Channel6->CNDTR;
  
  /* ��λDMA */
  USARTx_RxDMAReset(USARTx_2);
  
  /* ����֡���� */
  (void)nUart2RecvCnt;

}



/**
  * @brief  �����ж�ģʽ,������һ֡���ݺ�������״̬ʱ,�Զ�������ж�
  * @note   ��ǰģʽΪUART+DMA+�����ж�
  * @param  None
  * @retval None
  */
void USART3_IDLE_IRQCallBack(void)  
{
  uint16_t nUart3RecvCnt = 0;  //�洢��ǰ���յ������ݵĻ���������

  /* ������յ���֡�� */
  nUart3RecvCnt = RBUF_SIZE - DMA1_Channel3->CNDTR;
  
  /* ��λDMA */
  USARTx_RxDMAReset(USARTx_3);
  
  /* ����֡���� */
  (void)nUart3RecvCnt;

}



/**
  * @brief  �����ж�ģʽ,������һ֡���ݺ�������״̬ʱ,�Զ�������ж�
  * @note   ��ǰģʽΪUART+DMA+�����ж�
  * @param  None
  * @retval None
  */
void USART4_IDLE_IRQCallBack(void)  
{
  uint16_t nUart4RecvCnt = 0;  //�洢��ǰ���յ������ݵĻ���������

  /* ������յ���֡�� */
  nUart4RecvCnt = RBUF_SIZE - DMA2_Channel3->CNDTR;
  
  /* ��λDMA */
  USARTx_RxDMAReset(USARTx_4);
  
  /* ����֡���� */
  (void)nUart4RecvCnt;

}




