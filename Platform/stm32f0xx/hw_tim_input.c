/**
  ******************************************************************************
  * @file    tim_capture.c
  * @author  Duhanfeng
  * @version V4.0 寄存器版本
  * @date    2017.02.14
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
  * 有效电平: 默认高电平为有效电平  TIM[Timer]->CCER 中配置
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
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_input.h"
#include "SourceLib.h"



//TIMx模式配置
static void TIMx_Input_ModeConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK ChannelMask, TIMx_INPUT_MODE Mode)
{
  /* 开时钟 */
  TIMx_EnableClock(Timer);
  
  //mode configure
  TIM[Timer]->CR1 &= ~(0X3<<8); //时钟分频因子
  TIM[Timer]->CR1 |=  (0X1<<7); //开启影子
  TIM[Timer]->CR1 &= ~(0X3<<5); //边沿对齐模式
  TIM[Timer]->CR1 &= ~(0X1<<4); //计数器向上计数
  TIM[Timer]->CR1 &= ~(0X1<<3); //非单脉冲模式
  TIM[Timer]->CR1 &= ~(0X1<<2); //配置更新源--允许软件触发更新
  TIM[Timer]->CR1 &= ~(0X1<<1); //允许更新事件
  
  /* 配置时序参数 */
  TIM[Timer]->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000);  //预分频器:分频,计数频率为1MHz
  TIM[Timer]->ARR = 0XFFFF;     //最大自动重装载值
  
  /* 选择工作时钟 */
  TIM[Timer]->SMCR &= ~(0X7<<0);//关闭从模式,选择内部时钟
  
  /* 配置捕获模式 */
  if (Mode == TIMx_Input_Normal)
  {
    //Channel 1
    TIM[Timer]->CCMR1 &= ~(0xF<<4);
    TIM[Timer]->CCMR1 |=  (0x3<<4); //以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIM[Timer]->CCMR1 &= ~(0x3<<0);
    TIM[Timer]->CCMR1 |=  (0x1<<0); //捕获TI1
    TIM[Timer]->CCER  &= ~(0x1<<1); //捕获上升沿
    TIM[Timer]->CCER  |=  (ChannelMask & 0x01) ? (0x1<<0) : 0x00;
    
    //Channel 2
    TIM[Timer]->CCMR1 &= ~(0xF<<12);
    TIM[Timer]->CCMR1 |=  (0x3<<12);//以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIM[Timer]->CCMR1 &= ~(0x3<<8);
    TIM[Timer]->CCMR1 |=  (0x1<<8); //捕获TI2
    TIM[Timer]->CCER  &= ~(0x1<<5); //捕获上升沿
    TIM[Timer]->CCER  |=  (ChannelMask & 0x02) ? (0x1<<4) : 0x00;
    
    //Channel 3
    TIM[Timer]->CCMR2 &= ~(0xF<<4);
    TIM[Timer]->CCMR2 |=  (0x3<<4); //以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIM[Timer]->CCMR2 &= ~(0x3<<0);
    TIM[Timer]->CCMR2 |=  (0x1<<0); //捕获TI3
    TIM[Timer]->CCER  &= ~(0x1<<13); //捕获上升沿
    TIM[Timer]->CCER  |=  (ChannelMask & 0x04) ? (0x1<<8) : 0x00;
    
    //Channel 4
    TIM[Timer]->CCMR2 &= ~(0xF<<12);
    TIM[Timer]->CCMR2 |=  (0x3<<12);//以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIM[Timer]->CCMR2 &= ~(0x3<<8);
    TIM[Timer]->CCMR2 |=  (0x1<<8); //捕获TI4
    TIM[Timer]->CCER  &= ~(0x1<<9); //捕获上升沿
    TIM[Timer]->CCER  |=  (ChannelMask & 0x08) ? (0x1<<12) : 0x00;
    
  }
  //Pwm input mode
  else if ((Mode == TIMx_Input_Pwm_1) || (Mode == TIMx_Input_Pwm_2))
  {
    //Channel 1
    TIM[Timer]->CCMR1 &= ~(0xF<<4);
    TIM[Timer]->CCMR1 |=  (0x3<<4); //以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIM[Timer]->CCMR1 &= ~(0x3<<0);
    TIM[Timer]->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? (0x1<<0) : (0x2<<0)); //捕获TI1/TI2
    TIM[Timer]->CCER  &= ~(0x1<<1); //捕获上升沿
    
    //Channel 2
    TIM[Timer]->CCMR1 &= ~(0xF<<12);
    TIM[Timer]->CCMR1 |=  (0x3<<12);//以Fck_int频率采样,8次相同的捕获结果来确认电平
    TIM[Timer]->CCMR1 &= ~(0x3<<8);
    TIM[Timer]->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? (0x2<<8) : (0x1<<8)); //捕获TI1/TI2
    TIM[Timer]->CCER  |=  (0x1<<5); //捕获下降沿
    
    //Save mode config
    TIM[Timer]->SMCR  &= ~(0x7<<4);
    TIM[Timer]->SMCR  |=  (0x5<<4); //TI1触发
    TIM[Timer]->SMCR  &= ~(0x7<<0);
    TIM[Timer]->SMCR  |=  (0x4<<0); //从模式:复位模式
    
    TIM[Timer]->CCER  |=  (0x1<<0) | (0x1<<4);  //开启输入通道1,2
  }
  //Encode mode
  else if (Mode == TIMx_Input_Encode)
  {
    /* 配置编码器模式 */
    TIM[Timer]->CR2 &= ~(0X1<<7);  //CH1脚连到TI1输入
    TIM[Timer]->CR2 &= ~(0X7<<4);
    TIM[Timer]->CR2 |=  (0X1<<4);  //主模式:使能

    TIM[Timer]->SMCR &= ~(0XF<<8);
    TIM[Timer]->SMCR |=  (0X2<<8); //触发滤波,每记录4个事件就输出一个跳变

    TIM[Timer]->SMCR &= ~(0X7<<0);
    TIM[Timer]->SMCR |=  (0X3<<0); //从模式: 编码器模式3 
    
    //input port capture
    TIM[Timer]->CCMR1 &= ~(0XF<<(4+0));
    TIM[Timer]->CCMR1 |=  (0X0<<(4+0));  //输入捕获1无滤波器
    TIM[Timer]->CCMR1 &= ~(0X3<<(2+0));
    TIM[Timer]->CCMR1 |=  (0X3<<(2+0));  //输入捕获1无预分频
    TIM[Timer]->CCMR1 &= ~(0X1<<(0+0));
    TIM[Timer]->CCMR1 |=  (0X1<<(0+0));  //输入,IC1映射到TI1上
    
    TIM[Timer]->CCMR1 &= ~(0XF<<(4+8));
    TIM[Timer]->CCMR1 |=  (0X0<<(4+8));  //输入捕获2无滤波器
    TIM[Timer]->CCMR1 &= ~(0X3<<(2+8));
    TIM[Timer]->CCMR1 |=  (0X0<<(2+8));  //输入捕获2无预分频
    TIM[Timer]->CCMR1 &= ~(0X1<<(0+8));
    TIM[Timer]->CCMR1 |=  (0X1<<(0+8));  //输入,IC2映射到TI2上
    
    //capture port enable
    TIM[Timer]->CCER  &= ~(0X1<<5);      //输入不反向
    TIM[Timer]->CCER  |=  (0X1<<4);      //CH2使能
    TIM[Timer]->CCER  &= ~(0X1<<1);      //输入不反向
    TIM[Timer]->CCER  |=  (0X1<<0);      //CH1使能
    
    TIM[Timer]->PSC = TIM_GET_PSC_BY_CNT_FRE(TIMx_FCLK);       //预分频器:不分频
    
  }
  
  /* 开启定时器 */
  TIM[Timer]->CR1 |=  (0x1<<0);
  
}


