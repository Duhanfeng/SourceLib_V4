/**
  ******************************************************************************
  * @file    Usart.h
  * @author  �Ź��Ӻ���
  * @version V4.1 �Ĵ����汾
  * @date    2017.05.23
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
  * ����ԭ��: ������IO��Ϊ����ģʽ��,���ڴ�ʱUARTx��ʱ����δ����,���Ե�ƽ������.
  *           ������,����UARTx��ʱ�Ӳ���ʹ�ܷ��͹��ܺ�,IOͨ��,�ָ��ߵ�ƽ,�ʶ�����Ϊ
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
  * V4.1------------
  * �޸�����: 1.�޸Ĵ������û���,�����ɼĴ���ָ������������ĳ�ֱ�ӵĴ���ָ�����
  *           2.���þ���һ���װ��DMA����,����ֱ�Ӷ�DMA�Ĵ������в���
  *           3.���ټ��ɴ��ڿ���֡�жϵĻص�,���ǽ����ⲿȥ����,���̶��ϱ���
  *             �Դ��ļ����޸�
  *           4.�����������д,β�ڵ��next����ָ��NULL,����ָ������,�����ڴ�
  *             ��ض�NULL���в���ʱ����HardFault
  *           5.�ں����ദ�������,���ڷ���Ǳ�ڵ�BUG
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.1
  * �޸�����: 2017.05.23
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_usart.h"
#include "SourceLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw_dma.h"

/* ---���ڷ�Ƶϵ�������--- */
#define USARTx_GET_BRR(Fpclk, BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))
  
/* ------����ͨ������------ */
#define USART1_TX_PORT    GPIOx_PA
#define USART1_TX_PIN     GPIOx_9
#define USART1_RX_PORT    GPIOx_PA
#define USART1_RX_PIN     GPIOx_10

#define USART2_TX_PORT    GPIOx_PA
#define USART2_TX_PIN     GPIOx_2
#define USART2_RX_PORT    GPIOx_PA
#define USART2_RX_PIN     GPIOx_3

#define USART3_TX_PORT    GPIOx_PB
#define USART3_TX_PIN     GPIOx_10
#define USART3_RX_PORT    GPIOx_PB
#define USART3_RX_PIN     GPIOx_11

#define USART4_TX_PORT    GPIOx_PC
#define USART4_TX_PIN     GPIOx_10
#define USART4_RX_PORT    GPIOx_PC
#define USART4_RX_PIN     GPIOx_11


static void USART1_DMA_TC_CallBack(void);
static void USART2_DMA_TC_CallBack(void);
static void USART3_DMA_TC_CallBack(void);
static void USART4_DMA_TC_CallBack(void);


/*----------------------------------------------------------------------------
    ���ݻ���������
 *----------------------------------------------------------------------------*/

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



/* USART�����Դ���------------------------------------------------------- */

//����IO����
static void USARTx_IOConfig(USART_TypeDef *USARTx)
{
  switch ((uint32_t)USARTx)
  {
    case USART1_BASE: 
      GPIOx_FastInit(USART1_TX_PORT, USART1_TX_PIN, GPIOx_AF_PP_50M);
      GPIOx_FastInit(USART1_RX_PORT, USART1_RX_PIN, GPIOx_PUSH_UP);
      break;
    case USART2_BASE: 
      GPIOx_FastInit(USART2_TX_PORT, USART2_TX_PIN, GPIOx_AF_PP_50M);
      GPIOx_FastInit(USART2_RX_PORT, USART2_RX_PIN, GPIOx_PUSH_UP);
      break;
    case USART3_BASE: 
      GPIOx_FastInit(USART3_TX_PORT, USART3_TX_PIN, GPIOx_AF_PP_50M);
      GPIOx_FastInit(USART3_RX_PORT, USART3_RX_PIN, GPIOx_PUSH_UP);
      break;
    case UART4_BASE: 
      GPIOx_FastInit(USART4_TX_PORT, USART4_TX_PIN, GPIOx_AF_PP_50M);
      GPIOx_FastInit(USART4_RX_PORT, USART4_RX_PIN, GPIOx_PUSH_UP);
      break;
    default: break;
  }
  
}


