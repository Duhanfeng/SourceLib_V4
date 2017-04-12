/**
  ******************************************************************************
  * @file    tim_output.c
  * @author  杜公子寒枫
  * @version V4.0 寄存器版本
  * @date    2017.03.06
  * @brief   TIMx PWM模式配置
  ******************************************************************************
  * @attention
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
  *      开启影子寄存器,向上计数,非单脉冲模式,允许更新,PWM模式
  * 参数,
  *      时钟:   72MHz
  * 
  * 有效电平: 默认高电平为有效电平  TIM[Timer]->CCER 中配置
  *
  * 输出模式,
  *   输出比较: 每当CNT=CCR时,电平翻转.高低电平时间固定为一个周期
  *   PWM模式1: 在向上计数时，一旦CNT<CCRx时,通道为有效电平,否则为无效电平
  *   PWM模式2: 在向上计数时，一旦CNT<CCRx时,通道为无效电平,否则为有效电平
  *   当占空比一致时,PWM1和PWM2波形为互补波形
  *   
  * 注1: 定时器4个通道共用自动重装载和分频,即频率跟分辨率保持一致.但是占空比跟有效
  *      电平可以各自配置.
  *
  * 注2: PSC(预分频系数)=时钟/(频率*分辨率),右侧结果应为一个整数,否则实际的频率值
  *      会有误差(丢失小数部分).
  * 
  * 注3: 占空比=比较值/分辨率*100%; 如果要配置PWM波的占空比,可以通过调用函数
  *      TIMx_SetPwmDutyRatio()来实现.如果要针对于高分辨率高精度要求来进行配置,
  *      则可以根据分辨率通过接口函数修改其比较值来实现.
  * 
  * V2.0------------    
  * 修改描述: 1. 修改文件头描述,开放对自动重装载以及捕获/比较寄存器的接口函数.
  *           2. 将PWM模式与比较模式合并,开放输出模式来配置;
  *           3. 增加比较中断的接口,可以通过修改_TIMx_COMPARE_INT_的值来开关中断
  * 输出比较: 用于控制一个输出波形,或者是指示一段给定的时间已经到时了.
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.15
  *
  * V3.0------------    
  * 修改描述: 1. 修改函数机制,本文件可以用于TIM2~5的输出(比较/PWM),不同于tim_delay
  *              等文件需指定单个定时器并通过此定时器实现功能,本文件可以被多定时器
  *              同时调用,类似于usart.c与spi.c的实现机制.
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2016.08.25
  * 
  * V4.0------------    
  * 修改描述: 1. 修改函数机制,兼容TIM1~8
  *           2. 定时器的调用统一规范,部分功能的实现在hw_platform文件中
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2017.03.06
  *
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_output.h"
#include "SourceLib.h"



/* 内部函数-------------------------------------------------------------- */


