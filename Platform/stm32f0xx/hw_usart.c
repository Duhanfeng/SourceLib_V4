/**
  ******************************************************************************
  * @file    Uart1.c
  * @author  杜公子寒枫 
  * @version V2.0 寄存器版本
  * @date    2016.05.16
  * @brief   UART1实现文件
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *      PA9  --- TX  复用推挽,10M
  *      PA10 --- RX  上拉输入
  * 
  * 波特率: 115200
  * 数据帧: 1位起始位,8位数据位,1位停止位,无校验 
  * 
  * 重定向: printf函数(底层实现于fputc())重定于串口1,若要重定向于别的模块,删除fputc()
  *        并在目标模块中实现.
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

/* 波特率配置宏 */
#define USART1_BRR_VAL     USARTx_GET_BRR(APB2_FCLK,115200)
#define USART2_BRR_VAL     USARTx_GET_BRR(APB2_FCLK,115200)

/* ---串口分频系数计算宏--- */
#define USARTx_GET_BRR(Fpclk,BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))
  
  
/*----------------------------------------------------------------------------
    数据缓冲区定义
 *----------------------------------------------------------------------------*/
 
/* ---定义接收缓冲区--- */
uint8_t Uart1_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart2_RecvBuff[RBUF_SIZE] = {0};


static USART_TypeDef * const USART[USART_COUNT] = {USART1,USART2};
static DMA_Channel_TypeDef * const USART_RxDmaChannel[USART_COUNT] = {DMA1_Channel3, DMA1_Channel5};
static DMA_Channel_TypeDef * const USART_TxDmaChannel[USART_COUNT] = {DMA1_Channel2, DMA1_Channel4};
static const IRQn_Type USARTx_IRQn[USART_COUNT] = {USART1_IRQn};
static const IRQn_Type USARTx_DMA_IRQn[USART_COUNT] = {DMA1_Channel2_3_IRQn, DMA1_Channel4_5_IRQn};
// const IRQn_Type USARTx_DMA_IRQn[USART_COUNT] = {DMA1_Channel2_3_IRQn, (IRQn_Type)11};



/* 内部函数-------------------------------------------------------------- */

//IO配置
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
  


//串口模式配置
static void USARTx_ModeConfig(USART_TYPE Port)
{
  const uint16_t BRR_VAL[4] = { USART1_BRR_VAL, USART2_BRR_VAL};
  
  /* 开时钟 */
  if (Port == USARTx_1)
  {
    RCC->APB2ENR |=  (0X1<<14);
  }
  else 
  {
    RCC->APB1ENR |=  (0X1<<(16+Port));
  }
  
  /* 配置工作模式 */
  USART[Port]->CR1 &= ~(0X1<<12);  //8位字长
  USART[Port]->CR1 &= ~(0X1<<10);  //不使用校验位
  USART[Port]->CR1 |=  (0X1<<3);   //使能发送
  USART[Port]->CR1 |=  (0X1<<2);   //使用接收
  USART[Port]->CR2 &= ~(0X3<<12);  //1位停止位
  USART[Port]->CR3 |=  (0X1<<7);   //DMA发送使能
  USART[Port]->CR3 |=  (0X1<<6);   //DMA接收使能
  
  /* 配置中断 */
  USART[Port]->CR1 &= ~(0X1<<6);   //关发送完成中断
  USART[Port]->CR1 &= ~(0X1<<5);   //关接收中断
  USART[Port]->CR1 |=  (0X1<<4);   //开空闲中断
  
  /* 清标志位 */
  USART[Port]->ICR |=  (0X1<<4);
  
  NVIC_Enable(USARTx_IRQn[Port], 2); //开内核中断
  
  /* 配置波特率 */
  USART[Port]->BRR = BRR_VAL[Port];
  
  /* 开串口 */
  USART[Port]->CR1 |=  (0x1<<0);
  
}



