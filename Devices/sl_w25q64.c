/**
  ******************************************************************************
  * @file    sl_w25q64.c
  * @author  杜公子寒枫
  * @version V2.1 寄存器版本
  * @date    2016.07.14
  * @brief   W25Q654驱动
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *      FLASH_CS  --- SPI_NSS(上拉)
  *      FLASH_CLK --- SPI_SCK  
  *      FLASH_DO  --- SPI_MISO 
  *      FLASH_DI  --- SPI_MOSI 
  * 
  * 存储,
  *      1页   --- 256 Byte
  *      1扇区 --- 4K  Byte
  * 
  * 容量: 64M Bit(8M Byte)
  * 
  * 注1: W25Q64只能进行页编程,即一次最多只可以写256Byte,而且一次的操作不能跨页.为
  *      解决此限制,W25Q64_WriteMultiPage()利用算法将跨页的情况进行拆分成多次页内
  *      的写操作.但是这种情况下仍受到无法跨扇区写的限制,故而还需对跨页写的函数再
  *      进行一层的封装,即W25Q64_WriteMultiByte()函数.此函数可以进行跨扇区,多扇区
  *      的操作,修改FLASH芯片中任意位置的数据也不会影响到其他位.
  * 
  * 注2: 若引脚的翻转速度不够,则会导致数据的读写出错,建议IO的速度都配置为高速模式
  * 
  * 
  * V2.0------------
  * 修改描述: 修改文件头描述,开放SPI宏接口以增强其兼容性,增加整片擦除的函数;
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.13
  * 
  * V2.1------------
  * 修改描述: 1. 修复W25Q64_WriteMultiByte()函数的BUG,此BUG导致当以第二个扇区(地
  *              址:4096)及之后的扇区作为存储地址时,如果数据量大于4K,则会导致数据
  *              写入错误并错误地擦除了第一个扇区所有数据;
  *           2. 优化W25Q64_WriteMultiByte()的算法,去除多余操作,以节省CPU开销;
  *              
  * 错误原因: 扇区地址计算公式为:iSectorAddr = (iAddr/(4*1024))*(4*1024); 而原函数
  *           中一个擦除的函数错误地将iSectorAddr = (iAddr/(4*1024)) ,导致擦除了第
  *           一个扇区而且本扇区未擦除,导致无法写入失败;
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.1
  * 修改日期: 2016.07.14
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
  * @brief  NRF24L01 硬件接口
  * @param  无
  * @retval 无
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
  
  /* 验证是否正常工作 */
  while (W25Q64_ReadDeviceID() != 0XEF16);
  
}



/**
  * @brief  W25Q64写使能,在扇区擦除,页编程时必须要写使能
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
  * @brief  等待不忙,写/擦除操作后,需等待W25Q64完成从缓存中写入内存的操作.
  *         BUSY: 0--不忙  1--在忙  在sta1最高位
  * @param  None
  * @retval None
  */
static void W25Q64_WaitNoBusy(void)
{
  uint8_t cStaReg = 0;
  
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X05);  //获取sta1
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
  * @brief  W25Q64扇区擦除函数
  * @param  iAddr 要擦除的扇区地址
  * @retval None
  */
static void W25Q64_SectorErase(uint32_t iAddr)
{
  /*写使能*/
  W25Q64_WriteEnable();
  
  /*擦除*/
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X20);
  SPI_ReadWriteByte((iAddr & 0XFF0000)>>16);
  SPI_ReadWriteByte((iAddr & 0X00FF00)>>8 );
  SPI_ReadWriteByte((iAddr & 0X0000FF)>>0 );
  
  SPI_CS_H();
  
  /*等待擦除完成*/
  W25Q64_WaitNoBusy();
  
}



/**
  * @brief  W25Q64页编程  1页--256个字节
  * @param  iAddr 写入的地址
  * @param  nNum  写入的数量,范围为1-256
  * @param  pWriteBuff 要写入的内容 以数组/字符串/指针的方式入参  
  * @retval None
  */
