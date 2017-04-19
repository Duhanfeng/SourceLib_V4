#ifndef  __HW_SPI_H
#define  __HW_SPI_H

#include "stm32f10x.h"
#include "SourceLib.h"

/* ---�����궨��--- */
#define SPI1_ReadByte()              SPIx_ReadWriteByte(SPI_1, 0XFF)         //SPI1����������
#define SPI1_WriteByte(cWriteData)   SPIx_ReadWriteByte(SPI_1, cWriteData)   //SPI1д��������
#define SPI2_ReadByte()              SPIx_ReadWriteByte(SPI_2, 0XFF)         //SPI2����������
#define SPI2_WriteByte(cWriteData)   SPIx_ReadWriteByte(SPI_2, cWriteData)   //SPI2д��������
#define SPI3_ReadByte()              SPIx_ReadWriteByte(SPI_3, 0XFF)         //SPI3����������
#define SPI3_WriteByte(cWriteData)   SPIx_ReadWriteByte(SPI_3, cWriteData)   //SPI3д��������

/* ---SPIƬѡ�Ŷ���--- */
#define SPI1_NSS  PAout(4)
#define SPI2_NSS  PBout(12)
#define SPI3_NSS  PAout(15)

/* SPI�˿ںŶ��� */
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