//UARTx_Tx DMA通道配置
static void USARTx_TxDMAConfig(USART_TYPE Port)
{
  /* 开时钟 */

  RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
  
  /* 配置工作模式 */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<14);  //非内存-内存模式
  USART_TxDmaChannel[Port]->CCR &= ~(0X3<<12);
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<12);  //中等优先级
  
  USART_TxDmaChannel[Port]->CCR &= ~(0X3<<10);
  USART_TxDmaChannel[Port]->CCR |=  (0X0<<10);  //内存:8位数据帧格式
  USART_TxDmaChannel[Port]->CCR &= ~(0X3<<8);
  USART_TxDmaChannel[Port]->CCR |=  (0X0<<8);   //外设:8位数据帧格式
  
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<7);   //内存增量模式
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<6);   //外设非增量模式
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<5);   //非循环模式
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<4);   //从内存中读
  
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<3);   //关闭错误中断
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<2);   //关闭半传输中断
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<1);   //开传输完成中断
  
  NVIC_Enable(USARTx_DMA_IRQn[Port], 2); //开内核中断
  
  /* 配置外设地址 */
  USART_TxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->TDR);
  
  /* 关闭通道 */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);   //关闭通道,在开启接受/发送时再启动
  
}
  

//UARTx_Rx DMA通道配置
static void USARTx_RxDMAConfig(USART_TYPE Port)
{
  const uint8_t * const arBuffAddr[4] = {Uart1_RecvBuff,Uart2_RecvBuff};
  
  /* 开时钟 */
  RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
  
  /* 配置工作模式 */
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<14);  //非内存-内存模式
  USART_RxDmaChannel[Port]->CCR &= ~(0X3<<12);
  USART_RxDmaChannel[Port]->CCR |=  (0X1<<12);  //中等优先级

  USART_RxDmaChannel[Port]->CCR &= ~(0X3<<10);
  USART_RxDmaChannel[Port]->CCR |=  (0X0<<10);  //内存:8位数据帧格式
  USART_RxDmaChannel[Port]->CCR &= ~(0X3<<8);
  USART_RxDmaChannel[Port]->CCR |=  (0X0<<8);   //外设:8位数据帧格式

  USART_RxDmaChannel[Port]->CCR |=  (0X1<<7);   //内存增量模式
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<6);   //外设非增量模式
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<5);   //非循环模式
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<4);   //从外设中读

  /* 配置外设中断 */
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<3);   //关闭错误中断
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<2);   //关闭半传输中断
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<1);   //关闭传输完成中断
  
  /* 配置外设地址 */
  USART_RxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->RDR);
  
  /* 配置内存地址 */
  USART_RxDmaChannel[Port]->CMAR = (uint32_t)arBuffAddr[Port];
  
  /* 配置传输数量 */
  USART_RxDmaChannel[Port]->CNDTR = RBUF_SIZE;
  
  /* 开启通道 */
  USART_RxDmaChannel[Port]->CCR |=  (0X1<<0);   //关闭通道,在开启接受/发送时再启动
  
}


//UART1_Rx DMA通道复位
void USARTx_RxDMAReset(USART_TYPE Port)
{
  /* 关闭通道 */
  USART_RxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* 配置发送数量 */
  USART_RxDmaChannel[Port]->CNDTR = RBUF_SIZE;
    
  /* 开启通道 */
  USART_RxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}



/* 接口函数-------------------------------------------------------------- */

/**
  * @brief  UART1初始化函数
  * @note   应该先配置工作模式再配置IO,否则在配置IO时产生一个负脉冲
  * @param  Port 选择要初始化的串口
  * @retval None
  */
void USARTx_Init(USART_TYPE Port)
{
  /* 判断入参 */
  if (Port >= USART_COUNT)
  {
    return;
  }
  
  /* 配置引脚 */
  USARTx_IOConfig(Port);
  
  /* 配置工作模式 */
  USARTx_ModeConfig(Port);
  
  /* 配置DMA */
  USARTx_TxDMAConfig(Port);
  USARTx_RxDMAConfig(Port);
  
}



/**
  * @brief  串口字节发送函数,作为printf函数的底层驱动函数
  * @param  Port 要发送的串口端号
  * @param  cSendData 要发送的数据(1个字节)
  * @retval None
  */
