#ifndef  __KER_TIMER_H
#define  __KER_TIMER_H

typedef unsigned int ClockTime;

typedef struct
{
  unsigned char isEnable;     //ʹ�ܱ�־
  unsigned char isOneshot;    //���μ�ʱ��־
  unsigned char Reserver[2];  //Ԥ������,����ȷ��ӳ�ṹ��Ĵ�С
  ClockTime  msDelay;         //��ʱʱ��
  ClockTime  msStart;         //��ʼִ�е�ʱ��

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
