/**
  ******************************************************************************
  * @file    Delay.h
  * @author  �Ź��Ӻ���
  * @version V3.0 �Ĵ����汾
  * @date    2016.08.09
  * @brief   ��ʱ����ʱ ����ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_tim_delay.h"
#include "hw_platform.h"
#include "stm32f10x.h"

/*----------------------------------------------------------------------------
    ģ�鹦����ƥ��
 *----------------------------------------------------------------------------*/
 
 static TIM_TypeDef *DELAY_TIM = 0;

/**
  * @brief  DELAY_TIM��ʼ��
  * @param  None
  * @retval None
  */
void TIMx_DelayInit(TIM_TYPE Timer)
{
  DELAY_TIM = TIM[Timer];
  
  TIMx_EnableClock(Timer);
  
  /* ���ù���ģʽ */
  DELAY_TIM->CR1 |=  (0X1<<7);  //��Ӱ��
  DELAY_TIM->CR1 |=  (0X1<<4);  //������ģʽ
  DELAY_TIM->CR1 &= ~(0X1<<2);  //���ø���Դ
  DELAY_TIM->CR1 &= ~(0X1<<1);  //ʹ�ܸ���Դ
  
  /* �����¼�,�ж� */
  DELAY_TIM->EGR  |=  (0X1<<0); //���������¼�
  DELAY_TIM->DIER &= ~(0X1<<8); //��ֹ����DMA
  DELAY_TIM->DIER &= ~(0X1<<0); //��ֹ�����ж�
  
  /* �ض�ʱ�� */
  DELAY_TIM->CR1 &= ~(0X1<<0);  
  
}



/**
  * @brief  ��ʱ��7��us����ʱ
  * @param  Ҫ��ʱ��us��,ȡֵ��ΧΪ: 1-65535
  * @retval None
  */
void TIMx_DelayUs(uint32_t nus)
{
  if (!nus) return;
  
  /* ����ʱ����� */
  DELAY_TIM->PSC = 72 - 1;   //72��Ƶ,1MH
  DELAY_TIM->ARR = nus;      //������ÿ��1����Ϊ1us
  DELAY_TIM->EGR  |=  (0X1<<0); //������,ˢ��Ӱ��
  DELAY_TIM->SR   &= ~(0X1<<0); //���־λ
  
  /* ����ʱ�� */
  DELAY_TIM->CR1 |=  (0X1<<0);
  
  /* �ȴ�������� */
  while (!(DELAY_TIM->SR & (1<<0)));  //�ȴ��������
  
}



/**
  * @brief  ��ʱ��7��ms����ʱ
  * @param  Ҫ��ʱ��ms��,ȡֵ��ΧΪ: 1-3276
  * @retval None
  */
void TIMx_DelayMs(uint32_t nms)  //Ϊ���Ӿ�ȷ��,�����˷�Ƶϵ��,�ʶ������ʱԼΪ3s
{
  if (!nms) return;
  
  /* ����ʱ����� */
  DELAY_TIM->PSC = 3600 - 1; //3600��Ƶ
  DELAY_TIM->ARR = 20 * nms; //������ÿ��20����Ϊ1ms
  DELAY_TIM->EGR  |=  (0X1<<0); //������,ˢ��Ӱ��
  DELAY_TIM->SR   &= ~(0X1<<0); //���־λ
  
  /* ����ʱ�� */
  DELAY_TIM->CR1 |=  (0X1<<0);
  
  /* �ȴ�������� */
  while (!(DELAY_TIM->SR & (1<<0)));  //�ȴ��������
  
}


