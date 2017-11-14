#ifndef  __HW_IWDG_H
#define  __HW_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

  void IWDG_Init    (void);    //独立看门狗初始化
  void IWDG_FeedDog (void);    //喂狗

#ifdef __cplusplus
}
#endif

#endif /* __HW_IWDG_H */
