/**
  ******************************************************************************
  * @file    Usart.h
  * @author  杜公子寒枫
  * @version V4.1 寄存器版本
  * @date    2017.05.23
  * @brief   USART1~4 驱动实现文件
  ******************************************************************************
  * @attention
  * 
  * USART1: TX --- PA9  复用推挽,50M
  *         RX --- PA10 上拉输入
  *
  * USART2: TX --- PA2  复用推挽,50M
  *         RX --- PA3  上拉输入
  *
  * USART3: TX --- PB10 复用推挽,50M
  *         RX --- PB11 上拉输入
  *
  * USART4: TX --- PC10 复用推挽,50M
  *         RX --- PC11 上拉输入
  *
  * V1.1------------
  * 修改描述: 修复波特率配置不准确的错误
  * 错误原因: 波特率分频因子计算公式(宏)中,默认定义为整型,导致小数部分丢失
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2015.12.13
  * 
  * V1.2------------
  * 修改描述: 增加单独的接收中断开启与关闭接口函数
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.2
  * 修改日期: 2015.12.21
  * 
  * V1.3------------
  * 修改描述: 修改文件架构,加入UART+DMA功能,修改H文件描述,增加文件注释头
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.3
  * 修改日期: 2016.02.18
  * 
  * V1.4------------
  * 修改描述: 1.修复在初始化阶段Tx脚产生一个负脉冲的错误(此负脉冲会被识别为一个字符)
  *           2.将串口接收修改为空闲中断模式
  * 错误原因: 在配置IO口为复用模式后,由于此时UARTx的时钟尚未开启,所以电平被拉低.
  *           紧接着,开启UARTx的时钟并且使能发送功能后,IO通电,恢复高电平,故而整体为
  *           一个负脉冲,并可被识别为一个字符(0X80,0XFF,0XFE等).
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.4
  * 修改日期: 2016.02.20
  *
  * V1.5------------
  * 修改描述: 1.增加了地址标志模式,方便用于多处理器模式下的硬件识别(一般是从机用)
  *           2.删除部分不必要的函数,精简架构
  * 地址标记: 可为本机配置一个本机地址(4位),并将接收器进入静默状态.此时,接收器会将最
  *           高位为'1'的数据识别为"地址",最高位为'0'的识别为数据.当接收器接收到一个
  *           "地址"时,会对此"地址"进行判断:低4位是否与"本机地址匹配",若匹配,则接收接
  *           下来的"数据",直到下一次"地址"的到来,若匹配,则继续接收,若不匹配,则
  *           重新进入静默状态.
  *           地址标记模式的使用在多处理器通信且大数据吞吐中,能利用硬件的筛选减少CPU
  *           的运行. 详见RM0008 P805  Multiprocessor communication
  * 已知缺陷: 1.地址标记模式中,当接收数据完成后,空闲帧的到来并不会触发"空闲帧"中断,这
  *             样会使UART的可用性降低
  *           2.地址标记模式中,由于是对最高位进行对数据的分类(地址/数据),导致最高位的
  *             使用受到了限制.
  * 避免方法: 开启"接收缓冲区非空中断","地址"字符匹配之后会触发此中断,在此中断内暂时关
  *           闭接收器静默状态,在触发空闲帧中断后再开启静默状态.
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.5
  * 修改日期: 2016.03.15
  *
  * V2.0------------
  * 修改描述: 1.移除地址标志模式
  *           2.发现一个关于DMA发送的BUG
  * 地址标志: 地址标志模式适用于同一条串口总线中一主多从的情况,使用上较为繁琐,不
  *           适用于常规的使用场合,而且地址标志模式会加大驱动的复杂程度,已知的缺
  *           陷较难避免,目前没有找到比较简单方便的使用方法,故而移除地址标志模式.
  * DMA 发送: 利用DMA发送一组数据,当触发"DMA传输完成(TCIF)"条件后关闭串口发送功能,
  *           使用示波器发现串口只发送了一部分的数据.根据 STM32参考手册(Rev 16)
  *           818页 Figure 296 可知,DMA搬运完成即触发"TCIF"标志,而此时串口尚未发
  *           送完成,关闭串口会对尚在发送的数据造成截断.若要解决此BUG,可以把DMA发
  *           送的完成标志判断由"DMA_TCIF"改成"USART_TC".
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.05.16
  *
  * V3.0------------
  * 修改描述: 1.将串口1~4整合到同一个文件中,同一个函数可以被各个串口进行调用,通
  *             过结构体指针数组的方式映射到每一个寄存器组之中,下标访问的实现机
  *             制避免了冗余的条件判断.
  *           2.兼容STM32F10x全系列的芯片,加入安全机制,避免因不同芯片的差异性或
  *             错误调用是对非法地址的访问.
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2016.08.09
  * 
  * V3.1------------
  * 修改描述: 修改DMA发送机制
  * 修改原因: 假如外部函数调用此DMA发送函数,入参的缓冲区是栈上的局部变量,那么在
  *           外部函数结束后栈上的空间被回收(此时USART还在发送中).如果此时在别的
  *           函数中修改了栈上的数据,串口的发送便无效.故而修改其发送机制,在发送
  *           函数中,将栈上临时存储的数据复制到堆上的空间中,用链表的方式链接多个
  *           发送缓冲区,当DMA发送完成后(DMA发送完成中断),释放其申请的空间.只要
  *           堆上可用的空间足够,发送几十K的数据,CPU的占用也不超过2MS
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.1
  * 修改日期: 2016.11.18
  * 
  * V4.1------------
  * 修改描述: 1.修改串口配置机制,将其由寄存器指针数组的索引改成直接的串口指针入参
  *           2.调用经过一层封装的DMA函数,而非直接对DMA寄存器进行操作
  *           3.不再集成串口空闲帧中断的回调,而是交由外部去调用,最大程度上避免
  *             对此文件的修改
  *           4.链表管理函数重写,尾节点的next不再指向NULL,而是指向自身,避免在错
  *             误地对NULL进行操作时产生HardFault
  *           5.在函数多处加入断言,便于发现潜在的BUG
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
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

/* ---串口分频系数计算宏--- */
#define USARTx_GET_BRR(Fpclk, BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))
  
