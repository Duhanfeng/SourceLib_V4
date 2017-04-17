/**
  ******************************************************************************
  * @file    ker_timer.c
  * @author  �Ź��Ӻ���
  * @version V4.1
  * @date    2017.03.20
  * @brief   ʱ��������
  ******************************************************************************
  * @attention
  * 
  * V4.0------------
  * �޸�����: ʵ��ʱ��������,���Ӷ�ʱ����Ĺ���(������)
  * ��������: �޸��������,��������˫��������ƽ��й���,���մ���ʱ���������������ݶ���
  *           ��ǰ�İ�����Ƕ����Ⱥ��������.(�����)
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.0
  * �޸�����: 2016.11.03
  * 
  * V4.1------------
  * �޸�����: ��UPDATE_TICK�ɺ�ĳɱ���SL_SysTick,��������ؽӿ������޸Ĵ�ֵ
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.1
  * �޸�����: 2017.03.20
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
// �����ʱ��������
#include "ker_timer.h"
#include <stdlib.h>
#include <string.h>

#define CLOCK_TIME_MAX  (0xFFFFFFFF)  //32λ,ʱ�ӱ���λ��
//#define SL_SysTick     (10)          //��ʱ�ж���ִ�е�ʱ����(��λ:MS)

static ClockTime SL_SysTick = 10;          //��ʱ�ж���ִ�е�ʱ����(��λ:MS)
static volatile ClockTime SL_ClockTime = 0; //�ڲ���ʱ����

// ����������ֵ֮��ֵ�Ĳ�(�����,�������Ӧ�Ĵ���)
static ClockTime SLTimer_GetTimeDiffMs(ClockTime StartTime, ClockTime FinalTime)
{
  if (FinalTime >= StartTime)
  {
    return (FinalTime - StartTime);
  }

  return ((CLOCK_TIME_MAX - StartTime + 1) + FinalTime);  // ���봦��
}


// ����������������ʱ�����(Լ49.7��һ��ѭ��)
static ClockTime SLTimer_GetTotalElapsed(void)
{
  return SL_ClockTime;
}



// �˺������ڶ�ʱ�ж���ִ��, ÿ SL_SysTick ����ִ��һ��
void SLTimer_Update(void)
{
  SL_ClockTime += SL_SysTick;

}


//���õδ�ֵ
void SLTimer_SetTick(unsigned int Tick)
{
  SL_SysTick = Tick;
  
}


// ������ʱ��
void SLTimer_Start(SLTimerType *timerInfo, ClockTime msDelay)
{
  timerInfo->isEnable  = 1;
  timerInfo->isOneshot = 0;
  timerInfo->msDelay = msDelay;
  timerInfo->msStart = SLTimer_GetTotalElapsed();
}



// ������ʱ��(����)
void SLTimer_StartOneShot(SLTimerType *timerInfo, ClockTime msDelay)
{
  timerInfo->isEnable  = 1;
  timerInfo->isOneshot = 1;
  timerInfo->msDelay = msDelay;
  timerInfo->msStart = SLTimer_GetTotalElapsed();
}



// ȡ����ʱ��
void SLTimer_Cancel(SLTimerType *timerInfo)
{
  timerInfo->isEnable  = 0;

}



// ��鶨ʱ���Ƿ���
// 1: ����
// 0: δ����
// ����:  _____-_____  ������һ��"1"
// ����:  _____------  ���ں�һֱ����"1"
unsigned char SLTimer_GetExpiredState(SLTimerType *timerInfo)
{
  if (!timerInfo->isEnable)
  {
    return 0;
  }

  if (SLTimer_GetTimeDiffMs(timerInfo->msStart, SLTimer_GetTotalElapsed()) >= timerInfo->msDelay)
  {
    if (timerInfo->isOneshot)
    {
      timerInfo->isEnable = 0;
    }
    return 1;
  }

  return 0;
}



// ��ʱ����(�����ʱ49.7��)
void SLTimer_Delay(ClockTime msDelay)
{
  SLTimerType timerInfo = {0};

  SLTimer_Start(&timerInfo, msDelay);
  while (!SLTimer_GetExpiredState(&timerInfo));

}



// ��ʱ��������(���whileʹ��,��ʵ��ĳ�δ�����һ��ʱ����ѭ��ִ��,��ʱ���˳�)
// �߼�:  ----_----_----_ ..... ��������ʱ���³�ʱʱ��
// 1: ��Ч
// 0: ��ʱ
unsigned char SLTimer_SetLimitTime(ClockTime msOverTime)
{
  static SLTimerType timerInfo = {0};
  static unsigned char isValid = 0;

  if (!isValid)
  {
    SLTimer_Start(&timerInfo, msOverTime);
  }

  isValid = !SLTimer_GetExpiredState(&timerInfo);

  return isValid;
}



//��ʼ��ʱ
void SLTimer_StartReckonByTime(SLTimerType *timerInfo)
{
  timerInfo->isEnable  = 1;
  timerInfo->isOneshot = 0;
  timerInfo->msDelay = 0;
  timerInfo->msStart = SLTimer_GetTotalElapsed();
  
}



//��ȡ����ֵ
ClockTime SLTimer_GetReckonValue(SLTimerType *timerInfo)
{
  
  return SLTimer_GetTimeDiffMs(timerInfo->msStart, SLTimer_GetTotalElapsed());
}



/*----------------------------------------------------------------------------
    �����ʱ����������
 *----------------------------------------------------------------------------*/

