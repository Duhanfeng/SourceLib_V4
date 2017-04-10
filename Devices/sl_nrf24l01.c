/**
  ******************************************************************************
  * @file    sl_nrf24l01.c
  * @author  �Ź��Ӻ���
  * @version V1.0
  * @date    2017.04.09
  * @brief   control nrf24l01
  ******************************************************************************
  * @attention
  * 
  * ����nrf24l01ͨ�ţ���Ҫ����3��������ͬ��
  * 1.Ƶ����ͬ������Ƶ���Ĵ���RF_CH��
  * 2.��ַ��ͬ������TX_ADDR��RX_ADDR_P0��ͬ��
  * 3.ÿ�η��ͽ��յ��ֽ�����ͬ�����������ͨ������Ч���ݿ��Ϊn����ôÿ�η��͵��ֽ���Ҳ����Ϊn����Ȼ��n<=32��
  * 
  * 
  * 
  *     
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_nrf24l01.h"
#include "SourceLib.h"


static void    *PrivateData;

static uint8_t (*SPI_NRF_RW)(void *Data, uint8_t cWriteData);
static void    (*NRF_CSN)(void *Data, uint8_t State);
static void    (*NRF_CE)(void *Data, uint8_t State);
static void    (*NRF_DelayUs)(uint16_t Us);

static uint8_t NRF24L01_Flag = 0;

uint8_t NRF_RX_BUF[RX_PLOAD_WIDTH];   //�������ݻ���
uint8_t NRF_TX_BUF[TX_PLOAD_WIDTH];   //�������ݻ���
uint8_t NRF_TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // ����һ����̬���͵�ַ
uint8_t NRF_RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};




/**
  * @brief ������NRF�ض��ļĴ���д������
  * @param reg:NRF������+�Ĵ�����ַ
  * @param dat:��Ҫ��Ĵ���д�������
  * @retval  NRF��status�Ĵ�����״̬
  */
static uint8_t nRF24L01_WriteReg(uint8_t reg,uint8_t dat)
{
  uint8_t status = 0;
  NRF_CE(PrivateData, 0);

  /*�õ�CSN��ʹ��SPI����*/
  NRF_CSN(PrivateData, 0);
      
  /*��������Ĵ����� */
  status = SPI_NRF_RW(PrivateData, reg);
   
  /*��Ĵ���д������*/
  SPI_NRF_RW(PrivateData, dat); 
          
  /*CSN���ߣ����*/     
  NRF_CSN(PrivateData, 1);  

  /*����״̬�Ĵ�����ֵ*/
  return(status);
}



/**
  * @brief ���ڴ�NRF�ض��ļĴ�����������
  * @param reg:NRF������+�Ĵ�����ַ
  * @retval  �Ĵ����е�����
  */
static uint8_t nRF24L01_ReadReg(uint8_t reg)
{
  uint8_t reg_val = 0;

  NRF_CE(PrivateData, 0);

  /*�õ�CSN��ʹ��SPI����*/
  NRF_CSN(PrivateData, 0);

  /*���ͼĴ�����*/
  SPI_NRF_RW(PrivateData, reg); 

  /*��ȡ�Ĵ�����ֵ */
  reg_val = SPI_NRF_RW(PrivateData, NOP);

  /*CSN���ߣ����*/
  NRF_CSN(PrivateData, 1);    

  return reg_val;
}



/**
  * @brief ������NRF�ļĴ�����д��һ������
  * @param reg:NRF������+�Ĵ�����ַ
  * @param pBuf:���ڴ洢���������ļĴ������ݵ����飬�ⲿ����
  * @param bytes:pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
static uint8_t nRF24L01_ReadBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
  uint8_t status, byte_cnt;

  NRF_CE(PrivateData, 0);
  
  /*�õ�CSN��ʹ��SPI����*/
  NRF_CSN(PrivateData, 0);

  /*���ͼĴ�����*/    
  status = SPI_NRF_RW(PrivateData, reg); 

  /*��ȡ����������*/
  for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
  {
    pBuf[byte_cnt] = SPI_NRF_RW(PrivateData, NOP); //��NRF24L01��ȡ����  
  }
  
  /*CSN���ߣ����*/
  NRF_CSN(PrivateData, 1);  

  return status;    //���ؼĴ���״ֵ̬
}



