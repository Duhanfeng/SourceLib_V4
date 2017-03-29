#ifndef  __SL_PT2259_H
#define  __SL_PT2259_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif

  void PT2259_HwCtrlInterFaces (void    (*PT_PortInit)(void),
                                void    *Data,
                                void    (*SetSDA)(void *Data, uint8_t State),
                                void    (*SetSCL)(void *Data, uint8_t State),
                                uint8_t (*GetSDA)(void *Data),
                                uint8_t (*GetSCL)(void *Data),
                                void    (*BitDelayUs)(uint16_t Us));
  void PT2259_deviceInit(void);
  int8_t PT2259_SetVolume(int8_t AudiodB);
  void PT2259_Mute(uint8_t isMute);
  void PT2259_ClearRegister(void);
  

#ifdef __cplusplus
}
#endif
                       
#endif /* __SL_PT2259_H */
