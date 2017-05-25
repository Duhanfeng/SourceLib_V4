/**
  ******************************************************************************
  * @file    hw_tim_delay.h
  * @author  �Ź��Ӻ���
  * @version V4.0 �Ĵ����汾
  * @date    2017.05.23
  * @brief   ��ʱ����ʱ ����ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * V4.0------------
  * �޸�����: 1.�޸������û���,�����ָ������������ĳ�ֱ�ӵ�����ָ�����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.0
  * �޸�����: 2017.05.23
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_tim_delay.h"
#include "SourceLib.h"


/**
  * @brief  TIMx��ʼ��
  * @param  None
  * @retval None
  */
void TIMx_DelayInit(TIM_TypeDef *TIMx)
{
  //��ʱ��
  RCC_EnableClock(TIMx, 1);

  //���ù���ģʽ
  TIMx->CR1  |=  TIM_CR1_ARPE;   //��Ӱ��
  TIMx->CR1  |=  TIM_CR1_OPM;    //������ģʽ
  TIMx->CR1  &= ~TIM_CR1_URS;    //���ø���Դ
  TIMx->CR1  &= ~TIM_CR1_UDIS;   //ʹ�ܸ���Դ
  
  //�����¼�,�ж�
  TIMx->EGR  |=  TIM_EGR_UG;    //���������¼�
  TIMx->DIER &= ~TIM_DIER_UDE;  //��ֹ����DMA
  TIMx->DIER &= ~TIM_DIER_UIE;  //��ֹ�����ж�
  
  //�ض�ʱ��
  TIMx->CR1  &= ~TIM_CR1_CEN;  
  
}



/**
  * @brief  ��ʱ��x��us����ʱ
  * @param  Ҫ��ʱ��us��,ȡֵ��ΧΪ: 1-65535
  * @retval None
  */
void TIMx_DelayUs(TIM_TypeDef *TIMx, uint16_t nus)
{
  if (!nus) return;
  
  //����ʱ�����
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000); //����Ƶ��Ϊ1MHz
  TIMx->ARR = nus;            //������ÿ��1����Ϊ1us
  TIMx->EGR  |=  TIM_EGR_UG;  //������,ˢ��Ӱ��
  TIMx->SR   &= ~TIM_SR_UIF;  //���־λ
  
  //����ʱ��
  TIMx->CR1  |=  TIM_CR1_CEN;
  
  //�ȴ��������
  while (!(TIMx->SR & TIM_SR_UIF)); //�ȴ��������
  
}



/**
  * @brief  ��ʱ��x��ms����ʱ
  * @param  Ҫ��ʱ��ms��,ȡֵ��ΧΪ: 1-3276
  * @retval None
  */
void TIMx_DelayMs(TIM_TypeDef *TIMx, uint16_t nms)  //Ϊ���Ӿ�ȷ��,�����˷�Ƶϵ��,�ʶ������ʱԼΪ3s
{
  if (!nms) return;
  
  //����ʱ�����
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(2000);   //����Ƶ��Ϊ2KHz
  TIMx->ARR = 2 * nms;        //������ÿ��2����Ϊ1ms
  TIMx->EGR  |=  TIM_EGR_UG;  //������,ˢ��Ӱ��
  TIMx->SR   &= ~TIM_SR_UIF;  //���־λ
  
  //����ʱ��
  TIMx->CR1  |=  TIM_CR1_CEN;
  
  //�ȴ��������
  while (!(TIMx->SR & TIM_SR_UIF)); //�ȴ��������
  
}