static void W25Q64_PageProgram(uint32_t iAddr, uint16_t nNum, const uint8_t *pWriteBuff)  //最大可写1页--256个字节
{
  uint16_t i = 0;
  
  /*写使能*/
  W25Q64_WriteEnable();
  
  /*写数据*/
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

  /*等待写入完成*/
  W25Q64_WaitNoBusy();
  
}



/**
  * @brief  算法: 实现跨页写的多字节写函数,并作为跨扇区写的算法的基础(只能在同个扇区中写)
  * @param  iAddr 写入的地址
  * @param  nNum  写入的数量,1--4096
  * @param  pWriteBuff 要写入的内容 以数组/字符串/指针的方式入参  
  * @retval None
  */
static void W25Q64_WriteMultiPage(uint32_t iAddr, uint32_t iNum, const uint8_t *pWriteBuff)
{
  uint32_t i = 0;
	uint32_t iPageNum = 0;         //第二阶段需要的页数
	uint32_t iSurplusNum = 0;      //当前页剩余的可操作数据
  
  W25Q64_SectorErase(iAddr);
  
  /*第一阶段*/
  if (iNum <= (256-iAddr%256))   //当前页可写完所有的数据
  {
    iSurplusNum = iNum;
    
    W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
    return;
  }
  else
  {
    iSurplusNum = (256-iAddr%256);  //当前页可写的内容
    
    W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
    
    iAddr += iSurplusNum; //地址偏移
    iNum  -= iSurplusNum; //剩余未写入的字数偏移
    pWriteBuff += iSurplusNum; //数据指针偏移
  }
  
  /*第二阶段*/
  iPageNum = iNum/256;
  for (i = 0; i < iPageNum; i++)
  {
    iSurplusNum = 256;
    
    W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
    
    iAddr += iSurplusNum; //地址偏移
    iNum  -= iSurplusNum; //剩余未写入的字数偏移
    pWriteBuff += iSurplusNum; //数据指针偏移
    
  }
  
  /*第三阶段*/
  iSurplusNum = iNum;
  W25Q64_PageProgram(iAddr,iSurplusNum,pWriteBuff);
  
}



/**
  * @brief  擦除整块FLASH芯片的数据
  * @param  None
  * @retval None
  * @note   根据芯片手册的描述,擦除整片芯片需要15S(type)-60S(max),测试结果为20S
  */
void W25Q64_EraseChip(void)
{
  /*写使能*/
  W25Q64_WriteEnable();
  
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X60);
  
  SPI_CS_H();
  
  /*等待擦除完成*/
  W25Q64_WaitNoBusy();
  
}



/**
  * @brief  获取W25Q64设备ID,若成功获取,其ID应该为0XEF16. 
  *         可以作为验证W25Q64是否能正常工作的验证
  * @param  None
  * @retval nDeviceID 正常的情况下,返回的值应该是0XEF16
  */
uint16_t W25Q64_ReadDeviceID(void)
{
  uint16_t nDeviceID = 0;
  SPI_CS_L();
  
  SPI_ReadWriteByte(0X90);
  SPI_ReadWriteByte(0);
  SPI_ReadWriteByte(0);
  SPI_ReadWriteByte(0);
  
  nDeviceID  = (SPI_ReadWriteByte(0XFF)<<8);  //设备ID的高八位
  nDeviceID |=  SPI_ReadWriteByte(0XFF);      //低8位
  
  SPI_CS_H();
  
  return nDeviceID;
}



/**
  * @brief  多字节读程序,可连续寻址所有的地址,地址范围内无数量限制
  * @param  iAddr 读取的地址
  * @param  nNum  读取的数量
  * @param  pWriteBuff 读取到的内容 以数组/指针的方式出参  
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
  * @brief  多字节写函数,可以修改FLASH芯片中任意位置的数据而不影响其他位.
  *         可以进行跨扇区,多扇区的操作. 调用此程序不需要执行扇区擦除操作
  * @param  iAddr 写入的地址
  * @param  nNum  写入的数量,最大可实现写满一个扇区
  * @param  pWriteBuff 要写入的内容 以数组/字符串/指针的方式入参  
  * @retval None
  * @note   const uint8_t * 指针指向地址的数据不能改变,指针指向的地址可以改变
  */
