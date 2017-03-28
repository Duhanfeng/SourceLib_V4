#ifndef  __HW_PLATFORM_H
#define  __HW_PLATFORM_H

#include "hw_io_bit.h"
#include "ker_timer.h"
#include "stm32f10x.h"

/*----------------------------------------------------------------------------
    GPIOx 相关
 *----------------------------------------------------------------------------*/

/* ---定义IO端口--- */
typedef enum
{
  GPIOx_PA = 0x0,
  GPIOx_PB = 0x1,
  GPIOx_PC = 0x2,
  GPIOx_PD = 0x3,
  GPIOx_PE = 0x4,
  GPIOx_PF = 0x5,
  GPIOx_PG = 0x6,
  
}GPIOx_PORT;


/* ---定义IO引脚--- */
typedef enum
{
  GPIOx_0  = 0,    
  GPIOx_1  = 1,
  GPIOx_2  = 2,    
  GPIOx_3  = 3,
  GPIOx_4  = 4,    
  GPIOx_5  = 5,
  GPIOx_6  = 6,    
  GPIOx_7  = 7,
  GPIOx_8  = 8,    
  GPIOx_9  = 9,
  GPIOx_10 = 10,  
  GPIOx_11 = 11,
  GPIOx_12 = 12,  
  GPIOx_13 = 13,
  GPIOx_14 = 14,  
  GPIOx_15 = 15,
  
}GPIOx_PIN;


/* ---定义IO模式--- */
typedef enum
{
  GPIOx_ANALOG    = 0x0U,  //模拟输入
  GPIOx_GP_PP_10M = 0x1U,  //10M通用推挽
  GPIOx_GP_PP_2M  = 0x2U,  //2M 通用推挽
  GPIOx_GP_PP_50M = 0x3U,  //50M通用推挽
  GPIOx_FLOAT     = 0x4U,  //浮空输入
  GPIOx_GP_OD_10M = 0x5U,  //10M通用开漏
  GPIOx_GP_OD_2M  = 0x6U,  //2M 通用开漏
  GPIOx_GP_OD_50M = 0x7U,  //50M通用开漏
  
  GPIOx_PU_PD     = 0x8U,  //上/下拉输入(不调用)
  
  GPIOx_PUSH_UP   = 0x8U,  //上拉输入
  GPIOx_AF_PP_10M = 0x9U,  //10M复用推挽
  GPIOx_AF_PP_2M  = 0xAU,  //2M 复用推挽
  GPIOx_AF_PP_50M = 0xBU,  //50M复用推挽
  GPIOx_PUSH_DOWM = 0xCU,  //下拉输入
  GPIOx_AF_OD_10M = 0xDU,  //10M复用开漏
  GPIOx_AF_OD_2M  = 0xEU,  //2M 复用开漏
  GPIOx_AF_OD_50M = 0xFU,  //50M复用开漏
  
}GPIOx_MODE;


/*----------------------------------------------------------------------------
    EXTIx 相关
 *----------------------------------------------------------------------------*/

/* ---定义EXTI触发条件--- */
typedef enum 
{
  EXTIx_R_TRG  = 0x1, //上升沿触发
  EXTIx_F_TRG  = 0x2, //下降沿触发
  EXTIx_RF_TRG = 0x3, //上升及下降沿触发
  
}EXTIx_TRIGGER;



/*----------------------------------------------------------------------------
    SPIx 相关
 *----------------------------------------------------------------------------*/

/* ---定义SPI分频系数--- */
typedef enum
{
  SPIx_SPEED_DIV2   = 0X0,   //2分频(SPI1不可以2分频)
  SPIx_SPEED_DIV4   = 0X1,   //4分频
  SPIx_SPEED_DIV8   = 0X2,   //8分频
  SPIx_SPEED_DIV16  = 0X3,   //16分频
  SPIx_SPEED_DIV32  = 0X4,   //32分频
  SPIx_SPEED_DIV64  = 0X5,   //64分频
  SPIx_SPEED_DIV128 = 0X6,   //128分频
  SPIx_SPEED_DIV256 = 0X7,   //256分频
  
}SPIx_SPEED_DVI;



/*----------------------------------------------------------------------------
    TIMx 相关
 *----------------------------------------------------------------------------*/

/* ---定义TIM序号--- */
typedef enum
{
  TIMx_1,
  TIMx_2,
  TIMx_3,
  TIMx_4,
  TIMx_5,
  TIMx_6,
  TIMx_7,
  TIMx_8,
  
}TIM_TYPE;


