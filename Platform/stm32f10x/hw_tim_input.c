/**
  ******************************************************************************
  * @file    tim_input.c
  * @author  Duhanfeng
  * @version V4.1 寄存器版本
  * @date    2017.05.23
  * @brief   TIMx CAPTURE模式配置
  ******************************************************************************
  * @attention
  *
  *
  * TIM1:   CH1 --- PA8  复用推挽,50M
  *         CH2 --- PA9  复用推挽,50M
  *         CH3 --- PA10 复用推挽,50M
  *         CH4 --- PA11 复用推挽,50M
  *
  * TIM2:   CH1 --- PA0  复用推挽,50M
  *         CH2 --- PA1  复用推挽,50M
  *         CH3 --- PA2  复用推挽,50M
  *         CH4 --- PA3  复用推挽,50M
  *
  * TIM3:   CH1 --- PA6  复用推挽,50M
  *         CH2 --- PA7  复用推挽,50M
  *         CH3 --- PB0  复用推挽,50M
  *         CH4 --- PB1  复用推挽,50M
  *
  * TIM4:   CH1 --- PB6  复用推挽,50M
  *         CH2 --- PB7  复用推挽,50M
  *         CH3 --- PB8  复用推挽,50M
  *         CH4 --- PB9  复用推挽,50M
  *
  * TIM5:   CH1 --- PA0  复用推挽,50M
  *         CH2 --- PA1  复用推挽,50M
  *         CH3 --- PA2  复用推挽,50M
  *         CH4 --- PA3  复用推挽,50M
  *
  * TIM8:   CH1 --- PC6  复用推挽,50M
  *         CH2 --- PC7  复用推挽,50M
  *         CH3 --- PC8  复用推挽,50M
  *         CH4 --- PC9  复用推挽,50M
  * 
  * 配置,
  *      PWM输入捕获,每捕获到一个脉冲,读取TIMx->CCR1可以得到脉冲的周期时间,
  *      TIMx->CCR2可以得到脉冲的高电平时间
  * 参数,
  *      时钟:   72MHz
  * 
  * 有效电平: 默认高电平为有效电平  TIMx->CCER 中配置
  *
  *   
  * V3.0------------    
  * 修改描述: 开放定时器配置接口,传定时器的编号进来即可对此定时器配置成输入
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2016.07.15
  * 
  * V4.0------------    
  * 修改描述: 规范相关接口,兼容常规的输入捕获模式以及特殊的PWM输入模式
  * PWM输入:  该模式是输入捕获模式的一个特例,其特性是两个ICx被映射到同一个TIx输
  *           入....
  * 编码接口: ....
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2017.02.14
  *  
  * V4.1------------
  * 修改描述: 1.修改其配置机制,将其从指针数组的索引改成直接的外设指针访问
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
  * 
  * 
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_input.h"
#include "SourceLib.h"



//TIMx模式配置
static void TIMx_Input_ModeConfig(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_INPUT_MODE Mode)
{
  //开时钟
  RCC_EnableClock(TIMx, 1);
  
  //模式配置
  TIMx->CR1 &= ~TIM_CR1_CKD;  //时钟分频因子
  TIMx->CR1 |=  TIM_CR1_ARPE; //开启影子
  TIMx->CR1 &= ~TIM_CR1_CMS;  //边沿对齐模式
  TIMx->CR1 &= ~TIM_CR1_DIR;  //计数器向上计数
  TIMx->CR1 &= ~TIM_CR1_OPM;  //非单脉冲模式
  TIMx->CR1 &= ~TIM_CR1_URS;  //配置更新源--允许软件触发更新
  TIMx->CR1 &= ~TIM_CR1_UDIS; //允许更新事件
  
  //配置时序参数
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000);  //预分频器:分频,计数频率为1MHz
  TIMx->ARR = TIM_ARR_ARR;     //最大自动重装载值
  
  //选择工作时钟
  TIMx->SMCR &= ~TIM_SMCR_SMS;//关闭从模式,选择内部时钟
  
  //配置捕获模式
  if (Mode == TIMx_Input_Normal)
  {
    //ChMask 1
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; //以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC1S_0; //捕获TI1
    TIMx->CCER  &= ~TIM_CCER_CC1P;    //捕获上升沿
    TIMx->CCER  |=  (ChMask & 0x01) ? TIM_CCER_CC1E : 0x00;
    
    //ChMask 2
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1;//以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC2S_0; //捕获TI2
    TIMx->CCER  &= ~TIM_CCER_CC2P;    //捕获上升沿
    TIMx->CCER  |=  (ChMask & 0x02) ? TIM_CCER_CC2E : 0x00;
    
    //ChMask 3
    TIMx->CCMR2 &= ~TIM_CCMR2_IC3F;
    TIMx->CCMR2 |=  TIM_CCMR2_IC3F_0 | TIM_CCMR2_IC3F_1; //以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;
    TIMx->CCMR2 |=  TIM_CCMR2_CC3S_0; //捕获TI3
    TIMx->CCER  &= ~TIM_CCER_CC3P;    //捕获上升沿
    TIMx->CCER  |=  (ChMask & 0x04) ? TIM_CCER_CC3E : 0x00;
    
    //ChMask 4
    TIMx->CCMR2 &= ~TIM_CCMR2_IC4F;
    TIMx->CCMR2 |=  TIM_CCMR2_IC4F_0 | TIM_CCMR2_IC4F_1;//以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIMx->CCMR2 &= ~TIM_CCMR2_CC4S;
    TIMx->CCMR2 |=  TIM_CCMR2_CC4S_0; //捕获TI4
    TIMx->CCER  &= ~TIM_CCER_CC4P;    //捕获上升沿
    TIMx->CCER  |=  (ChMask & 0x08) ? TIM_CCER_CC4E : 0x00;
    
  }
  //PWM输入模式
  else if ((Mode == TIMx_Input_Pwm_1) || (Mode == TIMx_Input_Pwm_2))
  {
    //ChMask 1
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; //以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? TIM_CCMR1_CC1S_0 : TIM_CCMR1_CC1S_1); //捕获TI1/TI2
    TIMx->CCER  &= ~TIM_CCER_CC1P; //捕获上升沿
    
    //ChMask 2
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1;//以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIMx->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? TIM_CCMR1_CC2S_1 : TIM_CCMR1_CC2S_0); //捕获TI1/TI2
    TIMx->CCER  |=  TIM_CCER_CC2P; //捕获下降沿
    
    //Save mode config
    TIMx->SMCR  &= ~TIM_SMCR_TS;
    TIMx->SMCR  |=  TIM_SMCR_TS_0 | TIM_SMCR_TS_2; //TI1触发
    TIMx->SMCR  &= ~TIM_SMCR_SMS;
    TIMx->SMCR  |=  TIM_SMCR_SMS_2; //从模式:复位模式
    
    TIMx->CCER  |=  TIM_CCER_CC1E | TIM_CCER_CC2E;  //开启输入通道1,2
  }
  //编码器模式
  else if (Mode == TIMx_Input_Encode)
  {
    //配置编码器模式
    TIMx->CR2 &= ~TIM_CR2_TI1S;   //CH1脚连到TI1输入
    TIMx->CR2 &= ~TIM_CR2_MMS;
    TIMx->CR2 |=  TIM_CR2_MMS_0;  //主模式:使能

    TIMx->SMCR &= ~TIM_SMCR_ETF;
    TIMx->SMCR |=  TIM_SMCR_ETF_1;//触发滤波,每记录4个事件就输出一个跳变

    TIMx->SMCR &= ~TIM_SMCR_SMS;
    TIMx->SMCR |=  TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; //从模式: 编码器模式3 
    
    //输入端口捕获配置
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1F;   //输入捕获1无滤波器
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1PSC; //输入捕获1无预分频
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC1S_0; //输入,IC1映射到TI1上
    
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;   //输入捕获2无滤波器
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2PSC;
    TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC2S_0; //输入,IC2映射到TI2上
    
    //捕获端口使能
    TIMx->CCER  &= ~TIM_CCER_CC2P;    //输入不反向
    TIMx->CCER  |=  TIM_CCER_CC2E;    //CH2使能
    TIMx->CCER  &= ~TIM_CCER_CC1P;    //输入不反向
    TIMx->CCER  |=  TIM_CCER_CC1E;    //CH1使能
    
    TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(TIMx_FCLK);       //预分频器:不分频
    
  }
  
  //开启定时器
  TIMx->CR1 |=  TIM_CR1_CEN;
  
}


/**
  * @brief  定时器输入模式初始化
  * @param  Timer 定时器标号
  * @param  ChMask 要配置的通道掩码
  * @param  Mode 输入模式(普通/PWM输入/编码器模式)
  * @retval None
  */
