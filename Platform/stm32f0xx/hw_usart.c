/**
  ******************************************************************************
  * @file    Uart1.c
  * @author  �Ź��Ӻ��� 
  * @version V2.0 �Ĵ����汾
  * @date    2016.05.16
  * @brief   UART1ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * ����,
  *      PA9  --- TX  ��������,10M
  *      PA10 --- RX  ��������
  * 
  * ������: 115200
  * ����֡: 1λ��ʼλ,8λ����λ,1λֹͣλ,��У�� 
  * 
  * �ض���: printf����(�ײ�ʵ����fputc())�ض��ڴ���1,��Ҫ�ض����ڱ��ģ��,ɾ��fputc()
  *        ����Ŀ��ģ����ʵ��.
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "stm32f0xx.h"
#include "hw_usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SourceLib.h"

/* ���������ú� */
#define USART1_BRR_VAL     USARTx_GET_BRR(APB2_FCLK,115200)
#define USART2_BRR_VAL     USARTx_GET_BRR(APB2_FCLK,115200)

/* ---���ڷ�Ƶϵ�������--- */
#define USARTx_GET_BRR(Fpclk,BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))
  
  
/*----------------------------------------------------------------------------
    ���ݻ���������
 *----------------------------------------------------------------------------*/
 
/* ---������ջ�����--- */
uint8_t Uart1_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart2_RecvBuff[RBUF_SIZE] = {0};


static USART_TypeDef * const USART[USART_COUNT] = {USART1,USART2};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[USART_COUNT] = {DMA1_Channel3, DMA1_Channel5};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[USART_COUNT] = {DMA1_Channel2, DMA1_Channel4};
static const IRQn_Type USARTx_IRQn[USART_COUNT] = {USART1_IRQn};
static const IRQn_Type USARTx_DMA_IRQn[USART_COUNT] = {DMA1_Channel2_3_IRQn, DMA1_Channel4_5_IRQn};
// const IRQn_Type USARTx_DMA_IRQn[USART_COUNT] = {DMA1_Channel2_3_IRQn, (IRQn_Type)11};



/* �ڲ�����-------------------------------------------------------------- */

//IO����
static void USARTx_IOConfig(USART_TYPE Port)
{
  switch (Port)
  {
    case USARTx_1: 
    {
      // TX: PA9
      GPIOx_FastInit(GPIOx_PA, GPIOx_9, GPIOx_AF_PP_H);
      GPIOA->AFR[1]  &= ~(0XFU<<((9-8)*4));
      GPIOA->AFR[1]  |=  (0X1U<<((9-8)*4)); //USART_TX
      
      // RX: PA10
      GPIOx_FastInit(GPIOx_PA, GPIOx_10, GPIOx_AF_PP_H);
      GPIOA->AFR[1]  &= ~(0XFU<<((10-8)*4));
      GPIOA->AFR[1]  |=  (0X1U<<((10-8)*4)); //USART_RX
      
      break;
    }
    case USARTx_2: 
    {
      // TX: PA2
//      GPIOx_FastInit(GPIOx_PA, GPIOx_2, GPIOx_AF_PP_H);
//      GPIOA->AFR[0]  &= ~(0XFU<<(2*4));
//      GPIOA->AFR[0]  |=  (0X1U<<(2*4)); //USART_TX
//      
//      // RX: PA3
//      GPIOx_FastInit(GPIOx_PA, GPIOx_3, GPIOx_AF_PP_H);
//      GPIOA->AFR[0]  &= ~(0XFU<<(3*4));
//      GPIOA->AFR[0]  |=  (0X1U<<(3*4)); //USART_RX
      
      break;
    }
    
    default: break;
  }
  
}
  


//����ģʽ����
static void USARTx_ModeConfig(USART_TYPE Port)
{
  const uint16_t BRR_VAL[4] = { USART1_BRR_VAL, USART2_BRR_VAL};
  
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
  
  /* ���־λ */
  USART[Port]->ICR |=  (0X1<<4);
  
  NVIC_Enable(USARTx_IRQn[Port], 2); //���ں��ж�
  
  /* ���ò����� */
  USART[Port]->BRR = BRR_VAL[Port];
  
  /* ������ */
  USART[Port]->CR1 |=  (0x1<<0);
  
}



