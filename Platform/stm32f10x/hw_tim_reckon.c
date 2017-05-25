/**
  ******************************************************************************
  * @file    hw_tim_reckon.h
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
#include "hw_tim_reckon.h"
#include "SourceLib.h"


/**
  * @brief  TIMx初始化
  * @param  None
  * @retval None
  */
void TIM_REK_Init(TIM_TypeDef *TIMx)
{
  //开时钟
  RCC_EnableClock(TIMx, 1);
  
  //配置时序参数
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(2000); //计数频率为2KHz
  TIMx->ARR = TIM_ARR_ARR;       //计数器每记2个数为1ms
  
  //配置工作模式
  TIMx->CR1 |=  TIM_CR1_ARPE;   //开影子
  TIMx->CR1 &= ~TIM_CR1_OPM;    //非单脉冲
  TIMx->CR1 &= ~TIM_CR1_URS;    //配置更新源:允许软件更新
  TIMx->CR1 &= ~TIM_CR1_UDIS;   //使能更新
  
  //配置事件/中断
  TIMx->DIER &= ~TIM_DIER_UDE;  //禁止更新DMA请求
  TIMx->DIER &= ~TIM_DIER_UIE;  //禁止更新中断
  TIMx->SR   &= ~TIM_SR_UIF;    //清标志位
  
  //关闭定时器
  TIMx->CR1 &= ~TIM_CR1_CEN;
  
}



/**
  * @brief  定时器x使能函数
  * @param  TIM_TypeDef *TIMx
  * @param  cFlag 0:关闭定时器 1:开启定时器
  * @retval None
  */
void TIMx_REK_Enable(TIM_TypeDef *TIMx, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->EGR |=  TIM_EGR_UG;   //给更新,刷新影子
    TIMx->SR  &= ~TIM_SR_UIF;   //清标志位
    TIMx->CR1 |=  TIM_CR1_CEN;  //开启定时器  
  }
  else 
  {
    TIMx->CR1 &= ~TIM_CR1_CEN;  //关闭定时器
  }

}



/**
  * @brief  获取定时器计数值
  * @param  Timer 定时器标号
  * @retval None
  */
uint16_t TIMx_REK_GetCount(TIM_TypeDef *TIMx)
{
  
  return TIMx->CNT;
}



/**
  * @brief  设置定时器计数值
  * @param  Timer 定时器标号
  * @retval None
  */
void TIMx_REK_SetCount(TIM_TypeDef *TIMx, uint16_t nCount)
{
  TIMx->CNT = nCount;
  
}






