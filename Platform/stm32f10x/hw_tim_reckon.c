/**
  ******************************************************************************
  * @file    hw_tim_reckon.h
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
#include "hw_tim_reckon.h"
#include "SourceLib.h"


/**
  * @brief  TIMx��ʼ��
  * @param  None
  * @retval None
  */
void TIM_REK_Init(TIM_TypeDef *TIMx)
{
  //��ʱ��
  RCC_EnableClock(TIMx, 1);
  
  //����ʱ�����
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(2000); //����Ƶ��Ϊ2KHz
  TIMx->ARR = TIM_ARR_ARR;       //������ÿ��2����Ϊ1ms
  
  //���ù���ģʽ
  TIMx->CR1 |=  TIM_CR1_ARPE;   //��Ӱ��
  TIMx->CR1 &= ~TIM_CR1_OPM;    //�ǵ�����
  TIMx->CR1 &= ~TIM_CR1_URS;    //���ø���Դ:�����������
  TIMx->CR1 &= ~TIM_CR1_UDIS;   //ʹ�ܸ���
  
  //�����¼�/�ж�
  TIMx->DIER &= ~TIM_DIER_UDE;  //��ֹ����DMA����
  TIMx->DIER &= ~TIM_DIER_UIE;  //��ֹ�����ж�
  TIMx->SR   &= ~TIM_SR_UIF;    //���־λ
  
  //�رն�ʱ��
  TIMx->CR1 &= ~TIM_CR1_CEN;
  
}



/**
  * @brief  ��ʱ��xʹ�ܺ���
  * @param  TIM_TypeDef *TIMx
  * @param  cFlag 0:�رն�ʱ�� 1:������ʱ��
  * @retval None
  */
void TIMx_REK_Enable(TIM_TypeDef *TIMx, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->EGR |=  TIM_EGR_UG;   //������,ˢ��Ӱ��
    TIMx->SR  &= ~TIM_SR_UIF;   //���־λ
    TIMx->CR1 |=  TIM_CR1_CEN;  //������ʱ��  
  }
  else 
  {
    TIMx->CR1 &= ~TIM_CR1_CEN;  //�رն�ʱ��
  }

}



/**
  * @brief  ��ȡ��ʱ������ֵ
  * @param  Timer ��ʱ�����
  * @retval None
  */
uint16_t TIMx_REK_GetCount(TIM_TypeDef *TIMx)
{
  
  return TIMx->CNT;
}



/**
  * @brief  ���ö�ʱ������ֵ
  * @param  Timer ��ʱ�����
  * @retval None
  */
void TIMx_REK_SetCount(TIM_TypeDef *TIMx, uint16_t nCount)
{
  TIMx->CNT = nCount;
  
}






