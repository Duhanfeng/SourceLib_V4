/**
  ******************************************************************************
  * @file    SourceLib.c
  * @author  杜公子寒枫
  * @version V4.0
  * @date    2017.03.09
  * @brief   代码库相关功能实现文件
  ******************************************************************************
  * @attention
  * 
  * V4.0------------
  * 功能描述: 1. 延时回调函数
  *           2. printf 函数的底层接口
  *           3. 断言功能实现
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2016.11.03
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "SourceLib.h"
#include <stdio.h>


/*----------------------------------------------------------------------------
    延时相关的函数
 *----------------------------------------------------------------------------*/


static struct SL_Delay_Ops
{
  void (*DelayMs)(unsigned int MS);
  void (*DelayUs)(unsigned int US);
  
}SL_SysDelayOps;



//延时函数回调接口
void SL_DelayOperation(void (*SlDelayUs)(unsigned int US),
                       void (*SlDelayMs)(unsigned int MS))
{
  
  SL_SysDelayOps.DelayMs = SlDelayMs;
  SL_SysDelayOps.DelayUs = SlDelayUs;
  
}


//毫秒级延时函数
void SL_DelayMs(unsigned int MS)
{
  if (SL_SysDelayOps.DelayMs != NULL)
  {
    (SL_SysDelayOps.DelayMs)(MS);
  }
  
}


//微秒级延时函数
void SL_DelayUs(unsigned int US)
{
  if (SL_SysDelayOps.DelayUs != NULL)
  {
    (SL_SysDelayOps.DelayUs)(US);
  }
}


/*----------------------------------------------------------------------------
    printf输出相关的函数
 *----------------------------------------------------------------------------*/

static void (*fSendByte)(unsigned char data) = NULL;

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000


/**
  * @brief  printf函数
  * @param  要发送的数据
  * @param  文件指针(没用上)
  * @retval 要发送的数据
  */
void SL_PrintOperation(void (*SendByte)(unsigned char Data))
{
  fSendByte = SendByte;
  
}


/**
  * @brief  重定向c库函数printf到fSendByte
  * @param  要发送的数据
  * @param  文件指针(没用上)
  * @retval 要发送的数据
  */
int fputc(int ch, FILE *f)
{
  if (fSendByte != NULL)
  {
    fSendByte(ch);
  }
  
  #if USING_SW_DEBUG
  //通过SW模式下进行调试信息的打印
  
  if (DEMCR & TRCENA) 
	{
		while (ITM_Port32(0) == 0);
		ITM_Port8(0) = ch ;
	}
  #endif
  
	return ch;
}



/*----------------------------------------------------------------------------
    断言实现函数
 *----------------------------------------------------------------------------*/
void SL_AssertHandler(const char* ex_string, const char* func, const int line)
{
  #if (USING_ASSERT)
  
  DEBUG_PRINT("(%s) assertion failed at function: %s , line number:%d \n", ex_string, func, line);
  while(1);
  
  #endif
}
















