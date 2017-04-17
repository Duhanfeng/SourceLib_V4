#ifndef  __SL_W25Q64_H
#define  __SL_W25Q64_H

#include "SourceLib.h"


#ifdef __cplusplus
extern "C" {
#endif
  
  /* ---��������--- */
  void     W25Q64_HwCtrlInterFaces(void    (*W25Q64_PortInit)(void),
                                   void     *Data,
                                   uint8_t (*SPI_RW)(void *Data, uint8_t cWriteData),
                                   void    (*SetCS)(void *Data, uint8_t State));
  uint16_t W25Q64_ReadDeviceID    (void);  //��ȡW25Q64���豸ID.
  void     W25Q64_EraseChip       (void);  //������һ��FLASH������
  void     W25Q64_ReadMultiByte   (uint32_t r_addr, uint32_t num, uint8_t *r_buff);  //��ȡ���ֽڵ�����
  void     W25Q64_WriteMultiByte  (uint32_t w_addr, uint32_t num, const uint8_t *w_buff);  //д����ֽڵ�����

#ifdef __cplusplus
}
#endif
  
#endif
