/**
  ******************************************************************************
  * @file    hw_tim_itr.c
  * @author  杜公子寒枫
  * @version V4.0 寄存器版本
  * @date    2017.05.23
  * @brief   定时器x中断功能配置
  ******************************************************************************
  * @attention
  * 修改描述: 修复初始化阶段执行未开启定时器便执行中断函数的错误
  * 错误描述: 在定时器初始化时,在配置时序参数后便给个更新事件,希望借此刷新影子寄
  *           存器中的PSC与ARR值,但是这导致中断挂起位置1,一旦开启核级中断(这时定
  *           时器还没开启),便会立即执行一次中断服务函数.
  * 解决方法: 删除初始化阶段中的软件更新的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2015.12.30
  * 
  * V2.0------------
  * 修改描述: 优化框架,删除冗余函数,整合定时器的使能/禁止函数,
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.05.16
  * 
  * V4.0------------
  * 修改描述: 1.修改其配置机制,将其从指针数组的索引改成直接的外设指针访问
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2017.05.23
  * 
  *  
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_tim_itr.h"
#include "SourceLib.h"



/**
  * @brief  TIMx初始化
  * @param  nms 中断间隔时间,单位:ms,范围: 1-3276
  * @retval None
  */
void TIMx_Itr_Init(TIM_TypeDef *TIMx, uint16_t nms, uint8_t isOnePulse)
{
  //开时钟
  RCC_EnableClock(TIMx, 1);
  
  //配置时序参数
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(20000);  //计数频率为20KHz
  TIMx->ARR = 20 * nms;         //计数器每记20个数为1ms
  
  //配置工作模式
  TIMx->CR1  |=  TIM_CR1_ARPE;  //开影子
  TIMx->CR1  |=  isOnePulse ? TIM_CR1_OPM : 0;  //单脉冲
  TIMx->CR1  &= ~TIM_CR1_URS;   //配置更新源:允许软件更新
  TIMx->CR1  &= ~TIM_CR1_UDIS;  //使能更新
  
  //配置事件/中断
  TIMx->DIER &= ~TIM_DIER_UDE;  //禁止更新DMA请求
  TIMx->DIER |=  TIM_DIER_UIE;  //使能更新中断
  TIMx->SR   &= ~TIM_SR_UIF;    //清标志位
  
  //配置核级中断
  if (TIMx == TIM1)
  {
    NVIC_Enable(TIM1_UP_IRQn, 2, 2);
  }
  else if (TIMx == TIM8)
  {
    NVIC_Enable(TIM8_UP_IRQn, 2, 2);
  }
  else 
  {
    NVIC_Config(TIMx, 2, 2);
  }
  
  //关闭定时器
  TIMx->CR1 |=  TIM_CR1_CEN;

}



/**
  * @brief  定时器x使能函数
  * @param  isEnable 0:关闭定时器 1:开启定时器
  * @retval None
  */
void TIMx_Itr_Enable(TIM_TypeDef *TIMx, uint8_t isEnable)
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