//��ȡ���ڶ�Ӧ��Rx_DMAͨ��ָ��
static DMA_Channel_TypeDef * USARTx_GetRxDmaChannel(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef * RxDmaChannel = NULL;
  
  switch ((uint32_t)USARTx)
  {
    case USART1_BASE: RxDmaChannel = DMA1_Channel5; break;
    case USART2_BASE: RxDmaChannel = DMA1_Channel6; break;
    case USART3_BASE: RxDmaChannel = DMA1_Channel3; break;
    case UART4_BASE : RxDmaChannel = DMA2_Channel3; break;
    default: break;
  }
  
  return RxDmaChannel;
}


//��ȡ���ڶ�Ӧ��Tx_DMAͨ��ָ��
static DMA_Channel_TypeDef * USARTx_GetTxDmaChannel(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef * TxDmaChannel = NULL;
  
  switch ((uint32_t)USARTx)
  {
    case USART1_BASE: TxDmaChannel = DMA1_Channel4; break;
    case USART2_BASE: TxDmaChannel = DMA1_Channel7; break;
    case USART3_BASE: TxDmaChannel = DMA1_Channel2; break;
    case UART4_BASE : TxDmaChannel = DMA2_Channel5; break;
    default: break;
  }
  
  return TxDmaChannel;
}


//��ȡ���ջ�����ָ��
static void *USARTx_GetBuffPtr(USART_TypeDef *USARTx)
{
  uint8_t *pBuff = NULL;
  
  switch ((uint32_t)USARTx)
  {
    case USART1_BASE: pBuff = Uart1_RecvBuff; break;
    case USART2_BASE: pBuff = Uart2_RecvBuff; break;
    case USART3_BASE: pBuff = Uart3_RecvBuff; break;
    case UART4_BASE : pBuff = Uart4_RecvBuff; break;
    default: break;
  }
  
  return pBuff;
}


//ע��DMA��������жϵĻص�����
static void USARTx_LoginIRQFunc(USART_TypeDef *USARTx)
{
  switch ((uint32_t)USARTx)
  {
    case USART1_BASE: IT_IRQ_FuncLogin(USART1_DMA_TC_CallBack, IT_DMA1_CH4); break;
    case USART2_BASE: IT_IRQ_FuncLogin(USART2_DMA_TC_CallBack, IT_DMA1_CH7); break;
    case USART3_BASE: IT_IRQ_FuncLogin(USART3_DMA_TC_CallBack, IT_DMA1_CH2); break;
    case UART4_BASE:  IT_IRQ_FuncLogin(USART4_DMA_TC_CallBack, IT_DMA2_CH5); break;
    default: break;
  }
}


/* ģʽ����-------------------------------------------------------------- */

//����ģʽ����
static void USARTx_ModeConfig(USART_TypeDef *USARTx, uint32_t iBaudRate)
{
  //����ʱ��
  RCC_EnableClock(USARTx, 1);
  
  //���ù���ģʽ
  USARTx->CR1 &= ~USART_CR1_M;      //8λ�ֳ�
  USARTx->CR1 &= ~USART_CR1_PCE;    //��ʹ��У��λ
  USARTx->CR1 |=  USART_CR1_TE;     //ʹ�ܷ���
  USARTx->CR1 |=  USART_CR1_RE;     //ʹ�ý���
  USARTx->CR2 &= ~USART_CR2_STOP;   //1λֹͣλ
  USARTx->CR3 |=  USART_CR3_DMAT;   //DMA����ʹ��
  USARTx->CR3 |=  USART_CR3_DMAR;   //DMA����ʹ��
  
  //�����ж�
  USARTx->CR1 &= ~USART_CR1_TCIE;   //�ط�������ж�
  USARTx->CR1 &= ~USART_CR1_RXNEIE; //�ؽ����ж�
  USARTx->CR1 |=  USART_CR1_IDLEIE; //�������ж�
  
  NVIC_Config(USARTx, 2, 2);        //���ں��ж�
  
  //���ò�����
  USARTx->BRR = USARTx_GET_BRR((USARTx == USART1)? APB2_FCLK : APB1_FCLK, iBaudRate);
  
  //������
  USARTx->CR1 |=  USART_CR1_UE;
  
}


