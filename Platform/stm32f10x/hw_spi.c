/**
  ******************************************************************************
  * @file    SPI.c
  * @author  杜公子寒枫
  * @version V2.0 寄存器版本
  * @date    2017.04.18
  * @brief   SPIx实现文件
  ******************************************************************************
  * @attention
  * 
  *
  * SPIx:   NSS  --- PA4   通用推挽,10M
  *         SCK  --- PA5   复用推挽,50M
  *         MISO --- PA6   上拉输入
  *         MOSI --- PA7   复用推挽,50M
  *
  * SPI2:   NSS  --- PB12  通用推挽,10M
  *         SCK  --- PB13  复用推挽,50M
  *         MISO --- PB14  上拉输入
  *         MOSI --- PB15  复用推挽,50M
  *
  * SPI3:   NSS  --- PA15  通用推挽,10M
  *         SCK  --- PB3   复用推挽,50M
  *         MISO --- PB4   上拉输入
  *         MOSI --- PB5   复用推挽,50M
  * 
  * 数据帧: 8位数据位,高位在前,每发一位出去从机都会返回一位的数据回来
  * 
  * 本文件中将SPI的NSS片选功能配置为软件管理模式,这样,外接SPI设备时不再受限于此片选位
  * 的实际连接情况(防止片选脚复用).
  * 
  *
  * V2.0------------
  * 修改描述: 修复配置速度函数无效的错误
  * 错误原因: 应清除的是CR1[5:3]位,原语句"SPIx->CR1 &= ~(cSeep<<3)"为无效的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.08
  *
  * V3.0------------
  * 修改描述: 修改底层实现机制,将本文件匹配SPI1~3
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2017.03.10
  *
  * V4.0------------
  * 修改描述: 1.增加SPI的主从机模式配置
  *           2.发现其主从模式下从机接收数据移位的原因及解决方案
  *           3.发现仿真情况下SPI卡死的原因以及解决方案(适用于其他外设类似的情况)
  *           4.开放部分接口
  *           5.将SPI读写函数输入参的数据类型从8位改成16位(兼容16位数据)
  * 主从模式: 在SPI从模式中,SLK,MISO,MOSI和主机的IO一一对应连接,但是需要根据实际的
  *           情况来配置其输入输出模式.从模式下,从机发送的数据只是存在在发送缓存寄
  *           存器中,等待主机的下一次数据时钟的到来才逐位移位.
  * 数据移位: 在SPI从模式接收中,接收到的数据有可能和实际主机发送的数据不一致,存在数
  *           据移位(不一致)的现象,这时主从模式的时钟不同步导致的.这个现象往往是主
  *           从机在初始化过程中,IO不稳定的状态导致其从机误识别为其接收到的数据,从
  *           而让两者的数据不同步.以主从机环回通信测试为例,这个问题的解决方法是先
  *           对主从机IO进行模式配置,再对SPI进行初始化(先配置主机再配置从机),让从
  *           机在启动的时候已经错过了主机导致的IO抖动的情况.
  * 仿真卡死: 在仿真时,如果在SPI读写函数中的 while (!(SPI[Port]->SR & (0X1<<0)));
  *           语句处打断点(尚未执行),再执行此条件判断语句,则发现其无法通过其条件判
  *           断,从而让系统卡死在此处,但是直接的执行就不会出现这个问题.经调试发现,
  *           只要是在判断SPI_RXNE之前进入了断点或者是停止执行,都会导致其RXNE标志
  *           清零,原因是在调试模式下(非连续执行),会对SPI_DR寄存器进行读取,而读取
  *           DR寄存器会导致其RXNE标志位清零,最终导致其判断错误从而卡死.这个问题
  *           在读寄存器清零的情况下都会有可能出现,这一点值得注意.
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2017.04.18
  *
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "hw_spi.h"


/* ---映射外设寄存器--- */
#if(defined(STM32F10X_HD)||defined(STM32F10X_HD_VL)||defined(STM32F10X_XL)||defined(STM32F10X_CL))
static SPI_TypeDef  * const SPI[3] = {SPI1, SPI2, SPI3};
static const uint8_t s_SpiPortNum = 3;

#elif(defined(STM32F10X_MD)||defined(STM32F10X_MD_VL))
static SPI_TypeDef  * const SPI[2] = {SPI1, SPI2};
static const uint8_t s_SpiPortNum = 2;

