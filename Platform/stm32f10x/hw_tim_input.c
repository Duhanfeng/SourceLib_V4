/**
  ******************************************************************************
  * @file    tim_input.c
  * @author  �Ź��Ӻ���
  * @version V4.1 �Ĵ����汾
  * @date    2017.05.23
  * @brief   TIMx CAPTUREģʽ����
  ******************************************************************************
  * @attention
  *
  *
  * TIM1:   CH1 --- PA8  ��������,50M
  *         CH2 --- PA9  ��������,50M
  *         CH3 --- PA10 ��������,50M
  *         CH4 --- PA11 ��������,50M
  *
  * TIM2:   CH1 --- PA0  ��������,50M
  *         CH2 --- PA1  ��������,50M
  *         CH3 --- PA2  ��������,50M
  *         CH4 --- PA3  ��������,50M
  *
  * TIM3:   CH1 --- PA6  ��������,50M
  *         CH2 --- PA7  ��������,50M
  *         CH3 --- PB0  ��������,50M
  *         CH4 --- PB1  ��������,50M
  *
  * TIM4:   CH1 --- PB6  ��������,50M
  *         CH2 --- PB7  ��������,50M
  *         CH3 --- PB8  ��������,50M
  *         CH4 --- PB9  ��������,50M
  *
  * TIM5:   CH1 --- PA0  ��������,50M
  *         CH2 --- PA1  ��������,50M
  *         CH3 --- PA2  ��������,50M
  *         CH4 --- PA3  ��������,50M
  *
  * TIM8:   CH1 --- PC6  ��������,50M
  *         CH2 --- PC7  ��������,50M
  *         CH3 --- PC8  ��������,50M
  *         CH4 --- PC9  ��������,50M
  * 
  * ����,
  *      PWM���벶��,ÿ����һ������,��ȡTIMx->CCR1���Եõ����������ʱ��,
  *      TIMx->CCR2���Եõ�����ĸߵ�ƽʱ��
  * ����,
  *      ʱ��:   72MHz
  * 
  * ��Ч��ƽ: Ĭ�ϸߵ�ƽΪ��Ч��ƽ  TIMx->CCER ������
  *
  *   
  * V3.0------------    
  * �޸�����: ���Ŷ�ʱ�����ýӿ�,����ʱ���ı�Ž������ɶԴ˶�ʱ�����ó�����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V3.0
  * �޸�����: 2016.07.15
  * 
  * V4.0------------    
  * �޸�����: �淶��ؽӿ�,���ݳ�������벶��ģʽ�Լ������PWM����ģʽ
  * PWM����:  ��ģʽ�����벶��ģʽ��һ������,������������ICx��ӳ�䵽ͬһ��TIx��
  *           ��....
  * ����ӿ�: ....
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.0
  * �޸�����: 2017.02.14
  *  
  * V4.1------------
  * �޸�����: 1.�޸������û���,�����ָ������������ĳ�ֱ�ӵ�����ָ�����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.1
  * �޸�����: 2017.05.23
  * 
  * 
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_input.h"
#include "SourceLib.h"



//TIMxģʽ����
static void TIMx_Input_ModeConfig(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_INPUT_MODE Mode)
{
  //��ʱ��
  RCC_EnableClock(TIMx, 1);
  
  //ģʽ����
  TIMx->CR1 &= ~TIM_CR1_CKD;  //ʱ�ӷ�Ƶ����
  TIMx->CR1 |=  TIM_CR1_ARPE; //����Ӱ��
  TIMx->CR1 &= ~TIM_CR1_CMS;  //���ض���ģʽ
  TIMx->CR1 &= ~TIM_CR1_DIR;  //���������ϼ���
  TIMx->CR1 &= ~TIM_CR1_OPM;  //�ǵ�����ģʽ
  TIMx->CR1 &= ~TIM_CR1_URS;  //���ø���Դ--���������������
  TIMx->CR1 &= ~TIM_CR1_UDIS; //��������¼�
  
  //����ʱ�����
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000);  //Ԥ��Ƶ��:��Ƶ,����Ƶ��Ϊ1MHz
  TIMx->ARR = TIM_ARR_ARR;     //����Զ���װ��ֵ
  
  //ѡ����ʱ��
  TIMx->SMCR &= ~TIM_SMCR_SMS;//�رմ�ģʽ,ѡ���ڲ�ʱ��
  
  //���ò���ģʽ
  if (Mode == TIMx_Input_Normal)
  {
    //ChMask 1
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; //��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC1S_0; //����TI1
    TIMx->CCER  &= ~TIM_CCER_CC1P;    //����������
    TIMx->CCER  |=  (ChMask & 0x01) ? TIM_CCER_CC1E : 0x00;
    
    //ChMask 2
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1;//��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC2S_0; //����TI2
    TIMx->CCER  &= ~TIM_CCER_CC2P;    //����������
    TIMx->CCER  |=  (ChMask & 0x02) ? TIM_CCER_CC2E : 0x00;
    
    //ChMask 3
    TIMx->CCMR2 &= ~TIM_CCMR2_IC3F;
    TIMx->CCMR2 |=  TIM_CCMR2_IC3F_0 | TIM_CCMR2_IC3F_1; //��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;
    TIMx->CCMR2 |=  TIM_CCMR2_CC3S_0; //����TI3
    TIMx->CCER  &= ~TIM_CCER_CC3P;    //����������
    TIMx->CCER  |=  (ChMask & 0x04) ? TIM_CCER_CC3E : 0x00;
    
    //ChMask 4
    TIMx->CCMR2 &= ~TIM_CCMR2_IC4F;
    TIMx->CCMR2 |=  TIM_CCMR2_IC4F_0 | TIM_CCMR2_IC4F_1;//��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIMx->CCMR2 &= ~TIM_CCMR2_CC4S;
    TIMx->CCMR2 |=  TIM_CCMR2_CC4S_0; //����TI4
    TIMx->CCER  &= ~TIM_CCER_CC4P;    //����������
    TIMx->CCER  |=  (ChMask & 0x08) ? TIM_CCER_CC4E : 0x00;
    
  }
  //PWM����ģʽ
  else if ((Mode == TIMx_Input_Pwm_1) || (Mode == TIMx_Input_Pwm_2))
  {
    //ChMask 1
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; //��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? TIM_CCMR1_CC1S_0 : TIM_CCMR1_CC1S_1); //����TI1/TI2
    TIMx->CCER  &= ~TIM_CCER_CC1P; //����������
    
    //ChMask 2
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
    TIMx->CCMR1 |=  TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1;//��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIMx->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? TIM_CCMR1_CC2S_1 : TIM_CCMR1_CC2S_0); //����TI1/TI2
    TIMx->CCER  |=  TIM_CCER_CC2P; //�����½���
    
    //Save mode config
    TIMx->SMCR  &= ~TIM_SMCR_TS;
    TIMx->SMCR  |=  TIM_SMCR_TS_0 | TIM_SMCR_TS_2; //TI1����
    TIMx->SMCR  &= ~TIM_SMCR_SMS;
    TIMx->SMCR  |=  TIM_SMCR_SMS_2; //��ģʽ:��λģʽ
    
    TIMx->CCER  |=  TIM_CCER_CC1E | TIM_CCER_CC2E;  //��������ͨ��1,2
  }
  //������ģʽ
  else if (Mode == TIMx_Input_Encode)
  {
    //���ñ�����ģʽ
    TIMx->CR2 &= ~TIM_CR2_TI1S;   //CH1������TI1����
    TIMx->CR2 &= ~TIM_CR2_MMS;
    TIMx->CR2 |=  TIM_CR2_MMS_0;  //��ģʽ:ʹ��

    TIMx->SMCR &= ~TIM_SMCR_ETF;
    TIMx->SMCR |=  TIM_SMCR_ETF_1;//�����˲�,ÿ��¼4���¼������һ������

    TIMx->SMCR &= ~TIM_SMCR_SMS;
    TIMx->SMCR |=  TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; //��ģʽ: ������ģʽ3 
    
    //����˿ڲ�������
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1F;   //���벶��1���˲���
    TIMx->CCMR1 &= ~TIM_CCMR1_IC1PSC; //���벶��1��Ԥ��Ƶ
    TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC1S_0; //����,IC1ӳ�䵽TI1��
    
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;   //���벶��2���˲���
    TIMx->CCMR1 &= ~TIM_CCMR1_IC2PSC;
    TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIMx->CCMR1 |=  TIM_CCMR1_CC2S_0; //����,IC2ӳ�䵽TI2��
    
    //����˿�ʹ��
    TIMx->CCER  &= ~TIM_CCER_CC2P;    //���벻����
    TIMx->CCER  |=  TIM_CCER_CC2E;    //CH2ʹ��
    TIMx->CCER  &= ~TIM_CCER_CC1P;    //���벻����
    TIMx->CCER  |=  TIM_CCER_CC1E;    //CH1ʹ��
    
    TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(TIMx_FCLK);       //Ԥ��Ƶ��:����Ƶ
    
  }
  
  //������ʱ��
  TIMx->CR1 |=  TIM_CR1_CEN;
  
}


/**
  * @brief  ��ʱ������ģʽ��ʼ��
  * @param  Timer ��ʱ�����
  * @param  ChMask Ҫ���õ�ͨ������
  * @param  Mode ����ģʽ(��ͨ/PWM����/������ģʽ)
  * @retval None
  */
