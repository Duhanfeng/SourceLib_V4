/**
  ******************************************************************************
  * @file    tim_capture.c
  * @author  �Ź��Ӻ���
  * @version V4.0 �Ĵ����汾
  * @date    2017.02.14
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
  * ��Ч��ƽ: Ĭ�ϸߵ�ƽΪ��Ч��ƽ  TIM[Timer]->CCER ������
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
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_input.h"
#include "SourceLib.h"



//TIMxģʽ����
static void TIMx_Input_ModeConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK ChannelMask, TIMx_INPUT_MODE Mode)
{
  /* ��ʱ�� */
  TIMx_EnableClock(Timer);
  
  //mode configure
  TIM[Timer]->CR1 &= ~(0X3<<8); //ʱ�ӷ�Ƶ����
  TIM[Timer]->CR1 |=  (0X1<<7); //����Ӱ��
  TIM[Timer]->CR1 &= ~(0X3<<5); //���ض���ģʽ
  TIM[Timer]->CR1 &= ~(0X1<<4); //���������ϼ���
  TIM[Timer]->CR1 &= ~(0X1<<3); //�ǵ�����ģʽ
  TIM[Timer]->CR1 &= ~(0X1<<2); //���ø���Դ--���������������
  TIM[Timer]->CR1 &= ~(0X1<<1); //��������¼�
  
  /* ����ʱ����� */
  TIM[Timer]->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000);  //Ԥ��Ƶ��:��Ƶ,����Ƶ��Ϊ1MHz
  TIM[Timer]->ARR = 0XFFFF;     //����Զ���װ��ֵ
  
  /* ѡ����ʱ�� */
  TIM[Timer]->SMCR &= ~(0X7<<0);//�رմ�ģʽ,ѡ���ڲ�ʱ��
  
  /* ���ò���ģʽ */
  if (Mode == TIMx_Input_Normal)
  {
    //Channel 1
    TIM[Timer]->CCMR1 &= ~(0xF<<4);
    TIM[Timer]->CCMR1 |=  (0x3<<4); //��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIM[Timer]->CCMR1 &= ~(0x3<<0);
    TIM[Timer]->CCMR1 |=  (0x1<<0); //����TI1
    TIM[Timer]->CCER  &= ~(0x1<<1); //����������
    TIM[Timer]->CCER  |=  (ChannelMask & 0x01) ? (0x1<<0) : 0x00;
    
    //Channel 2
    TIM[Timer]->CCMR1 &= ~(0xF<<12);
    TIM[Timer]->CCMR1 |=  (0x3<<12);//��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIM[Timer]->CCMR1 &= ~(0x3<<8);
    TIM[Timer]->CCMR1 |=  (0x1<<8); //����TI2
    TIM[Timer]->CCER  &= ~(0x1<<5); //����������
    TIM[Timer]->CCER  |=  (ChannelMask & 0x02) ? (0x1<<4) : 0x00;
    
    //Channel 3
    TIM[Timer]->CCMR2 &= ~(0xF<<4);
    TIM[Timer]->CCMR2 |=  (0x3<<4); //��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIM[Timer]->CCMR2 &= ~(0x3<<0);
    TIM[Timer]->CCMR2 |=  (0x1<<0); //����TI3
    TIM[Timer]->CCER  &= ~(0x1<<13); //����������
    TIM[Timer]->CCER  |=  (ChannelMask & 0x04) ? (0x1<<8) : 0x00;
    
    //Channel 4
    TIM[Timer]->CCMR2 &= ~(0xF<<12);
    TIM[Timer]->CCMR2 |=  (0x3<<12);//��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIM[Timer]->CCMR2 &= ~(0x3<<8);
    TIM[Timer]->CCMR2 |=  (0x1<<8); //����TI4
    TIM[Timer]->CCER  &= ~(0x1<<9); //����������
    TIM[Timer]->CCER  |=  (ChannelMask & 0x08) ? (0x1<<12) : 0x00;
    
  }
  //Pwm input mode
  else if ((Mode == TIMx_Input_Pwm_1) || (Mode == TIMx_Input_Pwm_2))
  {
    //Channel 1
    TIM[Timer]->CCMR1 &= ~(0xF<<4);
    TIM[Timer]->CCMR1 |=  (0x3<<4); //��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIM[Timer]->CCMR1 &= ~(0x3<<0);
    TIM[Timer]->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? (0x1<<0) : (0x2<<0)); //����TI1/TI2
    TIM[Timer]->CCER  &= ~(0x1<<1); //����������
    
    //Channel 2
    TIM[Timer]->CCMR1 &= ~(0xF<<12);
    TIM[Timer]->CCMR1 |=  (0x3<<12);//��Fck_intƵ�ʲ���,8����ͬ�Ĳ�������ȷ�ϵ�ƽ
    TIM[Timer]->CCMR1 &= ~(0x3<<8);
    TIM[Timer]->CCMR1 |=  ((Mode == TIMx_Input_Pwm_1) ? (0x2<<8) : (0x1<<8)); //����TI1/TI2
    TIM[Timer]->CCER  |=  (0x1<<5); //�����½���
    
    //Save mode config
    TIM[Timer]->SMCR  &= ~(0x7<<4);
    TIM[Timer]->SMCR  |=  (0x5<<4); //TI1����
    TIM[Timer]->SMCR  &= ~(0x7<<0);
    TIM[Timer]->SMCR  |=  (0x4<<0); //��ģʽ:��λģʽ
    
    TIM[Timer]->CCER  |=  (0x1<<0) | (0x1<<4);  //��������ͨ��1,2
  }
  //Encode mode
  else if (Mode == TIMx_Input_Encode)
  {
    /* ���ñ�����ģʽ */
    TIM[Timer]->CR2 &= ~(0X1<<7);  //CH1������TI1����
    TIM[Timer]->CR2 &= ~(0X7<<4);
    TIM[Timer]->CR2 |=  (0X1<<4);  //��ģʽ:ʹ��

    TIM[Timer]->SMCR &= ~(0XF<<8);
    TIM[Timer]->SMCR |=  (0X2<<8); //�����˲�,ÿ��¼4���¼������һ������

    TIM[Timer]->SMCR &= ~(0X7<<0);
    TIM[Timer]->SMCR |=  (0X3<<0); //��ģʽ: ������ģʽ3 
    
    //input port capture
    TIM[Timer]->CCMR1 &= ~(0XF<<(4+0));
    TIM[Timer]->CCMR1 |=  (0X0<<(4+0));  //���벶��1���˲���
    TIM[Timer]->CCMR1 &= ~(0X3<<(2+0));
    TIM[Timer]->CCMR1 |=  (0X3<<(2+0));  //���벶��1��Ԥ��Ƶ
    TIM[Timer]->CCMR1 &= ~(0X1<<(0+0));
    TIM[Timer]->CCMR1 |=  (0X1<<(0+0));  //����,IC1ӳ�䵽TI1��
    
    TIM[Timer]->CCMR1 &= ~(0XF<<(4+8));
    TIM[Timer]->CCMR1 |=  (0X0<<(4+8));  //���벶��2���˲���
    TIM[Timer]->CCMR1 &= ~(0X3<<(2+8));
    TIM[Timer]->CCMR1 |=  (0X0<<(2+8));  //���벶��2��Ԥ��Ƶ
    TIM[Timer]->CCMR1 &= ~(0X1<<(0+8));
    TIM[Timer]->CCMR1 |=  (0X1<<(0+8));  //����,IC2ӳ�䵽TI2��
    
    //capture port enable
    TIM[Timer]->CCER  &= ~(0X1<<5);      //���벻����
    TIM[Timer]->CCER  |=  (0X1<<4);      //CH2ʹ��
    TIM[Timer]->CCER  &= ~(0X1<<1);      //���벻����
    TIM[Timer]->CCER  |=  (0X1<<0);      //CH1ʹ��
    
    TIM[Timer]->PSC = TIM_GET_PSC_BY_CNT_FRE(TIMx_FCLK);       //Ԥ��Ƶ��:����Ƶ
    
  }
  
  /* ������ʱ�� */
  TIM[Timer]->CR1 |=  (0x1<<0);
  
}


