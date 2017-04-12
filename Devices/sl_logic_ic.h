#ifndef  __SL_LOGIC_H
#define  __SL_LOGIC_H

#include "SourceLib.h"

typedef enum
{
  SWITCH_HC4051,
  SWITCH_HC4052,
  SWITCH_HC4053,

}HC405x_TYPE;


#ifdef __cplusplus
extern "C" {
#endif
  
  /*----------------------------------------------------------------------------
      74HC4052 4路双通道模拟开关
   *----------------------------------------------------------------------------*/
  //硬件控制接口
  void HC405x_HwCtrlInterFaces(void    (*HC_PortInit)(void),
                               void    *Data,
                               void    (*SetINH) (void *Data, uint8_t State),
                               void    (*SetSEL0)(void *Data, uint8_t State),
                               void    (*SetSEL1)(void *Data, uint8_t State),
                               void    (*SetSEL2)(void *Data, uint8_t State),
                               HC405x_TYPE DevType);
                               
  void HC405x_Enable(uint8_t isEnable);
  void HC405x_SetChannel(uint8_t cChannel);
  
  
  /*----------------------------------------------------------------------------
    74HC595 串转并芯片
   *----------------------------------------------------------------------------*/                   
  void HC595_HwCtrlInterFaces(void    (*HC_PortInit)(void),
                              void    *Data,
                              void    (*SetSHCP)(void *Data, uint8_t State),
                              void    (*SetSTCP)(void *Data, uint8_t State),
                              void    (*SetDATA)(void *Data, uint8_t State),
                              void    (*HC_DelayUs)(uint16_t Us),
                              uint8_t ParBitNum);
  void HC595_OutputParallelData(uint32_t iSerialData);
  
#ifdef __cplusplus
}
#endif


#endif /* __SL_LOGIC_H */
