/**
  ******************************************************************************
  * @file    i2c_bit_ops.c
  * @author  杜公子寒枫
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C框架:位操作实现文件
  ******************************************************************************
  * @attention
  * 
  * 对象: SL_I2C_BIT_OPS_TYPE
  * 
  * typedef struct sl_i2c_bit_ops
  * {
  *   void    (*Data);    //私有数据 
  *   void    (*SetSDA)(void *Data, uint8_t State);
  *   void    (*SetSCL)(void *Data, uint8_t State);
  *   uint8_t (*GetSDA)(void *Data);
  *   uint8_t (*GetSCL)(void *Data);
  * 
  *   void    (*BitDelayUs)(uint16_t Us);
  *   
  *   uint16_t DelayTime; //单位: 微秒 
  *   uint32_t iTimeOut;  //单位: 毫秒
  *   
  * }SL_I2C_BIT_OPS_TYPE;
  * 
  * 本文件主要是通过BIT_OPS来实现基本IIC时序: 
  * I2C_Start   
  * I2C_Stop    
  * I2C_SendAckOrNack 
  * I2C_WaitAck  
  * I2C_SendByte 
  * I2C_RecvByte 
  * 
  * 上面的时序接口函数可封装成结构体SL_I2C_BUS_BASE_OPS_TYPE,并交由I2C总线设备结构体
  * 管理及调用.
  * 
  * 
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "SourceLib.h"
#include "i2c.h"

#if USING_I2C

#define SET_SDA(Ops, Value)   Ops->SetSDA(Ops->Data, Value)
#define SET_SCL(Ops, Value)   Ops->SetSCL(Ops->Data, Value)
#define GET_SDA(Ops)          Ops->GetSDA(Ops->Data)
#define GET_SCL(Ops)          Ops->GetSCL(Ops->Data)
  
#define SDA_L(Ops)            SET_SDA(Ops, 0)
#define SDA_H(Ops)            SET_SDA(Ops, 1)
#define SCL_L(Ops)            SET_SCL(Ops, 0)
#define SCL_H(Ops)            SET_SCL(Ops, 1)


Sl_inline void I2C_DelayUs(SL_I2C_BIT_OPS_TYPE *Ops)
{
    Ops->BitDelayUs(Ops->DelayTime);
  
}

/*----------------------------------------------------------------------------
    底层操作函数,以实现最基本的I2C时序逻辑,直接对IO进行控制
    其操作的对象是 SL_I2C_BIT_OPS_TYPE 
 *----------------------------------------------------------------------------*/

/**
  * @brief  I2C开始标志(在SCL高电平时拉低SDA)
  * @param  无
  * @retval 无
  */
static void I2C_Bit_Start(SL_I2C_BIT_OPS_TYPE *Ops)
{
  SDA_H(Ops);
  SCL_H(Ops);
  I2C_DelayUs(Ops);
  SDA_L(Ops);
  I2C_DelayUs(Ops);
  SCL_L(Ops);
}



/**
  * @brief  I2C结束标志(在SCL高电平时拉高SDA)
  * @param  无
  * @retval 无
  */
static void I2C_Bit_Stop(SL_I2C_BIT_OPS_TYPE *Ops)
{
  SCL_L(Ops);
  SDA_L(Ops);
  
  SCL_H(Ops);
  I2C_DelayUs(Ops);
  SDA_H(Ops);
  I2C_DelayUs(Ops);
}



/**
  * @brief  I2C等待应答信号(在SCL为高时等待应答,SDA变低为应答)
  * @param  无
  * @retval isAck 是否有应答信号,有则返回1,无则返回0
  */
static uint8_t I2C_Bit_WaitAck(SL_I2C_BIT_OPS_TYPE *Ops)
{
  uint8_t isAck = 1;
  SLTimerType I2C_Timer = {0};
  
  SDA_H(Ops); //释放总线
  SCL_L(Ops);
  I2C_DelayUs(Ops);
  SCL_H(Ops);
  
  SLTimer_Start(&I2C_Timer, Ops->iTimeOut);  //设置超时时间
  while (GET_SDA(Ops)) //等待SDA变低
  {
    if (SLTimer_GetExpiredState(&I2C_Timer))
    {
      isAck = 0;
      break;
    }
    I2C_DelayUs(Ops);
  }
  
  I2C_DelayUs(Ops);
  SCL_L(Ops);
  
  return isAck;
}



/**
  * @brief  I2C给应答/不应答信号(在SCL为低时SDA给个低电平即为应答信号)
  * @param  无
  * @retval 无
  */
static void I2C_Bit_SendAckOrNack(SL_I2C_BIT_OPS_TYPE *Ops, uint8_t ack)
{
  SCL_L(Ops);
  
  if (ack) 
  {
    SDA_L(Ops);  //应答信号
  }
  else 
  {
    SDA_H(Ops);  //应答信号
  }
  
  I2C_DelayUs(Ops);
  
  SCL_H(Ops);
  I2C_DelayUs(Ops);
  
  SCL_L(Ops);
  
}



/**
  * @brief  I2C字节发送函数(在SCL低电平时拉高/拉低SDA)
  * @param  cSendData 要发送的字节
  * @retval 无
  */
static uint8_t I2C_Bit_SendByte(SL_I2C_BIT_OPS_TYPE *Ops, uint8_t cByte)
{
  uint8_t i = 0;
  
  for (i = 0; i < 8; i++)
  {
    SCL_L(Ops);
    
    if (cByte & (0X1<<(7-i)))  //发送数据
    {
      SDA_H(Ops);
    }
    else
    {
      SDA_L(Ops);
    }
    
    I2C_DelayUs(Ops);
    
    SCL_H(Ops);
    I2C_DelayUs(Ops);
    
  }
  SCL_L(Ops);
  
  return I2C_Bit_WaitAck(Ops);
}


/**
  * @brief  I2C字节接收函数(在SCL高电平时读SDA电平状态)
  * @param  无
  * @retval cRecvData 接收到的字节
  */
static uint8_t I2C_Bit_RecvByte(SL_I2C_BIT_OPS_TYPE *Ops)
{
  uint8_t i = 0;
  uint8_t cRecvData = 0;

  SDA_H(Ops);   //凡是读取操作前,都要释放总线,否则从机无法操作总线
  
  for (i = 0; i < 8; i++)
  {
    SCL_L(Ops);
    I2C_DelayUs(Ops);
    
    SCL_H(Ops);
    
    if (GET_SDA(Ops))
    {
      cRecvData |=  (0X1<<(7-i));
    }
      
    I2C_DelayUs(Ops);
  }
  SCL_L(Ops);
  
  return cRecvData;
}



//封装上方的I2C基本控制时序的接口函数
static SL_I2C_BUS_BASE_OPS_TYPE I2C_BusBaseOps = 
{
  I2C_Bit_Start,
  I2C_Bit_Stop,
  I2C_Bit_WaitAck,
  I2C_Bit_SendAckOrNack,
  I2C_Bit_SendByte,
  I2C_Bit_RecvByte,
  0,
};



//获取I2C基本时序函数的接口(交由设备层调用)
sl_err_t SL_I2C_Bit_GetBitOpsInterfaces(SL_I2C_BUS_DEV_TYPE *Bus)
{
  Bus->Priv_BaseOps = &I2C_BusBaseOps;  //获取其位操作指针
  
  return SL_EOK;
}

#endif /* USING_I2C */