/* ------串口通道引脚------ */
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
    数据缓冲区定义
 *----------------------------------------------------------------------------*/

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

/* ---定义接收缓冲区--- */
uint8_t Uart1_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart2_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart3_RecvBuff[RBUF_SIZE] = {0};
uint8_t Uart4_RecvBuff[RBUF_SIZE] = {0};



/* USART兼容性处理------------------------------------------------------- */

//串口IO配置
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


//获取串口对应的Rx_DMA通道指针
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


//获取串口对应的Tx_DMA通道指针
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


//获取接收缓冲区指针
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


//注册DMA发送完成中断的回调函数
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


/* 模式配置-------------------------------------------------------------- */

//串口模式配置
static void USARTx_ModeConfig(USART_TypeDef *USARTx, uint32_t iBaudRate)
{
  //配置时钟
  RCC_EnableClock(USARTx, 1);
  
  //配置工作模式
  USARTx->CR1 &= ~USART_CR1_M;      //8位字长
  USARTx->CR1 &= ~USART_CR1_PCE;    //不使用校验位
  USARTx->CR1 |=  USART_CR1_TE;     //使能发送
  USARTx->CR1 |=  USART_CR1_RE;     //使用接收
  USARTx->CR2 &= ~USART_CR2_STOP;   //1位停止位
  USARTx->CR3 |=  USART_CR3_DMAT;   //DMA发送使能
  USARTx->CR3 |=  USART_CR3_DMAR;   //DMA接收使能
  
  //配置中断
  USARTx->CR1 &= ~USART_CR1_TCIE;   //关发送完成中断
  USARTx->CR1 &= ~USART_CR1_RXNEIE; //关接收中断
  USARTx->CR1 |=  USART_CR1_IDLEIE; //开空闲中断
  
  NVIC_Config(USARTx, 2, 2);        //开内核中断
  
  //配置波特率
  USARTx->BRR = USARTx_GET_BRR((USARTx == USART1)? APB2_FCLK : APB1_FCLK, iBaudRate);
  
  //开串口
  USARTx->CR1 |=  USART_CR1_UE;
  
}


//UARTx_Tx DMA通道配置
static void USARTx_TxDMAConfig(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *TxDmaChannel = NULL;
  
  //获取DMA通道指针
  TxDmaChannel = USARTx_GetTxDmaChannel(USARTx);
  
  //配置DMA传输模式
  DMAx_PeriTransferConfig(TxDmaChannel, (void *)&USARTx->DR, NULL, DMA_DIR_MEM_TO_PERI);
  DMAx_ITConfig(TxDmaChannel, DMA_IT_TCIE);  //传输完成中断
  NVIC_Config(TxDmaChannel, 2, 2);  //开启内核中断
  
}


//UARTx_Rx DMA通道配置
static void USARTx_RxDMAConfig(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *RxDmaChannel = NULL;
  
  //获取DMA通道指针
  RxDmaChannel = USARTx_GetRxDmaChannel(USARTx);
  
  //配置DMA传输模式
  DMAx_PeriTransferConfig(RxDmaChannel, (void *)&USARTx->DR, USARTx_GetBuffPtr(USARTx), DMA_DIR_PERI_TO_MEM);
  DMAx_SetTransferCount(RxDmaChannel, RBUF_SIZE); //配置传输数量
  DMAx_Enable(RxDmaChannel, 1); //开启DMA传输
  
}


