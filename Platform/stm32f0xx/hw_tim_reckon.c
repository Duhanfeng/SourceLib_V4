/**
  ******************************************************************************
  * @file    tim_reckon.h
  * @author  �Ź��Ӻ���
  * @version V3.0 �Ĵ����汾
  * @date    2016.08.11
  * @brief   ��ʱ����ʱ ����ʵ���ļ�
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
    ģ�鹦����ƥ��
 *----------------------------------------------------------------------------*/
 
/**
  * @brief  TIMx��ʼ��
  * @param  None
  * @retval None
  */
void TIM_REK_Init(TIM_TYPE Timer)
{
  /* ��ʱ�� */
  TIMx_EnableClock(Timer);
  
  /* ����ʱ����� */
  TIM[Timer]->PSC = 36000 - 1;    //3600��Ƶ
  TIM[Timer]->ARR = 0xFFFF;       //������ÿ��20����Ϊ1ms
  
  /* ���ù���ģʽ */
  TIM[Timer]->CR1 |=  (0X1<<7);   //��Ӱ��
  TIM[Timer]->CR1 &= ~(0X1<<3);   //�ǵ�����
  TIM[Timer]->CR1 &= ~(0X1<<2);   //���ø���Դ:�����������
  TIM[Timer]->CR1 &= ~(0X1<<1);   //ʹ�ܸ���
  
  /* �����¼�/�ж� */
  TIM[Timer]->DIER &= ~(0X1<<8); //��ֹ����DMA����
  TIM[Timer]->DIER &= ~(0X1<<0); //��ֹ�����ж�
  TIM[Timer]->SR   &= ~(0X1<<0); //���־λ
  
  /* �رն�ʱ�� */
  TIM[Timer]->CR1 &= ~(0X1<<0);
  
}



/**
  * @brief  ��ʱ��xʹ�ܺ���
  * @param  TIM_TYPE Timer
  * @param  cFlag 0:�رն�ʱ�� 1:������ʱ��
  * @retval None
  */
void TIMx_REK_Enable(TIM_TYPE Timer, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->EGR |=  (0X1<<0); //������,ˢ��Ӱ��
    TIM[Timer]->SR  &= ~(0X1<<0); //���־λ
    TIM[Timer]->CR1 |= (0X1<<0);  //������ʱ��  
  }
  else 
  {
    TIM[Timer]->CR1 &= ~(0X1<<0);   //�رն�ʱ��
  }

}



/**
  * @brief  ��ȡ��ʱ������ֵ
  * @param  Timer ��ʱ�����
  * @retval None
  */
uint16_t TIMx_REK_GetCount(TIM_TYPE Timer)
{
  
  return TIM[Timer]->CNT;
}



/**
  * @brief  ���ö�ʱ������ֵ
  * @param  Timer ��ʱ�����
  * @retval None
  */
void TIMx_REK_SetCount(TIM_TYPE Timer, uint16_t nCount)
{
  TIM[Timer]->CNT = nCount;
  
}






