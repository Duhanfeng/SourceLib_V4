/**
  ******************************************************************************
  * @file    Usart.h
  * @author  杜公子寒枫
  * @version V3.0 寄存器版本
  * @date    2016.11.18
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
  * 错误原因: 在配置IO口为复用模式后,由于此时UART4的时钟尚未开启,所以电平被拉低.
  *           紧接着,开启UART4的时钟并且使能发送功能后,IO通电,恢复高电平,故而整体为
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


/* ---串口分频系数计算宏--- */
#define USARTx_GET_BRR(Fpclk, BaudRate)  \
  (((uint16_t)((Fpclk)/(16*(BaudRate)))<<4) | \
  ((uint8_t)((((Fpclk)/(16.0*(BaudRate))) - (uint16_t)((Fpclk)/(16*(BaudRate))))*16)))
  

/*----------------------------------------------------------------------------
    数据缓冲区定义
 *----------------------------------------------------------------------------*/
 
 /* ---定义数据缓冲大小--- */
#define RBUF_SIZE   256      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

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

/*----------------------------------------------------------------------------
    相关寄存器映射
 *----------------------------------------------------------------------------*/

/* ---映射外设寄存器--- */
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



/* 内部函数-------------------------------------------------------------- */

//IO配置
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
  


//串口模式配置
static void USARTx_ModeConfig(USART_TYPE Port, uint32_t iBaudRate)
{
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
  
  NVIC_Enable(USARTx_IRQn[Port],2,2); //开内核中断
  
  /* 配置波特率 */
  USART[Port]->BRR = USARTx_GET_BRR((Port == USARTx_1 )? APB2_FCLK : APB1_FCLK, iBaudRate);
  
  /* 开串口 */
  USART[Port]->CR1 |=  (0x1<<13);
  
}



//UARTx_Tx DMA通道配置
static void USARTx_TxDMAConfig(USART_TYPE Port)
{
  /* 开时钟 */
  if (Port != USARTx_4)
  {
    RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
  }
  else
  {
    RCC->AHBENR |= (0X1<<1);  //DMA2时钟使能
  }
  
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
  
  NVIC_Enable(USARTx_DMA_IRQn[Port],2,2); //开内核中断
  
  /* 配置外设地址 */
  USART_TxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->DR);
  
  /* 关闭通道 */
  USART_TxDmaChannel[Port]->CCR &= ~(0X1<<0);   //关闭通道,在开启接受/发送时再启动
  
}
  

//UARTx_Rx DMA通道配置
static void USARTx_RxDMAConfig(USART_TYPE Port)
{
  const uint8_t * const arBuffAddr[4] = { Uart1_RecvBuff,Uart2_RecvBuff,
                                          Uart3_RecvBuff,Uart4_RecvBuff };
  
  /* 开时钟 */
  if (Port != USARTx_4)
  {
    RCC->AHBENR |= (0X1<<0);  //DMA1时钟使能 
  }
  else
  {
    RCC->AHBENR |= (0X1<<1);  //DMA2时钟使能
  }
  
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
  USART_RxDmaChannel[Port]->CPAR = (uint32_t)&(USART[Port]->DR);
  
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



//注册串口空闲中断的回调代码
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


/* 接口函数-------------------------------------------------------------- */

/**
  * @brief  UART1初始化函数
  * @note   应该先配置工作模式再配置IO,否则在配置IO时产生一个负脉冲
  * @param  Port 选择要初始化的串口
  * @retval None
  */
void USARTx_Init(USART_TYPE Port, uint32_t iBaudRate)
{
  /* 判断入参 */
  if (Port >= USARTx_COUNT)
  {
    return;
  }
  
  /* 配置工作模式 */
  USARTx_ModeConfig(Port, iBaudRate);
  
  /* 配置DMA */
  USARTx_TxDMAConfig(Port);
  USARTx_RxDMAConfig(Port);
  
  /* 配置引脚 */
  USARTx_IOConfig(Port);
  
  /* 配置中断回调函数 */
  USARTx_LoginIRQFunc(Port);
  
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
  while (!(USART[Port]->SR & (1<<6)));
  
  //将要发送的数据写入到发生缓冲区中
  USART[Port]->DR = cSendData;
  
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
  while (!(USART[Port]->SR & (1<<6)));
  
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
  SL_ASSERT(nSize);
  
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


/* 中断回调函数-------------------------------------------------------------- */


/**
  * @brief  DMA1_CH4 中断,为USART1的TX通道
  * @note   当前模式为DMA发送完成中断
  * @param  None
  * @retval None
  */
void DMA1_Channe4_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_1);
  
  if (g_HeadTxNodePtr[USARTx_1] != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USARTx_1);
  }
  
}