void TIMx_Input_Init(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_INPUT_MODE Mode)
{
  //��������
  TIMx_PortConfig(TIMx, ChMask, TIMx_Port_Input);

  //ģʽ����
  TIMx_Input_ModeConfig(TIMx, ChMask, Mode);
  
}



/**
  * @brief  ��ʱ��x ���ģʽ�ж�ʹ�ܺ���
  * @param  Mask ��ʱ��ͨ������,λֵΪ1ʱ������Ӧ��ͨ��
  * @retval None
  */
void TIMx_Input_IRQEnable(TIM_TypeDef *TIMx, uint8_t ChMask, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->DIER |= (ChMask<<1); //�������ж�
    NVIC_Config(TIMx, 2, 2);    //���ں��ж�
  }
  else 
  {
    NVIC_Disable(NVIC_GetIRQType(TIMx));
  }
  
}



/**
  * @brief  ��ʱ������ģʽ��ʼ��
  * @param  None
  * @retval ��ʱ�������Ĵ���ֵ
  */
uint16_t TIMx_Input_GetCount(TIM_TypeDef *TIMx)
{
  
  return TIMx->CNT;
}



/**
  * @brief  ��ʱ������ģʽ��ʼ��
  * @param  None
  * @retval ��ʱ�������Ĵ���ֵ
  */
uint16_t TIMx_Input_GetCaptureValue(TIM_TypeDef *TIMx, uint8_t ChMask)
{
  uint16_t CaptureValue = 0;
  
  switch (ChMask)
  {
    case 1: CaptureValue = TIMx->CCR1; break;
    case 2: CaptureValue = TIMx->CCR2; break;
    case 4: CaptureValue = TIMx->CCR3; break;
    case 8: CaptureValue = TIMx->CCR4; break;
    
    default: break;
  }
  
  return CaptureValue;
}

