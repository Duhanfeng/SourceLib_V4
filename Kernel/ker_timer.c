/**
  ******************************************************************************
  * @file    ker_timer.c
  * @author  Duhanfeng
  * @version V4.1
  * @date    2017.03.20
  * @brief   时间管理机制
  ******************************************************************************
  * @attention
  * 
  * V4.0------------
  * 修改描述: 实现时间管理机制,增加定时任务的功能(链表功能)
  * 后续功能: 修改链表机制,利用线性双向链表机制进行管理,按照触发时间来排序链表内容而非
  *           当前的按任务嵌入的先后进行排序.(待完成)
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2016.11.03
  * 
  * V4.1------------
  * 修改描述: 将UPDATE_TICK由宏改成变量SL_SysTick,并开放相关接口用于修改此值
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.03.20
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
// 抽象层时间管理机制
#include "ker_timer.h"
#include <stdlib.h>
#include <string.h>

#define CLOCK_TIME_MAX  (0xFFFFFFFF)  //32位,时钟变量位数
//#define SL_SysTick     (10)          //定时中断中执行的时间间隔(单位:MS)

static ClockTime SL_SysTick = 10;          //定时中断中执行的时间间隔(单位:MS)
static volatile ClockTime SL_ClockTime = 0; //内部计时因子

// 计算两个数值之间值的差(若溢出,则进行相应的处理)
static ClockTime SLTimer_GetTimeDiffMs(ClockTime StartTime, ClockTime FinalTime)
{
  if (FinalTime >= StartTime)
  {
    return (FinalTime - StartTime);
  }

  return ((CLOCK_TIME_MAX - StartTime + 1) + FinalTime);  // 补码处理
}


// 返回自启动以来的时间计数(约49.7天一个循环)
static ClockTime SLTimer_GetTotalElapsed(void)
{
  return SL_ClockTime;
}



// 此函数放在定时中断中执行, 每 SL_SysTick 毫秒执行一次
void SLTimer_Update(void)
{
  SL_ClockTime += SL_SysTick;

}


//设置滴答值
void SLTimer_SetTick(unsigned int Tick)
{
  SL_SysTick = Tick;
  
}


// 启动定时器
void SLTimer_Start(SLTimerType *timerInfo, ClockTime msDelay)
{
  timerInfo->isEnable  = 1;
  timerInfo->isOneshot = 0;
  timerInfo->msDelay = msDelay;
  timerInfo->msStart = SLTimer_GetTotalElapsed();
}



// 启动定时器(单次)
void SLTimer_StartOneShot(SLTimerType *timerInfo, ClockTime msDelay)
{
  timerInfo->isEnable  = 1;
  timerInfo->isOneshot = 1;
  timerInfo->msDelay = msDelay;
  timerInfo->msStart = SLTimer_GetTotalElapsed();
}



// 取消定时器
void SLTimer_Cancel(SLTimerType *timerInfo)
{
  timerInfo->isEnable  = 0;

}



// 检查定时器是否到期
// 1: 到期
// 0: 未到期
// 单次:  _____-_____  仅返回一次"1"
// 常规:  _____------  到期后一直返回"1"
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



// 延时函数(最大延时49.7天)
void SLTimer_Delay(ClockTime msDelay)
{
  SLTimerType timerInfo = {0};

  SLTimer_Start(&timerInfo, msDelay);
  while (!SLTimer_GetExpiredState(&timerInfo));

}



// 限时计数函数(配合while使用,可实现某段代码在一定时间内循环执行,超时则退出)
// 逻辑:  ----_----_----_ ..... 在上升沿时更新超时时间
// 1: 有效
// 0: 超时
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



//开始计时
void SLTimer_StartReckonByTime(SLTimerType *timerInfo)
{
  timerInfo->isEnable  = 1;
  timerInfo->isOneshot = 0;
  timerInfo->msDelay = 0;
  timerInfo->msStart = SLTimer_GetTotalElapsed();
  
}



//获取计数值
ClockTime SLTimer_GetReckonValue(SLTimerType *timerInfo)
{
  
  return SLTimer_GetTimeDiffMs(timerInfo->msStart, SLTimer_GetTotalElapsed());
}



/*----------------------------------------------------------------------------
    软件定时触发任务功能
 *----------------------------------------------------------------------------*/

typedef struct Timer_ItTaskNote
{
  struct Timer_ItTaskNote *pNext;
  struct Timer_ItTaskNote *pLast;
  SL_TIMER_CALLBACK_FUNC ptrFunc;
  SLTimerType timerInfo;
  
}TIMER_IT_TASK_NOTE;

static TIMER_IT_TASK_NOTE *HeadNodePtr = NULL; //首节点指针


//将节点嵌入链表尾部
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
    //指针偏移到链表尾部
    while (pHeadNode->pNext != NULL)
    {
      pHeadNode = pHeadNode->pNext;
    }

    //嵌入尾部
    pHeadNode->pNext = pTailNode;
    pTailNode->pLast = pHeadNode;
  }
  
}



//释放节点
static void LinkedList_FreeNode(TIMER_IT_TASK_NOTE *pCrtNote)
{
  //假如是首节点
  if (pCrtNote == HeadNodePtr)
  {
    HeadNodePtr = pCrtNote->pNext;  //首节点指向下一个节点
    
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



//注册软件定时中断任务
//每注册一个任务,需要开辟24个字节的栈空间,其空间将在任务执行完成后释放
unsigned char SLTimer_LoginSwItTask(SL_TIMER_CALLBACK_FUNC ptrFunc, ClockTime Period)
{
  TIMER_IT_TASK_NOTE *pNode = NULL;
  
  //分配空间(若分配不成功,则返回0)
  pNode = (TIMER_IT_TASK_NOTE *)malloc(sizeof(TIMER_IT_TASK_NOTE));
  
  if (pNode == NULL)
  {
    return 0;
  }
  
  //初始化结构体空间
  memset(pNode, 0, sizeof(TIMER_IT_TASK_NOTE));
  pNode->pNext = NULL;
  pNode->pLast = NULL;
  pNode->ptrFunc = ptrFunc;
  pNode->timerInfo.msDelay  = Period;
  
  SLTimer_StartOneShot(&pNode->timerInfo, Period);
  
  //嵌入链表
  LinkedList_SinkNode(pNode);
  
  return 1;
}



//处理任务
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
      pShadowNode->ptrFunc();  //执行注册的任务
      LinkedList_FreeNode(pShadowNode);  //释放节点
    }
  }

}


