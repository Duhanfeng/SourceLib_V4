/**
  ******************************************************************************
  * @file    at24c02.c
  * @author  �Ź��Ӻ���
  * @version V2.0 
  * @date    2015.12.24
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * �ڴ�: 1/2/4/8/16K (AT24C01/02/04/08/16)
  * ����: ��λ�ȷ� 
  *
  * A2A1A0: ���ļ�Ĭ��оƬA2A1A0λ�ӵ�,��ʵ����˲�ͬ,���޸ĺ� AT_BASE_ADDR
  *
  * ����AT24CxxоƬ��������,AT24Cxx���ܽ��п�ҳ(ÿ8/16���ֽ�Ϊ1ҳ)д����,������
  * ���˿�ҳд���㷨.
  * 
  * ע: 1K/2K�ڴ��оƬ, ÿ8���ֽ�Ϊ1ҳ. ��4/8/16K��, 16���ֽ�Ϊ1ҳ.
  * 
  * V2.0------------
  * �޸�����: 1.��AT24Cxx�ĵײ�ʱ��ʵ�ֽ���I2C���ȥ���,������ֻ��Ҫ�����书��
  *             ��ʵ��.����صĽӿ������ĺ���ָ���ṩ
  *           2.�޸�AT24Cxx�޷�����������256���ֽڵ�����(�޿�ҳ����).���һ����
  *             ��ȡ����256���ֽ�,�����ִ��
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2017.03.20
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_at24cxx.h"
#include "i2c.h"
#include "SourceLib.h"


/* ---�豸��ַ--- */
#define AT_BASE_ADDR      (0XA0)

static SL_I2C_BIT_OPS_TYPE I2C_BitOps = {0};
static SL_I2C_BUS_DEV_TYPE AT24Cxx_I2C_BUS = {0};
static SL_DEVICE_TYPE *AT_I2C_DEV = &AT24Cxx_I2C_BUS.Parent;

static AT24Cxx_PAGE_SIZE AT_PAGE_SIZE;


//��ʼ��I2C�����豸
static void AT24Cxx_DeviceInit(void)
{
  SL_I2C_BusDevieInit(&AT24Cxx_I2C_BUS, "AT24Cxx");
  SL_Device_Open(AT_I2C_DEV, 0);
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS, SL_NULL);
  
}



/**
  * @brief  �����ⲿ���ƽӿ�
  * @param  AT_PortInit �˿ڳ�ʼ������
  * @param  SetSDA ����SDA�ĵ�ƽ
  * @param  SetSCL ����SCL�ĵ�ƽ
  * @param  GetSDA ��ȡSDA�ĵ�ƽ
  * @param  GetSCL ��ȡSCL�ĵ�ƽ
  * @param  BitDelayUs ΢�뼶��ʱ����
  * @retval ��
  * @note  ֵ��ע�����, SL_Device_Control ����Ҫ�� AT24Cxx_DeviceInit ����֮��ִ��,��Ϊ���豸��
  *  ʼ������ִ��֮ǰ,�豸����еĺ���ָ����δ���豸�ҽ�,�����������, SL_Device_Control ָ��NULL,
  *  ��ʱ���޷��� I2C_BitOps ��ָ����ȷ�ش��ݸ�BUS����,��BUS�������� I2C_BitOps ��ָ��(Ұָ��)ʱ,
  *  �ᵼ�´����ָ����ת,�Ӷ���ϵͳ�ܷ�.....
  */
void AT24Cxx_SetHwCtrlInterFaces( void    (*AT_PortInit)(void),
                                  void    (*SetSDA)(void *Data, uint8_t State),
                                  void    (*SetSCL)(void *Data, uint8_t State),
                                  uint8_t (*GetSDA)(void *Data),
                                  uint8_t (*GetSCL)(void *Data),
                                  void    (*BitDelayUs)(uint16_t Us),
                                  AT24Cxx_PAGE_SIZE    PAGE_SIZE)
{
  I2C_BitOps.SetSDA = SetSDA;
  I2C_BitOps.SetSCL = SetSCL;
  I2C_BitOps.GetSDA = GetSDA;
  I2C_BitOps.GetSCL = GetSCL;
  I2C_BitOps.BitDelayUs = BitDelayUs;
  I2C_BitOps.DelayTime = 5; //5Us
  I2C_BitOps.iTimeOut = 100; //100MS

  AT_PortInit();
  AT24Cxx_DeviceInit();
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_BIT_OPS, &I2C_BitOps);
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_TIMEOUT, &I2C_BitOps.iTimeOut);
  
  AT_PAGE_SIZE = PAGE_SIZE;
  
  //�����32KBit���ϵ�EEPROM,������Ϊ16λ�Ĵ�����ַ
  if (AT_PAGE_SIZE >= AT24C32_PAGE_SIZE)
  {
    SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_REG_ADDR_16BIT, SL_NULL);
  }
  
  
}



/**
  * @brief  AT24Cxx�ֽ�д����
  * @param  cAddr Ҫд�ĵ�ַ
  * @param  cWriteData Ҫд������
  * @retval ��
  */
void AT24Cxx_WriteByte(uint16_t nAddr, uint8_t cWriteData)
{
  uint16_t cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Write(AT_I2C_DEV, nAddr, &cWriteData, 1);
  
  Delay_ms(5);   //д����ʱ,����Ҫ�ȴ�5ms,�ȴ����ݴӻ�������AT24Cxxд�����
}



/**
  * @brief  AT24Cxx���������
  * @param  cAddr Ҫ���ĵ�ַ
  * @retval cReadData ��ȡ��������
  */