/* ---定义TIM通道掩码--- */
typedef enum
{
  TIMx_CH1 = 0X1,   //2分频(SPI1不可以2分频)
  TIMx_CH2 = 0x2,
  TIMx_CH3 = 0x4,
  TIMx_CH4 = 0x8,
  TIMx_CH_ALL = 0xF,
  
}TIMx_CHANNEL_MASK;



/* ---定义TIM输出模式--- */
typedef enum
{
  TIMx_Output_Compare  = 3,
  TIMx_Output_Pwm_1    = 6,
  TIMx_Output_Pwm_2    = 7,
  
}TIMx_OUTPUT_MODE;



/* ---定义TIM输入模式--- */
typedef enum
{
  TIMx_Input_Normal  = 0,
  TIMx_Input_Pwm_1   = 1,
  TIMx_Input_Pwm_2   = 2,
  TIMx_Input_Encode  = 3,
  
  
}TIMx_INPUT_MODE;


/* ---定义TIM输入模式--- */
typedef enum
{
  TIMx_Port_Output = 0,
  TIMx_Port_Input,
  
}TIMx_PORT_DIRECTION;


/* ---定义TIM中断掩码--- */
typedef enum
{
  TIMx_IRQ_UIF   = 0x1,
  TIMx_IRQ_CC1IF = 0x2,
  TIMx_IRQ_CC2IF = 0x4,
  TIMx_IRQ_CC3IF = 0x8,
  TIMx_IRQ_CC4IF = 0x10,
  TIMx_IRQ_TIF   = 0x40,
  TIMx_IRQ_GIF   = 0x80,
  
}TIMx_IRQ_CODE;



/*----------------------------------------------------------------------------
    内核/全局相关宏
 *----------------------------------------------------------------------------*/

/* ---时钟总线定义--- */
#ifndef SYSTEM_FCLK
#define SYSTEM_FCLK    (72000000)       //系统时钟总线
#endif

#ifndef AHB_FCLK
#define AHB_FCLK       SYSTEM_FCLK      //高性能总线
#endif

#ifndef APB2_FCLK
#define APB2_FCLK      SYSTEM_FCLK      //高速外设时钟总线(最高72M)
#endif

#ifndef APB1_FCLK
#define APB1_FCLK      (SYSTEM_FCLK/2)  //低速外设时钟总线(最高36M)
#endif

#ifndef TIMx_FCLK
#define TIMx_FCLK      SYSTEM_FCLK      //低速外设时钟总线2倍频(最高72M)
#endif


/*----------------------------------------------------------------------------
    内核NVIC相关宏
 *----------------------------------------------------------------------------*/

/* ---全局中断分组--- */
#define NVIC_PRIORITY_GROUP_0   (7 - 0)
#define NVIC_PRIORITY_GROUP_1   (7 - 1)
#define NVIC_PRIORITY_GROUP_2   (7 - 2)
#define NVIC_PRIORITY_GROUP_3   (7 - 3)
#define NVIC_PRIORITY_GROUP_4   (7 - 4)
#define NVIC_PRIORITY_GROUP	    (NVIC_PRIORITY_GROUP_2)      //设置为组2


/*----------------------------------------------------------------------------
    定时器相关宏
 *----------------------------------------------------------------------------*/

#define TIM   STM32_TIMER


/* ---定时器分频系数计算宏--- */
#define TIM_GET_PSC(frequency, resolution)   ((TIMx_FCLK/(frequency*resolution))-1)


#ifdef __cplusplus
extern "C" {
#endif
  
  extern TIM_TypeDef * const STM32_TIMER[8];
  extern IRQn_Type const TIMx_IRQn[8];
  
  void System_Init(unsigned int Ticks);
  void System_SoftwareReset(void);
  void System_SetSysClockTo56(void);
  
  void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
  void NVIC_Disable(IRQn_Type IRQn);
  
  void SysTick_Init(uint16_t Ms);
  void GPIOx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, GPIOx_MODE Mode);
  void EXTIx_FastInit(GPIOx_PORT Port, GPIOx_PIN Pin, EXTIx_TRIGGER Trigger);
  
  void TIMx_EnableClock(TIM_TYPE Timer);
  void TIMx_PortConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_PORT_DIRECTION Direction);
  void TIMx_IQR_Enable(TIM_TYPE Timer, TIMx_IRQ_CODE NvicCode, uint8_t isEnable);
  
#ifdef __cplusplus
}
#endif


#endif /* __HW_PLATFORM_H */