//TIMx模式配置
static void TIMx_Output_ModeConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_OUTPUT_MODE Mode)
{
  /* 开时钟 */
  TIMx_EnableClock(Timer);
                             
  /* 配置工作模式 */          
  TIM[Timer]->CR1 |=  (0X1<<7);    //开启影子寄存器
  TIM[Timer]->CR1 &= ~(0X3<<5);
  TIM[Timer]->CR1 |=  (0X0<<5);    //边沿对齐模式
  TIM[Timer]->CR1 &= ~(0X1<<4);    //向上计数
  TIM[Timer]->CR1 &= ~(0X1<<3);    //非单脉冲模式
  TIM[Timer]->CR1 &= ~(0X1<<2);    //配置更新源
  TIM[Timer]->CR1 &= ~(0X1<<1);    //允许更新

  /* 选择工作时钟 */          
  TIM[Timer]->SMCR &= ~(0X7<<0);   //关闭从模式--内部时钟72M

  /* 配置中断 */              
  TIM[Timer]->DIER &= ~(0X1<<14);  //禁止触发DMA请求
  TIM[Timer]->DIER &= ~(0X1<< 0);  //关闭中断
  TIM[Timer]->EGR  |=  (0X1<<0);   //开启更新事件
  
  #if 0
  if (Def_IntFlag[Timer])  //根据标志位配置中断
  {
    TIM[Timer]->DIER |= (Channel<<1);  //开外设中断
    NVIC_Enable(TIMx_IRQn[Timer],2,2); //开内核中断
  }
  #endif
  
  /* 配置CH1 PWM模式 */
  TIM[Timer]->CCMR1 &= ~(0X1<<(7+0));  //OC1REF不受ETRF影响
  TIM[Timer]->CCMR1 &= ~(0X7<<(4+0));  
  TIM[Timer]->CCMR1 |=  (Mode<<(4+0)); //输出模式
  TIM[Timer]->CCMR1 |=  (0X1<<(3+0));  //输出比较1预装载使能
  TIM[Timer]->CCMR1 |=  (0X1<<(2+0));  //输出比较1快速使能
  TIM[Timer]->CCMR1 &= ~(0X3<<(0+0));  //CH1配置为输出模式

  /* 配置CH2 PWM模式 */
  TIM[Timer]->CCMR1 &= ~(0X1<<(7+8));  //OC2REF不受ETRF影响
  TIM[Timer]->CCMR1 &= ~(0X7<<(4+8));  
  TIM[Timer]->CCMR1 |=  (Mode<<(4+8)); //输出模式
  TIM[Timer]->CCMR1 |=  (0X1<<(3+8));  //输出比较2预装载使能
  TIM[Timer]->CCMR1 |=  (0X1<<(2+8));  //输出比较2快速使能
  TIM[Timer]->CCMR1 &= ~(0X3<<(0+8));  //CH2配置为输出模式
  
  /* 配置CH3 PWM模式 */
  TIM[Timer]->CCMR2 &= ~(0X1<<(7+0));  //OC3REF不受ETRF影响
  TIM[Timer]->CCMR2 &= ~(0X7<<(4+0));  
  TIM[Timer]->CCMR2 |=  (Mode<<(4+0)); //输出模式
  TIM[Timer]->CCMR2 |=  (0X1<<(3+0));  //输出比较3预装载使能
  TIM[Timer]->CCMR2 |=  (0X1<<(2+0));  //输出比较3快速使能
  TIM[Timer]->CCMR2 &= ~(0X3<<(0+0));  //CH3配置为输出模式

  /* 配置CH4 PWM模式 */
  TIM[Timer]->CCMR2 &= ~(0X1<<(7+8));  //OC4REF不受ETRF影响
  TIM[Timer]->CCMR2 &= ~(0X7<<(4+8));  
  TIM[Timer]->CCMR2 |=  (Mode<<(4+8)); //输出模式
  TIM[Timer]->CCMR2 |=  (0X1<<(3+8));  //输出比较4预装载使能
  TIM[Timer]->CCMR2 |=  (0X1<<(2+8));  //输出比较4快速使能
  TIM[Timer]->CCMR2 &= ~(0X3<<(0+8));  //CH4配置为输出模式
  
  /* 配置有效电平 */
  TIM[Timer]->CCER  &= ~(0X1<<(1+0) );  //CH1默认高电平为有效电平
  TIM[Timer]->CCER  &= ~(0X1<<(1+4) );  //CH2默认高电平为有效电平
  TIM[Timer]->CCER  &= ~(0X1<<(1+8) );  //CH3默认高电平为有效电平
  TIM[Timer]->CCER  &= ~(0X1<<(1+12));  //CH4默认高电平为有效电平
  
  /* 根据关键字配置输出 */
  TIM[Timer]->CCER |= (Channel & (0X1<<0)) ? (0X1<<(0+0) ) : 0;
  TIM[Timer]->CCER |= (Channel & (0X1<<1)) ? (0X1<<(0+4) ) : 0;
  TIM[Timer]->CCER |= (Channel & (0X1<<2)) ? (0X1<<(0+8) ) : 0;
  TIM[Timer]->CCER |= (Channel & (0X1<<3)) ? (0X1<<(0+12) ) : 0;
  
  /* 配置PWM参数 */
  TIM[Timer]->PSC  = TIM_GET_PSC_BY_OP_FRE(1000, 1000);  //频率配置
  TIM[Timer]->ARR  = 1000;   //分辨率配置(默认)
  
  if(Timer == TIMx_1)
  {
    TIM[Timer]->BDTR |= (0x1<<15);
    TIM[Timer]->BDTR |= (0x1<<14);
  }
  
  /* 开启定时器 */
//  TIM[Timer]->CR1  |= (0X1<<0);

}



