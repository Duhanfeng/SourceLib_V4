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


//复位对应的通道的计数器
void DMAx_ResetCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  //关闭通道
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
  //配置发送数量
  DMA_Channelx->CNDTR = nCount;
    
  //开启通道
  DMA_Channelx->CCR |=  DMA_CCR1_EN;
  
}


//复位对应的通道的内存地址以及计数器
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



//外设中断配置
void DMAx_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint8_t IRQ_Mask)
{
  //关闭所有中断
  DMA_Channelx->CCR &= ~(DMA_CCR1_TCIE | DMA_CCR1_HTIE | DMA_CCR1_TEIE);
  
  //开启对应的中断
  DMA_Channelx->CCR |= (IRQ_Mask);
  
}



//外设传输模式配置
//外设<--->内存
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


//开始/关闭DMA
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


//设置传输数量
void DMAx_SetTransferCount(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  DMA_Channelx->CNDTR = nCount;
  
}


//获取传输的数量
uint16_t DMAx_GetTransferCount(DMA_Channel_TypeDef *DMA_Channelx)
{
  
  return DMA_Channelx->CNDTR;
}


//配置外设地址
void DMAx_SetPeriAddr(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr)
{
  DMA_Channelx->CPAR = (uint32_t)PeriAddr;
  
}


//配置内存地址
void DMAx_SetMemAddr(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr)
{
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
}


//获取DMA的状态
uint32_t DMAx_GetITStatus(DMA_TypeDef *DMAx)
{
  
  return DMAx->ISR;
}


//清除对应的标志位
void DMAx_ClearITStatus(DMA_TypeDef *DMAx, uint32_t iMask)
{
  DMAx->IFCR = iMask;
  
}





