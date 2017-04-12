/**
  ******************************************************************************
  * @file    i2c_dev.c
  * @author  杜公子寒枫
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C框架:应用层实现文件
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

//读I2C总线设备
static sl_size_t I2C_BusDeviceRead(SL_DEVICE_TYPE *Dev, sl_off_t Offset, void *Buff, sl_size_t Count)
{
  SL_I2C_BUS_DEV_TYPE *Bus = (SL_I2C_BUS_DEV_TYPE *)Dev->UserData;
  uint16_t nRegAddr = 0;
  
  SL_ASSERT(Bus != SL_NULL);
  SL_ASSERT(Buff != SL_NULL);
  SL_ASSERT(Bus->Priv_BitOps != SL_NULL);
  
  nRegAddr = Offset & 0xFFFF;
//  Bus->nDevAddr = (Offset >> 16) & 0xFFFF;
  
  if (Bus->Ops->I2C_Master_RecvBuff(Bus, nRegAddr, Buff, Count) != SL_EOK)
  {
    return 0;
  }

  return Count;
}


//写I2C总线设备
static sl_size_t I2C_BusDeviceWrite(SL_DEVICE_TYPE *Dev, sl_off_t Offset, const void *Buff, sl_size_t Count)
{
  SL_I2C_BUS_DEV_TYPE *Bus = (SL_I2C_BUS_DEV_TYPE *)Dev->UserData;
  uint16_t nRegAddr = 0;
  
  SL_ASSERT(Bus != SL_NULL);
  SL_ASSERT(Buff != SL_NULL);
  SL_ASSERT(Bus->Priv_BitOps != SL_NULL);
  
  nRegAddr = Offset & 0xFFFF;
//  Bus->nDevAddr = (Offset >> 16) & 0xFFFF;
  
  if (Bus->Ops->I2C_Master_SendBuff(Bus, nRegAddr, Buff, Count) != SL_EOK)
  {
    return 0;
  }

  return Count;
}


//控制I2C总线的相关配置
static sl_err_t I2C_BusDeviceControl(SL_DEVICE_TYPE *Dev, uint32_t Cmd, void *args)
{
  SL_I2C_BUS_DEV_TYPE *Bus = (SL_I2C_BUS_DEV_TYPE *)Dev->UserData;
  
  switch (Cmd)
  {
    //清除所有的标志位
    case SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS:
    {
      Bus->nFlags = 0;
      break;
    }
    
    //8位设备地址字长(清除16位的标志)
    case SL_I2C_DEV_CTRL_DEV_ADDR_8BIT:
    {
      Bus->nFlags &= ~SL_I2C_DEV_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_DEV_ADDR;
      break;
    }
    
    //16位设备地址字长
    case SL_I2C_DEV_CTRL_DEV_ADDR_16BIT:
    {
      Bus->nFlags |=  SL_I2C_DEV_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_DEV_ADDR;
      break;
    }
    
    //无设备地址
    case SL_I2C_DEV_CTRL_NO_DEV_ADDR:
    {
      Bus->nFlags |=  SL_I2C_NO_DEV_ADDR;
      break;
    }
    
    //8位寄存器地址字长(清除16位的标志)
    case SL_I2C_DEV_CTRL_REG_ADDR_8BIT:
    {
      Bus->nFlags &= ~SL_I2C_REG_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_REG_ADDR;
      break;
    }
    
    //16位寄存器地址字长
    case SL_I2C_DEV_CTRL_REG_ADDR_16BIT:
    {
      Bus->nFlags |=  SL_I2C_REG_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_REG_ADDR;
      break;
    }
    
    //无寄存器地址
    case SL_I2C_DEV_CTRL_NO_REG_ADDR:
    {
      Bus->nFlags |=  SL_I2C_NO_REG_ADDR;
      break;
    }
    
    //设置超时时间
    case SL_I2C_DEV_CTRL_SET_TIMEOUT:
    {
      Bus->iTimeOut = *(uint32_t *)args;
      ((SL_I2C_BIT_OPS_TYPE*)Bus->Priv_BitOps)->iTimeOut = Bus->iTimeOut;
      break;
    }
    
    //设置重发次数
    case SL_I2C_DEV_CTRL_SET_RETRIES:
    {
      Bus->iRetries = *(uint32_t *)args;
      break;
    }
    
    //设置设备地址
    case SL_I2C_DEV_CTRL_SET_DEV_ADDR:
    {
      Bus->nDevAddr = *(uint16_t *)args;
      break;
    }
    
    //设置BIT_OPS的指针
    case SL_I2C_DEV_CTRL_SET_BIT_OPS:
    {
      Bus->Priv_BitOps = (SL_I2C_BIT_OPS_TYPE *)args;
      break;
    }
    
    default: break;
  }
  
  return SL_EOK;
}


extern sl_err_t SL_I2C_Bit_GetBitOpsInterfaces(SL_I2C_BUS_DEV_TYPE *Bus);
extern sl_err_t SL_I2C_Core_GetDeviceOpsInterfaces(SL_I2C_BUS_DEV_TYPE *Bus);

//I2C总线设备初始化
sl_err_t SL_I2C_BusDevieInit(SL_I2C_BUS_DEV_TYPE *Bus, const char *name)
{
  struct sl_device *Device;
  SL_ASSERT(Bus != SL_NULL);

  Device = &Bus->Parent;

  Device->UserData = Bus;

  /* set device type */
  Device->type    = SL_Device_Class_I2CBUS;

  /* initialize device interface */
  Device->Init    = SL_NULL;
  Device->Open    = SL_NULL;
  Device->Close   = SL_NULL;
  Device->Read    = I2C_BusDeviceRead;
  Device->Write   = I2C_BusDeviceWrite;
  Device->Control = I2C_BusDeviceControl;
  
  SL_I2C_Bit_GetBitOpsInterfaces(Bus);
  SL_I2C_Core_GetDeviceOpsInterfaces(Bus);

  return SL_EOK;
}

#endif /* USING_I2C */
