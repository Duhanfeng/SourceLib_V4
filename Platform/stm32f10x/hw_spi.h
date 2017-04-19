#ifndef  __HW_SPI_H
#define  __HW_SPI_H

#include "stm32f10x.h"
#include "SourceLib.h"

/* ---函数宏定义--- */
#define SPI1_ReadByte()              SPIx_ReadWriteByte(SPI_1, 0XFF)         //SPI1读操作函数
#define SPI1_WriteByte(cWriteData)   SPIx_ReadWriteByte(SPI_1, cWriteData)   //SPI1写操作函数
#define SPI2_ReadByte()              SPIx_ReadWriteByte(SPI_2, 0XFF)         //SPI2读操作函数
#define SPI2_WriteByte(cWriteData)   SPIx_ReadWriteByte(SPI_2, cWriteData)   //SPI2写操作函数
#define SPI3_ReadByte()              SPIx_ReadWriteByte(SPI_3, 0XFF)         //SPI3读操作函数
#define SPI3_WriteByte(cWriteData)   SPIx_ReadWriteByte(SPI_3, cWriteData)   //SPI3写操作函数

/* ---SPI片选脚定义--- */
#define SPI1_NSS  PAout(4)
#define SPI2_NSS  PBout(12)
#define SPI3_NSS  PAout(15)

/* SPI端口号定义 */
typedef enum
{
  SPIx_1 = 0,
  SPIx_2 = 1,
  SPIx_3 = 2,
  SPIx_COUNT,
}SPI_TYPE;

#ifdef __cplusplus
extern "C" {
#endif
  
  void SPIx_IOConfig(SPI_TYPE Port, SPIx_WORK_MODE Mode);
  void SPIx_Init(SPI_TYPE Port, SPIx_WORK_MODE Mode);
  void SPIx_SetSpeed(SPI_TYPE Port, SPIx_SPEED_DVI SpeedDvi);
  void SPIx_WriteDataReg(SPI_TYPE Port, uint16_t cWriteData);
  uint16_t SPIx_ReadDataReg(SPI_TYPE Port);
  uint16_t SPIx_ReadWriteByte(SPI_TYPE Port, uint16_t cWriteData);

#ifdef __cplusplus
}
#endif

#endif /* __HW_SPI_H */
