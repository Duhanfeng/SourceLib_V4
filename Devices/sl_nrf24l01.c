/**
  ******************************************************************************
  * @file    sl_nrf24l01.c
  * @author  杜公子寒枫
  * @version V1.0
  * @date    2017.04.09
  * @brief   control nrf24l01
  ******************************************************************************
  * @attention
  * 
  * 两个nrf24l01通信，需要满足3个条件相同：
  * 1.频道相同（设置频道寄存器RF_CH）
  * 2.地址相同（设置TX_ADDR和RX_ADDR_P0相同）
  * 3.每次发送接收的字节数相同（如果设置了通道的有效数据宽度为n，那么每次发送的字节数也必须为n，当然，n<=32）
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

uint8_t NRF_RX_BUF[RX_PLOAD_WIDTH];   //接收数据缓存
uint8_t NRF_TX_BUF[TX_PLOAD_WIDTH];   //发射数据缓存
uint8_t NRF_TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
uint8_t NRF_RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};




/**
  * @brief 用于向NRF特定的寄存器写入数据
  * @param reg:NRF的命令+寄存器地址
  * @param dat:将要向寄存器写入的数据
  * @retval  NRF的status寄存器的状态
  */
static uint8_t nRF24L01_WriteReg(uint8_t reg,uint8_t dat)
{
  uint8_t status = 0;
  NRF_CE(PrivateData, 0);

  /*置低CSN，使能SPI传输*/
  NRF_CSN(PrivateData, 0);
      
  /*发送命令及寄存器号 */
  status = SPI_NRF_RW(PrivateData, reg);
   
  /*向寄存器写入数据*/
  SPI_NRF_RW(PrivateData, dat); 
          
  /*CSN拉高，完成*/     
  NRF_CSN(PrivateData, 1);  

  /*返回状态寄存器的值*/
  return(status);
}



/**
  * @brief 用于从NRF特定的寄存器读出数据
  * @param reg:NRF的命令+寄存器地址
  * @retval  寄存器中的数据
  */
static uint8_t nRF24L01_ReadReg(uint8_t reg)
{
  uint8_t reg_val = 0;

  NRF_CE(PrivateData, 0);

  /*置低CSN，使能SPI传输*/
  NRF_CSN(PrivateData, 0);

  /*发送寄存器号*/
  SPI_NRF_RW(PrivateData, reg); 

  /*读取寄存器的值 */
  reg_val = SPI_NRF_RW(PrivateData, NOP);

  /*CSN拉高，完成*/
  NRF_CSN(PrivateData, 1);    

  return reg_val;
}



/**
  * @brief 用于向NRF的寄存器中写入一串数据
  * @param reg:NRF的命令+寄存器地址
  * @param pBuf:用于存储将被读出的寄存器数据的数组，外部定义
  * @param bytes:pBuf的数据长度
  * @retval  NRF的status寄存器的状态
  */
static uint8_t nRF24L01_ReadBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
  uint8_t status, byte_cnt;

  NRF_CE(PrivateData, 0);
  
  /*置低CSN，使能SPI传输*/
  NRF_CSN(PrivateData, 0);

  /*发送寄存器号*/    
  status = SPI_NRF_RW(PrivateData, reg); 

  /*读取缓冲区数据*/
  for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
  {
    pBuf[byte_cnt] = SPI_NRF_RW(PrivateData, NOP); //从NRF24L01读取数据  
  }
  
  /*CSN拉高，完成*/
  NRF_CSN(PrivateData, 1);  

  return status;    //返回寄存器状态值
}



/**
  * @brief 用于向NRF的寄存器中写入一串数据
  * @param reg: NRF的命令+寄存器地址
  * @param pBuf：存储了将要写入写寄存器数据的数组，外部定义
  * @param bytes: pBuf的数据长度
  * @retval  NRF的status寄存器的状态
  */
static uint8_t nRF24L01_WriteBuf(uint8_t reg ,uint8_t *pBuf,uint8_t bytes)
{
  uint8_t status,byte_cnt;

  NRF_CE(PrivateData, 0);

  /*置低CSN，使能SPI传输*/
  NRF_CSN(PrivateData, 0);      

  /*发送寄存器号*/  
  status = SPI_NRF_RW(PrivateData, reg); 

  /*向缓冲区写入数据*/
  for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
  {
    SPI_NRF_RW(PrivateData, *pBuf++);  //写数据到缓冲区 
  }

  /*CSN拉高，完成*/
  NRF_CSN(PrivateData, 1);      

  return (status);  //返回NRF24L01的状态     
}


/* 接口函数------------------------------------------------------ */


/**
  * @brief  NRF24L01 硬件接口
  * @param  无
  * @retval 无
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
  * @brief  配置并进入接收模式
  * @param  无
  * @retval 无
  */
void nRF24L01_RxMode(void)
{
  uint8_t status = 0; 
  
	NRF_CE(PrivateData, 0);
  
  status=nRF24L01_ReadReg(STATUS);
  nRF24L01_WriteReg(NRF_WRITE_REG+STATUS,status);        /* 清除中断标志*/    
  
	nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, NRF_RX_ADDRESS, RX_ADR_WIDTH);  /* 写RX节点地址 */
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);               			/* 使能通道0的自动应答 */
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);           			/* 使能通道0的接收地址 */
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);             			/* 设置RF通信频率 */
	nRF24L01_WriteReg(NRF_WRITE_REG+RX_PW_P0, RX_PLOAD_WIDTH);  			/* 选择通道0的有效数据宽度 */      
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f); /* 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */   
	nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);   /* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 */ 