void USARTx_SendData(USART_TYPE Port, uint8_t cSendData)
{
  //等待发送完成(发送缓冲区空)
  while (!(USART[Port]->ISR & (1<<6)));
  
  //将要发送的数据写入到发生缓冲区中
  USART[Port]->TDR = cSendData;
  
}
  


/**
  * @brief  串口字符串发送函数
  * @param  Port 要发送的串口端号
  * @param  pSendBuff 数据发送缓冲区的地址
  * @retval 无
  */
void USARTx_SendStr(USART_TYPE Port, char *pSendBuff)
{
  uint16_t i = 0;
  
  while(pSendBuff[i] != '\0')
  {
    USARTx_SendData(Port, pSendBuff[i]);  //单字节发送函数
    i++;
  }
  
}



/**
  * @brief  串口多字节发送,其依赖于单字节发送函数
  * @param  Port 要发送的串口端号
  * @param  pSendBuff 数据发送缓冲区的地址
  * @param  iSize 要发送的数据数量
  * @retval 无
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
  * @brief  串口DMA 数据发送
  * @param  pSendBuff 要发送的数据的缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
void USARTx_DMASendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint16_t nSize)
{
  /* 判断参数 */
  if (nSize == 0)
  {
    return;
  }
  
  /* 等待串口上轮发送完成 */
  while (!(USART[Port]->SR & (1<<6)));
  
  /* 关闭通道 */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* 配置内存地址 */
  USART_TxDmaChannel[Port]->CMAR = (uint32_t)pSendBuff;
  
  /* 配置发送数量 */
  USART_TxDmaChannel[Port]->CNDTR = nSize;
  
  /* 启动传输 */
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}



/**
  * @brief  串口DMA 数据发送
  * @param  pSendBuff 要发送的数据的缓冲区地址(末尾必须是'\0')
  * @retval None
  */
void USARTx_DMASendStr(USART_TYPE Port, uint8_t *pSendBuff)
{
  uint16_t i = 0;
  
  /* 判断参数长度 */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  /* 等待串口上轮发送完成 */
  while (!(USART[Port]->SR & (1<<6)));
  
  /* 关闭通道 */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* 配置内存地址 */
  USART_TxDmaChannel[Port]->CMAR = (uint32_t)pSendBuff;
  
  /* 配置发送数量 */
  USART_TxDmaChannel[Port]->CNDTR = i;
  
  /* 启动传输 */
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}

#endif


/* DMA管理函数----------------------------------------------------------- */


typedef struct Usart_Tx_Node
{
  struct Usart_Tx_Node *pNext;  //下一个节点的地址(若为节点尾,则定义为NULL)
  unsigned short int Len; //数据长度
  unsigned char Buff[];   //空数组
}USART_TX_NODE;


USART_TX_NODE *g_HeadTxNodePtr[4] = {0};  //首节点指针



//将节点嵌入链表尾部
static void LinkedList_SinkNode(USART_TX_NODE *pHeadNode, USART_TX_NODE *pTailNode)
{
  //指针偏移到链表尾部
  while (pHeadNode->pNext != NULL)
  {
    pHeadNode = pHeadNode->pNext;
  }

  //嵌入尾部
  pHeadNode->pNext = pTailNode;
  
}


//发送当前链表首节点的数据
static void USARTx_SendHeadNodeBuff(USART_TYPE Port)
{
  if (g_HeadTxNodePtr[Port] == NULL)
  {
    return;
  }
  
  /* 等待串口上轮发送完成 */
  while (!(USART[Port]->ISR & (1<<6)));
  
  /* 关闭通道 */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);
  
  /* 配置内存地址 */
  USART_TxDmaChannel[Port]->CMAR = (uint32_t)g_HeadTxNodePtr[Port]->Buff;
  
  /* 配置发送数量 */
  USART_TxDmaChannel[Port]->CNDTR = g_HeadTxNodePtr[Port]->Len;
  
  /* 启动传输 */
  USART_TxDmaChannel[Port]->CCR |=  (0X1<<0);
  
}


