#ifndef  __HW_PLATFORM_H
#define  __HW_PLATFORM_H

//#include "hw_io_bit.h"
#include "ker_timer.h"
#include "stm32f0xx.h"





/*----------------------------------------------------------------------------
    GPIOx 相关
 *----------------------------------------------------------------------------*/

typedef struct
{
  volatile unsigned int P0    : 1;
  volatile unsigned int P1    : 1;
  volatile unsigned int P2    : 1;
  volatile unsigned int P3    : 1;
  volatile unsigned int P4    : 1;
  volatile unsigned int P5    : 1;
  volatile unsigned int P6    : 1;
  volatile unsigned int P7    : 1;
  volatile unsigned int P8    : 1;
  volatile unsigned int P9    : 1;
  volatile unsigned int P10   : 1;
  volatile unsigned int P11   : 1;
  volatile unsigned int P12   : 1;
  volatile unsigned int P13   : 1;
  volatile unsigned int P14   : 1;
  volatile unsigned int P15   : 1;
  volatile unsigned int Reserver : 16; //[31:16]
  
}GPIOx_DR_CTRL;






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
  // 输入
  GPIOx_FLOAT     = 0x00,   //输入浮空    
  GPIOx_PUSH_UP   = 0x04,   //输入上拉    
  GPIOx_PUSH_DOWM = 0x08,   //输入下拉    

  // 通用输出
  GPIOx_GP_PP_L   = 0x40,   //通用推挽低速
  GPIOx_GP_PP_M   = 0x41,   //通用推挽中速
  GPIOx_GP_PP_H   = 0x43,   //通用推挽高速

  #if 0
  GPIOx_GP_OD_L   = 0x50,   //通用开漏低速
  GPIOx_GP_OD_M   = 0x51,   //通用开漏中速
  GPIOx_GP_OD_H   = 0x53,   //通用开漏高速
  #else 
  GPIOx_GP_OD_L   = 0x54,   //通用开漏低速(内部上拉)
  GPIOx_GP_OD_M   = 0x55,   //通用开漏中速(内部上拉)
  GPIOx_GP_OD_H   = 0x57,   //通用开漏高速(内部上拉)
  #endif

  // 复用输出
  #if 1
  GPIOx_AF_PP_L   = 0x80,   //复用推挽低速
  GPIOx_AF_PP_M   = 0x81,   //复用推挽中速
  GPIOx_AF_PP_H   = 0x83,   //复用推挽高速
  #else 
  GPIOx_AF_PP_L   = 0x88,   //复用推挽低速(内部下拉)
  GPIOx_AF_PP_M   = 0x89,   //复用推挽中速(内部下拉)
  GPIOx_AF_PP_H   = 0x8B,   //复用推挽高速(内部下拉)
  #endif
     
  GPIOx_AF_OD_L   = 0x90,   //复用开漏低速
  GPIOx_AF_OD_M   = 0x91,   //复用开漏中速
  GPIOx_AF_OD_H   = 0x93,   //复用开漏高速
             
  // 模拟模式
  GPIOx_ANALOG    = 0xC0,   //模拟模式    
  
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
  TIMx_1 = 0,
  TIMx_3,
  TIMx_6,
  TIMx_7,
  TIMx_14,
  TIMx_15,
  TIMx_16,
  TIMx_17,
  
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
#define SYSTEM_FCLK    (48000000)       //系统时钟总线
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
#define TIM_GET_PSC_BY_CNT_FRE(CntFre)            (TIMx_FCLK/(CntFre) - 1)   //通过计数频率计算预分频值
#define TIM_GET_PSC_BY_OP_FRE(OutFre, AutoLoad)   (TIM_GET_PSC_BY_CNT_FRE((OutFre) * (AutoLoad))) //通过输出频率计算预分频值(计数频率=输出频率*自动重装载值)



extern GPIOx_DR_CTRL *GPIOA_IN_CTRL;
extern GPIOx_DR_CTRL *GPIOB_IN_CTRL;
extern GPIOx_DR_CTRL *GPIOC_IN_CTRL;
extern GPIOx_DR_CTRL *GPIOD_IN_CTRL;
extern GPIOx_DR_CTRL *GPIOE_IN_CTRL;
extern GPIOx_DR_CTRL *GPIOF_IN_CTRL;

extern GPIOx_DR_CTRL *GPIOA_OUT_CTRL;
extern GPIOx_DR_CTRL *GPIOB_OUT_CTRL;
extern GPIOx_DR_CTRL *GPIOC_OUT_CTRL;
extern GPIOx_DR_CTRL *GPIOD_OUT_CTRL;
extern GPIOx_DR_CTRL *GPIOE_OUT_CTRL;
extern GPIOx_DR_CTRL *GPIOF_OUT_CTRL;


#ifdef __cplusplus
extern "C" {
#endif
  
  extern TIM_TypeDef * const STM32_TIMER[8];
  extern IRQn_Type const TIMx_IRQn[8];
  
  void System_Init(unsigned int Ticks);
  void System_SoftwareReset(void);
  void System_CoreClockConfigure(void);
  
  void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority);
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
