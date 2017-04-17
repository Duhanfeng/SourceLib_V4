#ifndef  __KER_TIMER_H
#define  __KER_TIMER_H

typedef unsigned int ClockTime;

typedef struct
{
  unsigned char isEnable;     //使能标志
  unsigned char isOneshot;    //单次计时标志
  unsigned char Reserver[2];  //预留变量,以正确反映结构体的大小
  ClockTime  msDelay;         //延时时间
  ClockTime  msStart;         //开始执行的时刻

}SLTimerType;

typedef void (*SL_TIMER_CALLBACK_FUNC)(void);

#ifdef __cplusplus
extern "C" {
#endif

  void SLTimer_Update(void);
  void SLTimer_SetTick(unsigned int Tick);
  void SLTimer_Start(SLTimerType *timerInfo, ClockTime msDelay);
  void SLTimer_StartOneShot(SLTimerType *timerInfo, ClockTime msDelay);
  void SLTimer_Cancel(SLTimerType *timerInfo);
  unsigned char SLTimer_GetExpiredState(SLTimerType *timerInfo);
  void SLTimer_Delay(ClockTime msDelay);
  unsigned char SLTimer_SetLimitTime(ClockTime msOverTime);
  
  unsigned char SLTimer_LoginSwItTask(SL_TIMER_CALLBACK_FUNC ptrFunc, ClockTime Period);
  void SLTimer_SoftwareItTaskHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __KER_TIMER_H */