uint8_t AT24Cxx_RandomRead(uint16_t nAddr)
{
  uint8_t cReadData = 0;
  uint16_t cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Read(AT_I2C_DEV, nAddr, &cReadData, 1);
  
  return cReadData;
}



/**
  * @brief  AT24Cxxҳд����,ֻ��дһҳ
  * @param  cAddr Ҫд�ĵ�ַ
  * @param  cNum  Ҫд������
  * @param  pWriteBuff Ҫд������(����/�ַ���/ָ�����ʽ)
  * @retval ��
  * @note   ����1K/2K��EEPROM, 8�ֽ�Ϊ1ҳ, ����4/8/16K��,16�ֽ�Ϊ֮1ҳ.
  *         ���ڿ�ҳ��д, ����ҳβ���л��ص�ҳ��
  */
void AT24Cxx_PageWrite(uint16_t nAddr, const uint8_t *pWriteBuff, uint8_t cNum)
{
  uint16_t cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Write(AT_I2C_DEV, nAddr, pWriteBuff, cNum);
  
  Delay_ms(5);   //д����ʱ,����Ҫ�ȴ�5ms,�ȴ����ݴӻ�������AT24Cxxд�����
  
}



/**
  * @brief  AT24Cx����������
  * @param  cAddr Ҫ���ĵ�ַ
  * @param  cNum  Ҫ��������
  * @param  pReadBuff Ҫ��������(����/ָ�����ʽ)
  * @retval ��
  * @note   һ�ζ�ȡ�����ݲ����Գ���256���ֽ�,�����Ҫ��ȡ���������,��Ҫ��������ȡ
  */
void AT24Cxx_SequentialRead(uint16_t nAddr, uint8_t *pReadBuff, uint16_t nNum)
{
  uint16_t cDevAddr = 0;
  
  nNum = (nNum > 256) ? 256 : nNum; //AT24Cxx ���ֻ��֧��256���ֽڵ���������
  
  cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Read(AT_I2C_DEV, nAddr, pReadBuff, nNum);
  
}



/**
  * @brief  AT24Cxx���ֽڿ�ҳ������
  * @param  cAddr Ҫ���ĵ�ַ
  * @param  cNum  Ҫ��������
  * @param  pReadBuff Ҫ��������(����/ָ�����ʽ)
  * @retval ��
  * @note   �Ƕ������������ķ�װ,���һ��Ҫ�������ݳ���256�ֽ�,�������ȡ
  */
void AT24Cxx_ReadMultiBytes(uint16_t nAddr, uint8_t *pReadBuff, uint32_t iNum)
{
  uint16_t i = 0;
  uint16_t nBloakNum = iNum/256;

  for (i = 0 ; i < nBloakNum; i++)
  {
    AT24Cxx_SequentialRead(nAddr+i*256, &pReadBuff[i*256], 256);
  }
  
  AT24Cxx_SequentialRead(nAddr+i*256, &pReadBuff[i*256], iNum%256);
  
}



/**
  * @brief  AT24Cxx���ֽڿ�ҳд����
  * @param  nAddr Ҫд�ĵ�ַ
  * @param  nNum  Ҫд������
  * @param  pWriteBuff Ҫд������(����/�ַ���/ָ�����ʽ)
  * @retval ��
  */
void AT24Cxx_WriteMultiBytes(uint16_t nAddr, const uint8_t *pWriteBuff, uint32_t iNum)
{
  uint8_t i = 0;
  uint16_t nSurplusNum = iNum;  // ʣ��δд�������
  uint16_t nCurrentPageNum = 0; // ��ǰ�׶���Ҫд���ҳ��
  
  uint16_t nCurrentAddr = nAddr;  // ��ǰ�����ĵ�ַ
  const uint8_t *pStoreBuff = pWriteBuff; // ��ǰ�洢�����ݻ�����
  uint16_t nCurrentWriteNum = 0;  // ��ǰ��Ҫд�������
  
  // ��һ�׶�
  if (nCurrentAddr & (AT_PAGE_SIZE - 1)) // ���統ǰ��ַ��δ��ҳ��ַ����
  {
    nCurrentWriteNum = AT_PAGE_SIZE - (nCurrentAddr & (AT_PAGE_SIZE - 1));
    
    if (nSurplusNum < nCurrentWriteNum) // ����ʣ����������ڵ�ǰ��д������
    {
      nCurrentWriteNum = nSurplusNum; 
    }
    
    AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nCurrentWriteNum);
    
    pStoreBuff   += nCurrentWriteNum; // ָ��ƫ��
    nSurplusNum  -= nCurrentWriteNum; // ����ʣ������ݵ�����
    nCurrentAddr += nCurrentWriteNum; // ���µ�ǰҪд�ĵ�ַ
  }
  
  // �ڶ��׶�
  nCurrentPageNum = nSurplusNum/AT_PAGE_SIZE;
  
  nCurrentWriteNum = AT_PAGE_SIZE;
  for (i = 0; i < nCurrentPageNum; i++)
  {
    AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nCurrentWriteNum);
    
    pStoreBuff   += nCurrentWriteNum; // ָ��ƫ��
    nSurplusNum  -= nCurrentWriteNum; // ����ʣ������ݵ�����
    nCurrentAddr += nCurrentWriteNum; // ���µ�ǰҪд�ĵ�ַ
  }
  
  // �����׶�
  if (nSurplusNum != 0)
  {
    AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nSurplusNum);
  }
  
}

