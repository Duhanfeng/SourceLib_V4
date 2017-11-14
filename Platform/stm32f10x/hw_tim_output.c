/**
  ******************************************************************************
  * @file    tim_output.c
  * @author  Duhanfeng
  * @version V4.1 寄存器版本
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
  * 有效电平: 默认高电平为有效电平  TIMx->CCER 中配置
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
  * V4.1------------
  * 修改描述: 1.修改其配置机制,将其从指针数组的索引改成直接的外设指针访问
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
  * 
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_output.h"
#include "SourceLib.h"



/* 内部函数-------------------------------------------------------------- */


/**
  * @brief  定时器x 模式配置
  * @param  TIMx 定时器指针
  * @param  ChMask 定时器通道掩码,位值为1时开启对应的通道
  * @param  Mode 工作模式
  * @retval None
  */
static void TIMx_Output_ModeConfig(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_OUTPUT_MODE Mode)
{
  //开时钟
  RCC_EnableClock(TIMx, 1);
                             
  //配置工作模式
  TIMx->CR1 |=  TIM_CR1_ARPE; //开启影子寄存器
  TIMx->CR1 &= ~TIM_CR1_CMS;  //边沿对齐模式
  TIMx->CR1 &= ~TIM_CR1_DIR;  //向上计数
  TIMx->CR1 &= ~TIM_CR1_OPM;  //非单脉冲模式
  TIMx->CR1 &= ~TIM_CR1_URS;  //配置更新源
  TIMx->CR1 &= ~TIM_CR1_UDIS; //允许更新

  //选择工作时钟
  TIMx->SMCR &= ~TIM_SMCR_SMS;   //关闭从模式--内部时钟72M

  //配置中断             
  TIMx->DIER &= ~TIM_DIER_UDE;  //禁止触发DMA请求
  TIMx->DIER &= ~TIM_DIER_UIE;  //关闭更新中断
  TIMx->EGR  |=  TIM_EGR_UG;    //开启更新事件
  
  #if 0
  if (Def_IntFlag[Timer])  //根据标志位配置中断
  {
    TIMx->DIER |= (ChMask<<1);  //开外设中断
    NVIC_Enable(TIMx_IRQn[Timer],2,2); //开内核中断
  }
  #endif
  
  //配置CH1 PWM模式
  TIMx->CCMR1 &= ~TIM_CCMR1_OC1CE;  //OC1REF不受ETRF影响
  TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;  
  TIMx->CCMR1 |=  (Mode<<(4+0));    //输出模式
  TIMx->CCMR1 |=  TIM_CCMR1_OC1PE;  //输出比较1预装载使能
  TIMx->CCMR1 |=  TIM_CCMR1_OC1FE;  //输出比较1快速使能
  TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;   //CH1配置为输出模式

  //配置CH2 PWM模式
  TIMx->CCMR1 &= ~TIM_CCMR1_OC2CE;  //OC2REF不受ETRF影响
  TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;  
  TIMx->CCMR1 |=  (Mode<<(4+8));    //输出模式
  TIMx->CCMR1 |=  TIM_CCMR1_OC2PE;  //输出比较2预装载使能
  TIMx->CCMR1 |=  TIM_CCMR1_OC2FE;  //输出比较2快速使能
  TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;   //CH2配置为输出模式
  
  
  //配置CH3 PWM模式
  TIMx->CCMR2 &= ~TIM_CCMR2_OC3CE;  //OC3REF不受ETRF影响
  TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;  
  TIMx->CCMR2 |=  (Mode<<(4+0));    //输出模式
  TIMx->CCMR2 |=  TIM_CCMR2_OC3PE;  //输出比较3预装载使能
  TIMx->CCMR2 |=  TIM_CCMR2_OC3FE;  //输出比较3快速使能
  TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;   //CH3配置为输出模式
  
  //配置CH4 PWM模式
  TIMx->CCMR2 &= ~TIM_CCMR2_OC4CE;  //OC4REF不受ETRF影响
  TIMx->CCMR2 &= ~TIM_CCMR2_OC4M;   
  TIMx->CCMR2 |=  (Mode<<(4+8));    //输出模式
  TIMx->CCMR2 |=  TIM_CCMR2_OC4PE;  //输出比较4预装载使能
  TIMx->CCMR2 |=  TIM_CCMR2_OC4FE;  //输出比较4快速使能
  TIMx->CCMR2 &= ~TIM_CCMR2_CC4S;   //CH4配置为输出模式
  
  //配置有效电平
  TIMx->CCER  &= ~TIM_CCER_CC1P;    //CH1默认高电平为有效电平
  TIMx->CCER  &= ~TIM_CCER_CC2P;    //CH2默认高电平为有效电平
  TIMx->CCER  &= ~TIM_CCER_CC3P;    //CH3默认高电平为有效电平
  TIMx->CCER  &= ~TIM_CCER_CC4P;    //CH4默认高电平为有效电平
  
  //根据关键字配置输出
  TIMx->CCER |= (ChMask & (0X1<<0)) ? TIM_CCER_CC1E : 0;
  TIMx->CCER |= (ChMask & (0X1<<1)) ? TIM_CCER_CC2E : 0;
  TIMx->CCER |= (ChMask & (0X1<<2)) ? TIM_CCER_CC3E : 0;
  TIMx->CCER |= (ChMask & (0X1<<3)) ? TIM_CCER_CC4E : 0;
  
  //配置PWM参数
  TIMx->PSC  = TIM_GET_PSC_BY_OP_FRE(1000, 1000);  //频率配置
  TIMx->ARR  = 1000;   //分辨率配置(默认)
  
  //使能输出
  if((TIMx == TIM1) || ((TIMx == TIM8)))
  {
    TIMx->BDTR |= TIM_BDTR_MOE; //主输出使能
    TIMx->BDTR |= TIM_BDTR_AOE; //自动输出使能
  }
  
  //开启定时器
//  TIMx->CR1  |= TIM_CR1_CEN;

}



