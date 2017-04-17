/**
  ******************************************************************************
  * @file    sl_pt2259.c
  * @author  杜公子寒枫
  * @version V1.0
  * @date    2017.03.29
  * @brief   control pt2259
  ******************************************************************************
  * @attention
  * PT2259 is an 8-pin 2-channel volume controller which utilizes CMOS technology 
  * and incorporates the I2C interface control. The controller features an attenuation 
  * range of 0 to -79dB, low noise output, a high degree of stereo separation and 
  * requires only a small number of external components. PT2259 isan essential component 
  * for modern audio visual systems.
  * 
  * FEATURES:
  *     Attenuation range: 0 to –79dB in 1dB steps
  *     Operating voltage: 4 to 9V
  *     2-channel volume individual adjust
  *     control by i2c
  * 
  * communication frame: 
  *     START+DEV_ADDR+CMD+STOP (control command)
  *     START+DEV_ADDR+10dB_CMD+1dB_CMD+STOP  (set volume command)
  *     
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_pt2259.h"
#include "i2c.h"
#include "SourceLib.h"


/* ---设备地址--- */
#define   PT2259_ADDR         (0x88)
#define   PT_CLEAR_REG        (0xF0)  //清除寄存器
#define   PT_MUTE_ENABLE      (0x77)  //静音
#define   PT_MUTE_DISABLE     (0x74)  //取消静音
#define   PT_VOL_CTRL_1       (0xD0)  //以1为步进控制音量
#define   PT_VOL_CTRL_10      (0xE0)  //以10为步进控制音量



/* 内部函数-------------------------------------------------------------- */
static SL_I2C_BIT_OPS_TYPE I2C_BitOps = {0};
static SL_I2C_BUS_DEV_TYPE PT_I2C_BUS = {0};
static SL_DEVICE_TYPE *PT_I2C_DEV = &PT_I2C_BUS.Parent;



//IIC初始化
void PT2259_HwCtrlInterFaces (void    (*PT_PortInit)(void),
                              void    *Data,
                              void    (*SetSDA)(void *Data, uint8_t State),
                              void    (*SetSCL)(void *Data, uint8_t State),
                              uint8_t (*GetSDA)(void *Data),
                              uint8_t (*GetSCL)(void *Data),
                              void    (*BitDelayUs)(uint16_t Us))
{
  uint16_t DevAddr = PT2259_ADDR;
  
  I2C_BitOps.SetSDA = SetSDA;
  I2C_BitOps.SetSCL = SetSCL;
  I2C_BitOps.GetSDA = GetSDA;
  I2C_BitOps.GetSCL = GetSCL;
  I2C_BitOps.BitDelayUs = BitDelayUs;
  I2C_BitOps.DelayTime = 5; //5Us
  I2C_BitOps.iTimeOut = 100; //100MS
  I2C_BitOps.Data = Data;
  
  PT_PortInit();  //Init hardware GPIO
  
  SL_I2C_BusDevieInit(&PT_I2C_BUS, "PT2259");
  
  SL_Device_Control(PT_I2C_DEV, SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS, SL_NULL);
  SL_Device_Control(PT_I2C_DEV, SL_I2C_DEV_CTRL_SET_BIT_OPS, &I2C_BitOps);
  SL_Device_Control(PT_I2C_DEV, SL_I2C_DEV_CTRL_SET_TIMEOUT, &I2C_BitOps.iTimeOut);
  SL_Device_Control(PT_I2C_DEV, SL_I2C_DEV_CTRL_SET_DEV_ADDR, &DevAddr);
  SL_Device_Control(PT_I2C_DEV, SL_I2C_DEV_CTRL_NO_REG_ADDR, SL_NULL);
  
  SL_Device_Open(PT_I2C_DEV, 0);
  
}



//Set pt2259 ctrl function
static void PT2259_I2C_Control(uint8_t Code)
{
  //send command
  SL_Device_Write(PT_I2C_DEV, SL_NULL, &Code, 1);
  
}


//Set audio volume, the unit is dB, valid data range: -79~0(dB)
int8_t PT2259_SetVolume(int8_t AudiodB)
{
  uint8_t AudioCmd[2] = {0};
  
  //data range limit
  AudiodB = (AudiodB > 0) ? 0 : AudiodB;
  AudiodB = (AudiodB < -79) ? -79 : AudiodB;
  
  //package I2C data
  AudioCmd[0] = PT_VOL_CTRL_10 | (((-AudiodB)/10) & 0x07);
  AudioCmd[1] = PT_VOL_CTRL_1  | ((-AudiodB)%10);
  
  //send command
  SL_Device_Write(PT_I2C_DEV, SL_NULL, AudioCmd, 2);
  
  return AudiodB;
}


//mute or unmute
void PT2259_Mute(uint8_t isMute)
{
  PT2259_I2C_Control(isMute ? PT_MUTE_ENABLE : PT_MUTE_DISABLE);
  
}


//clear pt2259 regiser to default
void PT2259_ClearRegister(void)
{
  PT2259_I2C_Control(PT_CLEAR_REG);
  
}


//Init pt2259
void PT2259_deviceInit(void)
{
  PT2259_ClearRegister();
  PT2259_Mute(0);
  PT2259_SetVolume(-79);
  
}










