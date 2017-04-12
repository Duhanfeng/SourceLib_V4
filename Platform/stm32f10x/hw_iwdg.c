/**
  ******************************************************************************
  * @file    hw_iwdg.c
  * @author  DuHanFeng
  * @version V1.0 
  * @date    2015.12.28
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 注:看门狗一旦开启就不能停下
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_iwdg.h"
#include "hw_platform.h"


/* ---独立看门狗命令--- */
#define IWDG_ACCESS_REG_ENABLE  ( 0X5555 )     //看门狗写寄存器使能
#define IWDG_START              ( 0XCCCC )     //看门狗开始工作
#define IWDG_FEED               ( 0XAAAA )     //喂狗,需定期发送此命令到IWDG_KEY中,否则系统会复位


/**
  * @brief  独立看门狗初始化
  * @param  None
  * @retval None
  */
void IWDG_Init(void)
{
  /* 配置工作模式 */
  IWDG->KR = IWDG_ACCESS_REG_ENABLE;  //操作寄存器使能
  IWDG->PR = 3;           //32分频,0.8S记一个数,最大超时时间为3276.8S
  IWDG->RLR = 2500;       //设置超时时间为2S
  IWDG->KR = IWDG_FEED;   //喂狗
  
  /* 开启看门狗 */
  IWDG->KR = IWDG_START;  //独立看门狗开始工作
  
}



/**
  * @brief  喂狗,系统需要定时执行此函数,否则系统会复位
  * @param  None
  * @retval None
  */
void IWDG_FeedDog(void)
{
  IWDG->KR = IWDG_FEED;   //喂狗
  
}