#elif(defined(STM32F10X_LD)||defined(STM32F10X_LD_VL))
static SPI_TypeDef  * const SPI[1] = {SPI1};
static const uint8_t s_SpiPortNum = 1;

#else
#error Undefine STM32F10x Flash Size

#endif 

/**
  * @brief  SPIx引脚配置
  * @param  Port SPI端口
  * @param  Mode SPI端口
  * @retval None
  */
void SPIx_IOConfig(SPI_TYPE Port, SPIx_WORK_MODE Mode)
{
  switch (Port)
  {
    case SPIx_1: 
    {
      if (Mode == SPIx_MASTER_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_4, GPIOx_AF_PP_50M);
        GPIOx_FastInit(GPIOx_PA, GPIOx_5, GPIOx_AF_PP_50M);   //CLK
        GPIOx_FastInit(GPIOx_PA, GPIOx_6, GPIOx_PUSH_UP);     //MISO
        GPIOx_FastInit(GPIOx_PA, GPIOx_7, GPIOx_AF_PP_50M);   //MOSI
        
        /*初始化输出*/
//        SPI1_NSS = 1;
      }
      else if (Mode == SPIx_SLAVE_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_4, GPIOx_PUSH_UP);
        GPIOx_FastInit(GPIOx_PA, GPIOx_5, GPIOx_FLOAT);     //CLK
        GPIOx_FastInit(GPIOx_PA, GPIOx_6, GPIOx_AF_PP_50M); //MISO
        GPIOx_FastInit(GPIOx_PA, GPIOx_7, GPIOx_FLOAT);     //MOSI
      }

      break; 
    }
    
    case SPIx_2: 
    {
      if (Mode == SPIx_MASTER_MODE)
      {
//        GPIOx_FastInit(GPIOx_PB, GPIOx_12, GPIOx_GP_PP_10M);
        GPIOx_FastInit(GPIOx_PB, GPIOx_13, GPIOx_AF_PP_50M);  //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_14, GPIOx_FLOAT);      //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_15, GPIOx_AF_PP_50M);  //MOSI
        
        /*初始化输出*/
//        SPI2_NSS = 1;
      }
      
      else if (Mode == SPIx_SLAVE_MODE)
      {
//        GPIOx_FastInit(GPIOx_PB, GPIOx_12, GPIOx_FLOAT);
        GPIOx_FastInit(GPIOx_PB, GPIOx_13, GPIOx_FLOAT);      //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_14, GPIOx_AF_PP_50M);  //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_15, GPIOx_FLOAT);      //MOSI
      }
      
      break; 
    }
    
    case SPIx_3: 
    {
      if (Mode == SPIx_MASTER_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_15, GPIOx_GP_PP_10M);
        GPIOx_FastInit(GPIOx_PB, GPIOx_3,  GPIOx_AF_PP_50M);  //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_4,  GPIOx_PUSH_UP);    //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_5,  GPIOx_AF_PP_50M);  //MOSI
        
        /*初始化输出*/
//        SPI3_NSS = 1;
      }
      else if (Mode == SPIx_SLAVE_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_15, GPIOx_PUSH_UP);
        GPIOx_FastInit(GPIOx_PB, GPIOx_3,  GPIOx_PUSH_UP);    //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_4,  GPIOx_AF_PP_50M);  //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_5,  GPIOx_PUSH_UP);    //MOSI
      }
      
      break; 
    }
    
    default: break;
  }
  
}



/**
  * @brief  SPIx模式配置
  * @param  Port SPI端口
  * @retval None
  */
