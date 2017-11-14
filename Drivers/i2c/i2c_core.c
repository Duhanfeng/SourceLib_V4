/**
  ******************************************************************************
  * @file    i2c_core.c
  * @author  Duhanfeng
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C框架:内核实现文件
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  * 
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

/*----------------------------------------------------------------------------
    对底层操作函数再一层的封装,以实现基本的协议通信时序
    其操作的对象是 SL_I2C_BUS_DEV_TYPE 
 *----------------------------------------------------------------------------*/
sl_err_t I2C_Core_Init(void)
{
  
  return SL_EOK;
}


//多字节发送函数
//对 SL_I2C_BUS_BASE_OPS_TYPE的应用,实现字节发送的时序
static uint32_t I2C_Core_SendBuff(SL_I2C_BUS_DEV_TYPE *Bus, uint16_t nFlags, uint16_t cDevAddr, 
                             uint16_t nRegAddr, const uint8_t *pBuff, uint32_t iCount)
{
  SL_I2C_BUS_BASE_OPS_TYPE *BaseOps = Bus->Priv_BaseOps;
  SL_I2C_BIT_OPS_TYPE *BitOps = Bus->Priv_BitOps;
  uint32_t i = 0;
  
  while (1)
  {
    BaseOps->I2C_Start(BitOps);
  
    if (!(nFlags & SL_I2C_NO_DEV_ADDR))
    {
      //发送设备地址
      if (nFlags & SL_I2C_DEV_ADDR_16BIT)
      {
        if (BaseOps->I2C_SendByte(BitOps, (cDevAddr>>8) & 0xFF) == 0) break;
      }
      if (BaseOps->I2C_SendByte(BitOps, (cDevAddr & 0xFF) | SL_I2C_WR) == 0) break;
    }
    
    //发送寄存器地址
    if (!(nFlags & SL_I2C_NO_REG_ADDR))
    {
      if (nFlags & SL_I2C_REG_ADDR_16BIT)
      {
        if (BaseOps->I2C_SendByte(BitOps, (nRegAddr>>8) & 0xFF) == 0) break;
      }
      if (BaseOps->I2C_SendByte(BitOps, nRegAddr& 0xFF) == 0) break;
    }
    
    for (i = 0; i < iCount; i++)
    {
      if (BaseOps->I2C_SendByte(BitOps, pBuff[i]) == 0) break;
    }
    
    BaseOps->I2C_Stop(BitOps);
    return iCount;
  }
  
  BaseOps->I2C_Stop(BitOps);
  return 0;
}


//多字节接收函数
//对 SL_I2C_BUS_BASE_OPS_TYPE的应用,实现字节接收的时序
static uint32_t I2C_Core_RecvBuff(SL_I2C_BUS_DEV_TYPE *Bus, uint16_t nFlags, uint16_t cDevAddr, 
                             uint16_t nRegAddr, uint8_t *pBuff, uint32_t iCount)
{
  SL_I2C_BUS_BASE_OPS_TYPE *BaseOps = Bus->Priv_BaseOps;
  SL_I2C_BIT_OPS_TYPE *BitOps = Bus->Priv_BitOps;
  uint32_t i = 0;
  
  while (1)
  {
    BaseOps->I2C_Start(BitOps);
    
    //发送设备地址
    if (nFlags & SL_I2C_DEV_ADDR_16BIT)
    {
      if (BaseOps->I2C_SendByte(BitOps, (cDevAddr>>8) & 0xFF) == 0) break;
    }
    if (BaseOps->I2C_SendByte(BitOps, (cDevAddr & 0xFF) | SL_I2C_WR) == 0) break;
    
    //发送寄存器地址
    if (nFlags & SL_I2C_REG_ADDR_16BIT)
    {
      if (BaseOps->I2C_SendByte(BitOps, (nRegAddr>>8) & 0xFF) == 0) break;
    }
    if (BaseOps->I2C_SendByte(BitOps, nRegAddr& 0xFF) == 0) break;
    
    BaseOps->I2C_Start(BitOps);
    
    //发送设备地址
    if (nFlags & SL_I2C_DEV_ADDR_16BIT)
    {
      if (BaseOps->I2C_SendByte(BitOps, (cDevAddr>>8) & 0xFF) == 0) break;
    }
    if (BaseOps->I2C_SendByte(BitOps, (cDevAddr & 0xFF) | SL_I2C_RD) == 0) break;
    
    //接收数据
    for (i = 0; i < iCount; i++)
    {
      pBuff[i] = BaseOps->I2C_RecvByte(BitOps);
      BaseOps->I2C_SendAckOrNack(BitOps, iCount - (i+1));  //最后1个字节给非应答信号
    }
    
    BaseOps->I2C_Stop(BitOps);
    
    return iCount;
  }
  
  BaseOps->I2C_Stop(BitOps);
  return 0;
}



