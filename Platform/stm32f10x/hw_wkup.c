/**
  ******************************************************************************
  * @file    hw_wkup.c
  * @author  杜公子寒枫
  * @version V1.0 
  * @date    20170407
  * @brief   系统唤醒
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_platform.h"
#include "hw_wkup.h"



//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
__asm void WFI_SET(void)
{
    WFI;    
}


//进入待机模式
void Sys_Standby(void)
{
  SCB->SCR |= SCB_SCR_SLEEPDEEP;      //使能SLEEPDEEP位 (SYS->CTRL)       
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;  //使能电源时钟        
  PWR->CSR |= PWR_CSR_EWUP; //设置WKUP用于唤醒
  PWR->CR  |= PWR_CSR_PVDO; //清除Wake-up标志
  PWR->CR  |= PWR_CSR_SBF;  //PDDS置位          
  WFI_SET();                //执行WFI指令         

}


//系统进入待机模式
void WKUP_Enter_Standby(void)
{
  //关闭所有外设(根据实际情况写)
  RCC->APB2RSTR |= 0X01FC;//复位所有IO口
  Sys_Standby();//进入待机模式
  
}