//UARTx_Tx DMAͨ������
static void USARTx_TxDMAConfig(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *TxDmaChannel = NULL;
  
  //��ȡDMAͨ��ָ��
  TxDmaChannel = USARTx_GetTxDmaChannel(USARTx);
  
  //����DMA����ģʽ
  DMAx_PeriTransferConfig(TxDmaChannel, (void *)&USARTx->DR, NULL, DMA_DIR_MEM_TO_PERI);
  DMAx_ITConfig(TxDmaChannel, DMA_IT_TCIE);  //��������ж�
  NVIC_Config(TxDmaChannel, 2, 2);  //�����ں��ж�
  
}


//UARTx_Rx DMAͨ������
static void USARTx_RxDMAConfig(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *RxDmaChannel = NULL;
  
  //��ȡDMAͨ��ָ��
  RxDmaChannel = USARTx_GetRxDmaChannel(USARTx);
  
  //����DMA����ģʽ
  DMAx_PeriTransferConfig(RxDmaChannel, (void *)&USARTx->DR, USARTx_GetBuffPtr(USARTx), DMA_DIR_PERI_TO_MEM);
  DMAx_SetTransferCount(RxDmaChannel, RBUF_SIZE); //���ô�������
  DMAx_Enable(RxDmaChannel, 1); //����DMA����
  
}


/* �ӿں���-------------------------------------------------------------- */

/**
  * @brief  UART1��ʼ������
  * @param  USARTx ���ڱ��
  * @param  iBaudRate Ҫ���õĲ�����
  * @retval None
  * @note   Ӧ�������ù���ģʽ������IO,����������IOʱ����һ��������
  */
void USARTx_Init(USART_TypeDef *USARTx, uint32_t iBaudRate)
{
  //���ù���ģʽ
  USARTx_ModeConfig(USARTx, iBaudRate);
  
  //����DMA
  USARTx_TxDMAConfig(USARTx);
  USARTx_RxDMAConfig(USARTx);
  
  //��������
  USARTx_IOConfig(USARTx);
  
  //ע���жϻص�����
  USARTx_LoginIRQFunc(USARTx);
  
}



/**
  * @brief  �����ֽڷ��ͺ���,��Ϊprintf�����ĵײ���������
  * @param  USARTx ���ڱ��
  * @param  cSendData Ҫ���͵�����(1���ֽ�)
  * @retval None
  */
void USARTx_SendData(USART_TypeDef *USARTx, uint8_t cSendData)
{
  //�ȴ��������(���ͻ�������)
  while (!(USARTx->SR & USART_SR_TC));
  
  //��Ҫ���͵�����д�뵽������������
  USARTx->DR = cSendData;
  
}



/**
  * @brief  ���ڶ��ֽڷ���,�������ڵ��ֽڷ��ͺ���
  * @param  USARTx ���ڱ��
  * @param  pSendBuff ���ݷ��ͻ������ĵ�ַ
  * @param  iSize Ҫ���͵���������
  * @retval ��
  */
void USARTx_SendBuff(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint32_t iSize)
{
  uint32_t i = 0;
  
  for (i = 0; i < iSize; i++)
  {
    USARTx_SendData(USARTx, pSendBuff[i]);
  }
  
}



/**
  * @brief  �����ַ������ͺ���
  * @param  USARTx ���ڱ��
  * @param  pSendBuff ���ݷ��ͻ������ĵ�ַ
  * @retval ��
  */
void USARTx_SendStr(USART_TypeDef *USARTx, char *pSendBuff)
{
  uint16_t i = 0;
  
  /* �жϲ������� */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  USARTx_SendBuff(USARTx, (uint8_t *)pSendBuff, i);
  
}



/**
  * @brief  Rx DMAͨ����λ
  * @param  USARTx ���ڱ��
  * @param  nSize ���͵�����
  * @retval None
  */
