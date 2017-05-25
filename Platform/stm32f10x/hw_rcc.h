#ifndef  __HW_RCC_H
#define  __HW_RCC_H

#ifdef __cplusplus
extern "C" {
#endif
  
  void RCC_EnableClock(void *PERI_ADDR, unsigned char isEnable);
  void RCC_ResetClock (void *PERI_ADDR, unsigned char isEnable);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_RCC_H */
