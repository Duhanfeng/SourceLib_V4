/**
  ******************************************************************************
  * @file    hw_tim_delay.h
  * @author  杜公子寒枫
  * @version V4.0 寄存器版本
  * @date    2017.05.23
  * @brief   定时器延时 驱动实现文件
  ******************************************************************************
  * @attention
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
#include "hw_tim_delay.h"
#include "SourceLib.h"


/**
  * @brief  TIMx初始化
  * @param  None
  * @retval None
  */
void TIMx_DelayInit(TIM_TypeDef *TIMx)
{
  //开时钟
  RCC_EnableClock(TIMx, 1);

  //配置工作模式
  TIMx->CR1  |=  TIM_CR1_ARPE;   //开影子
  TIMx->CR1  |=  TIM_CR1_OPM;    //单脉冲模式
  TIMx->CR1  &= ~TIM_CR1_URS;    //配置更新源
  TIMx->CR1  &= ~TIM_CR1_UDIS;   //使能更新源
  
  //配置事件,中断
  TIMx->EGR  |=  TIM_EGR_UG;    //开启更新事件
  TIMx->DIER &= ~TIM_DIER_UDE;  //禁止更新DMA
  TIMx->DIER &= ~TIM_DIER_UIE;  //禁止更新中断
  
  //关定时器
  TIMx->CR1  &= ~TIM_CR1_CEN;  
  
}



/**
  * @brief  定时器x的us级延时
  * @param  要延时的us数,取值范围为: 1-65535
  * @retval None
  */
void TIMx_DelayUs(TIM_TypeDef *TIMx, uint16_t nus)
{
  if (!nus) return;
  
  //配置时序参数
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000); //计数频率为1MHz
  TIMx->ARR = nus;            //计数器每记1个数为1us
  TIMx->EGR  |=  TIM_EGR_UG;  //给更新,刷新影子
  TIMx->SR   &= ~TIM_SR_UIF;  //清标志位
  
  //开定时器
  TIMx->CR1  |=  TIM_CR1_CEN;
  
  //等待计数完成
  while (!(TIMx->SR & TIM_SR_UIF)); //等待计数完成
  
}



/**
  * @brief  定时器x的ms级延时
  * @param  要延时的ms数,取值范围为: 1-3276
  * @retval None
  */
void TIMx_DelayMs(TIM_TypeDef *TIMx, uint16_t nms)  //为增加精确率,降低了分频系数,故而最大延时约为3s
{
  if (!nms) return;
  
  //配置时序参数
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(2000);   //计数频率为2KHz
  TIMx->ARR = 2 * nms;        //计数器每记2个数为1ms
  TIMx->EGR  |=  TIM_EGR_UG;  //给更新,刷新影子
  TIMx->SR   &= ~TIM_SR_UIF;  //清标志位
  
  //开定时器
  TIMx->CR1  |=  TIM_CR1_CEN;
  
  //等待计数完成
  while (!(TIMx->SR & TIM_SR_UIF)); //等待计数完成
  
}


