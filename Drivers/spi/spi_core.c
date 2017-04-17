
#include "spi.h"
#include "SourceLib.h"


#if USING_SPI

/*----------------------------------------------------------------------------
    ���ⲿ�ṩ��SPI_Transfer ������һ��ķ�װ
    ������Ķ����� SL_SPI_BUS_DEV_TYPE 
 *----------------------------------------------------------------------------*/
sl_err_t SPI_Core_Init(void)
{
  
  return SL_EOK;
}



void SPI_Core_SendByte(SL_SPI_BUS_DEV_TYPE *Bus, uint16_t nAddr, uint8_t cData)
{
  SL_SPI_BUS_BASE_OPS_TYPE *BaseOps = Bus->Priv;  //��ȡ�ײ��������
  
  BaseOps->SPI_SetNss(0);
  
  
  
  
  BaseOps->SPI_SetNss(1);
  
}


uint32_t SPI_Core_ReadByte(SL_SPI_BUS_DEV_TYPE *Bus, uint16_t nAddr)
{
  
  
}











//���ֽڷ��ͺ���
//�� SL_SPI_BUS_BASE_OPS_TYPE��Ӧ��,ʵ���ֽڷ��͵�ʱ��
uint32_t SPI_Core_SendBuff(SL_SPI_BUS_DEV_TYPE *Bus, const uint8_t *pBuff, uint32_t iCount)
{
  uint32_t i = 0;
  
  for (i = 0; i < iCount; i++)
  {
    Bus->Ops->SPI_Transfer(Bus, pBuff[i]);
  }
  
  return 0;
}


//���ֽڽ��պ���
//�� SL_SPI_BUS_BASE_OPS_TYPE��Ӧ��,ʵ���ֽڽ��յ�ʱ��
uint32_t SPI_Core_RecvBuff(SL_SPI_BUS_DEV_TYPE *Bus, uint8_t *pBuff, uint32_t iCount)
{
  uint32_t i = 0;
  
  for (i = 0; i < iCount; i++)
  {
    pBuff[i] = Bus->Ops->SPI_Transfer(Bus, 0xFF);
  }
  
  return 0;
}



#endif /* USING_SPI */







