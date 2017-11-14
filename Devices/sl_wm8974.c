/**
  ******************************************************************************
  * @file    sl_wm8974.c
  * @author  杜公子寒枫
  * @version V2.1 寄存器版本
  * @date    2017.04.27
  * @brief   WM8974驱动
  ******************************************************************************
  * @attention
  * 
  *     
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "sl_wm8974.h"
#include "SourceLib.h"
#include "i2c.h"


/* ---设备地址--- */
#define   WM8974_ADDR         (0x34)  //设备地址


/* 内部函数-------------------------------------------------------------- */
static SL_I2C_BIT_OPS_TYPE I2C_BitOps = {0};
static SL_I2C_BUS_DEV_TYPE WM_I2C_BUS = {0};
static SL_DEVICE_TYPE *WM_I2C_DEV = &WM_I2C_BUS.Parent;



//写WM8974的寄存器
static void WM8974_IIC_WriteByte(uint8_t cRegAddr, uint16_t nData)
{
  SL_Device_Write(WM_I2C_DEV, cRegAddr, &nData, 1);
  
}


//寄存器初始化
static void WM8974_RegInit(void)
{
  // 根据手册自行修改
  WM8974_IIC_WriteByte(0x00,  0x055);
  
  DelayMs(200);
  
  WM8974_IIC_WriteByte(0x38,  0x040);
  WM8974_IIC_WriteByte(0x32,  0x000);
  
  WM8974_IIC_WriteByte(0x01,  0x00D);
  WM8974_IIC_WriteByte(0x02,  0x015);
  WM8974_IIC_WriteByte(0x03,  0x0ED);
  WM8974_IIC_WriteByte(0x04,  0x050); // 24bits I2S_format 左对齐
  WM8974_IIC_WriteByte(0x05,  0x000); // DAC ADC companding OFF
  WM8974_IIC_WriteByte(0x06,  0x000); // slave mode PLL
  WM8974_IIC_WriteByte(0x07,  0x000); // 48kHz
  WM8974_IIC_WriteByte(0x0A,  0x008); // 128x oversample
  WM8974_IIC_WriteByte(0x0B,  0x0FF); // 0dB
  WM8974_IIC_WriteByte(0x0E,  0x100); // High_Pass_Filter
  WM8974_IIC_WriteByte(0x2C,  0x003);
  WM8974_SetInpgaVolume(12);
  WM8974_IIC_WriteByte(0x2F,  0x000);
  WM8974_IIC_WriteByte(0x31,  0x00A);
  WM8974_IIC_WriteByte(0x32,  0x001);
  WM8974_SetSpkVolCtrl(1);
  WM8974_SetMonoVolCtrl(0);
  
  // PLL计算
  WM8974_IIC_WriteByte(0x01,  0x02D); // PLL ENABLE
  WM8974_IIC_WriteByte(0x06,  0x140); // PLL output slave mode PLL
  WM8974_IIC_WriteByte(0x0A,  0x008); // 128x oversample
  
  WM8974_IIC_WriteByte(0x24,  0x008); // PRESCALE DIVIDE: 1, N: 8
  WM8974_IIC_WriteByte(0x25,  0x000);
  WM8974_IIC_WriteByte(0x26,  0x000);
  WM8974_IIC_WriteByte(0x27,  0x000);
  
  WM8974_IIC_WriteByte(0x08,  0x014); // PLL 
  
  // 主模式
//  WM8974_IIC_WriteByte(0x06,  0x149);

  // MONO 模式
  WM8974_IIC_WriteByte(0x07,  0x000 | 0x088); // 48kHz
  WM8974_IIC_WriteByte(0x38,  0x001);
  
}


//设备初始化
void WM8974_HwCtrlInterFaces (void    (*WM_PortInit)(void),
                              void    *Data,
                              void    (*SetSDA)(void *Data, uint8_t State),
                              void    (*SetSCL)(void *Data, uint8_t State),
                              uint8_t (*GetSDA)(void *Data),
                              uint8_t (*GetSCL)(void *Data),
                              void    (*BitDelayUs)(uint16_t Us))
{
  uint16_t DevAddr = WM8974_ADDR;
  
  I2C_BitOps.SetSDA = SetSDA;
  I2C_BitOps.SetSCL = SetSCL;
  I2C_BitOps.GetSDA = GetSDA;
  I2C_BitOps.GetSCL = GetSCL;
  I2C_BitOps.BitDelayUs = BitDelayUs;
  I2C_BitOps.DelayTime = 5; //5Us
  I2C_BitOps.iTimeOut = 100; //100MS
  I2C_BitOps.Data = Data;
  
  WM_PortInit();  //Init hardware GPIO
  
  SL_I2C_BusDevieInit(&WM_I2C_BUS, "WM8974");
  
  SL_Device_Control(WM_I2C_DEV, SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS, SL_NULL);
  SL_Device_Control(WM_I2C_DEV, SL_I2C_DEV_CTRL_SET_BIT_OPS, &I2C_BitOps);
  SL_Device_Control(WM_I2C_DEV, SL_I2C_DEV_CTRL_SET_TIMEOUT, &I2C_BitOps.iTimeOut);
  SL_Device_Control(WM_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &DevAddr);
  SL_Device_Control(WM_I2C_DEV, SL_I2C_DEV_CTRL_DEV_ADDR_16BIT, SL_NULL); //16位设备地址
  
  SL_Device_Open(WM_I2C_DEV, 0);
  WM8974_RegInit();
  
}



//only 0.25 or 0.00
void WM8974_SetInpgaVolume(int8_t n_db)
{
  uint8_t data;
  data = (n_db + 12) / 0.75;
  WM8974_IIC_WriteByte(0x2D, data);
}


//设置Spk音量
void WM8974_SetSpkVolCtrl(uint8_t flag)
{
  WM8974_IIC_WriteByte(0x36, (flag) ? 0x0B9 : 0x0F9);

}


//设置Mono音量
void WM8974_SetMonoVolCtrl(uint8_t flag)
{
  WM8974_IIC_WriteByte(0x38, (flag) ? 0x001 : 0x041);
  
}




