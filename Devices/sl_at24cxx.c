/**
  ******************************************************************************
  * @file    at24c02.c
  * @author  杜公子寒枫
  * @version V2.0 
  * @date    2015.12.24
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 内存: 1/2/4/8/16K (AT24C01/02/04/08/16)
  * 数据: 高位先发 
  *
  * A2A1A0: 本文件默认芯片A2A1A0位接地,若实际与此不同,需修改宏 AT_BASE_ADDR
  *
  * 由于AT24Cxx芯片特性限制,AT24Cxx不能进行跨页(每8/16个字节为1页)写操作,所以设
  * 计了跨页写的算法.
  * 
  * 注: 1K/2K内存的芯片, 每8个字节为1页. 而4/8/16K的, 16个字节为1页.
  * 
  * V2.0------------
  * 修改描述: 1.将AT24Cxx的底层时序实现交由I2C框架去完成,本驱动只需要关心其功能
  *             的实现.而相关的接口由外界的函数指针提供
  *           2.修复AT24Cxx无法连续读大于256个字节的限制(无跨页限制).如果一次性
  *             读取大于256个字节,则分批执行
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2017.03.20
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_at24cxx.h"
#include "i2c.h"
#include "SourceLib.h"


/* ---设备地址--- */
#define AT_BASE_ADDR      (0XA0)

static SL_I2C_BIT_OPS_TYPE I2C_BitOps = {0};
static SL_I2C_BUS_DEV_TYPE AT24Cxx_I2C_BUS = {0};
static SL_DEVICE_TYPE *AT_I2C_DEV = &AT24Cxx_I2C_BUS.Parent;

static AT24Cxx_PAGE_SIZE AT_PAGE_SIZE;


//初始化I2C总线设备
static void AT24Cxx_DeviceInit(void)
{
  SL_I2C_BusDevieInit(&AT24Cxx_I2C_BUS, "AT24Cxx");
  SL_Device_Open(AT_I2C_DEV, 0);
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS, SL_NULL);
  
}



/**
  * @brief  设置外部控制接口
  * @param  AT_PortInit 端口初始化函数
  * @param  SetSDA 设置SDA的电平
  * @param  SetSCL 设置SCL的电平
  * @param  GetSDA 获取SDA的电平
  * @param  GetSCL 获取SCL的电平
  * @param  BitDelayUs 微秒级延时函数
  * @retval 无
  * @note  值得注意的是, SL_Device_Control 函数要在 AT24Cxx_DeviceInit 函数之后执行,因为在设备初
  *  始化函数执行之前,设备框架中的函数指针尚未和设备挂接,这样的情况下, SL_Device_Control 指向NULL,
  *  这时候无法将 I2C_BitOps 的指针正确地传递给BUS变量,当BUS变量调用 I2C_BitOps 的指针(野指针)时,
  *  会导致错误的指针跳转,从而让系统跑飞.....
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
  
  //如果是32KBit以上的EEPROM,则设置为16位寄存器地址
  if (AT_PAGE_SIZE >= AT24C32_PAGE_SIZE)
  {
    SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_REG_ADDR_16BIT, SL_NULL);
  }
  
  
}



/**
  * @brief  AT24Cxx字节写函数
  * @param  cAddr 要写的地址
  * @param  cWriteData 要写的数据
  * @retval 无
  */
void AT24Cxx_WriteByte(uint16_t nAddr, uint8_t cWriteData)
{
  uint16_t cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Write(AT_I2C_DEV, nAddr, &cWriteData, 1);
  
  Delay_ms(5);   //写操作时,必须要等待5ms,等待数据从缓存器往AT24Cxx写入完成
}



/**
  * @brief  AT24Cxx随机读函数
  * @param  cAddr 要读的地址
  * @retval cReadData 读取到的数据
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
  * @brief  AT24Cxx页写函数,只能写一页
  * @param  cAddr 要写的地址
  * @param  cNum  要写的数量
  * @param  pWriteBuff 要写的内容(数组/字符串/指针的形式)
  * @retval 无
  * @note   对于1K/2K的EEPROM, 8字节为1页, 对于4/8/16K的,16字节为之1页.
  *         对于跨页的写, 会在页尾进行环回到页首
  */