void USARTx_RxDMAReset(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *RxDmaChannel = NULL;
  
  //��ȡDMAͨ��ָ��
  RxDmaChannel = USARTx_GetRxDmaChannel(USARTx);
  
  //��λͨ��
  DMAx_ResetCounter(RxDmaChannel, RBUF_SIZE);
  
}



/**
  * @brief  ��ȡ�Ѿ�ͨ��DMA���յ����ݵ�����
  * @param  USARTx ���ڱ��
  * @param  nSize ���͵�����
  * @retval None
  */
uint16_t USARTx_GetRxDMACount(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *RxDmaChannel = NULL;
  
  //��ȡDMAͨ��ָ��
  RxDmaChannel = USARTx_GetRxDmaChannel(USARTx);
  
  return RBUF_SIZE - DMAx_GetCurrentCount(RxDmaChannel);
}



/* ���������----------------------------------------------------------- */

typedef struct Usart_Tx_Node
{
  struct Usart_Tx_Node *pNext;  //��һ���ڵ�ĵ�ַ(��Ϊ�ڵ�β,����ΪNULL)
  unsigned short int Len; //���ݳ���
  unsigned char Buff[];   //������
}USART_TX_NODE;

static USART_TX_NODE *g_HeadNodePtr1 = NULL;
static USART_TX_NODE *g_HeadNodePtr2 = NULL;
static USART_TX_NODE *g_HeadNodePtr3 = NULL;
static USART_TX_NODE *g_HeadNodePtr4 = NULL;


//���ڵ�ָ��ָ������
static void List_Init(USART_TX_NODE *pNode)
{
  pNode->pNext = pNode;
  
}


//���ڵ�Ƕ������β��
static void List_SinkTail(USART_TX_NODE *pHeadNode, USART_TX_NODE *pTailNode)
{
  SL_ASSERT(pHeadNode);
  SL_ASSERT(pTailNode);
  
  //ָ��ƫ�Ƶ�����β��
  while (pHeadNode->pNext != pHeadNode)
  {
    pHeadNode = pHeadNode->pNext;
  }

  //Ƕ��β��
  pHeadNode->pNext = pTailNode;
  
}


//�ͷŽڵ�
static USART_TX_NODE *List_FreeHead(USART_TX_NODE *pHeadNode)
{
  USART_TX_NODE *tmpNode = NULL;
  
  SL_ASSERT(pHeadNode);
  
  //����������ֻ�е����ڵ�,�򷵻����µ��׽ڵ�ĵ�ַ
  if (pHeadNode->pNext != pHeadNode)
  {
    tmpNode = pHeadNode->pNext;
    
  }
  free(pHeadNode);
  
  return tmpNode;
}


//��ȡ�׽ڵ��ָ��
static USART_TX_NODE **List_GetHead(USART_TypeDef *USARTx)
{
  USART_TX_NODE **pHeadNode = NULL;
  
  switch ((uint32_t)USARTx)
  {
    case USART1_BASE: pHeadNode = &g_HeadNodePtr1; break;
    case USART2_BASE: pHeadNode = &g_HeadNodePtr2; break;
    case USART3_BASE: pHeadNode = &g_HeadNodePtr3; break;
    case UART4_BASE : pHeadNode = &g_HeadNodePtr4; break;
    default: break;
  }
  
  return pHeadNode;
}


//ͨ��DMA��������(������)
static void USARTx_DMASend(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint16_t nSize)
{
  DMA_Channel_TypeDef *TxDmaChannel = NULL;
  
  //��ȡDMAͨ��ָ��
  TxDmaChannel = USARTx_GetTxDmaChannel(USARTx);
  
  //��λͨ��
  DMAx_ResetMemAddrAndCounter(TxDmaChannel, pSendBuff, nSize);
  
}


//���͵�ǰ�����׽ڵ������
static void USARTx_SendHeadNodeBuff(USART_TypeDef *USARTx)
{
  //��ȡ�ڵ�ָ��
  USART_TX_NODE **pHeadNode = List_GetHead(USARTx);
  SL_ASSERT(pHeadNode);
  SL_ASSERT(*pHeadNode);
  
  //�ȴ��������ַ������
  while (!(USARTx->SR & USART_SR_TC));
  
  //���ͽڵ��е�����
  USARTx_DMASend(USARTx, (*pHeadNode)->Buff, (*pHeadNode)->Len);
  
}


