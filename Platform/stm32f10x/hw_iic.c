/**
  ******************************************************************************
  * @file    multi_iic.c
  * @author  杜公子寒枫
  * @version V3.0 寄存器版本
  * @date    2016.08.05
  * @brief   IIC协议配置(模拟方式)
  ******************************************************************************
  * @attention
  * 
  * 引脚,通用开漏输出,2M
  * 
  * 数据帧: 高位先发
  * 
  * 注1: 凡是读取操作前,都要释放总线(置高电平),否则从机无法操作总线!
  * 注2: 在硬件实现中,如果没有在传输当前字节之前处理EV7、EV7_1、EV6_1、EV2、
  *      EV8和EV3事件,有可能产生问题,如收到一个额外的字节、两次读到相同的数据
  *      或丢失数据——<<勘误手册>>.   硬件的使用存在BUG,故而用软件方式实现
  * 
  * 为针对一些需要多IIC通道并且并非挂在同一总线的场合,特此编写的驱动文件.
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "hw_iic.h"
#include "SourceLib.h"
#include "stm32f10x.h"

#define IIC_RESERVER      *((volatile unsigned long  *)0x00000000)
  
//SII9575
#define IIC1_SCL      (PBout(9))
#define IIC1_SDA      (PBout(8))
#define IIC1_R_SDA    (PBin (8))

//M21452_2
#define IIC2_SCL      (PDout(2))
#define IIC2_SDA      (PDout(1))
#define IIC2_R_SDA    (PDin (1))

//IIC_RESERVER
#define IIC3_SCL      (IIC_RESERVER)
#define IIC3_SDA      (IIC_RESERVER)
#define IIC3_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC4_SCL      (IIC_RESERVER)
#define IIC4_SDA      (IIC_RESERVER)
#define IIC4_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC5_SCL      (IIC_RESERVER)
#define IIC5_SDA      (IIC_RESERVER)
#define IIC5_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC6_SCL      (IIC_RESERVER)
#define IIC6_SDA      (IIC_RESERVER)
#define IIC6_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC7_SCL      (IIC_RESERVER)
#define IIC7_SDA      (IIC_RESERVER)
#define IIC7_R_SDA    (IIC_RESERVER)


static volatile unsigned long * const pSDA_Port[7]  = {&IIC1_SDA, &IIC2_SDA, &IIC3_SDA, &IIC4_SDA, &IIC5_SDA, &IIC6_SDA};
static volatile unsigned long * const pSCL_Port[7]  = {&IIC1_SCL, &IIC2_SCL, &IIC3_SCL, &IIC4_SCL, &IIC5_SCL, &IIC6_SCL};
static volatile unsigned long * const pRead_Port[7] = {&IIC1_R_SDA, &IIC2_R_SDA, &IIC3_R_SDA, &IIC4_R_SDA, &IIC5_R_SDA, &IIC6_R_SDA};

#define IICx_SDA    *pSDA_Port
#define IICx_SCL    *pSCL_Port
#define IICx_R_SDA  *pRead_Port

#define IIC_HIGH_SPEED    ( 1)  //快速,约150KHz  H:3us  L:4us    
#define IIC_MEDIUM_SPEED  ( 5)  //中速,约70KHz   H:7us  L:8us    
#define IIC_LOW_SPEED     (10)  //慢速,约40KHz   H:12us L:13us     

#define IIC_Delay()  IICx_Delay(Port)

void IICx_Delay(IIC_PORT Port)
{
  const unsigned short int nTime[7] = { 
    IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED,
    IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED};
  
  DelayUs(nTime[Port]);
  
}



//IIC引脚配置
void IICx_IOConfig(IIC_PORT Port)
{
  switch (Port)
  {
    case IIC_PORT1:
    {
      GPIOx_FastInit(GPIOx_PB, GPIOx_8, GPIOx_GP_OD_10M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_9, GPIOx_GP_OD_10M);
      break;
    }
    case IIC_PORT2:
    {
      
      break;
    }
    case IIC_PORT3:
    {
      
      break;
    }
    case IIC_PORT4:
    {
      
      break;
    }
    case IIC_PORT5:
    {
      
      break;
    }
    case IIC_PORT6:
    {
      
      break;
    }
    default: break;
  }
  
  IICx_SDA[Port] = 1;
  IICx_SCL[Port] = 1;
}


//IIC初始化
void IICx_Init(IIC_PORT Port)
{
  IICx_IOConfig(Port);
  
}



/**
  * @brief  IIC1开始标志(在SCL高电平时拉低SDA)
  * @param  无
  * @retval 无
  */
