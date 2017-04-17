/**
  ******************************************************************************
  * @file    sl_w25q64.c
  * @author  �Ź��Ӻ���
  * @version V2.1 �Ĵ����汾
  * @date    2016.07.14
  * @brief   W25Q654����
  ******************************************************************************
  * @attention
  * 
  * ����,
  *      FLASH_CS  --- SPI_NSS(����)
  *      FLASH_CLK --- SPI_SCK  
  *      FLASH_DO  --- SPI_MISO 
  *      FLASH_DI  --- SPI_MOSI 
  * 
  * �洢,
  *      1ҳ   --- 256 Byte
  *      1���� --- 4K  Byte
  * 
  * ����: 64M Bit(8M Byte)
  * 
  * ע1: W25Q64ֻ�ܽ���ҳ���,��һ�����ֻ����д256Byte,����һ�εĲ������ܿ�ҳ.Ϊ
  *      ���������,W25Q64_WriteMultiPage()�����㷨����ҳ��������в�ֳɶ��ҳ��
  *      ��д����.����������������ܵ��޷�������д������,�ʶ�����Կ�ҳд�ĺ�����
  *      ����һ��ķ�װ,��W25Q64_WriteMultiByte()����.�˺������Խ��п�����,������
  *      �Ĳ���,�޸�FLASHоƬ������λ�õ�����Ҳ����Ӱ�쵽����λ.
  * 
  * ע2: �����ŵķ�ת�ٶȲ���,��ᵼ�����ݵĶ�д����,����IO���ٶȶ�����Ϊ����ģʽ
  * 
  * 
  * V2.0------------
  * �޸�����: �޸��ļ�ͷ����,����SPI��ӿ�����ǿ�������,������Ƭ�����ĺ���;
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.07.13
  * 
  * V2.1------------
  * �޸�����: 1. �޸�W25Q64_WriteMultiByte()������BUG,��BUG���µ��Եڶ�������(��
  *              ַ:4096)��֮���������Ϊ�洢��ַʱ,�������������4K,��ᵼ������
  *              д����󲢴���ز����˵�һ��������������;
  *           2. �Ż�W25Q64_WriteMultiByte()���㷨,ȥ���������,�Խ�ʡCPU����;
  *              
  * ����ԭ��: ������ַ���㹫ʽΪ:iSectorAddr = (iAddr/(4*1024))*(4*1024); ��ԭ����
  *           ��һ�������ĺ�������ؽ�iSectorAddr = (iAddr/(4*1024)) ,���²����˵�
  *           һ���������ұ�����δ����,�����޷�д��ʧ��;
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.1
  * �޸�����: 2016.07.14
  *     
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "sl_w25q64.h"
#include "SourceLib.h"
#include <string.h>


static void    *PrivateData;
static uint8_t (*SPI_ReadWrite)(void *Data, uint8_t cWriteData);
static void    (*SPI_CS)(void *Data, uint8_t State);

#define SPI_CS_L()              SPI_CS(PrivateData, 0)
#define SPI_CS_H()              SPI_CS(PrivateData, 1)
#define SPI_ReadWriteByte(Data) SPI_ReadWrite(PrivateData, (Data))



/**
  * @brief  NRF24L01 Ӳ���ӿ�
  * @param  ��
  * @retval ��
  */
void W25Q64_HwCtrlInterFaces(void    (*W25Q64_PortInit)(void),
                             void     *Data,
                             uint8_t (*SPI_RW)(void *Data, uint8_t cWriteData),
                             void    (*SetCS)(void *Data, uint8_t State))
{
  PrivateData = Data;
  SPI_ReadWrite = SPI_RW;
  SPI_CS = SetCS;
  
  W25Q64_PortInit();
  
  /* ��֤�Ƿ��������� */
  while (W25Q64_ReadDeviceID() != 0XEF16);
  
}



/**
  * @brief  W25Q64дʹ��,����������,ҳ���ʱ����Ҫдʹ��
  * @param  None
  * @retval None
  */
static void W25Q64_WriteEnable(void)
{
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X06);
  
  SPI_CS_H();
  
}



/**
  * @brief  �ȴ���æ,д/����������,��ȴ�W25Q64��ɴӻ�����д���ڴ�Ĳ���.
  *         BUSY: 0--��æ  1--��æ  ��sta1���λ
  * @param  None
  * @retval None
  */
static void W25Q64_WaitNoBusy(void)
{
  uint8_t cStaReg = 0;
  
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X05);  //��ȡsta1
  while (1)
  {
    cStaReg = SPI_ReadWriteByte(0XFF);
    
    if (!(cStaReg & (1<<0)))
    {
      break;
    }
  }
  
  SPI_CS_H();
  
}



/**
  * @brief  W25Q64������������
  * @param  iAddr Ҫ������������ַ
  * @retval None
  */