/**
  * @brief  ����DMA ���ݷ���(�������ʽ)
  * @param  USARTx ���ڱ��
  * @param  pSendBuff Ҫ���͵����ݵĻ�������ַ
  * @param  nSize ���͵�����
  * @retval None
  */
void USARTx_DMASendBuff(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint16_t nSize)
{
  USART_TX_NODE *pTxNode = NULL;
  USART_TX_NODE **pHeadNode = List_GetHead(USARTx);

  //�жϲ���
  SL_ASSERT(nSize);
  
  //����ռ�(�����䲻�ɹ�,���ظ�����,�Ա�֤�����ܷ��ͳ�ȥ)
  while ( (pTxNode = (USART_TX_NODE *)malloc(sizeof(USART_TX_NODE) + nSize*sizeof(unsigned char))) == NULL );

  //��ʼ������
  List_Init(pTxNode);
  
  //��ջ�ϵ����ݴ洢���ڵ�ռ���
  memcpy(pTxNode->Buff, pSendBuff, nSize);
  pTxNode->Len = nSize;
  
  //Ƕ������
  if (*pHeadNode != NULL)  //���統ǰ����ǿ�(������)
  {
    //���ڵ�Ƕ������
    List_SinkTail(*pHeadNode, pTxNode);
  }
  else //���統ǰ����Ϊ��(���ڿ���),��������
  {
    *pHeadNode = pTxNode; //����ǰ�ڵ�����Ϊ����ͷ
    USARTx_SendHeadNodeBuff(USARTx);  //��������ͷ�ڵ������
  }

}



/**
  * @brief  ����DMA ���ݷ���
  * @param  USARTx ���ڱ��
  * @param  pSendBuff Ҫ���͵����ݵĻ�������ַ(ĩβ������'\0')
  * @retval None
  */
void USARTx_DMASendStr(USART_TypeDef *USARTx, char *pSendBuff)
{
  uint16_t i = 0;
  
  /* �жϲ������� */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  USARTx_DMASendBuff(USARTx, (uint8_t *)pSendBuff, i);
  
}

/* �жϻص�����-------------------------------------------------------------- */

/**
  * @brief  DMA1_CH4 �ж�,ΪUSART1��TXͨ��
  * @param  None
  * @retval None
  * @note   ��ǰģʽΪDMA��������ж�
  */
static void USART1_DMA_TC_CallBack(void)
{
  g_HeadNodePtr1 = List_FreeHead(g_HeadNodePtr1);
  
  if (g_HeadNodePtr1 != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USART1);
  }
  
}



/**
  * @brief  DMA1_CH7 �ж�,ΪUSART2��TXͨ��
  * @param  None
  * @retval None
  * @note   ��ǰģʽΪDMA��������ж�
  */
static void USART2_DMA_TC_CallBack(void)
{
  g_HeadNodePtr2 = List_FreeHead(g_HeadNodePtr2);
  
  if (g_HeadNodePtr2 != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USART2);
  }
  
}



/**
  * @brief  DMA1_CH2 �ж�,ΪUSART3��TXͨ��
  * @param  None
  * @retval None
  * @note   ��ǰģʽΪDMA��������ж�
  */
static void USART3_DMA_TC_CallBack(void)
{
  g_HeadNodePtr3 = List_FreeHead(g_HeadNodePtr3);
  
  if (g_HeadNodePtr3 != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(USART3);
  }

}



/**
  * @brief  DMA2_CH5 �ж�,ΪUSART4��TXͨ��
  * @param  None
  * @retval None
  * @note   ��ǰģʽΪDMA��������ж�
  */
static void USART4_DMA_TC_CallBack(void)
{
  g_HeadNodePtr4 = List_FreeHead(g_HeadNodePtr4);
  
  if (g_HeadNodePtr4 != NULL)  //����ǰ���׽ڵ�ǿ�
  {
    USARTx_SendHeadNodeBuff(UART4);
  }

}