/**
  * @brief ������NRF�ļĴ�����д��һ������
  * @param reg: NRF������+�Ĵ�����ַ
  * @param pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
  * @param bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
static uint8_t nRF24L01_WriteBuf(uint8_t reg ,uint8_t *pBuf,uint8_t bytes)
{
  uint8_t status,byte_cnt;

  NRF_CE(PrivateData, 0);

  /*�õ�CSN��ʹ��SPI����*/
  NRF_CSN(PrivateData, 0);      

  /*���ͼĴ�����*/  
  status = SPI_NRF_RW(PrivateData, reg); 

  /*�򻺳���д������*/
  for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
  {
    SPI_NRF_RW(PrivateData, *pBuf++);  //д���ݵ������� 
  }

  /*CSN���ߣ����*/
  NRF_CSN(PrivateData, 1);      

  return (status);  //����NRF24L01��״̬     
}


/* �ӿں���------------------------------------------------------ */


/**
  * @brief  NRF24L01 Ӳ���ӿ�
  * @param  ��
  * @retval ��
  */
void nRF24L01_HwCtrlInterFaces(void    (*NRF_PortInit)(void),
                               void    *Data,
                               uint8_t (*SPI_ReadWriteByte)(void *Data, uint8_t cWriteData),
                               void    (*SetCSN)(void *Data, uint8_t State),
                               void    (*SetCE)(void *Data, uint8_t State),
                               void    (*BitDelayUs)(uint16_t Us))
{
  NRF_PortInit();
  PrivateData = Data;
  SPI_NRF_RW = SPI_ReadWriteByte;
  NRF_CSN = SetCSN;
  NRF_CE  = SetCE;
  NRF_DelayUs = BitDelayUs;
  
}


/**
  * @brief  ���ò��������ģʽ
  * @param  ��
  * @retval ��
  */
void nRF24L01_RxMode(void)
{
  uint8_t status = 0; 
  
	NRF_CE(PrivateData, 0);
  
  status=nRF24L01_ReadReg(STATUS);
  nRF24L01_WriteReg(NRF_WRITE_REG+STATUS,status);        /* ����жϱ�־*/    
  
	nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, NRF_RX_ADDRESS, RX_ADR_WIDTH);  /* дRX�ڵ��ַ */
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);               			/* ʹ��ͨ��0���Զ�Ӧ�� */
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);           			/* ʹ��ͨ��0�Ľ��յ�ַ */
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);             			/* ����RFͨ��Ƶ�� */
	nRF24L01_WriteReg(NRF_WRITE_REG+RX_PW_P0, RX_PLOAD_WIDTH);  			/* ѡ��ͨ��0����Ч���ݿ�� */      
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f); /* ����TX�������,0db����,2Mbps,���������濪�� */   
	nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);   /* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ */ 

//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_AA, 0x00);        //ʧ��ͨ��0�Զ�Ӧ�� 
//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01);    //����Ҫʹ�ܽ���ͨ��
  
  nRF24L01_WriteReg(FLUSH_RX,NOP);          /* ���RX FIFO�Ĵ��� */
  nRF24L01_WriteReg(FLUSH_TX,NOP);          /* ���TX FIFO�Ĵ��� */
  
	NRF_CE(PrivateData, 1); 

}    

/**
  * @brief  ���÷���ģʽ
  * @param  ��
  * @retval ��
  */
void nRF24L01_TxMode(void)
{  
	uint8_t status = 0; 
	NRF_CE(PrivateData, 0);
  
  status=nRF24L01_ReadReg(STATUS);
  nRF24L01_WriteReg(NRF_WRITE_REG+STATUS,status);        /* ����жϱ�־*/  
  
	nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR, NRF_TX_ADDRESS, TX_ADR_WIDTH);    /* дTX�ڵ��ַ */ 
	nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, NRF_RX_ADDRESS, RX_ADR_WIDTH); /* ����TX�ڵ��ַ,��ҪΪ��ʹ��ACK */   
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     /* ʹ��ͨ��0���Զ�Ӧ�� */    	
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); /* ʹ��ͨ��0�Ľ��յ�ַ */  	
	nRF24L01_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x15);/* �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:5�� */	
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);   /* ����RFͨ��ΪCHANAL */	
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);  /* ����TX�������,0db����,2Mbps,���������濪�� */   	
	nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    /* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж� */	
	
//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_AA, 0x00);        //ʧ��ͨ��0�Զ�Ӧ�� 
//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x00);    //ʧ�ܽ���ͨ��0       
//  nRF24L01_WriteReg(NRF_WRITE_REG + SETUP_RETR, 0x00);   //ʧ���Զ��ط� 
  
  nRF24L01_WriteReg(FLUSH_RX,NOP);          /* ���RX FIFO�Ĵ��� */
  nRF24L01_WriteReg(FLUSH_TX,NOP);          /* ���TX FIFO�Ĵ��� */
  
	NRF_CE(PrivateData, 1);
	NRF_DelayUs(20);  /* CEҪ����һ��ʱ��Ž��뷢��ģʽ��ʱ�����10us */
  
}


