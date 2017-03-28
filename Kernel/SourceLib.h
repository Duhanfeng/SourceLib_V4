#ifndef  __SOURCELIB_H
#define  __SOURCELIB_H

/*----------------------------------------------------------------------------
    功能裁剪
 *----------------------------------------------------------------------------*/
#define USING_ASSERT    (1)   //使用/禁止断言
#define USING_SW_DEBUG  (1)
#define USING_I2C       (1)   
#define USING_SPI       (0)
#define USING_SERIAL    (0)


//算法声明
#include "algorithm.h"

//内核头文件
#include "ker_timer.h"
#include "ker_typedef.h"
#include "ker_list.h"
#include "ker_device.h"

//通用框架
#if USING_I2C
#include "i2c.h"
#endif

//平台文件
#include "hw_platform.h"
#include "hw_it_handler.h"


/*----------------------------------------------------------------------------
    通用功能宏
 *----------------------------------------------------------------------------*/

/* ---常用功能宏--- */
#define SL_ASSERT(EX)                                                         \
if (!(EX))                                                                    \
{                                                                             \
    SL_AssertHandler(#EX, __FUNCTION__, __LINE__);                            \
}

#define DEBUG_PRINT(...)    printf(__VA_ARGS__)
#define getBOOL( a )        ( (a) ? (1) : (0) )  //数据布尔化


//兼容性处理
#define DelayUs(nus)  SL_DelayUs(nus)
#define DelayMs(nms)  SL_DelayMs(nms)

#define Delay_us(nus) SL_DelayUs(nus)
#define Delay_ms(nms) SL_DelayMs(nms)  


#ifdef __cplusplus
extern "C" {
#endif

/* 延时实现-------------------------------------------------------------- */
  void SL_DelayOperation(void (*SlDelayMs)(unsigned int MS),
                         void (*SlDelayUs)(unsigned int US));
  void SL_DelayMs(unsigned int MS);
  void SL_DelayUs(unsigned int US);

/* 格式化输出重定向------------------------------------------------------ */
  void SL_PrintOperation(void (*SendByte)(unsigned char Data));
  
/* 断言实现-------------------------------------------------------------- */
  void SL_AssertHandler(const char* ex_string, const char* func, const int line);
  
#ifdef __cplusplus
}
#endif

#endif /* __SOURCELIB_H */
