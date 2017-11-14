#ifndef __I2C_H__
#define __I2C_H__

#if USING_I2C

#include <SourceLib.h>
#include "ker_device.h"

#ifdef SL_I2C_DEBUG
#define I2C_DEBUG_PRINT(...)   DEBUG_PRINT(__VA_ARGS__)
#else
#define I2C_DEBUG_PRINT(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

  #define SL_I2C_WR                 (0u << 0)
  #define SL_I2C_RD                 (1u << 0)
  #define SL_I2C_DEV_ADDR_16BIT     (1u << 1)  /* this is a 16 bit chip address */
  #define SL_I2C_REG_ADDR_16BIT     (1u << 2)  /* this is a 16 bit reg  address */
  #define SL_I2C_NO_DEV_ADDR        (1u << 3)
  #define SL_I2C_NO_REG_ADDR        (1u << 4)
  #define SL_I2C_IGNORE_NACK        (1u << 5) 

  typedef enum
  {
    SL_I2C_DEV_CTRL_CLEAR_ALL_FLAGS = 0xA0,
    SL_I2C_DEV_CTRL_DEV_ADDR_8BIT   ,
    SL_I2C_DEV_CTRL_DEV_ADDR_16BIT  ,
    SL_I2C_DEV_CTRL_NO_DEV_ADDR     ,
    SL_I2C_DEV_CTRL_REG_ADDR_8BIT   ,
    SL_I2C_DEV_CTRL_REG_ADDR_16BIT  ,
    SL_I2C_DEV_CTRL_NO_REG_ADDR     ,
    SL_I2C_DEV_CTRL_SET_TIMEOUT     ,
    SL_I2C_DEV_CTRL_SET_RETRIES     ,
    SL_I2C_DEV_CTRL_SET_DEV_ADDR    ,
    SL_I2C_DEV_CTRL_SET_BIT_OPS     ,
    
  }SL_I2C_DEV_CTRL_TYPE;
  
    
  // I2C BIT OPS
  typedef struct sl_i2c_bit_ops
  {
    void    (*Data);    //私有数据 
    void    (*SetSDA)(void *Data, uint8_t State);
    void    (*SetSCL)(void *Data, uint8_t State);
    uint8_t (*GetSDA)(void *Data);
    uint8_t (*GetSCL)(void *Data);
  
    void    (*BitDelayUs)(uint16_t Us);
    
    uint16_t DelayTime; //单位: 微秒 
    uint32_t iTimeOut;  //单位: 毫秒
    
  }SL_I2C_BIT_OPS_TYPE;


  // I2C BUG BASE OPERATIONS
  typedef struct sl_i2c_bus_base_ops
  {
    void    (*I2C_Start)         (SL_I2C_BIT_OPS_TYPE *Ops);
    void    (*I2C_Stop)          (SL_I2C_BIT_OPS_TYPE *Ops);
    uint8_t (*I2C_WaitAck)       (SL_I2C_BIT_OPS_TYPE *Ops);
    void    (*I2C_SendAckOrNack) (SL_I2C_BIT_OPS_TYPE *Ops, uint8_t ack);
    uint8_t (*I2C_SendByte)      (SL_I2C_BIT_OPS_TYPE *Ops, uint8_t cByte);
    uint8_t (*I2C_RecvByte)      (SL_I2C_BIT_OPS_TYPE *Ops);
    void     *Priv;     //预留
    
  }SL_I2C_BUS_BASE_OPS_TYPE;


  typedef struct sl_i2c_bus_device SL_I2C_BUS_DEV_TYPE;


  // I2C BUS DEVICE OPERATIONS
  typedef struct sl_i2c_bus_device_ops
  {
    sl_err_t (*I2C_Master_SendBuff)(SL_I2C_BUS_DEV_TYPE *Bus, 
                               uint16_t nRegAddr, const uint8_t *pBuff, uint32_t iCount);
    sl_err_t (*I2C_Master_RecvBuff)(SL_I2C_BUS_DEV_TYPE *Bus, 
                               uint16_t nRegAddr, uint8_t *pBuff, uint32_t iCount);
    sl_err_t (*I2C_Master_Control)(void);
    
  }SL_I2C_BUS_DEVICE_OPS_TYPE;



  // I2C BUS DRIVER
  typedef struct sl_i2c_bus_device
  {
    SL_DEVICE_TYPE                      Parent; //继承SL设备框架
    const SL_I2C_BUS_DEVICE_OPS_TYPE    *Ops;
    
    uint16_t        nFlags;   //标志位,可实现设备地址8/16位模式,寄存器地址8/16位模式
    uint16_t        nDevAddr; //设备地址
                    
    uint32_t        iTimeOut;
    uint32_t        iRetries;
    void            *Priv_BaseOps;  //指向 SL_I2C_BUS_BASE_OPS_TYPE 结构体
    void            *Priv_BitOps;   //指向 SL_I2C_BIT_OPS_TYPE 结构体
    
  }SL_I2C_BUS_DEV_TYPE;

  sl_err_t SL_I2C_BusDevieInit(struct sl_i2c_bus_device *Bus, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* USING_I2C */

#endif /* __I2C_H__ */
