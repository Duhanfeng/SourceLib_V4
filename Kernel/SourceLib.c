/**
  ******************************************************************************
  * @file    SourceLib.c
  * @author  �Ź��Ӻ���
  * @version V4.0
  * @date    2017.03.09
  * @brief   �������ع���ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * V4.0------------
  * ��������: 1. ��ʱ�ص�����
  *           2. printf �����ĵײ�ӿ�
  *           3. ���Թ���ʵ��
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.0
  * �޸�����: 2016.11.03
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "SourceLib.h"
#include <stdio.h>


/*----------------------------------------------------------------------------
    ��ʱ��صĺ���
 *----------------------------------------------------------------------------*/


static struct SL_Delay_Ops
{
  void (*DelayMs)(unsigned int MS);
  void (*DelayUs)(unsigned int US);
  
}SL_SysDelayOps;



//��ʱ�����ص��ӿ�
void SL_DelayOperation(void (*SlDelayUs)(unsigned int US),
                       void (*SlDelayMs)(unsigned int MS))
{
  
  SL_SysDelayOps.DelayMs = SlDelayMs;
  SL_SysDelayOps.DelayUs = SlDelayUs;
  
}


//���뼶��ʱ����
void SL_DelayMs(unsigned int MS)
{
  if (SL_SysDelayOps.DelayMs != NULL)
  {
    (SL_SysDelayOps.DelayMs)(MS);
  }
  
}


//΢�뼶��ʱ����
void SL_DelayUs(unsigned int US)
{
  if (SL_SysDelayOps.DelayUs != NULL)
  {
    (SL_SysDelayOps.DelayUs)(US);
  }
}


/*----------------------------------------------------------------------------
    printf�����صĺ���
 *----------------------------------------------------------------------------*/

static void (*fSendByte)(unsigned char data) = NULL;

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000


/**
  * @brief  printf����
  * @param  Ҫ���͵�����
  * @param  �ļ�ָ��(û����)
  * @retval Ҫ���͵�����
  */
void SL_PrintOperation(void (*SendByte)(unsigned char Data))
{
  fSendByte = SendByte;
  
}


/**
  * @brief  �ض���c�⺯��printf��fSendByte
  * @param  Ҫ���͵�����
  * @param  �ļ�ָ��(û����)
  * @retval Ҫ���͵�����
  */
int fputc(int ch, FILE *f)
{
  if (fSendByte != NULL)
  {
    fSendByte(ch);
  }
  
  #if USING_SW_DEBUG
  //ͨ��SWģʽ�½��е�����Ϣ�Ĵ�ӡ
  
  if (DEMCR & TRCENA) 
	{
		while (ITM_Port32(0) == 0);
		ITM_Port8(0) = ch ;
	}
  #endif
  
	return ch;
}



/*----------------------------------------------------------------------------
    ����ʵ�ֺ���
 *----------------------------------------------------------------------------*/
void SL_AssertHandler(const char* ex_string, const char* func, const int line)
{
  #if (USING_ASSERT)
  
  DEBUG_PRINT("(%s) assertion failed at function: %s , line number:%d \n", ex_string, func, line);
  while(1);
  
  #endif
}
