static void SPIx_ModeConfig(SPI_TYPE Port, SPIx_WORK_MODE Mode)
{
  /* 开时钟 */
  if (Port == SPIx_1)
  {
    RCC->APB2ENR |= (0X1<<12);
  }
  else 
  {
    RCC->APB1ENR |= (0X1<<(13+Port));
  }
  
  //模式配置
  SPI[Port]->CR1 &= ~(0x1<<15);   //双线双向模式(全双工)
  SPI[Port]->CR1 &= ~(0x1<<13);   //硬件CRC校验,考虑可移植性,不开启
  SPI[Port]->CR1 &= ~(0x1<<11);   //数据帧格式:8位
  SPI[Port]->CR1 &= ~(0x1<<10);   //全双工模式配置
  SPI[Port]->CR1 |=  (0x1<<9);    //软件管理模式
  SPI[Port]->CR1 |=  (0x1<<8);
  SPI[Port]->CR1 &= ~(0x1<<7);    //数据帧:高位先发
  SPI[Port]->CR1 &= ~(0x7<<3);
  SPI[Port]->CR1 |=  (0x7<<3);    //SPIx分频: 256分频(先配置为最低,再根据实际修改)
  SPI[Port]->CR1 |=  (0x1<<2);    //配置为主机

  if (Mode == SPIx_SLAVE_MODE)
  {
    IRQn_Type SPIx_IRQ[3] = {SPI1_IRQn, SPI2_IRQn, SPI3_IRQn};
    
    SPI[Port]->CR1 &= ~(0x1<<8);  //内部NSS下拉
    SPI[Port]->CR1 &= ~(0x1<<2);  //配置为从机
    
    SPI[Port]->CR2 |=  (0x1<<6);  //开启接收中断
    NVIC_Enable(SPIx_IRQ[Port], 1, 1); //开内核中断
    
    SPI[Port]->DR = 0xFF;
  }
  
  /* 配置通信时序 */
  SPI[Port]->CR1 |=  (0x0<<0);    //模式x
  
  /* 开启SPI */
  SPI[Port]->CR1 |=  (0x1<<6);
  
}



/**
  * @brief  SPIx初始化
  * @param  Port SPI端口
  * @retval None
  */
void SPIx_Init(SPI_TYPE Port, SPIx_WORK_MODE Mode)
{
  /* 判断入参 */
  if (Port >= s_SpiPortNum)
  {
    return;
  }
  
  /* 配置工作模式 */
  SPIx_ModeConfig(Port, Mode);
  
  /* 配置引脚 */
//  SPIx_IOConfig(Port, Mode);
  
  /* 配置工作速度 */
  SPIx_SetSpeed(Port, SPIx_SPEED_DIV16);
  
}



/**----------------------------------------------/
  * @attention SPI1的时钟为72M,SPI2,3的时钟为36M
  *   SPI1不可以2分频(最大18M)
  *----------------------------------------------*/

/**
  * @brief  SPIx速度配置
  * @param  Port SPI端口
  * @param  cSeep 要配置的速度,已定义相关的宏
  * @retval None
  */
void SPIx_SetSpeed(SPI_TYPE Port, SPIx_SPEED_DVI SpeedDvi)
{
  /* 关闭SPIx */
  SPI[Port]->CR1 &= ~(0X1<<6);
  
  /* 配置SPIx速度 */
  SPI[Port]->CR1 &= ~(0X7<<3);
  SPI[Port]->CR1 |=  (SpeedDvi<<3);   //SPIx分频
  
  /* 开启SPIx */
  SPI[Port]->CR1 |=  (0X1<<6);
  
}



/**
  * @brief  SPIx状态寄存器写入
  * @param  Port SPI端口
  * @retval SPI[Port]->SR SPI状态寄存器获取
  */
uint16_t SPIx_GetStatus(SPI_TYPE Port)
{
  
  return SPI[Port]->SR;
}


/**
  * @brief  SPIx数据寄存器写入
  * @param  Port SPI端口
  * @param  cWriteData 要写入的数据
  * @retval Note
  */
void SPIx_WriteDataReg(SPI_TYPE Port, uint16_t cWriteData)
{
  SPI[Port]->DR = cWriteData;
  
}



/**
  * @brief  SPIx数据寄存器读取
  * @param  Port SPI端口
  * @retval cReadData  读取到的数据
  */
uint16_t SPIx_ReadDataReg(SPI_TYPE Port)
{
  
  return SPI[Port]->DR;
}



/**
  * @brief  SPIx数据读写
  * @param  cWriteData 要写入的数据
  * @retval Note
  */
uint16_t SPIx_ReadWriteByte(SPI_TYPE Port, uint16_t cWriteData)
{
  uint8_t cReadData = 0;

  /* 等待发送完成 */
  while (!(SPI[Port]->SR & (0X1<<1)));
  SPI[Port]->DR = cWriteData;
  
  /* 等待接收完成 */
  //值得注意的是,在仿真的时候如果在这里打断点(读RXNE状态位之前),则系统会将
  //卡死,因为仿真器会读取DR寄存器的值,从而RXNE状态位清零
  while (!(SPI[Port]->SR & (0X1<<0)));
  cReadData = SPI[Port]->DR;
  
  return cReadData;
}



