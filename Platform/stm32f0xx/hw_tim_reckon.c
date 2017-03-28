/**
  ******************************************************************************
  * @file    tim_reckon.h
  * @author  杜公子寒枫
  * @version V3.0 寄存器版本
  * @date    2016.08.11
  * @brief   定时器延时 驱动实现文件
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_tim_reckon.h"
#include "SourceLib.h"


/*----------------------------------------------------------------------------
    模块功能自匹配
 *----------------------------------------------------------------------------*/
 
/**
  * @brief  TIMx初始化
  * @param  None
  * @retval None
  */
void TIM_REK_Init(TIM_TYPE Timer)
{
  /* 开时钟 */
  TIMx_EnableClock(Timer);
  
  /* 配置时序参数 */
  TIM[Timer]->PSC = 36000 - 1;    //3600分频
  TIM[Timer]->ARR = 0xFFFF;       //计数器每记20个数为1ms
  
  /* 配置工作模式 */
  TIM[Timer]->CR1 |=  (0X1<<7);   //开影子
  TIM[Timer]->CR1 &= ~(0X1<<3);   //非单脉冲
  TIM[Timer]->CR1 &= ~(0X1<<2);   //配置更新源:允许软件更新
  TIM[Timer]->CR1 &= ~(0X1<<1);   //使能更新
  
  /* 配置事件/中断 */
  TIM[Timer]->DIER &= ~(0X1<<8); //禁止更新DMA请求
  TIM[Timer]->DIER &= ~(0X1<<0); //禁止更新中断
  TIM[Timer]->SR   &= ~(0X1<<0); //清标志位
  
  /* 关闭定时器 */
  TIM[Timer]->CR1 &= ~(0X1<<0);
  
}



/**
  * @brief  定时器x使能函数
  * @param  TIM_TYPE Timer
  * @param  cFlag 0:关闭定时器 1:开启定时器
  * @retval None
  */
void TIMx_REK_Enable(TIM_TYPE Timer, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->EGR |=  (0X1<<0); //给更新,刷新影子
    TIM[Timer]->SR  &= ~(0X1<<0); //清标志位
    TIM[Timer]->CR1 |= (0X1<<0);  //开启定时器  
  }
  else 
  {
    TIM[Timer]->CR1 &= ~(0X1<<0);   //关闭定时器
  }

}



/**
  * @brief  获取定时器计数值
  * @param  Timer 定时器标号
  * @retval None
  */
uint16_t TIMx_REK_GetCount(TIM_TYPE Timer)
{
  
  return TIM[Timer]->CNT;
}



/**
  * @brief  设置定时器计数值
  * @param  Timer 定时器标号
  * @retval None
  */
void TIMx_REK_SetCount(TIM_TYPE Timer, uint16_t nCount)
{
  TIM[Timer]->CNT = nCount;
  
}