void AT24Cxx_PageWrite(uint16_t nAddr, const uint8_t *pWriteBuff, uint8_t cNum)
{
  uint16_t cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Write(AT_I2C_DEV, nAddr, pWriteBuff, cNum);
  
  Delay_ms(5);   //写操作时,必须要等待5ms,等待数据从缓存器往AT24Cxx写入完成
  
}



/**
  * @brief  AT24Cx连续读函数
  * @param  cAddr 要读的地址
  * @param  cNum  要读的数量
  * @param  pReadBuff 要读的内容(数组/指针的形式)
  * @retval 无
  * @note   一次读取的数据不可以超过256个字节,如果需要读取更多的数据,需要分批来读取
  */
void AT24Cxx_SequentialRead(uint16_t nAddr, uint8_t *pReadBuff, uint16_t nNum)
{
  uint16_t cDevAddr = 0;
  
  nNum = (nNum > 256) ? 256 : nNum; //AT24Cxx 最高只能支持256个字节的连续接收
  
  cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
  
  SL_Device_Control(AT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
  SL_Device_Read(AT_I2C_DEV, nAddr, pReadBuff, nNum);
  
}



/**
  * @brief  AT24Cxx多字节跨页读函数
  * @param  cAddr 要读的地址
  * @param  cNum  要读的数量
  * @param  pReadBuff 要读的内容(数组/指针的形式)
  * @retval 无
  * @note   是对连续读函数的封装,如果一次要读的数据超过256字节,则分批读取
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
  * @brief  AT24Cxx多字节跨页写函数
  * @param  nAddr 要写的地址
  * @param  nNum  要写的数量
  * @param  pWriteBuff 要写的内容(数组/字符串/指针的形式)
  * @retval 无
  */
void AT24Cxx_WriteMultiBytes(uint16_t nAddr, const uint8_t *pWriteBuff, uint32_t iNum)
{
  uint8_t i = 0;
  uint16_t nSurplusNum = iNum;  // 剩余未写入的数量
  uint16_t nCurrentPageNum = 0; // 当前阶段需要写入的页数
  
  uint16_t nCurrentAddr = nAddr;  // 当前操作的地址
  const uint8_t *pStoreBuff = pWriteBuff; // 当前存储的数据缓存区
  uint16_t nCurrentWriteNum = 0;  // 当前需要写入的数量
  
  // 第一阶段
  if (nCurrentAddr & (AT_PAGE_SIZE - 1)) // 假如当前地址并未与页地址对齐
  {
    nCurrentWriteNum = AT_PAGE_SIZE - (nCurrentAddr & (AT_PAGE_SIZE - 1));
    
    if (nSurplusNum < nCurrentWriteNum) // 假如剩余的数量少于当前可写的数量
    {
      nCurrentWriteNum = nSurplusNum; 
    }
    
    AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nCurrentWriteNum);
    
    pStoreBuff   += nCurrentWriteNum; // 指针偏移
    nSurplusNum  -= nCurrentWriteNum; // 更新剩余的数据的数量
    nCurrentAddr += nCurrentWriteNum; // 更新当前要写的地址
  }
  
  // 第二阶段
  nCurrentPageNum = nSurplusNum/AT_PAGE_SIZE;
  
  nCurrentWriteNum = AT_PAGE_SIZE;
  for (i = 0; i < nCurrentPageNum; i++)
  {
    AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nCurrentWriteNum);
    
    pStoreBuff   += nCurrentWriteNum; // 指针偏移
    nSurplusNum  -= nCurrentWriteNum; // 更新剩余的数据的数量
    nCurrentAddr += nCurrentWriteNum; // 更新当前要写的地址
  }
  
  // 第三阶段
  if (nSurplusNum != 0)
  {
    AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nSurplusNum);
  }
  
}