//UARTx_Tx DMAͨ������
static void USARTx_TxDMAConfig(USART_TYPE Port)
{
  /* ��ʱ�� */

  RCC->AHBENR |= (0X1<<0);  //DMA1ʱ��ʹ�� 
  
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
  
  NVIC_Enable(USARTx_DMA_IRQn[Port], 2); //���ں��ж�
  
  /* ���������ַ */
  USART_TxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->TDR);
  
  /* �ر�ͨ�� */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);   //�ر�ͨ��,�ڿ�������/����ʱ������
  
}
  

//UARTx_Rx DMAͨ������
static void USARTx_RxDMAConfig(USART_TYPE Port)
{
  const uint8_t * const arBuffAddr[4] = {Uart1_RecvBuff,Uart2_RecvBuff};
  
  /* ��ʱ�� */
  RCC->AHBENR |= (0X1<<0);  //DMA1ʱ��ʹ�� 
  
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
  USART_RxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->RDR);
  
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



/* �ӿں���-------------------------------------------------------------- */

/**
  * @brief  UART1��ʼ������
  * @note   Ӧ�������ù���ģʽ������IO,����������IOʱ����һ��������
  * @param  Port ѡ��Ҫ��ʼ���Ĵ���
  * @retval None
  */
void USARTx_Init(USART_TYPE Port)
{
  /* �ж���� */
  if (Port >= USART_COUNT)
  {
    return;
  }
  
  /* �������� */
  USARTx_IOConfig(Port);
  
  /* ���ù���ģʽ */
  USARTx_ModeConfig(Port);
  
  /* ����DMA */
  USARTx_TxDMAConfig(Port);
  USARTx_RxDMAConfig(Port);
  
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
  while (!(USART[Port]->ISR & (1<<6)));
  
  //��Ҫ���͵�����д�뵽������������
  USART[Port]->TDR = cSendData;
  
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
  while (!(USART[Port]->ISR & (1<<6)));
  
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
  if (!nSize) return;
  
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




/* �жϺ���-------------------------------------------------------------- */


/**
  * @brief  DMA1_CH2 �ж�,ΪUSART1��TXͨ��
  * @note   ��ǰģʽΪDMA��������ж�
  * @param  None
  * @retval None
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  if (DMA1->ISR & (0x1<<5))
  {
    DMA1->IFCR |= (0x1<<4);  //��ȫ�ֱ�־λ
    
    USARTx_FreeHeadNode(USARTx_1);
    
    if (g_HeadTxNodePtr[USARTx_1] != NULL)  //����ǰ���׽ڵ�ǿ�
    {
      USARTx_SendHeadNodeBuff(USARTx_1);
    }
    
  }
  
}



/**
  * @brief  DMA1_CH4 �ж�,ΪUSART2��TXͨ��
  * @note   ��ǰģʽΪDMA��������ж�
  * @param  None
  * @retval None
  */
void DMA1_Channel4_5_6_7_IRQHandler(void)
{
  if (DMA1->ISR & (0x1<<13))
  {
    DMA1->IFCR |= (0x1<<12);  //��ȫ�ֱ�־λ
    
    USARTx_FreeHeadNode(USARTx_2);
    
    if (g_HeadTxNodePtr[USARTx_2] != NULL)  //����ǰ���׽ڵ�ǿ�
    {
      USARTx_SendHeadNodeBuff(USARTx_2);
    }
    
  }
  
}




#include "cmd_app.h"

extern CMD_PARAM Uart1Param;


/**
  * @brief  �����ж�ģʽ,������һ֡���ݺ�������״̬ʱ,�Զ�������ж�
  * @note   ��ǰģʽΪUART+DMA+�����ж�
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  uint16_t nUart1RecvCnt = 0;  //�洢��ǰ���յ������ݵĻ���������
  
  /* �ж�����ж� */
  if (USART1->ISR & (0X1<<4))  //����֡�ж����
  {
    /* ���־λ */
    USART1->ICR |= (0X1<<4);
    
    /* ������յ���֡�� */
    nUart1RecvCnt = RBUF_SIZE - DMA1_Channel3->CNDTR;
    
    /* ��λDMA */
    USARTx_RxDMAReset(USARTx_1);
    
    /* ����֡���� */
    (void)nUart1RecvCnt;
    AT_StoreCommand(&Uart1Param, Uart1_RecvBuff, nUart1RecvCnt);
  }
  
}


