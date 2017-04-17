#ifndef  __SL_W25Q64_H
#define  __SL_W25Q64_H

#include "SourceLib.h"


#ifdef __cplusplus
extern "C" {
#endif
  
  /* ---函数声明--- */
  void     W25Q64_HwCtrlInterFaces(void    (*W25Q64_PortInit)(void),
                                   void     *Data,
                                   uint8_t (*SPI_RW)(void *Data, uint8_t cWriteData),
                                   void    (*SetCS)(void *Data, uint8_t State));
  uint16_t W25Q64_ReadDeviceID    (void);  //获取W25Q64的设备ID.
  void     W25Q64_EraseChip       (void);  //擦除整一块FLASH的数据
  void     W25Q64_ReadMultiByte   (uint32_t r_addr, uint32_t num, uint8_t *r_buff);  //读取多字节的数据
  void     W25Q64_WriteMultiByte  (uint32_t w_addr, uint32_t num, const uint8_t *w_buff);  //写入多字节的数据

#ifdef __cplusplus
}
#endif
  
#endif