/* 接口函数-------------------------------------------------------------- */

/**
  * @brief  UART1初始化函数
  * @param  USARTx 串口标号
  * @param  iBaudRate 要配置的波特率
  * @retval None
  * @note   应该先配置工作模式再配置IO,否则在配置IO时产生一个负脉冲
  */
void USARTx_Init(USART_TypeDef *USARTx, uint32_t iBaudRate)
{
  //配置工作模式
  USARTx_ModeConfig(USARTx, iBaudRate);
  
  //配置DMA
  USARTx_TxDMAConfig(USARTx);
  USARTx_RxDMAConfig(USARTx);
  
  //配置引脚
  USARTx_IOConfig(USARTx);
  
  //注册中断回调函数
  USARTx_LoginIRQFunc(USARTx);
  
}



/**
  * @brief  串口字节发送函数,作为printf函数的底层驱动函数
  * @param  USARTx 串口标号
  * @param  cSendData 要发送的数据(1个字节)
  * @retval None
  */
void USARTx_SendData(USART_TypeDef *USARTx, uint8_t cSendData)
{
  //等待发送完成(发送缓冲区空)
  while (!(USARTx->SR & USART_SR_TC));
  
  //将要发送的数据写入到发生缓冲区中
  USARTx->DR = cSendData;
  
}



/**
  * @brief  串口多字节发送,其依赖于单字节发送函数
  * @param  USARTx 串口标号
  * @param  pSendBuff 数据发送缓冲区的地址
  * @param  iSize 要发送的数据数量
  * @retval 无
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
  * @brief  串口字符串发送函数
  * @param  USARTx 串口标号
  * @param  pSendBuff 数据发送缓冲区的地址
  * @retval 无
  */
void USARTx_SendStr(USART_TypeDef *USARTx, char *pSendBuff)
{
  uint16_t i = 0;
  
  /* 判断参数长度 */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  USARTx_SendBuff(USARTx, (uint8_t *)pSendBuff, i);
  
}



/**
  * @brief  Rx DMA通道复位
  * @param  USARTx 串口标号
  * @param  nSize 发送的数量
  * @retval None
  */
void USARTx_RxDMAReset(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *RxDmaChannel = NULL;
  
  //获取DMA通道指针
  RxDmaChannel = USARTx_GetRxDmaChannel(USARTx);
  
  //复位通道
  DMAx_ResetCounter(RxDmaChannel, RBUF_SIZE);
  
}



/**
  * @brief  获取已经通过DMA接收的数据的数量
  * @param  USARTx 串口标号
  * @param  nSize 发送的数量
  * @retval None
  */
uint16_t USARTx_GetRxDMACount(USART_TypeDef *USARTx)
{
  DMA_Channel_TypeDef *RxDmaChannel = NULL;
  
  //获取DMA通道指针
  RxDmaChannel = USARTx_GetRxDmaChannel(USARTx);
  
  return RBUF_SIZE - DMAx_GetCurrentCount(RxDmaChannel);
}



/* 链表管理函数----------------------------------------------------------- */

typedef struct Usart_Tx_Node
{
  struct Usart_Tx_Node *pNext;  //下一个节点的地址(若为节点尾,则定义为NULL)
  unsigned short int Len; //数据长度
  unsigned char Buff[];   //空数组
}USART_TX_NODE;

static USART_TX_NODE *g_HeadNodePtr1 = NULL;
static USART_TX_NODE *g_HeadNodePtr2 = NULL;
static USART_TX_NODE *g_HeadNodePtr3 = NULL;
static USART_TX_NODE *g_HeadNodePtr4 = NULL;


//将节点指针指向自身
static void List_Init(USART_TX_NODE *pNode)
{
  pNode->pNext = pNode;
  
}


//将节点嵌入链表尾部
static void List_SinkTail(USART_TX_NODE *pHeadNode, USART_TX_NODE *pTailNode)
{
  SL_ASSERT(pHeadNode);
  SL_ASSERT(pTailNode);
  
  //指针偏移到链表尾部
  while (pHeadNode->pNext != pHeadNode)
  {
    pHeadNode = pHeadNode->pNext;
  }

  //嵌入尾部
  pHeadNode->pNext = pTailNode;
  
}


//释放节点
static USART_TX_NODE *List_FreeHead(USART_TX_NODE *pHeadNode)
{
  USART_TX_NODE *tmpNode = NULL;
  
  SL_ASSERT(pHeadNode);
  
  //假如链表并非只有单个节点,则返回最新的首节点的地址
  if (pHeadNode->pNext != pHeadNode)
  {
    tmpNode = pHeadNode->pNext;
    
  }
  free(pHeadNode);
  
  return tmpNode;
}