void TIMx_Input_Init(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_INPUT_MODE Mode)
{
  //引脚配置
  TIMx_PortConfig(TIMx, ChMask, TIMx_Port_Input);

  //模式配置
  TIMx_Input_ModeConfig(TIMx, ChMask, Mode);
  
}



/**
  * @brief  定时器x 输出模式中断使能函数
  * @param  Mask 定时器通道掩码,位值为1时开启对应的通道
  * @retval None
  */
void TIMx_Input_IRQEnable(TIM_TypeDef *TIMx, uint8_t ChMask, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->DIER |= (ChMask<<1); //开外设中断
    NVIC_Config(TIMx, 2, 2);    //开内核中断
  }
  else 
  {
    NVIC_Disable(NVIC_GetIRQType(TIMx));
  }
  
}



/**
  * @brief  定时器输入模式初始化
  * @param  None
  * @retval 定时器计数寄存器值
  */
uint16_t TIMx_Input_GetCount(TIM_TypeDef *TIMx)
{
  
  return TIMx->CNT;
}



/**
  * @brief  定时器输入模式初始化
  * @param  None
  * @retval 定时器计数寄存器值
  */
uint16_t TIMx_Input_GetCaptureValue(TIM_TypeDef *TIMx, uint8_t ChMask)
{
  uint16_t CaptureValue = 0;
  
  switch (ChMask)
  {
    case 1: CaptureValue = TIMx->CCR1; break;
    case 2: CaptureValue = TIMx->CCR2; break;
    case 4: CaptureValue = TIMx->CCR3; break;
    case 8: CaptureValue = TIMx->CCR4; break;
    
    default: break;
  }
  
  return CaptureValue;
}