void W25Q64_WriteMultiByte(uint32_t iAddr, uint32_t iNum, const uint8_t *pWriteBuff)
{
  uint32_t i = 0;
  uint16_t nSectorNum = 0;    //待写入扇区数
  uint32_t iSectorAddr = 0;   //扇区地址(0/4096/8192...)
  uint32_t iStoreNum = 0;   //当前扇区剩余的可操作数据
  uint16_t nAddrOffset = 0;
  
  uint8_t szDataBuff[1024*4] = {0};
  
  ///////////////////////////// 1.x 当前扇区能存储完所有的数据 /////////////////////////////
  if (iNum <= (4096-iAddr%4096))  //4096-iAddr%4096: 从iAddr到扇区尾部的长度
  {
    iSectorAddr = (iAddr/(4*1024))*(4*1024);  //计算当前扇区地址
    nAddrOffset = iAddr%(4096);  
    
    W25Q64_ReadMultiByte(iSectorAddr,4*1024,szDataBuff);  //保存扇区数据
    memcpy(&szDataBuff[nAddrOffset],pWriteBuff,iNum);     //修改缓冲区
    W25Q64_WriteMultiPage(iSectorAddr,1024*4,szDataBuff); //将缓冲区写入扇区
  }
  
  
  /////////////////////////// 2.x 需要多个扇区才能存储当前的数据 ///////////////////////////
  else 
  {
    /* 2.1 从当前地址到扇区尾部进行存储 */
    iSectorAddr = (iAddr/(4*1024))*(4*1024);  //扇区起始地址
    nAddrOffset = iAddr%(4096);               //存储地址相对扇区地址的偏移
    iStoreNum   = 4096 - nAddrOffset;         //本轮要存储的数据数
    
    W25Q64_ReadMultiByte(iSectorAddr,nAddrOffset,szDataBuff);  //保存扇区数据
    memcpy(&szDataBuff[nAddrOffset],pWriteBuff,iStoreNum);     //修改缓冲区
    W25Q64_WriteMultiPage(iSectorAddr,1024*4,szDataBuff);      //将缓冲区写入扇区
    
    iNum  -= iStoreNum;       //剩余数据总数
    iAddr += iStoreNum;       //下次存储的地址
    pWriteBuff += iStoreNum;  //指向未存储的数据
    
    /* 2.2 剩余数据>4K,能填满整个扇区 */
    nSectorNum = iNum/(4*1024);   //要写满的扇区数
    iStoreNum  = 4*1024;    //要存储的数据数
    
    for (i = 0; i < nSectorNum; i++)
    {
      iSectorAddr = (iAddr/(4*1024))*(4*1024);  //扇区起始地址
      
      
      W25Q64_SectorErase(iSectorAddr);
      W25Q64_WriteMultiPage(iSectorAddr,1024*4,pWriteBuff); //将整个数据缓冲区写入到扇区中
      
      iNum  -= iStoreNum;       //剩余数据总数
      iAddr += iStoreNum;       //下次存储的地址
      pWriteBuff += iStoreNum;  //指向未存储的数据
      
    }
    
    /* 2.3 剩余数据<4K,存储在扇区首到结束地址 */
    iSectorAddr = (iAddr/(4*1024))*(4*1024);  //扇区起始地址
    iStoreNum  = iNum;          //本轮要存储的数据数
    
    W25Q64_ReadMultiByte(iSectorAddr+iStoreNum,4096-iStoreNum,&szDataBuff[iStoreNum]);  //保存扇区数据    
    memcpy(szDataBuff,pWriteBuff,iStoreNum);                //修改缓冲区  
    W25Q64_WriteMultiPage(iSectorAddr,1024*4,szDataBuff);   //将缓冲区写入扇区
  }
  
}