//释放首节点
static void USARTx_FreeHeadNode(USART_TYPE Port)
{
  USART_TX_NODE *pTemp = g_HeadTxNodePtr[Port];

  if (g_HeadTxNodePtr[Port] == NULL)
  {
    return;
  }

  g_HeadTxNodePtr[Port] = g_HeadTxNodePtr[Port]->pNext; //跳转到下一个节点

  free(pTemp);  //释放节点
}




/**
  * @brief  串口DMA 数据发送(链表管理方式)
  * @param  pSendBuff 要发送的数据的缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
void USARTx_DMASendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint16_t nSize)
{
  USART_TX_NODE *pTxNode = NULL;

  //判断参数
  if (!nSize) return;
  
  //分配空间(若分配不成功,则重复申请,以保证数据能发送出去)
  while ( (pTxNode = (USART_TX_NODE *)malloc(sizeof(USART_TX_NODE) + nSize*sizeof(unsigned char))) == NULL );

  //将栈上的数据存储到节点空间中
  memcpy(pTxNode->Buff, pSendBuff, nSize);
  pTxNode->pNext = NULL;
  pTxNode->Len = nSize;

  //嵌入链表
  if (g_HeadTxNodePtr[Port] != NULL)  //假如当前链表非空(发送中)
  {
    LinkedList_SinkNode(g_HeadTxNodePtr[Port], pTxNode);  //将节点嵌入链表
  }
  else //假如当前链表为空(串口空闲),启动传输
  {
    g_HeadTxNodePtr[Port] = pTxNode;  //将当前节点设置为链表头
    USARTx_SendHeadNodeBuff(Port); //发送链表头节点的数据
  }

}



/**
  * @brief  串口DMA 数据发送
  * @param  pSendBuff 要发送的数据的缓冲区地址(末尾必须是'\0')
  * @retval None
  */
void USARTx_DMASendStr(USART_TYPE Port, char *pSendBuff)
{
  uint16_t i = 0;
  
  /* 判断参数长度 */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  USARTx_DMASendBuff(Port, (uint8_t *)pSendBuff, i);
  
}




/* 中断函数-------------------------------------------------------------- */


/**
  * @brief  DMA1_CH2 中断,为USART1的TX通道
  * @note   当前模式为DMA发送完成中断
  * @param  None
  * @retval None
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  if (DMA1->ISR & (0x1<<5))
  {
    DMA1->IFCR |= (0x1<<4);  //清全局标志位
    
    USARTx_FreeHeadNode(USARTx_1);
    
    if (g_HeadTxNodePtr[USARTx_1] != NULL)  //若当前的首节点非空
    {
      USARTx_SendHeadNodeBuff(USARTx_1);
    }
    
  }
  
}



/**
  * @brief  DMA1_CH4 中断,为USART2的TX通道
  * @note   当前模式为DMA发送完成中断
  * @param  None
  * @retval None
  */
void DMA1_Channel4_5_6_7_IRQHandler(void)
{
  if (DMA1->ISR & (0x1<<13))
  {
    DMA1->IFCR |= (0x1<<12);  //清全局标志位
    
    USARTx_FreeHeadNode(USARTx_2);
    
    if (g_HeadTxNodePtr[USARTx_2] != NULL)  //若当前的首节点非空
    {
      USARTx_SendHeadNodeBuff(USARTx_2);
    }
    
  }
  
}




#include "cmd_app.h"

extern CMD_PARAM Uart1Param;


/**
  * @brief  空闲中断模式,当发送一帧数据后进入空闲状态时,自动进入此中断
  * @note   当前模式为UART+DMA+空闲中断
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  uint16_t nUart1RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度
  
  /* 中断入口判断 */
  if (USART1->ISR & (0X1<<4))  //空闲帧中断入口
  {
    /* 清标志位 */
    USART1->ICR |= (0X1<<4);
    
    /* 计算接收到的帧长 */
    nUart1RecvCnt = RBUF_SIZE - DMA1_Channel3->CNDTR;
    
    /* 复位DMA */
    USARTx_RxDMAReset(USARTx_1);
    
    /* 数据帧处理 */
    (void)nUart1RecvCnt;
    AT_StoreCommand(&Uart1Param, Uart1_RecvBuff, nUart1RecvCnt);
  }
  
}


