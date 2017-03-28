/**
  ******************************************************************************
  * @file    i2c_core.c
  * @author  �Ź��Ӻ���
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C���:�ں�ʵ���ļ�
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
    �Եײ����������һ��ķ�װ,��ʵ�ֻ�����Э��ͨ��ʱ��
    ������Ķ����� SL_I2C_BUS_DEV_TYPE 
 *----------------------------------------------------------------------------*/
sl_err_t I2C_Core_Init(void)
{
  
  return SL_EOK;
}


//���ֽڷ��ͺ���
//�� SL_I2C_BUS_BASE_OPS_TYPE��Ӧ��,ʵ���ֽڷ��͵�ʱ��
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
      //�����豸��ַ
      if (nFlags & SL_I2C_DEV_ADDR_16BIT)
      {
        if (BaseOps->I2C_SendByte(BitOps, (cDevAddr>>8) & 0xFF) == 0) break;
      }
      if (BaseOps->I2C_SendByte(BitOps, (cDevAddr & 0xFF) | SL_I2C_WR) == 0) break;
    }
    
    //���ͼĴ�����ַ
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


//���ֽڽ��պ���
//�� SL_I2C_BUS_BASE_OPS_TYPE��Ӧ��,ʵ���ֽڽ��յ�ʱ��
static uint32_t I2C_Core_RecvBuff(SL_I2C_BUS_DEV_TYPE *Bus, uint16_t nFlags, uint16_t cDevAddr, 
                             uint16_t nRegAddr, uint8_t *pBuff, uint32_t iCount)
{
  SL_I2C_BUS_BASE_OPS_TYPE *BaseOps = Bus->Priv_BaseOps;
  SL_I2C_BIT_OPS_TYPE *BitOps = Bus->Priv_BitOps;
  uint32_t i = 0;
  
  while (1)
  {
    BaseOps->I2C_Start(BitOps);
    
    //�����豸��ַ
    if (nFlags & SL_I2C_DEV_ADDR_16BIT)
    {
      if (BaseOps->I2C_SendByte(BitOps, (cDevAddr>>8) & 0xFF) == 0) break;
    }
    if (BaseOps->I2C_SendByte(BitOps, (cDevAddr & 0xFF) | SL_I2C_WR) == 0) break;
    
    //���ͼĴ�����ַ
    if (nFlags & SL_I2C_REG_ADDR_16BIT)
    {
      if (BaseOps->I2C_SendByte(BitOps, (nRegAddr>>8) & 0xFF) == 0) break;
    }
    if (BaseOps->I2C_SendByte(BitOps, nRegAddr& 0xFF) == 0) break;
    
    BaseOps->I2C_Start(BitOps);
    
    //�����豸��ַ
    if (nFlags & SL_I2C_DEV_ADDR_16BIT)
    {
      if (BaseOps->I2C_SendByte(BitOps, (cDevAddr>>8) & 0xFF) == 0) break;
    }
    if (BaseOps->I2C_SendByte(BitOps, (cDevAddr & 0xFF) | SL_I2C_RD) == 0) break;
    
    //��������
    for (i = 0; i < iCount; i++)
    {
      pBuff[i] = BaseOps->I2C_RecvByte(BitOps);
      BaseOps->I2C_SendAckOrNack(BitOps, iCount - (i+1));  //���1���ֽڸ���Ӧ���ź�
    }
    
    BaseOps->I2C_Stop(BitOps);
    
    return iCount;
  }
  
  BaseOps->I2C_Stop(BitOps);
  return 0;
}



/*----------------------------------------------------------------------------
    ���ں˺����ķ�װ,������У���ط��Ĺ���
    ������Ķ����� I2C_Core_SendBuff �� I2C_Core_RecvBuff
 *----------------------------------------------------------------------------*/

//�������ͺ���
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


//�������պ���
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



//�ýṹ���װ�Ϸ������߲����ӿ�
static SL_I2C_BUS_DEVICE_OPS_TYPE I2C_BusDeviceOps = 
{
  I2C_MasterSend,
  I2C_MasterRecv,
  SL_NULL,
};



//��ȡ�豸�����ӿں���(�����豸�����)
sl_err_t SL_I2C_Core_GetDeviceOpsInterfaces(SL_I2C_BUS_DEV_TYPE *Bus)
{
  Bus->Ops = &I2C_BusDeviceOps;
  
  return SL_EOK;
}


#endif /* USING_I2C */

