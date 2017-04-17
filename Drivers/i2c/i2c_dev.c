/**
  ******************************************************************************
  * @file    i2c_dev.c
  * @author  �Ź��Ӻ���
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C���:Ӧ�ò�ʵ���ļ�
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

//��I2C�����豸
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


//дI2C�����豸
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


//����I2C���ߵ��������
static sl_err_t I2C_BusDeviceControl(SL_DEVICE_TYPE *Dev, uint32_t Cmd, void *args)
{
  SL_I2C_BUS_DEV_TYPE *Bus = (SL_I2C_BUS_DEV_TYPE *)Dev->UserData;
  
  switch (Cmd)
  {
    //������еı�־λ
    case SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS:
    {
      Bus->nFlags = 0;
      break;
    }
    
    //8λ�豸��ַ�ֳ�(���16λ�ı�־)
    case SL_I2C_DEV_CTRL_DEV_ADDR_8BIT:
    {
      Bus->nFlags &= ~SL_I2C_DEV_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_DEV_ADDR;
      break;
    }
    
    //16λ�豸��ַ�ֳ�
    case SL_I2C_DEV_CTRL_DEV_ADDR_16BIT:
    {
      Bus->nFlags |=  SL_I2C_DEV_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_DEV_ADDR;
      break;
    }
    
    //���豸��ַ
    case SL_I2C_DEV_CTRL_NO_DEV_ADDR:
    {
      Bus->nFlags |=  SL_I2C_NO_DEV_ADDR;
      break;
    }
    
    //8λ�Ĵ�����ַ�ֳ�(���16λ�ı�־)
    case SL_I2C_DEV_CTRL_REG_ADDR_8BIT:
    {
      Bus->nFlags &= ~SL_I2C_REG_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_REG_ADDR;
      break;
    }
    
    //16λ�Ĵ�����ַ�ֳ�
    case SL_I2C_DEV_CTRL_REG_ADDR_16BIT:
    {
      Bus->nFlags |=  SL_I2C_REG_ADDR_16BIT;
      Bus->nFlags &= ~SL_I2C_NO_REG_ADDR;
      break;
    }
    
    //�޼Ĵ�����ַ
    case SL_I2C_DEV_CTRL_NO_REG_ADDR:
    {
      Bus->nFlags |=  SL_I2C_NO_REG_ADDR;
      break;
    }
    
    //���ó�ʱʱ��
    case SL_I2C_DEV_CTRL_SET_TIMEOUT:
    {
      Bus->iTimeOut = *(uint32_t *)args;
      ((SL_I2C_BIT_OPS_TYPE*)Bus->Priv_BitOps)->iTimeOut = Bus->iTimeOut;
      break;
    }
    
    //�����ط�����
    case SL_I2C_DEV_CTRL_SET_RETRIES:
    {
      Bus->iRetries = *(uint32_t *)args;
      break;
    }
    
    //�����豸��ַ
    case SL_I2C_DEV_CTRL_SET_DEV_ADDR:
    {
      Bus->nDevAddr = *(uint16_t *)args;
      break;
    }
    
    //����BIT_OPS��ָ��
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

//I2C�����豸��ʼ��
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
