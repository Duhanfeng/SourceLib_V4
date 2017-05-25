#ifndef  __HW_NVIC_H
#define  __HW_NVIC_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif
  
  void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
  void NVIC_Disable(IRQn_Type IRQn);
  void NVIC_Config(void *PERI_ADDR, uint32_t PreemptPriority, uint32_t SubPriority);
  IRQn_Type NVIC_GetIRQType(void *PERI_ADDR);

#ifdef __cplusplus
}
#endif
  
#endif /* __HW_NVIC_H */