//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_AA, 0x00);        //失能通道0自动应答 
//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x01);    //接收要使能接收通道
  
  nRF24L01_WriteReg(FLUSH_RX,NOP);          /* 清除RX FIFO寄存器 */
  nRF24L01_WriteReg(FLUSH_TX,NOP);          /* 清除TX FIFO寄存器 */
  
	NRF_CE(PrivateData, 1); 

}    

/**
  * @brief  配置发送模式
  * @param  无
  * @retval 无
  */
void nRF24L01_TxMode(void)
{  
	uint8_t status = 0; 
	NRF_CE(PrivateData, 0);
  
  status=nRF24L01_ReadReg(STATUS);
  nRF24L01_WriteReg(NRF_WRITE_REG+STATUS,status);        /* 清除中断标志*/  
  
	nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR, NRF_TX_ADDRESS, TX_ADR_WIDTH);    /* 写TX节点地址 */ 
	nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, NRF_RX_ADDRESS, RX_ADR_WIDTH); /* 设置TX节点地址,主要为了使能ACK */   
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     /* 使能通道0的自动应答 */    	
	nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); /* 使能通道0的接收地址 */  	
	nRF24L01_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x15);/* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:5次 */	
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);   /* 设置RF通道为CHANAL */	
	nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);  /* 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */   	
	nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    /* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断 */	
	
//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_AA, 0x00);        //失能通道0自动应答 
//  nRF24L01_WriteReg(NRF_WRITE_REG + EN_RXADDR, 0x00);    //失能接收通道0       
//  nRF24L01_WriteReg(NRF_WRITE_REG + SETUP_RETR, 0x00);   //失能自动重发 
  
  nRF24L01_WriteReg(FLUSH_RX,NOP);          /* 清除RX FIFO寄存器 */
  nRF24L01_WriteReg(FLUSH_TX,NOP);          /* 清除TX FIFO寄存器 */
  
	NRF_CE(PrivateData, 1);
	NRF_DelayUs(20);  /* CE要拉高一段时间才进入发送模式，时间大于10us */
  
}


/**
  * @brief  主要用于NRF与MCU是否正常连接
  * @param  无
  * @retval 1/0 连接正常/连接失败
  */
uint8_t nRF24L01_ConnectCheck(void)
{
  uint8_t buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
  uint8_t buf1[5];
  uint8_t i = 0; 
   
  /*写入5个字节的地址.  */  
  nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

  /*读出写入的地址 */
  nRF24L01_ReadBuf(TX_ADDR,buf1,5); 
   
  /*比较*/               
  for(i = 0; i < 5; i++)
  {
    if(buf1[i] != 0xC2)
    break;
  } 

  if(i==5)
  {
    return 1;    //MCU与NRF成功连接 
  }
  else
  {
    return 0;      //MCU与NRF不正常连接
  }
    
}


uint8_t nRF24L01_GetState(void)
{
	uint8_t cStatus;
	NRF_CE(PrivateData, 0);
	cStatus=nRF24L01_ReadReg(STATUS);/*读取status寄存器的值  */       
  
	if(cStatus & RX_DR)    /* 接收到数据 */
	{
		nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, cStatus);/* 清除中断标志*/ 
		nRF24L01_ReadBuf(RD_RX_PLOAD, NRF_RX_BUF, RX_PLOAD_WIDTH); /* 读取数据 */
		nRF24L01_WriteReg(FLUSH_RX, NOP);                       /* 清除RX FIFO寄存器 */
		NRF24L01_Flag = RX_DR; 
	}
	else if(cStatus & MAX_RT) /* 达到最大重发次数 */
	{	
		nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, cStatus); /*清除TX_DS或MAX_RT中断标志*/ 
		nRF24L01_WriteReg(FLUSH_TX,NOP);      /* 清除TX FIFO寄存器 */
		NRF24L01_Flag = MAX_RT;
	}
	else if(cStatus & TX_DS)/* 发送完成 */
	{
		nRF24L01_WriteReg(NRF_WRITE_REG+STATUS, cStatus); /*清除TX_DS或MAX_RT中断标志*/ 
		nRF24L01_WriteReg(FLUSH_TX,NOP);      /* 清除TX FIFO寄存器 */
		NRF24L01_Flag = TX_DS;	
	}
	else 
		NRF24L01_Flag = 0;   /* 没收到任何数据 */
  
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
  
  nRF24L01_TxMode();  //进入发送模式
  
  NRF_CE(PrivateData, 0);
  nRF24L01_WriteBuf(WR_TX_PLOAD, Tx_Buff, TX_PLOAD_WIDTH);
  NRF_CE(PrivateData, 1);
  
  while ((NRF24L01_Flag != TX_DS) && (NRF24L01_Flag != MAX_RT));
  
  nRF24L01_RxMode();
  
  return NRF24L01_Flag;
}


//值得注意的是,这个函数不能在本模块的中断里面执行
uint8_t nRF24L01_SendData(uint32_t Data)
{
  uint8_t Tx_Buff[4] = {0};
  
  Tx_Buff[3] = ((Data & 0XFF000000)>>24);
  Tx_Buff[2] = ((Data & 0X00FF0000)>>16);
  Tx_Buff[1] = ((Data & 0X0000FF00)>>8);
  Tx_Buff[0] = ((Data & 0X000000FF)>>0);
  
  NRF24L01_Flag = 0;
  
  nRF24L01_TxMode();  //进入发送模式
  
  NRF_CE(PrivateData, 0);
  nRF24L01_WriteBuf(WR_TX_PLOAD, Tx_Buff, TX_PLOAD_WIDTH);
  NRF_CE(PrivateData, 1);
  
  while ((NRF24L01_Flag != TX_DS) && (NRF24L01_Flag != MAX_RT));
  
  nRF24L01_RxMode();
  
  return NRF24L01_Flag;
}


