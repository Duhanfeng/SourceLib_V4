#ifndef  __SL_WM8974_H
#define  __SL_WM8974_H

#include "SourceLib.h"

#ifdef __cplusplus
extern "C" {
#endif
  
  void WM8974_HwCtrlInterFaces (void    (*WM_PortInit)(void),
                                void    *Data,
                                void    (*SetSDA)(void *Data, uint8_t State),
                                void    (*SetSCL)(void *Data, uint8_t State),
                                uint8_t (*GetSDA)(void *Data),
                                uint8_t (*GetSCL)(void *Data),
                                void    (*BitDelayUs)(uint16_t Us));
  void WM8974_SetInpgaVolume(int8_t n_db);
  void WM8974_SetSpkVolCtrl(uint8_t flag);
  void WM8974_SetMonoVolCtrl(uint8_t flag);

#ifdef __cplusplus
}
#endif

#endif /* __SL_WM8974_H */
