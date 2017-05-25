#ifndef  __HW_SPI_H
#define  __HW_SPI_H

#include "stm32f10x.h"
#include "SourceLib.h"


#ifdef __cplusplus
extern "C" {
#endif
  
  void SPIx_Init(SPI_TypeDef *SPIx, SPIx_WORK_MODE Mode);
  void SPIx_SetSpeed(SPI_TypeDef *SPIx, SPIx_SPEED_DVI SpeedDvi);
  uint16_t SPIx_GetStatus(SPI_TypeDef *SPIx);
  void SPIx_WriteDataReg(SPI_TypeDef *SPIx, uint16_t cWriteData);
  uint16_t SPIx_ReadDataReg(SPI_TypeDef *SPIx);
  uint16_t SPIx_ReadWriteByte(SPI_TypeDef *SPIx, uint16_t cWriteData);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_SPI_H */