/**
  * @brief  DMA1_CH7 中断,为USART2的TX通道
  * @note   当前模式为DMA发送完成中断
  * @param  None
  * @retval None
  */
void DMA1_Channe7_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_2);
  
  if (g_HeadTxNodePtr[USARTx_2] != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USARTx_2);
  }
  
}



/**
  * @brief  DMA1_CH2 中断,为USART3的TX通道
  * @note   当前模式为DMA发送完成中断
  * @param  None
  * @retval None
  */
void DMA1_Channe2_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_3);
  
  if (g_HeadTxNodePtr[USARTx_3] != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USARTx_3);
  }

}



/**
  * @brief  DMA2_CH5 中断,为USART4的TX通道
  * @note   当前模式为DMA发送完成中断
  * @param  None
  * @retval None
  */
void DMA2_Channe5_IRQCallBack(void)
{
  USARTx_FreeHeadNode(USARTx_4);
  
  if (g_HeadTxNodePtr[USARTx_4] != NULL)  //若当前的首节点非空
  {
    USARTx_SendHeadNodeBuff(USARTx_4);
  }

}


extern void USART1_IdleTask(void *pBuff, uint16_t nCount);
/**
  * @brief  空闲中断模式,当发送一帧数据后进入空闲状态时,自动进入此中断
  * @note   当前模式为UART+DMA+空闲中断
  * @param  None
  * @retval None
  */
void USART1_IDLE_IRQCallBack(void)  
{
  uint16_t nUart1RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度
  
  /* 计算接收到的帧长 */
  nUart1RecvCnt = RBUF_SIZE - DMA1_Channel5->CNDTR;
  
  /* 复位DMA */
  USARTx_RxDMAReset(USARTx_1);
  
  /* 数据帧处理 */
  (void)nUart1RecvCnt;

}



/**
  * @brief  空闲中断模式,当发送一帧数据后进入空闲状态时,自动进入此中断
  * @note   当前模式为UART+DMA+空闲中断
  * @param  None
  * @retval None
  */
void USART2_IDLE_IRQCallBack(void)  
{
  uint16_t nUart2RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度

  /* 计算接收到的帧长 */
  nUart2RecvCnt = RBUF_SIZE - DMA1_Channel6->CNDTR;
  
  /* 复位DMA */
  USARTx_RxDMAReset(USARTx_2);
  
  /* 数据帧处理 */
  (void)nUart2RecvCnt;

}



/**
  * @brief  空闲中断模式,当发送一帧数据后进入空闲状态时,自动进入此中断
  * @note   当前模式为UART+DMA+空闲中断
  * @param  None
  * @retval None
  */
void USART3_IDLE_IRQCallBack(void)  
{
  uint16_t nUart3RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度

  /* 计算接收到的帧长 */
  nUart3RecvCnt = RBUF_SIZE - DMA1_Channel3->CNDTR;
  
  /* 复位DMA */
  USARTx_RxDMAReset(USARTx_3);
  
  /* 数据帧处理 */
  (void)nUart3RecvCnt;

}



/**
  * @brief  空闲中断模式,当发送一帧数据后进入空闲状态时,自动进入此中断
  * @note   当前模式为UART+DMA+空闲中断
  * @param  None
  * @retval None
  */
void USART4_IDLE_IRQCallBack(void)  
{
  uint16_t nUart4RecvCnt = 0;  //存储当前接收到的数据的缓冲区长度

  /* 计算接收到的帧长 */
  nUart4RecvCnt = RBUF_SIZE - DMA2_Channel3->CNDTR;
  
  /* 复位DMA */
  USARTx_RxDMAReset(USARTx_4);
  
  /* 数据帧处理 */
  (void)nUart4RecvCnt;

}