/*----------------------------------------------------------------------------
    对内核函数的封装,增加其校验重发的功能
    其操作的对象是 I2C_Core_SendBuff 和 I2C_Core_RecvBuff
 *----------------------------------------------------------------------------*/

//主机发送函数
static sl_err_t I2C_MasterSend(SL_I2C_BUS_DEV_TYPE *Bus, 
                             uint16_t nRegAddr, const uint8_t *pBuff, uint32_t iCount)
{
  uint32_t iRetries = (Bus->nFlags & SL_I2C_IGNORE_NACK) ? 1 : ((Bus->iRetries==0) ? 1 : Bus->iRetries);
  uint32_t i = 0;
  uint32_t retValue = 0;
  sl_err_t Result = -SL_EIO;
  
  #if 0
  if (iCount == 0)
  {
    return SL_EOK;
  }
  #endif
  
  for (i = 0; i < iRetries; i++)
  {
    retValue = I2C_Core_SendBuff(Bus, Bus->nFlags, Bus->nDevAddr, nRegAddr, pBuff, iCount);
    
    if (retValue == iCount) 
    {
      Result = SL_EOK;
      break;
    }
  }
  
  if (Bus->nFlags & SL_I2C_IGNORE_NACK)
  {
    return SL_EOK;
  }
  
  return Result;
}


//主机接收函数
static sl_err_t I2C_MasterRecv(SL_I2C_BUS_DEV_TYPE *Bus, 
                             uint16_t nRegAddr, uint8_t *pBuff, uint32_t iCount)
{
  uint32_t iRetries = (Bus->nFlags & SL_I2C_IGNORE_NACK) ? 1 : ((Bus->iRetries==0) ? 1 : Bus->iRetries);
  uint32_t i = 0;
  uint32_t retValue = 0;
  sl_err_t Result = -SL_EIO;
  
  #if 0
  if (iCount == 0)
  {
    return SL_EOK;
  }
  #endif
  
  for (i = 0; i < iRetries; i++)
  {
    retValue = I2C_Core_RecvBuff(Bus, Bus->nFlags, Bus->nDevAddr, nRegAddr, pBuff, iCount);
    
    if (retValue == iCount) 
    {
      Result = SL_EOK;
      break;
    }
  }
  
  if (Bus->nFlags & SL_I2C_IGNORE_NACK)
  {
    return SL_EOK;
  }
  
  return Result;
}



//用结构体封装上方的总线操作接口
static SL_I2C_BUS_DEVICE_OPS_TYPE I2C_BusDeviceOps = 
{
  I2C_MasterSend,
  I2C_MasterRecv,
  SL_NULL,
};



//获取设备操作接口函数(交由设备层调用)
sl_err_t SL_I2C_Core_GetDeviceOpsInterfaces(SL_I2C_BUS_DEV_TYPE *Bus)
{
  Bus->Ops = &I2C_BusDeviceOps;
  
  return SL_EOK;
}


#endif /* USING_I2C */

