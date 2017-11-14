/**
  ******************************************************************************
  * @file    Delay.h
  * @author  杜公子寒枫
  * @version V3.0 寄存器版本
  * @date    2016.08.09
  * @brief   定时器延时 驱动实现文件
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_tim_delay.h"
#include "hw_platform.h"

/*----------------------------------------------------------------------------
    模块功能自匹配
 *----------------------------------------------------------------------------*/
 
 static TIM_TypeDef *DELAY_TIM = 0;

/**
  * @brief  DELAY_TIM初始化
  * @param  None
  * @retval None
  */
void TIMx_DelayInit(TIM_TYPE Timer)
{
  DELAY_TIM = TIM[Timer];
  
  TIMx_EnableClock(Timer);
  
  /* 配置工作模式 */
  DELAY_TIM->CR1 |=  (0X1<<7);  //开影子
  DELAY_TIM->CR1 |=  (0X1<<4);  //单脉冲模式
  DELAY_TIM->CR1 &= ~(0X1<<2);  //配置更新源
  DELAY_TIM->CR1 &= ~(0X1<<1);  //使能更新源
  
  /* 配置事件,中断 */
  DELAY_TIM->EGR  |=  (0X1<<0); //开启更新事件
  DELAY_TIM->DIER &= ~(0X1<<8); //禁止更新DMA
  DELAY_TIM->DIER &= ~(0X1<<0); //禁止更新中断
  
  /* 关定时器 */
  DELAY_TIM->CR1 &= ~(0X1<<0);  
  
}



/**
  * @brief  定时器7的us级延时
  * @param  要延时的us数,取值范围为: 1-65535
  * @retval None
  */
void TIMx_DelayUs(uint32_t nus)
{
  if (!nus) return;
  
  /* 配置时序参数 */
  DELAY_TIM->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000); //计数频率为1MHz
  DELAY_TIM->ARR = nus;      //计数器每记1个数为1us
  DELAY_TIM->EGR  |=  (0X1<<0); //给更新,刷新影子
  DELAY_TIM->SR   &= ~(0X1<<0); //清标志位
  
  /* 开定时器 */
  DELAY_TIM->CR1 |=  (0X1<<0);
  
  /* 等待计数完成 */
  while (!(DELAY_TIM->SR & (1<<0)));  //等待计数完成
  
}



/**
  * @brief  定时器7的ms级延时
  * @param  要延时的ms数,取值范围为: 1-3276
  * @retval None
  */
void TIMx_DelayMs(uint32_t nms)  //为增加精确率,降低了分频系数,故而最大延时约为3s
{
  if (!nms) return;
  
  /* 配置时序参数 */
  DELAY_TIM->PSC = TIM_GET_PSC_BY_CNT_FRE(2000);   //计数频率为2KHz
  DELAY_TIM->ARR = 2 * nms; //计数器每记2个数为1ms
  DELAY_TIM->EGR  |=  (0X1<<0); //给更新,刷新影子
  DELAY_TIM->SR   &= ~(0X1<<0); //清标志位
  
  /* 开定时器 */
  DELAY_TIM->CR1 |=  (0X1<<0);
  
  /* 等待计数完成 */
  while (!(DELAY_TIM->SR & (1<<0)));  //等待计数完成
  
}


