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
  * ע:���Ź�һ�������Ͳ���ͣ��
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_iwdg.h"
#include "hw_platform.h"


/* ---�������Ź�����--- */
#define IWDG_ACCESS_REG_ENABLE  ( 0X5555 )     //���Ź�д�Ĵ���ʹ��
#define IWDG_START              ( 0XCCCC )     //���Ź���ʼ����
#define IWDG_FEED               ( 0XAAAA )     //ι��,�趨�ڷ��ʹ����IWDG_KEY��,����ϵͳ�Ḵλ


/**
  * @brief  �������Ź���ʼ��
  * @param  None
  * @retval None
  */
void IWDG_Init(void)
{
  /* ���ù���ģʽ */
  IWDG->KR = IWDG_ACCESS_REG_ENABLE;  //�����Ĵ���ʹ��
  IWDG->PR = 3;           //32��Ƶ,0.8S��һ����,���ʱʱ��Ϊ3276.8S
  IWDG->RLR = 2500;       //���ó�ʱʱ��Ϊ2S
  IWDG->KR = IWDG_FEED;   //ι��
  
  /* �������Ź� */
  IWDG->KR = IWDG_START;  //�������Ź���ʼ����
  
}



/**
  * @brief  ι��,ϵͳ��Ҫ��ʱִ�д˺���,����ϵͳ�Ḵλ
  * @param  None
  * @retval None
  */
void IWDG_FeedDog(void)
{
  IWDG->KR = IWDG_FEED;   //ι��
  
}
