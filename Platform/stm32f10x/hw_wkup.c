/**
  ******************************************************************************
  * @file    hw_wkup.c
  * @author  �Ź��Ӻ���
  * @version V1.0 
  * @date    20170407
  * @brief   ϵͳ����
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



//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
__asm void WFI_SET(void)
{
	WFI;    
}


//�������ģʽ
void Sys_Standby(void)
{
  SCB->SCR |= SCB_SCR_SLEEPDEEP;      //ʹ��SLEEPDEEPλ (SYS->CTRL)	   
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;  //ʹ�ܵ�Դʱ��	    
  PWR->CSR |= PWR_CSR_EWUP; //����WKUP���ڻ���
  PWR->CR  |= PWR_CSR_PVDO; //���Wake-up��־
  PWR->CR  |= PWR_CSR_SBF;  //PDDS��λ		  
  WFI_SET();                //ִ��WFIָ��		 

}


//ϵͳ�������ģʽ
void WKUP_Enter_Standby(void)
{
  //�ر���������(����ʵ�����д)
  RCC->APB2RSTR |= 0X01FC;//��λ����IO��
  Sys_Standby();//�������ģʽ
  
}