/**
  * @brief  ��ʱ������ģʽ��ʼ��
  * @param  Timer ��ʱ�����
  * @param  ChannelMask Ҫ���õ�ͨ������
  * @param  Mode ����ģʽ(��ͨ/PWM����/������ģʽ)
  * @retval None
  */
void TIMx_Input_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK ChannelMask, TIMx_INPUT_MODE Mode)
{
  /* �������� */
  TIMx_PortConfig(Timer, ChannelMask, TIMx_Port_Input);

  /* ģʽ���� */
  TIMx_Input_ModeConfig(Timer, ChannelMask, Mode);
  
}

 

/**
  * @brief  ��ʱ������ģʽ��ʼ��
  * @param  None
  * @retval ��ʱ�������Ĵ���ֵ
  */
uint16_t TIMx_Input_GetCount(TIM_TYPE Timer)
{
  
  return TIM[Timer]->CNT;
}




/**
  * @brief  ��ʱ������ģʽ��ʼ��
  * @param  None
  * @retval ��ʱ�������Ĵ���ֵ
  */
uint16_t TIMx_Input_GetCaptureValue(TIM_TYPE Timer, TIMx_CHANNEL_MASK cChannel)
{
  uint16_t CaptureValue = 0;
  
  switch (cChannel)
  {
    case 1: CaptureValue = TIM[Timer]->CCR1; break;
    case 2: CaptureValue = TIM[Timer]->CCR2; break;
    case 4: CaptureValue = TIM[Timer]->CCR3; break;
    case 8: CaptureValue = TIM[Timer]->CCR4; break;
    
    default: break;
  }
  
  return CaptureValue;
}