void IICx_Start(IIC_PORT Port)
{
  IICx_SDA[Port] = 1;
  IICx_SCL[Port] = 1;
  IIC_Delay();
  IICx_SDA[Port] = 0;
  IIC_Delay();
  IICx_SCL[Port] = 0;
  
}



/**
  * @brief  IIC1结束标志(在SCL高电平时拉高SDA)
  * @param  无
  * @retval 无
  */
void IICx_Stop(IIC_PORT Port)
{
  IICx_SCL[Port] = 0;
  IICx_SDA[Port] = 0;
  
  IICx_SCL[Port] = 1;
  IIC_Delay();
  IICx_SDA[Port] = 1;
  IIC_Delay();

}



/**
  * @brief  IIC1字节发送函数(在SCL低电平时拉高/拉低SDA)
  * @param  cSendData 要发送的字节
  * @retval 无
  */
void IICx_SendByte(IIC_PORT Port, uint8_t cSendData)
{
  uint8_t i = 0;
  
  for (i = 0; i < 8; i++)
  {
    IICx_SCL[Port] = 0;
    
    if (cSendData & (0X1<<(7-i)))  //发送数据
    {
      IICx_SDA[Port] = 1;      
    }
    else
    {
      IICx_SDA[Port] = 0;
    }
    
    IIC_Delay();
    
    IICx_SCL[Port] = 1;
    IIC_Delay();
    
    IICx_SCL[Port] = 0;
  }

}



/**
  * @brief  IIC1字节接收函数(在SCL高电平时读SDA电平状态)
  * @param  无
  * @retval cRecvData 接收到的字节
  */
uint8_t IICx_RecvByte(IIC_PORT Port)
{
  uint8_t i = 0;
  uint8_t cRecvData = 0;

  IICx_SDA[Port] = 1;   //凡是读取操作前,都要释放总线,否则从机无法操作总线
  
  for (i = 0; i < 8; i++)
  {
    IICx_SCL[Port] = 0;
    IIC_Delay();
    
    IICx_SCL[Port] = 1;
    
    if (IICx_R_SDA[Port])
    {
      cRecvData |=  (0X1<<(7-i));
    }
      
    IIC_Delay();
    
    IICx_SCL[Port] = 0;
  }
  
  return cRecvData;
}



/**
  * @brief  IIC1等待应答信号(在SCL为高时等待应答,SDA变低为应答)
  * @param  无
  * @retval 无
  */
void IICx_WaitAck(IIC_PORT Port)
{
  uint16_t nTime = 0;
  
  IICx_SDA[Port] = 1;  //释放总线
  
  IICx_SCL[Port] = 0;
  IIC_Delay();
  
  IICx_SCL[Port] = 1;
  
  while (IICx_R_SDA[Port])  //等待SDA变低
  {
    DelayUs(10);
    nTime++;
    if (nTime == 300)   //超时操作,避免出错时陷入死循环
    {
      break;
    }
  }
  
  IIC_Delay();
  
  IICx_SCL[Port] = 0;
  
}



/**
  * @brief  IIC1给应答信号(在SCL为低时SDA给个低电平)
  * @param  无
  * @retval 无
  */
void IICx_GiveAck(IIC_PORT Port)
{
  IICx_SCL[Port] = 0;
  IICx_SDA[Port] = 0;  //应答信号
  IIC_Delay();
  
  IICx_SCL[Port] = 1;
  IIC_Delay();
  
  IICx_SCL[Port] = 0;
  
}



/**
  * @brief  IIC1给不应答信号(在SCL为低时SDA给个高电平)
  * @param  无
  * @retval 无
  */
void IICx_GiveNoAck(IIC_PORT Port)
{
  IICx_SCL[Port] = 0;
  IICx_SDA[Port] = 1;  //不应答信号
  IIC_Delay();
  
  IICx_SCL[Port] = 1;
  IIC_Delay();
  
  IICx_SCL[Port] = 0;
  
}