typedef struct Timer_ItTaskNote
{
  struct Timer_ItTaskNote *pNext;
  struct Timer_ItTaskNote *pLast;
  SL_TIMER_CALLBACK_FUNC ptrFunc;
  SLTimerType timerInfo;
  
}TIMER_IT_TASK_NOTE;

static TIMER_IT_TASK_NOTE *HeadNodePtr = NULL; //�׽ڵ�ָ��


//���ڵ�Ƕ������β��
static void LinkedList_SinkNode(TIMER_IT_TASK_NOTE *pTailNode)
{
  TIMER_IT_TASK_NOTE *pHeadNode = HeadNodePtr;
  
  if (pHeadNode == NULL)
  {
    HeadNodePtr = pTailNode;
    
    HeadNodePtr->pLast = NULL;
    HeadNodePtr->pNext = NULL;
  }
  else 
  {
    //ָ��ƫ�Ƶ�����β��
    while (pHeadNode->pNext != NULL)
    {
      pHeadNode = pHeadNode->pNext;
    }

    //Ƕ��β��
    pHeadNode->pNext = pTailNode;
    pTailNode->pLast = pHeadNode;
  }
  
}



//�ͷŽڵ�
static void LinkedList_FreeNode(TIMER_IT_TASK_NOTE *pCrtNote)
{
  //�������׽ڵ�
  if (pCrtNote == HeadNodePtr)
  {
    HeadNodePtr = pCrtNote->pNext;  //�׽ڵ�ָ����һ���ڵ�
    
    if (HeadNodePtr != NULL)
    {
      HeadNodePtr->pLast = NULL;
    }
  }
  else 
  {
    pCrtNote->pLast->pNext = pCrtNote->pNext;
    
    if (pCrtNote->pNext != NULL)
    {
      pCrtNote->pNext->pLast = pCrtNote->pLast;
    }
  }
  
  free(pCrtNote);
}



//ע�������ʱ�ж�����
//ÿע��һ������,��Ҫ����24���ֽڵ�ջ�ռ�,��ռ佫������ִ����ɺ��ͷ�
unsigned char SLTimer_LoginSwItTask(SL_TIMER_CALLBACK_FUNC ptrFunc, ClockTime Period)
{
  TIMER_IT_TASK_NOTE *pNode = NULL;
  
  //����ռ�(�����䲻�ɹ�,�򷵻�0)
  pNode = (TIMER_IT_TASK_NOTE *)malloc(sizeof(TIMER_IT_TASK_NOTE));
  
  if (pNode == NULL)
  {
    return 0;
  }
  
  //��ʼ���ṹ��ռ�
  memset(pNode, 0, sizeof(TIMER_IT_TASK_NOTE));
  pNode->pNext = NULL;
  pNode->pLast = NULL;
  pNode->ptrFunc = ptrFunc;
  pNode->timerInfo.msDelay  = Period;
  
  SLTimer_StartOneShot(&pNode->timerInfo, Period);
  
  //Ƕ������
  LinkedList_SinkNode(pNode);
  
  return 1;
}



//��������
void SLTimer_SoftwareItTaskHandler(void)
{
  TIMER_IT_TASK_NOTE *pCurrentNode = HeadNodePtr;
  TIMER_IT_TASK_NOTE *pShadowNode = NULL;
  
  while (pCurrentNode != NULL)
  {
    pShadowNode = pCurrentNode;
    pCurrentNode = pCurrentNode->pNext;
    
    if (SLTimer_GetExpiredState(&pShadowNode->timerInfo))
    {
      pShadowNode->ptrFunc();  //ִ��ע�������
      LinkedList_FreeNode(pShadowNode);  //�ͷŽڵ�
    }
  }

}