static void W25Q64_SectorErase(uint32_t iAddr)
{
  /*дʹ��*/
  W25Q64_WriteEnable();
  
  /*����*/
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X20);
  SPI_ReadWriteByte((iAddr & 0XFF0000)>>16);
  SPI_ReadWriteByte((iAddr & 0X00FF00)>>8 );
  SPI_ReadWriteByte((iAddr & 0X0000FF)>>0 );
  
  SPI_CS_H();
  
  /*�ȴ��������*/
  W25Q64_WaitNoBusy();
  
}



/**
  * @brief  W25Q64ҳ���  1ҳ--256���ֽ�
  * @param  iAddr д��ĵ�ַ
  * @param  nNum  д�������,��ΧΪ1-256
  * @param  pWriteBuff Ҫд������� ������/�ַ���/ָ��ķ�ʽ���  
  * @retval None
  */
static void W25Q64_PageProgram(uint32_t iAddr, uint16_t nNum, const uint8_t *pWriteBuff)  //����д1ҳ--256���ֽ�
{
  uint16_t i = 0;
  
  /*дʹ��*/
  W25Q64_WriteEnable();
  
  /*д����*/
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X02);
  SPI_ReadWriteByte((iAddr & 0XFF0000)>>16);
  SPI_ReadWriteByte((iAddr & 0X00FF00)>>8 );
  SPI_ReadWriteByte((iAddr & 0X0000FF)>>0 );
  
  for (i = 0; i < nNum; i++)
  {
    SPI_ReadWriteByte(pWriteBuff[i]);
  }
  
  SPI_CS_H(); 

  /*�ȴ�д�����*/
  W25Q64_WaitNoBusy();
  
}



/**
  * @brief  �㷨: ʵ�ֿ�ҳд�Ķ��ֽ�д����,����Ϊ������д���㷨�Ļ���(ֻ����ͬ��������д)
  * @param  iAddr д��ĵ�ַ
  * @param  nNum  д�������,1--4096
  * @param  pWriteBuff Ҫд������� ������/�ַ���/ָ��ķ�ʽ���  
  * @retval None
  */
static void W25Q64_WriteMultiPage(uint32_t iAddr, uint32_t iNum, const uint8_t *pWriteBuff)
{
  uint32_t i = 0;
	uint32_t iPageNum = 0;         //�ڶ��׶���Ҫ��ҳ��
	uint32_t iSurplusNum = 0;      //��ǰҳʣ��Ŀɲ�������
  
  W25Q64_SectorErase(iAddr);
  
  /*��һ�׶�*/
  if (iNum <= (256-iAddr%256))   //��ǰҳ��д�����е�����
  {
    iSurplusNum = iNum;
    
    W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
    return;
  }
  else
  {
    iSurplusNum = (256-iAddr%256);  //��ǰҳ��д������
    
    W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
    
    iAddr += iSurplusNum; //��ַƫ��
    iNum  -= iSurplusNum; //ʣ��δд�������ƫ��
    pWriteBuff += iSurplusNum; //����ָ��ƫ��
  }
  
  /*�ڶ��׶�*/
  iPageNum = iNum/256;
  for (i = 0; i < iPageNum; i++)
  {
    iSurplusNum = 256;
    
    W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
    
    iAddr += iSurplusNum; //��ַƫ��
    iNum  -= iSurplusNum; //ʣ��δд�������ƫ��
    pWriteBuff += iSurplusNum; //����ָ��ƫ��
    
  }
  
  /*�����׶�*/
  iSurplusNum = iNum;
  W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
  
}



/**
  * @brief  ��������FLASHоƬ������
  * @param  None
  * @retval None
  * @note   ����оƬ�ֲ������,������ƬоƬ��Ҫ15S(type)-60S(max),���Խ��Ϊ20S
  */
void W25Q64_EraseChip(void)
{
  /*дʹ��*/
  W25Q64_WriteEnable();
  
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X60);
  
  SPI_CS_H();
  
  /*�ȴ��������*/
  W25Q64_WaitNoBusy();
  
}



/**
  * @brief  ��ȡW25Q64�豸ID,���ɹ���ȡ,��IDӦ��Ϊ0XEF16. 
  *         ������Ϊ��֤W25Q64�Ƿ���������������֤
  * @param  None
  * @retval nDeviceID �����������,���ص�ֵӦ����0XEF16
  */
uint16_t W25Q64_ReadDeviceID(void)
{
  uint16_t nDeviceID = 0;
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X90);
  SPI_ReadWriteByte(0);
  SPI_ReadWriteByte(0);
  SPI_ReadWriteByte(0);
  
  nDeviceID  = (SPI_ReadWriteByte(0XFF)<<8);  //�豸ID�ĸ߰�λ
  nDeviceID |=  SPI_ReadWriteByte(0XFF);      //��8λ
  
  SPI_CS_H();
  
  return nDeviceID;
}



/**
  * @brief  ���ֽڶ�����,������Ѱַ���еĵ�ַ,��ַ��Χ������������
  * @param  iAddr ��ȡ�ĵ�ַ
  * @param  nNum  ��ȡ������
  * @param  pWriteBuff ��ȡ�������� ������/ָ��ķ�ʽ����  
  * @retval None
  */
