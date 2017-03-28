#ifndef  __HW_PLATFORM_H
#define  __HW_PLATFORM_H

#include "hw_io_bit.h"
#include "ker_timer.h"
#include "stm32f10x.h"

/*----------------------------------------------------------------------------
    GPIOx ���
 *----------------------------------------------------------------------------*/

/* ---����IO�˿�--- */
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


/* ---����IO����--- */
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


/* ---����IOģʽ--- */
typedef enum
{
  GPIOx_ANALOG    = 0x0U,  //ģ������
  GPIOx_GP_PP_10M = 0x1U,  //10Mͨ������
  GPIOx_GP_PP_2M  = 0x2U,  //2M ͨ������
  GPIOx_GP_PP_50M = 0x3U,  //50Mͨ������
  GPIOx_FLOAT     = 0x4U,  //��������
  GPIOx_GP_OD_10M = 0x5U,  //10Mͨ�ÿ�©
  GPIOx_GP_OD_2M  = 0x6U,  //2M ͨ�ÿ�©
  GPIOx_GP_OD_50M = 0x7U,  //50Mͨ�ÿ�©
  
  GPIOx_PU_PD     = 0x8U,  //��/��������(������)
  
  GPIOx_PUSH_UP   = 0x8U,  //��������
  GPIOx_AF_PP_10M = 0x9U,  //10M��������
  GPIOx_AF_PP_2M  = 0xAU,  //2M ��������
  GPIOx_AF_PP_50M = 0xBU,  //50M��������
  GPIOx_PUSH_DOWM = 0xCU,  //��������
  GPIOx_AF_OD_10M = 0xDU,  //10M���ÿ�©
  GPIOx_AF_OD_2M  = 0xEU,  //2M ���ÿ�©
  GPIOx_AF_OD_50M = 0xFU,  //50M���ÿ�©
  
}GPIOx_MODE;


/*----------------------------------------------------------------------------
    EXTIx ���
 *----------------------------------------------------------------------------*/

/* ---����EXTI��������--- */
typedef enum 
{
  EXTIx_R_TRG  = 0x1, //�����ش���
  EXTIx_F_TRG  = 0x2, //�½��ش���
  EXTIx_RF_TRG = 0x3, //�������½��ش���
  
}EXTIx_TRIGGER;



/*----------------------------------------------------------------------------
    SPIx ���
 *----------------------------------------------------------------------------*/

/* ---����SPI��Ƶϵ��--- */
typedef enum
{
  SPIx_SPEED_DIV2   = 0X0,   //2��Ƶ(SPI1������2��Ƶ)
  SPIx_SPEED_DIV4   = 0X1,   //4��Ƶ
  SPIx_SPEED_DIV8   = 0X2,   //8��Ƶ
  SPIx_SPEED_DIV16  = 0X3,   //16��Ƶ
  SPIx_SPEED_DIV32  = 0X4,   //32��Ƶ
  SPIx_SPEED_DIV64  = 0X5,   //64��Ƶ
  SPIx_SPEED_DIV128 = 0X6,   //128��Ƶ
  SPIx_SPEED_DIV256 = 0X7,   //256��Ƶ
  
}SPIx_SPEED_DVI;



/*----------------------------------------------------------------------------
    TIMx ���
 *----------------------------------------------------------------------------*/

/* ---����TIM���--- */
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


/* ---����TIMͨ������--- */
typedef enum
{
  TIMx_CH1 = 0X1,   //2��Ƶ(SPI1������2��Ƶ)
  TIMx_CH2 = 0x2,
  TIMx_CH3 = 0x4,
  TIMx_CH4 = 0x8,
  TIMx_CH_ALL = 0xF,
  
}TIMx_CHANNEL_MASK;



/* ---����TIM���ģʽ--- */
typedef enum
{
  TIMx_Output_Compare  = 3,
  TIMx_Output_Pwm_1    = 6,
  TIMx_Output_Pwm_2    = 7,
  
}TIMx_OUTPUT_MODE;



/* ---����TIM����ģʽ--- */
typedef enum
{
  TIMx_Input_Normal  = 0,
  TIMx_Input_Pwm_1   = 1,
  TIMx_Input_Pwm_2   = 2,
  TIMx_Input_Encode  = 3,
  
  
}TIMx_INPUT_MODE;


/* ---����TIM����ģʽ--- */
typedef enum
{
  TIMx_Port_Output = 0,
  TIMx_Port_Input,
  
}TIMx_PORT_DIRECTION;


/* ---����TIM�ж�����--- */
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
    �ں�/ȫ����غ�
 *----------------------------------------------------------------------------*/

/* ---ʱ�����߶���--- */
#ifndef SYSTEM_FCLK
#define SYSTEM_FCLK    (72000000)       //ϵͳʱ������
#endif

#ifndef AHB_FCLK
#define AHB_FCLK       SYSTEM_FCLK      //����������
#endif

#ifndef APB2_FCLK
#define APB2_FCLK      SYSTEM_FCLK      //��������ʱ������(���72M)
#endif

#ifndef APB1_FCLK
#define APB1_FCLK      (SYSTEM_FCLK/2)  //��������ʱ������(���36M)
#endif

#ifndef TIMx_FCLK
#define TIMx_FCLK      SYSTEM_FCLK      //��������ʱ������2��Ƶ(���72M)
#endif


/*----------------------------------------------------------------------------
    �ں�NVIC��غ�
 *----------------------------------------------------------------------------*/

/* ---ȫ���жϷ���--- */
#define NVIC_PRIORITY_GROUP_0   (7 - 0)
#define NVIC_PRIORITY_GROUP_1   (7 - 1)
#define NVIC_PRIORITY_GROUP_2   (7 - 2)
#define NVIC_PRIORITY_GROUP_3   (7 - 3)
#define NVIC_PRIORITY_GROUP_4   (7 - 4)
#define NVIC_PRIORITY_GROUP	    (NVIC_PRIORITY_GROUP_2)      //����Ϊ��2


/*----------------------------------------------------------------------------
    ��ʱ����غ�
 *----------------------------------------------------------------------------*/

#define TIM   STM32_TIMER


/* ---��ʱ����Ƶϵ�������--- */
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