/* 接口函数-------------------------------------------------------------- */

/**
  * @brief  定时器x PWM输出模式配置函数
  * @param  TIMx 定时器指针
  * @param  ChMask 定时器通道掩码,位值为1时开启对应的通道
  * @retval None
  */
void TIMx_Output_Init(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_OUTPUT_MODE Mode)
{
  //引脚配置
  TIMx_PortConfig(TIMx, ChMask, TIMx_Port_Output);

  //模式配置
  TIMx_Output_ModeConfig(TIMx, ChMask, Mode);
  
  //时序配置
  TIMx_Output_SetPwmDutyRatio(TIMx, ChMask, 50);
  
  //开启定时器
  TIMx_Output_Enable(TIMx, 1);
  
}



/**
  * @brief  定时器x 输出模式中断使能函数
  * @param  TIMx 定时器指针
  * @param  ChMask 定时器通道掩码,位值为1时开启对应的通道
  * @retval None
  */
void TIMx_Output_IRQEnable(TIM_TypeDef *TIMx, uint8_t ChMask, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->DIER |= (ChMask<<1);  //开外设中断
    NVIC_Config(TIMx, 2, 2);    //开内核中断
  }
  else 
  {
    NVIC_Disable(NVIC_GetIRQType(TIMx));
  }
  
}


/**
  * @brief  定时器x通道的PWM占空比设置函数
  * @param  TIMx 定时器指针
  * @param  ChMask 定时器通道掩码,位值为1时开启对应的通道
  * @param  dDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void TIMx_Output_SetPwmDutyRatio(TIM_TypeDef *TIMx, uint8_t ChMask, float dDutyRatio)
{
  uint16_t RegValue = dDutyRatio * (TIMx->ARR) / 100;
  
  TIMx->CCR1 = (ChMask & 0X01) ? RegValue : TIMx->CCR1;
  TIMx->CCR2 = (ChMask & 0X02) ? RegValue : TIMx->CCR2;
  TIMx->CCR3 = (ChMask & 0X04) ? RegValue : TIMx->CCR3;
  TIMx->CCR4 = (ChMask & 0X08) ? RegValue : TIMx->CCR4;
  
}



/**
  * @brief  定时器x通道的PWM占空比设置函数
  * @param  TIMx 定时器指针
  * @param  ChMask 通道掩码
  * @param  nCompareVal 比较值,此值与
  * @retval None
  */
void TIMx_Output_SetCompareVal(TIM_TypeDef *TIMx, uint8_t ChMask, uint16_t nCompareVal)
{
  TIMx->CCR1 = (ChMask & 0X01) ? nCompareVal : TIMx->CCR1;
  TIMx->CCR2 = (ChMask & 0X02) ? nCompareVal : TIMx->CCR2;
  TIMx->CCR3 = (ChMask & 0X04) ? nCompareVal : TIMx->CCR3;
  TIMx->CCR4 = (ChMask & 0X08) ? nCompareVal : TIMx->CCR4;
  
}



/**
  * @brief  定时器x频率设置函数
  * @param  TIMx 定时器指针
  * @param  iFrq  PWM波的频率
  * @retval None
  */
void TIMx_Output_SetPwmFrq(TIM_TypeDef *TIMx, uint32_t iFrq)
{
  TIMx->PSC  = TIM_GET_PSC_BY_OP_FRE(iFrq, TIMx->ARR);  //频率配置
  
}



/**
  * @brief  定时器x设置自动重加载值函数(决定占空比)
  * @param  nReloadVal 重加载值
  * @retval None
  */
void TIMx_Output_SetAutoReloadReg(TIM_TypeDef *TIMx, uint16_t nReloadVal)
{
  TIMx->ARR = nReloadVal;
  
}



/**
  * @brief  获取定时器xARR寄存器中的重加载值
  * @param  TIMx 定时器指针
  * @retval ARR寄存器中的重加载值
  */
uint16_t TIMx_Output_GetAutoReloadReg(TIM_TypeDef *TIMx)
{
  
  return TIMx->ARR;
}



/**
  * @brief  定时器x使能函数
  * @param  TIMx 定时器指针
  * @param  isEnable 0:关闭定时器 1:开启定时器
  * @retval None
  */
void TIMx_Output_Enable(TIM_TypeDef *TIMx, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->CR1 |=  TIM_CR1_CEN;  //开启定时器  
  }
  else 
  {
    TIMx->CR1 &= ~TIM_CR1_CEN;  //关闭定时器
  }

}

