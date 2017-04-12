/**
  ******************************************************************************
  * @file    sl_logic_ic.c
  * @author  杜公子寒枫
  * @version V1.1
  * @date    2017.04.12
  * @brief   about some logic ic drivers
  ******************************************************************************
  * @attention
  * 
  * 74HC164:  串转并
  * 74HC4052: 双闸模拟开关  PORT_INH  PORT_SEL0  PORT_SEL1
  * 
  * V1.0------------
  * 修改描述: 增加74HC4052模拟开关的驱动文件
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.0
  * 修改日期: 2017.04.11
  * 
  * V1.1------------
  * 修改描述: 增加74HC595模拟开关的驱动文件
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2017.04.12
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_logic_ic.h"
#include "SourceLib.h"



/*----------------------------------------------------------------------------
    74HC405x 多路X通道模拟开关
 *----------------------------------------------------------------------------*/
 
static void    *HC405x_PrivateData;
static void    (*HC405x_SetINH)  (void *Data, uint8_t State);
static void    (*HC405x_SetSEL0) (void *Data, uint8_t State);
static void    (*HC405x_SetSEL1) (void *Data, uint8_t State);
static void    (*HC405x_SetSEL2) (void *Data, uint8_t State); //在4052不需要
static HC405x_TYPE HC405x_Type;

#define HC405x_INH(x)   HC405x_SetINH (HC405x_PrivateData, (x))
#define HC405x_SEL0(x)  HC405x_SetSEL0(HC405x_PrivateData, (x))
#define HC405x_SEL1(x)  HC405x_SetSEL1(HC405x_PrivateData, (x))
#define HC405x_SEL2(x)  HC405x_SetSEL2(HC405x_PrivateData, (x))


//硬件控制接口
void HC405x_HwCtrlInterFaces(void    (*HC_PortInit)(void),
                             void    *Data,
                             void    (*SetINH) (void *Data, uint8_t State),
                             void    (*SetSEL0)(void *Data, uint8_t State),
                             void    (*SetSEL1)(void *Data, uint8_t State),
                             void    (*SetSEL2)(void *Data, uint8_t State),
                             HC405x_TYPE DevType)
{
  HC405x_PrivateData = Data;
  HC405x_SetINH  = SetINH;
  HC405x_SetSEL0 = SetSEL0;
  HC405x_SetSEL1 = SetSEL1;
  HC405x_SetSEL2 = SetSEL2;
  
  HC_PortInit();  //初始化硬件端口
  
}
  

//INH脚低电平有效
void HC405x_Enable(uint8_t isEnable)
{
  HC405x_INH(!getBOOL(isEnable));
  
}


//选择通道
void HC405x_SetChannel(uint8_t cChannel)
{
  HC405x_Enable(cChannel != 0xFF);  //如果通道为0xFF,则关闭
  
  if (HC405x_Type != SWITCH_HC4052)
  {
    HC405x_SEL2(getBOOL(cChannel & 0x04));
  }
  HC405x_SEL1(getBOOL(cChannel & 0x02));
  HC405x_SEL0(getBOOL(cChannel & 0x01));
  
}



/*----------------------------------------------------------------------------
    74HC595 串转并芯片
 *----------------------------------------------------------------------------*/

static void    *HC595_PrivateData;
static void    (*HC595_SetSHCP) (void *Data, uint8_t State);
static void    (*HC595_SetSTCP) (void *Data, uint8_t State);
static void    (*HC595_SetDATA) (void *Data, uint8_t State);
static void    (*HC595_DelayUs) (uint16_t Us);
static uint8_t HC595_ParBitNum = 0;

#define HC595_SHCP(x)   HC595_SetSHCP(HC595_PrivateData, (x))
#define HC595_STCP(x)   HC595_SetSTCP(HC595_PrivateData, (x))
#define HC595_DATA(x)   HC595_SetDATA(HC595_PrivateData, (x))

typedef enum
{
  HC595_8_BIT   = 8,
  HC595_16_BIT  = 16,
  HC595_24_BIT  = 16,
  HC595_32_BIT  = 16,
  
}HC595_PARALLEL_BIT;




//硬件控制接口
void HC595_HwCtrlInterFaces(void    (*HC_PortInit)(void),
                            void    *Data,
                            void    (*SetSHCP)(void *Data, uint8_t State),
                            void    (*SetSTCP)(void *Data, uint8_t State),
                            void    (*SetDATA)(void *Data, uint8_t State),
                            void    (*HC_DelayUs)(uint16_t Us),
                            uint8_t ParBitNum)
{
  HC595_PrivateData = Data; //私有数据
  HC595_SetSHCP = SetSHCP;
  HC595_SetSTCP = SetSTCP;
  HC595_SetDATA = SetDATA;
  HC595_DelayUs = HC_DelayUs;
  HC595_ParBitNum = ParBitNum;
  
  HC_PortInit();  //初始化硬件端口
  
}



//输出并行数据
void HC595_OutputParallelData(uint32_t iSerialData)
{
  uint8_t i = 0;
  
  //数据移位
  for (i = 0; i < HC595_ParBitNum; i++)
  {
    HC595_SHCP(1);
    HC595_DelayUs(5);
    
    HC595_SHCP(0);
    
    HC595_DATA((iSerialData & (0x1<<((HC595_ParBitNum - 1)-i))) ? 1 : 0);  //从高位到低位扫描
    
    HC595_DelayUs(5);
    
    HC595_SHCP(1);
  }
  
  //数据锁存
  HC595_STCP(0);
  HC595_DelayUs(5);
  HC595_STCP(1);
  
}




