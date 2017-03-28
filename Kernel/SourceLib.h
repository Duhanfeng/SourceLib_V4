#ifndef  __SOURCELIB_H
#define  __SOURCELIB_H

/*----------------------------------------------------------------------------
    ���ܲü�
 *----------------------------------------------------------------------------*/
#define USING_ASSERT    (1)   //ʹ��/��ֹ����
#define USING_SW_DEBUG  (1)
#define USING_I2C       (1)   
#define USING_SPI       (0)
#define USING_SERIAL    (0)


//�㷨����
#include "algorithm.h"

//�ں�ͷ�ļ�
#include "ker_timer.h"
#include "ker_typedef.h"
#include "ker_list.h"
#include "ker_device.h"

//ͨ�ÿ��
#if USING_I2C
#include "i2c.h"
#endif

//ƽ̨�ļ�
#include "hw_platform.h"
#include "hw_it_handler.h"


/*----------------------------------------------------------------------------
    ͨ�ù��ܺ�
 *----------------------------------------------------------------------------*/

/* ---���ù��ܺ�--- */
#define SL_ASSERT(EX)                                                         \
if (!(EX))                                                                    \
{                                                                             \
    SL_AssertHandler(#EX, __FUNCTION__, __LINE__);                            \
}

#define DEBUG_PRINT(...)    printf(__VA_ARGS__)
#define getBOOL( a )        ( (a) ? (1) : (0) )  //���ݲ�����


//�����Դ���
#define DelayUs(nus)  SL_DelayUs(nus)
#define DelayMs(nms)  SL_DelayMs(nms)

#define Delay_us(nus) SL_DelayUs(nus)
#define Delay_ms(nms) SL_DelayMs(nms)  


#ifdef __cplusplus
extern "C" {
#endif

/* ��ʱʵ��-------------------------------------------------------------- */
  void SL_DelayOperation(void (*SlDelayMs)(unsigned int MS),
                         void (*SlDelayUs)(unsigned int US));
  void SL_DelayMs(unsigned int MS);
  void SL_DelayUs(unsigned int US);

/* ��ʽ������ض���------------------------------------------------------ */
  void SL_PrintOperation(void (*SendByte)(unsigned char Data));
  
/* ����ʵ��-------------------------------------------------------------- */
  void SL_AssertHandler(const char* ex_string, const char* func, const int line);
  
#ifdef __cplusplus
}
#endif

#endif /* __SOURCELIB_H */