/**
  * @brief  定时器输入模式初始化
  * @param  Timer 定时器标号
  * @param  ChannelMask 要配置的通道掩码
  * @param  Mode 输入模式(普通/PWM输入/编码器模式)
  * @retval None
  */
void TIMx_Input_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK ChannelMask, TIMx_INPUT_MODE Mode)
{
  /* 引脚配置 */
  TIMx_PortConfig(Timer, ChannelMask, TIMx_Port_Input);

  /* 模式配置 */
  TIMx_Input_ModeConfig(Timer, ChannelMask, Mode);
  
}

 

/**
  * @brief  定时器输入模式初始化
  * @param  None
  * @retval 定时器计数寄存器值
  */
uint16_t TIMx_Input_GetCount(TIM_TYPE Timer)
{
  
  return TIM[Timer]->CNT;
}




/**
  * @brief  定时器输入模式初始化
  * @param  None
  * @retval 定时器计数寄存器值
  */
uint16_t TIMx_Input_GetCaptureValue(TIM_TYPE Timer, TIMx_CHANNEL_MASK cChannel)
{
  uint16_t CaptureValue = 0;
  
  switch (cChannel)
  {
    case 1: CaptureValue = TIM[Timer]->CCR1; break;
    case 2: CaptureValue = TIM[Timer]->CCR2; break;
    case 4: CaptureValue = TIM[Timer]->CCR3; break;
    case 8: CaptureValue = TIM[Timer]->CCR4; break;
    
    default: break;
  }
  
  return CaptureValue;
}