/* 接口函数-------------------------------------------------------------- */

/**
  * @brief  定时器x PWM输出模式配置函数
  * @param  Mask 定时器通道屏蔽字,位值为1时开启对应的通道
  * @retval None
  */
void TIMx_Output_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_OUTPUT_MODE Mode)
{
  /* 引脚配置 */
  TIMx_PortConfig(Timer, Channel, TIMx_Port_Output);

  /* 模式配置 */
  TIMx_Output_ModeConfig(Timer, Channel, Mode);
  
  /* 时序配置 */
  TIMx_Output_SetPwmDutyRatio(Timer, Channel, 50);
  
  /* 开启定时器 */
  TIMx_Output_Enable(Timer, 1);
  
}



/**
  * @brief  定时器x 输出模式中断使能函数
  * @param  Mask 定时器通道屏蔽字,位值为1时开启对应的通道
  * @retval None
  */
void TIMx_Output_IRQEnable(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->DIER |= (Channel<<1);  //开外设中断
    NVIC_Enable(TIMx_IRQn[Timer], 2); //开内核中断
  }
  else 
  {
    NVIC_Disable(TIMx_IRQn[Timer]);
  }
  
}


/**
  * @brief  定时器x通道的PWM占空比设置函数
  * @param  Channel 通道掩码
  * @param  dDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void TIMx_Output_SetPwmDutyRatio(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, float dDutyRatio)
{
  uint16_t RegValue = dDutyRatio * (TIM[Timer]->ARR) / 100;
  
  TIM[Timer]->CCR1 = (Channel & 0X01) ? RegValue : TIM[Timer]->CCR1;
  TIM[Timer]->CCR2 = (Channel & 0X02) ? RegValue : TIM[Timer]->CCR2;
  TIM[Timer]->CCR3 = (Channel & 0X04) ? RegValue : TIM[Timer]->CCR3;
  TIM[Timer]->CCR4 = (Channel & 0X08) ? RegValue : TIM[Timer]->CCR4;
  
}



/**
  * @brief  定时器x通道的PWM占空比设置函数
  * @param  Channel 通道掩码
  * @param  nCompareVal 比较值,此值与
  * @retval None
  */
void TIMx_Output_SetCompareVal(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, uint16_t nCompareVal)
{
  TIM[Timer]->CCR1 = (Channel & 0X01) ? nCompareVal : TIM[Timer]->CCR1;
  TIM[Timer]->CCR2 = (Channel & 0X02) ? nCompareVal : TIM[Timer]->CCR2;
  TIM[Timer]->CCR3 = (Channel & 0X04) ? nCompareVal : TIM[Timer]->CCR3;
  TIM[Timer]->CCR4 = (Channel & 0X08) ? nCompareVal : TIM[Timer]->CCR4;
  
}



/**
  * @brief  定时器x频率设置函数
  * @param  uiFrq  PWM波的频率
  * @retval None
  */
void TIMx_Output_SetPwmFrq(TIM_TYPE Timer, uint32_t iFrq)
{
  TIM[Timer]->PSC  = TIM_GET_PSC_BY_OP_FRE(iFrq, TIM[Timer]->ARR);  //频率配置
  
}



/**
  * @brief  定时器x设置自动重加载值函数(决定占空比)
  * @param  nReloadVal 重加载值
  * @retval None
  */
void TIMx_Output_SetAutoReloadReg(TIM_TYPE Timer, uint16_t nReloadVal)
{
  TIM[Timer]->ARR = nReloadVal;
  
}



/**
  * @brief  获取定时器xARR寄存器中的重加载值
  * @param  None
  * @retval ARR寄存器中的重加载值
  */
uint16_t TIMx_Output_GetAutoReloadReg(TIM_TYPE Timer)
{
  
  return TIM[Timer]->ARR;
}



/**
  * @brief  定时器x使能函数
  * @param  cFlag 0:关闭定时器 1:开启定时器
  * @retval None
  */
void TIMx_Output_Enable(TIM_TYPE Timer, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->CR1 |= (0X1<<0);  //开启定时器  
  }
  else 
  {
    TIM[Timer]->CR1 &= ~(0X1<<0);   //关闭定时器
  }

}

