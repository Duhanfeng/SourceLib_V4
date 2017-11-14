/**
  ******************************************************************************
  * @file    hw_dma.h
  * @author  杜公子寒枫
  * @version V1.0 寄存器版本
  * @date    2017.05.22
  * @brief   DMA驱动层实现文件
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_dma.h"
#include "SourceLib.h"
#include "hw_rcc.h"


/**
  * @brief  外设传输模式配置
  * @param  DMA_Channelx DMA通道标号
  * @param  PeriAddr 外设地址
  * @param  MemAddr 内存地址
  * @param  cDir 方向(外设到内存/内存到外设)
  * @retval None
  */
void DMAx_PeriTransferConfig(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr, void *MemAddr, uint8_t cDir)
{
  //开时钟
  RCC_EnableClock(DMA_Channelx, 1);
  
  //配置模式
  DMA_Channelx->CCR &= ~DMA_CCR1_MEM2MEM; //非内存-内存模式
  DMA_Channelx->CCR &= ~DMA_CCR1_PL; 
  DMA_Channelx->CCR |=  DMA_CCR1_PL_0;    //中等优先级
  
  DMA_Channelx->CCR &= ~DMA_CCR1_MSIZE;   //内存:8位数据帧格式
  DMA_Channelx->CCR &= ~DMA_CCR1_PSIZE;   //外设:8位数据帧格式
  
  DMA_Channelx->CCR |=  DMA_CCR1_MINC;    //内存增量模式
  DMA_Channelx->CCR &= ~DMA_CCR1_PINC;    //外设非增量模式
  DMA_Channelx->CCR &= ~DMA_CCR1_CIRC;    //非循环模式
  
  //配置传输方向
  DMA_Channelx->CCR &= ~DMA_CCR1_DIR;
  DMA_Channelx->CCR |=  (cDir ? DMA_CCR1_DIR : 0);  //cDir: 0--外设到内存 1--内存到外设

  //配置外设中断
  DMA_Channelx->CCR &= ~DMA_CCR1_TEIE;    //关闭错误中断
  DMA_Channelx->CCR &= ~DMA_CCR1_HTIE;    //关闭半传输中断
  DMA_Channelx->CCR &= ~DMA_CCR1_TCIE;    //关闭传输完成中断
  
  //配置外设地址
  DMA_Channelx->CPAR = (uint32_t)PeriAddr;
  
  //配置内存地址
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
  //关闭DMA
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
}



/**
  * @brief  外设传输模式配置
  * @param  DMA_Channelx DMA通道标号
  * @param  isEnable 1-开启DMA,0-关闭DMA
  * @retval None
  */
void DMAx_Enable(DMA_Channel_TypeDef *DMA_Channelx, uint8_t isEnable)
{
  if (isEnable)
  {
    DMA_Channelx->CCR |=  DMA_CCR1_EN;
  }
  else 
  {
    DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  }
  
}



/**
  * @brief  复位对应的通道的计数器
  * @param  DMA_Channelx DMA通道标号
  * @param  nCount 计数器的复位值
  * @retval None
  */
void DMAx_ResetCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  //关闭通道
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
  //配置发送数量
  DMA_Channelx->CNDTR = nCount;
    
  //开启通道
  DMA_Channelx->CCR |=  DMA_CCR1_EN;
  
}



/**
  * @brief  复位对应的通道的内存地址以及计数器
  * @param  DMA_Channelx DMA通道标号
  * @param  MemAddr 内存地址的复位值
  * @param  nCount 计数器的复位值
  * @retval None
  */
void DMAx_ResetMemAddrAndCounter(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr, uint16_t nCount)
{
  //关闭通道
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
  //配置内存地址
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
  //配置发送数量
  DMA_Channelx->CNDTR = nCount;
    
  //开启通道
  DMA_Channelx->CCR |=  DMA_CCR1_EN;
  
}



/**
  * @brief  外设中断配置
  * @param  DMA_Channelx DMA通道标号
  * @param  IRQ_Mask 对应中断的掩码
  * @retval None
  */
void DMAx_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint8_t IRQ_Mask)
{
  //关闭所有中断
  DMA_Channelx->CCR &= ~(DMA_CCR1_TCIE | DMA_CCR1_HTIE | DMA_CCR1_TEIE);
  
  //开启对应的中断
  DMA_Channelx->CCR |= (IRQ_Mask);
  
}



/**
  * @brief  设置传输数量
  * @param  DMA_Channelx DMA通道标号
  * @param  nCount 要设置的计数器的值
  * @retval None
  */
void DMAx_SetTransferCount(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  DMA_Channelx->CNDTR = nCount;
  
}



/**
  * @brief  获取当前计数器的值
  * @param  DMA_Channelx DMA通道标号
  * @retval 当前计数器的值
  */
uint16_t DMAx_GetCurrentCount(DMA_Channel_TypeDef *DMA_Channelx)
{
  
  return DMA_Channelx->CNDTR;
}



/**
  * @brief  配置外设地址
  * @param  DMA_Channelx DMA通道标号
  * @param  PeriAddr 外设地址
  * @retval None
  */
void DMAx_SetPeriAddr(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr)
{
  DMA_Channelx->CPAR = (uint32_t)PeriAddr;
  
}



/**
  * @brief  配置内存地址
  * @param  DMA_Channelx DMA通道标号
  * @param  MemAddr 内存地址
  * @retval None
  */
void DMAx_SetMemAddr(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr)
{
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
}



/**
  * @brief  获取DMA的状态
  * @param  DMAx DMA标号
  * @retval 当前DMA的状态
  */
uint32_t DMAx_GetITStatus(DMA_TypeDef *DMAx)
{
  
  return DMAx->ISR;
}



/**
  * @brief  清除对应的标志位
  * @param  DMAx DMA标号
  * @param  iMask 要清除的状态位的掩码
  * @retval None
  */
void DMAx_ClearITStatus(DMA_TypeDef *DMAx, uint32_t iMask)
{
  DMAx->IFCR = iMask;
  
}

