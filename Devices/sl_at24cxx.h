#ifndef  __SL_AT24CXX_H
#define  __SL_AT24CXX_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum
  {
    AT24C01_PAGE_SIZE =  8,
    AT24C02_PAGE_SIZE =  8,
    AT24C04_PAGE_SIZE = 16,
    AT24C08_PAGE_SIZE = 16,
    AT24C16_PAGE_SIZE = 16,
    AT24C32_PAGE_SIZE = 32,
    AT24C64_PAGE_SIZE = 32,
    
  }AT24Cxx_PAGE_SIZE;


void AT24Cxx_HwCtrlInterFaces(void    (*AT_PortInit)(void),
                              void    *Data,
                              void    (*SetSDA)(void *Data, uint8_t State),
                              void    (*SetSCL)(void *Data, uint8_t State),
                              uint8_t (*GetSDA)(void *Data),
                              uint8_t (*GetSCL)(void *Data),
                              void    (*BitDelayUs)(uint16_t Us),
                              AT24Cxx_PAGE_SIZE    PAGE_SIZE);
  uint8_t AT24Cxx_RandomRead(uint16_t nAddr);
  void AT24Cxx_WriteByte(uint16_t nAddr, uint8_t cWriteData);
  void AT24Cxx_ReadMultiBytes(uint16_t nAddr, uint8_t *pReadBuff, uint32_t iNum);
  void AT24Cxx_WriteMultiBytes(uint16_t nAddr, const uint8_t *pWriteBuff, uint32_t iNum);


#ifdef __cplusplus
}
#endif
                                  

#endif  /* __AT24CXX_DEV_H */
