

#include "spi.h"
#include "SourceLib.h"


#if USING_SPI

extern uint32_t SPI_Core_SendBuff(SL_SPI_BUS_DEV_TYPE *Bus, const uint8_t *pBuff, uint32_t iCount);
extern uint32_t SPI_Core_RecvBuff(SL_SPI_BUS_DEV_TYPE *Bus, uint8_t *pBuff, uint32_t iCount);



//��SPI�����豸
static sl_size_t SPI_BusDeviceRead(SL_DEVICE_TYPE *Dev, sl_off_t Offset, void *Buff, sl_size_t Count)
{
  #if 0
  SL_SPI_BUS_DEV_TYPE *Bus = (SL_SPI_BUS_DEV_TYPE *)Dev->UserData;
  uint16_t nRegAddr = 0;
  
  SL_ASSERT(Bus != SL_NULL);
  SL_ASSERT(Buff != SL_NULL);
  
  
  #endif
  
  
  return Count;
}


//дSPI�����豸
static sl_size_t SPI_BusDeviceWrite(SL_DEVICE_TYPE *Dev, sl_off_t Offset, const void *Buff, sl_size_t Count)
{
  #if 0
  SL_SPI_BUS_DEV_TYPE *Bus = (SL_SPI_BUS_DEV_TYPE *)Dev->UserData;
  uint16_t nRegAddr = 0;
  
  SL_ASSERT(Bus != SL_NULL);
  SL_ASSERT(Buff != SL_NULL);
  #endif
  
  return Count;
}


//����SPI���ߵ��������
static sl_err_t SPI_BusDeviceControl(SL_DEVICE_TYPE *Dev, uint32_t Cmd, void *args)
{
  #if 0
  SL_SPI_BUS_DEV_TYPE *Bus = (SL_SPI_BUS_DEV_TYPE *)Dev->UserData;
  
  switch (Cmd)
  {
    
    default: break;
  }
  #endif
  return SL_EOK;
}


//SPI�����豸��ʼ��
sl_err_t SL_SPI_BusDevieInit(SL_SPI_BUS_DEV_TYPE *Bus, const char *name)
{
  struct sl_device *Device;
  SL_ASSERT(Bus != SL_NULL);

  Device = &Bus->Parent;

  Device->UserData = Bus;

  /* set device type */
  Device->type    = SL_Device_Class_SPIBUS;

  /* initialize device interface */
  Device->Init    = SL_NULL;
  Device->Open    = SL_NULL;
  Device->Close   = SL_NULL;
  Device->Read    = SPI_BusDeviceRead;
  Device->Write   = SPI_BusDeviceWrite;
  Device->Control = SPI_BusDeviceControl;
  
  return SL_EOK;
}


#endif /* USING_SPI */