/**
  * @brief  ��Ҫ����NRF��MCU�Ƿ���������
  * @param  ��
  * @retval 1/0 ��������/����ʧ��
  */
uint8_t nRF24L01_ConnectCheck(void)
{
  uint8_t buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
  uint8_t buf1[5];
  uint8_t i = 0; 
   
  /*д��5���ֽڵĵ�ַ.  */  
  nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

  /*����д��ĵ�ַ */
  nRF24L01_ReadBuf(TX_ADDR,buf1,5); 
   
  /*�Ƚ�*/               
  for(i = 0; i < 5; i++)
  {
    if(buf1[i] != 0xC2)
    break;
  } 

  if(i==5)
  {
    return 1;    //MCU��NRF�ɹ����� 
  }
  else
  {
    return 0;      //MCU��NRF����������
  }
    
}


uint8_t nRF24L01_GetState(void)
{
	uint8_t cStatus;
	NRF_CE(PrivateData, 0);
	cStatus=nRF24L01_ReadReg(STATUS);/*��ȡstatus�Ĵ�����ֵ  */       
  
	if(cStatus & RX_DR)    /* ���յ����� */
	{
		nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, cStatus);/* ����жϱ�־*/ 
		nRF24L01_ReadBuf(RD_RX_PLOAD, NRF_RX_BUF, RX_PLOAD_WIDTH); /* ��ȡ���� */
		nRF24L01_WriteReg(FLUSH_RX, NOP);                       /* ���RX FIFO�Ĵ��� */
		NRF24L01_Flag = RX_DR; 
	}
	else if(cStatus & MAX_RT) /* �ﵽ����ط����� */
	{	
		nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, cStatus); /*���TX_DS��MAX_RT�жϱ�־*/ 
		nRF24L01_WriteReg(FLUSH_TX,NOP);      /* ���TX FIFO�Ĵ��� */
		NRF24L01_Flag = MAX_RT;
	}
	else if(cStatus & TX_DS)/* ������� */
	{
		nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, cStatus); /*���TX_DS��MAX_RT�жϱ�־*/ 
		nRF24L01_WriteReg(FLUSH_TX,NOP);      /* ���TX FIFO�Ĵ��� */
		NRF24L01_Flag = TX_DS;	
	}
	else 
		NRF24L01_Flag = 0;   /* û�յ��κ����� */
  
  NRF_CE(PrivateData, 1);
  
  return NRF24L01_Flag;
}


#include <string.h>

void nRF24L01_SetAddress(uint8_t *nRFAddr)
{
  memcpy(NRF_TX_ADDRESS, nRFAddr, TX_ADR_WIDTH);
  memcpy(NRF_RX_ADDRESS, nRFAddr, TX_ADR_WIDTH);
  
}


uint8_t nRF24L01_SendBuff(uint8_t *Tx_Buff)
{
  NRF24L01_Flag = 0;
  
  nRF24L01_TxMode();  //���뷢��ģʽ
  
  NRF_CE(PrivateData, 0);
  nRF24L01_WriteBuf(WR_TX_PLOAD, Tx_Buff, TX_PLOAD_WIDTH);
  NRF_CE(PrivateData, 1);
  
  while ((NRF24L01_Flag != TX_DS) && (NRF24L01_Flag != MAX_RT));
  
  nRF24L01_RxMode();
  
  return NRF24L01_Flag;
}


//ֵ��ע�����,������������ڱ�ģ����ж�����ִ��
uint8_t nRF24L01_SendData(uint32_t Data)
{
  uint8_t Tx_Buff[4] = {0};
  
  Tx_Buff[3] = ((Data & 0XFF000000)>>24);
  Tx_Buff[2] = ((Data & 0X00FF0000)>>16);
  Tx_Buff[1] = ((Data & 0X0000FF00)>>8);
  Tx_Buff[0] = ((Data & 0X000000FF)>>0);
  
  NRF24L01_Flag = 0;
  
  nRF24L01_TxMode();  //���뷢��ģʽ
  
  NRF_CE(PrivateData, 0);
  nRF24L01_WriteBuf(WR_TX_PLOAD, Tx_Buff, TX_PLOAD_WIDTH);
  NRF_CE(PrivateData, 1);
  
  while ((NRF24L01_Flag != TX_DS) && (NRF24L01_Flag != MAX_RT));
  
  nRF24L01_RxMode();
  
  return NRF24L01_Flag;
}