void W25Q64_ReadMultiByte(uint32_t iAddr, uint32_t iNum, uint8_t *pReadBuff)
{
  uint32_t i = 0;
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X03);
  SPI_ReadWriteByte((iAddr & 0XFF0000)>>16);
  SPI_ReadWriteByte((iAddr & 0X00FF00)>>8 );
  SPI_ReadWriteByte((iAddr & 0X0000FF)>>0 );
  
  for (i = 0; i < iNum; i++)
  {
    pReadBuff[i] = SPI_ReadWriteByte(0XFF);
  }
  
  SPI_CS_H();
}




/**
  * @brief  ���ֽ�д����,�����޸�FLASHоƬ������λ�õ����ݶ���Ӱ������λ.
  *         ���Խ��п�����,�������Ĳ���. ���ô˳�����Ҫִ��������������
  * @param  iAddr д��ĵ�ַ
  * @param  nNum  д�������,����ʵ��д��һ������
  * @param  pWriteBuff Ҫд������� ������/�ַ���/ָ��ķ�ʽ���  
  * @retval None
  * @note   const uint8_t * ָ��ָ���ַ�����ݲ��ܸı�,ָ��ָ��ĵ�ַ���Ըı�
  */
void W25Q64_WriteMultiByte(uint32_t iAddr, uint32_t iNum, const uint8_t *pWriteBuff)
{
  uint32_t i = 0;
  uint16_t nSectorNum = 0;    //��д��������
  uint32_t iSectorAddr = 0;   //������ַ(0/4096/8192...)
  uint32_t iStoreNum = 0;   //��ǰ����ʣ��Ŀɲ�������
  uint16_t nAddrOffset = 0;
  
  uint8_t szDataBuff[1024*4] = {0};
  
  ///////////////////////////// 1.x ��ǰ�����ܴ洢�����е����� /////////////////////////////
  if (iNum <= (4096-iAddr%4096))  //4096-iAddr%4096: ��iAddr������β���ĳ���
  {
    iSectorAddr = (iAddr/(4*1024))*(4*1024);  //���㵱ǰ������ַ
    nAddrOffset = iAddr%(4096);  
    
    W25Q64_ReadMultiByte(iSectorAddr,4*1024,szDataBuff);  //������������
    memcpy(&szDataBuff[nAddrOffset],pWriteBuff,iNum);     //�޸Ļ�����
    W25Q64_WriteMultiPage(iSectorAddr,1024*4,szDataBuff); //��������д������
  }
  
  
  /////////////////////////// 2.x ��Ҫ����������ܴ洢��ǰ������ ///////////////////////////
  else 
  {
    /* 2.1 �ӵ�ǰ��ַ������β�����д洢 */
    iSectorAddr = (iAddr/(4*1024))*(4*1024);  //������ʼ��ַ
    nAddrOffset = iAddr%(4096);               //�洢��ַ���������ַ��ƫ��
    iStoreNum   = 4096 - nAddrOffset;         //����Ҫ�洢��������
    
    W25Q64_ReadMultiByte(iSectorAddr,nAddrOffset,szDataBuff);  //������������
    memcpy(&szDataBuff[nAddrOffset],pWriteBuff,iStoreNum);     //�޸Ļ�����
    W25Q64_WriteMultiPage(iSectorAddr,1024*4,szDataBuff);      //��������д������
    
    iNum  -= iStoreNum;       //ʣ����������
    iAddr += iStoreNum;       //�´δ洢�ĵ�ַ
    pWriteBuff += iStoreNum;  //ָ��δ�洢������
    
    /* 2.2 ʣ������>4K,�������������� */
    nSectorNum = iNum/(4*1024);   //Ҫд����������
    iStoreNum  = 4*1024;    //Ҫ�洢��������
    
    for (i = 0; i < nSectorNum; i++)
    {
      iSectorAddr = (iAddr/(4*1024))*(4*1024);  //������ʼ��ַ
      
      
      W25Q64_SectorErase(iSectorAddr);
      W25Q64_WriteMultiPage(iSectorAddr,1024*4,pWriteBuff); //���������ݻ�����д�뵽������
      
      iNum  -= iStoreNum;       //ʣ����������
      iAddr += iStoreNum;       //�´δ洢�ĵ�ַ
      pWriteBuff += iStoreNum;  //ָ��δ�洢������
      
    }
    
    /* 2.3 ʣ������<4K,�洢�������׵�������ַ */
    iSectorAddr = (iAddr/(4*1024))*(4*1024);  //������ʼ��ַ
    iStoreNum  = iNum;          //����Ҫ�洢��������
    
    W25Q64_ReadMultiByte(iSectorAddr+iStoreNum,4096-iStoreNum,&szDataBuff[iStoreNum]);  //������������    
    memcpy(szDataBuff,pWriteBuff,iStoreNum);                //�޸Ļ�����  
    W25Q64_WriteMultiPage(iSectorAddr,1024*4,szDataBuff);   //��������д������
  }
  
}