//获取首节点的指针
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


//通过DMA发送数据(非链表)
static void USARTx_DMASend(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint16_t nSize)
{
  DMA_Channel_TypeDef *TxDmaChannel = NULL;
  
  //获取DMA通道指针
  TxDmaChannel = USARTx_GetTxDmaChannel(USARTx);
  
  //复位通道
  DMAx_ResetMemAddrAndCounter(TxDmaChannel, pSendBuff, nSize);
  
}


//发送当前链表首节点的数据
static void USARTx_SendHeadNodeBuff(USART_TypeDef *USARTx)
{
  //获取节点指针
  USART_TX_NODE **pHeadNode = List_GetHead(USARTx);
  SL_ASSERT(pHeadNode);
  SL_ASSERT(*pHeadNode);
  
  //等待串口上轮发送完成
  while (!(USARTx->SR & USART_SR_TC));
  
  //发送节点中的数据
  USARTx_DMASend(USARTx, (*pHeadNode)->Buff, (*pHeadNode)->Len);
  
}


/**
  * @brief  串口DMA 数据发送(链表管理方式)
  * @param  USARTx 串口标号
  * @param  pSendBuff 要发送的数据的缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
void USARTx_DMASendBuff(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint16_t nSize)
{
  USART_TX_NODE *pTxNode = NULL;
  USART_TX_NODE **pHeadNode = List_GetHead(USARTx);

  //判断参数
  SL_ASSERT(nSize);
  
  //分配空间(若分配不成功,则重复申请,以保证数据能发送出去)
  while ( (pTxNode = (USART_TX_NODE *)malloc(sizeof(USART_TX_NODE) + nSize*sizeof(unsigned char))) == NULL );

  //初始化链表
  List_Init(pTxNode);
  
  //将栈上的数据存储到节点空间中
  memcpy(pTxNode->Buff, pSendBuff, nSize);
  pTxNode->Len = nSize;
  
  //嵌入链表
  if (*pHeadNode != NULL)  //假如当前链表非空(发送中)
  {
    //将节点嵌入链表
    List_SinkTail(*pHeadNode, pTxNode);
  }
  else //假如当前链表为空(串口空闲),启动传输
  {
    *pHeadNode = pTxNode; //将当前节点设置为链表头
    USARTx_SendHeadNodeBuff(USARTx);  //发送链表头节点的数据
  }

}



/**
  * @brief  串口DMA 数据发送
  * @param  USARTx 串口标号
  * @param  pSendBuff 要发送的数据的缓冲区地址(末尾必须是'\0')
  * @retval None
  */
void USARTx_DMASendStr(USART_TypeDef *USARTx, char *pSendBuff)
{
  uint16_t i = 0;
  
  /* 判断参数长度 */
  while (pSendBuff[i] != '\0')
  {
    i++;
  }
  
  USARTx_DMASendBuff(USARTx, (uint8_t *)pSendBuff, i);
  
}

/* 中断回调函数-------------------------------------------------------------- */

/**
  * @brief  DMA1_CH4 中断,为USART1的TX通道
  * @param  None
  * @retval None
  * @note   当前模式为DMA发送完成中断
  */
static void USART1_DMA_TC_CallBack(void)
{
  g_HeadNodePtr1 = List_FreeHead(g_HeadNodePtr1);
  
  if (g_HeadNodePtr1 != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USART1);
  }
  
}



/**
  * @brief  DMA1_CH7 中断,为USART2的TX通道
  * @param  None
  * @retval None
  * @note   当前模式为DMA发送完成中断
  */
static void USART2_DMA_TC_CallBack(void)
{
  g_HeadNodePtr2 = List_FreeHead(g_HeadNodePtr2);
  
  if (g_HeadNodePtr2 != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USART2);
  }
  
}



/**
  * @brief  DMA1_CH2 中断,为USART3的TX通道
  * @param  None
  * @retval None
  * @note   当前模式为DMA发送完成中断
  */
static void USART3_DMA_TC_CallBack(void)
{
  g_HeadNodePtr3 = List_FreeHead(g_HeadNodePtr3);
  
  if (g_HeadNodePtr3 != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USART3);
  }

}



/**
  * @brief  DMA2_CH5 中断,为USART4的TX通道
  * @param  None
  * @retval None
  * @note   当前模式为DMA发送完成中断
  */
static void USART4_DMA_TC_CallBack(void)
{
  g_HeadNodePtr4 = List_FreeHead(g_HeadNodePtr4);
  
  if (g_HeadNodePtr4 != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(UART4);
  }

}




