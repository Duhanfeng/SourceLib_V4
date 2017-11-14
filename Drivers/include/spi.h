#ifndef  __SPI_H
#define  __SPI_H

#include "SourceLib.h"


#if USING_SPI


#ifdef __cplusplus
extern "C" {
#endif
  
  #define SL_SPI_WR                 (0u << 0)
  #define SL_SPI_REG_ADDR_16BIT     (1u << 1)  /* this is a 16 bit reg  address */

  
  typedef struct sl_spi_bus_device SL_SPI_BUS_DEV_TYPE;
  
  typedef struct sl_spi_bus_base_ops
  {
    uint8_t (*SPI_ReadWriteByte)(uint8_t cWriteData);
    void    (*SPI_SetNss)       (uint8_t State);
  }SL_SPI_BUS_BASE_OPS_TYPE;
  
  
  // SPI BUS CONFIG
  typedef struct sl_spi_configuration
  {
    uint8_t  Mode;
    uint8_t  DataWidth;
    uint16_t Reserved;
    uint32_t MaxHz;
  
  }SL_SPI_CONFIG_TYPE;

  // SPI BUS DEVICE OPERATIONS
  typedef struct sl_spi_bus_device_ops
  {
    uint32_t (*SPI_Transfer)(SL_SPI_BUS_DEV_TYPE *Bus, uint32_t iData);
    sl_err_t (*SPI_Config)  (SL_SPI_BUS_DEV_TYPE *Bus, SL_SPI_CONFIG_TYPE ConifgPriv);
    
  }SL_SPI_BUS_DEVICE_OPS_TYPE;
  
  // SPI BUS DRIVER
  typedef struct sl_spi_bus_device
  {
    SL_DEVICE_TYPE                      Parent;     //继承SL设备框架
    const SL_SPI_BUS_DEVICE_OPS_TYPE    *Ops;       //SPI操作函数
    SL_SPI_CONFIG_TYPE                  ConfigParam;//配置参数
    uint16_t                            nFlags;     //标志
    void                                *Priv;   
    
  }SL_SPI_BUS_DEV_TYPE;

  sl_err_t SL_SPI_BusDevieInit(struct sl_spi_bus_device *Bus, const char *name);


#ifdef __cplusplus
}
#endif




#endif /*  */

#endif /* __SPI_H */

