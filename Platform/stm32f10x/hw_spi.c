/**
  ******************************************************************************
  * @file    SPI.c
  * @author  杜公子寒枫
  * @version V2.0 寄存器版本
  * @date    2016.08.11
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
  * V2.0------------
  * 修改描述: 修复配置速度函数无效的错误
  * 错误原因: 应清除的是CR1[5:3]位,原语句"SPIx->CR1 &= ~(cSeep<<3)"为无效的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.08
  *
  * V3.0------------
  * 修改描述: 修改底层实现机制,将本文件匹配SPI1~3
  * 错误原因: 应清除的是CR1[5:3]位,原语句"SPIx->CR1 &= ~(cSeep<<3)"为无效的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.08
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
  * @param  None
  * @retval None
  */
static void SPIx_IOConfig(SPI_TYPE Port)
{
  switch (Port)
  {
    case SPIx_1: 
    {
//      GPIOx_FastInit(GPIOx_PA, GPIOx_4, GPIOx_GP_PP_10M);
      GPIOx_FastInit(GPIOx_PA, GPIOx_5, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PA, GPIOx_6, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PA, GPIOx_7, GPIOx_AF_PP_50M);
      
      /*初始化输出*/
//      SPI1_NSS = 1;
      break; 
    }
    
    case SPIx_2: 
    {
//      GPIOx_FastInit(GPIOx_PB, GPIOx_12, GPIOx_GP_PP_10M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_13, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_14, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PB, GPIOx_15, GPIOx_AF_PP_50M);
      
      /*初始化输出*/
//      SPI2_NSS = 1;
      break; 
    }
    
    case SPIx_3: 
    {
      GPIOx_FastInit(GPIOx_PA, GPIOx_15, GPIOx_GP_PP_10M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_3, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_4, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PB, GPIOx_5, GPIOx_AF_PP_50M);
      
      /*初始化输出*/
//      SPI3_NSS = 1;
      break; 
    }
    
    default: break;
  }
  
}



/**
  * @brief  SPIx模式配置
  * @param  None
  * @retval None
  */
static void SPIx_ModeConfig(SPI_TYPE Port)
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
  
  /* 配置工作模式 */
  SPI[Port]->CR1 &= ~(0X1<<15);   //双线双向模式(全双工)
  SPI[Port]->CR1 &= ~(0X1<<13);   //硬件CRC校验,考虑可移植性,不开启
  SPI[Port]->CR1 &= ~(0X1<<11);   //数据帧格式:8位
  SPI[Port]->CR1 &= ~(0X1<<10);   //全双工模式配置
  SPI[Port]->CR1 |=  (0X1<<9);    //软件管理模式
  SPI[Port]->CR1 |=  (0X1<<8);
  SPI[Port]->CR1 &= ~(0X1<<7);    //数据帧:高位先发
  SPI[Port]->CR1 &= ~(0X7<<3);
  SPI[Port]->CR1 |=  (0X7<<3);    //SPIx分频: 256分频(先配置为最低,再根据实际修改)
  SPI[Port]->CR1 |=  (0X1<<2);    //配置为主机
  
  /* 配置通信时序 */
  SPI[Port]->CR1 |=  (0X0<<0);    //模式0
  
  /* 开启SPI */
  SPI[Port]->CR1 |=  (0X1<<6);
  
}



/**
  * @brief  SPIx初始化
  * @param  None
  * @retval None
  */
void SPIx_Init(SPI_TYPE Port)
{
  /* 判断入参 */
  if (Port >= s_SpiPortNum)
  {
    return;
  }
  
  /* 配置引脚 */
  SPIx_IOConfig(Port);
  
  /* 配置工作模式 */
  SPIx_ModeConfig(Port);
  
  /* 配置工作速度 */
  SPIx_SetSpeed(Port, SPIx_SPEED_DIV8);
  
}



/**----------------------------------------------/
  * @attention SPI1的时钟为72M,SPI2,3的时钟为36M
  *   SPI1不可以2分频(最大18M)
  *----------------------------------------------*/

/**
  * @brief  SPIx速度配置
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
  * @brief  SPIx数据读写
  * @param  cWriteData 要写入的数据
  * @retval cReadData  读取到的数据
  */
uint8_t SPIx_ReadWriteByte(SPI_TYPE Port, uint8_t cWriteData)
{
  uint8_t cReadData = 0;

  /* 等待发送完成 */
  while (!(SPI[Port]->SR & (0X1<<1)));
  SPI[Port]->DR = cWriteData;
  
  /* 等待接收完成 */
  while (!(SPI[Port]->SR & (0X1<<0)));
  cReadData = SPI[Port]->DR;
  
  return cReadData;
}

