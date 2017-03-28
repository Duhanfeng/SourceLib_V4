/**
  ******************************************************************************
  * @file    tim_output.c
  * @author  �Ź��Ӻ���
  * @version V4.0 �Ĵ����汾
  * @date    2017.03.06
  * @brief   TIMx PWMģʽ����
  ******************************************************************************
  * @attention
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
  *      ����Ӱ�ӼĴ���,���ϼ���,�ǵ�����ģʽ,�������,PWMģʽ
  * ����,
  *      ʱ��:   72MHz
  * 
  * ��Ч��ƽ: Ĭ�ϸߵ�ƽΪ��Ч��ƽ  TIM[Timer]->CCER ������
  *
  * ���ģʽ,
  *   ����Ƚ�: ÿ��CNT=CCRʱ,��ƽ��ת.�ߵ͵�ƽʱ��̶�Ϊһ������
  *   PWMģʽ1: �����ϼ���ʱ��һ��CNT<CCRxʱ,ͨ��Ϊ��Ч��ƽ,����Ϊ��Ч��ƽ
  *   PWMģʽ2: �����ϼ���ʱ��һ��CNT<CCRxʱ,ͨ��Ϊ��Ч��ƽ,����Ϊ��Ч��ƽ
  *   ��ռ�ձ�һ��ʱ,PWM1��PWM2����Ϊ��������
  *   
  * ע1: ��ʱ��4��ͨ�������Զ���װ�غͷ�Ƶ,��Ƶ�ʸ��ֱ��ʱ���һ��.����ռ�ձȸ���Ч
  *      ��ƽ���Ը�������.
  *
  * ע2: PSC(Ԥ��Ƶϵ��)=ʱ��/(Ƶ��*�ֱ���),�Ҳ���ӦΪһ������,����ʵ�ʵ�Ƶ��ֵ
  *      �������(��ʧС������).
  * 
  * ע3: ռ�ձ�=�Ƚ�ֵ/�ֱ���*100%; ���Ҫ����PWM����ռ�ձ�,����ͨ�����ú���
  *      TIMx_SetPwmDutyRatio()��ʵ��.���Ҫ����ڸ߷ֱ��ʸ߾���Ҫ������������,
  *      ����Ը��ݷֱ���ͨ���ӿں����޸���Ƚ�ֵ��ʵ��.
  * 
  * V2.0------------    
  * �޸�����: 1. �޸��ļ�ͷ����,���Ŷ��Զ���װ���Լ�����/�ȽϼĴ����Ľӿں���.
  *           2. ��PWMģʽ��Ƚ�ģʽ�ϲ�,�������ģʽ������;
  *           3. ���ӱȽ��жϵĽӿ�,����ͨ���޸�_TIMx_COMPARE_INT_��ֵ�������ж�
  * ����Ƚ�: ���ڿ���һ���������,������ָʾһ�θ�����ʱ���Ѿ���ʱ��.
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.07.15
  *
  * V3.0------------    
  * �޸�����: 1. �޸ĺ�������,���ļ���������TIM2~5�����(�Ƚ�/PWM),��ͬ��tim_delay
  *              ���ļ���ָ��������ʱ����ͨ���˶�ʱ��ʵ�ֹ���,���ļ����Ա��ඨʱ��
  *              ͬʱ����,������usart.c��spi.c��ʵ�ֻ���.
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V3.0
  * �޸�����: 2016.08.25
  * 
  * V4.0------------    
  * �޸�����: 1. �޸ĺ�������,����TIM1~8
  *           2. ��ʱ���ĵ���ͳһ�淶,���ֹ��ܵ�ʵ����hw_platform�ļ���
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.0
  * �޸�����: 2017.03.06
  *
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_output.h"
#include "SourceLib.h"



/* �ڲ�����-------------------------------------------------------------- */


//TIMxģʽ����
static void TIMx_Output_ModeConfig(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_OUTPUT_MODE Mode)
{
  /* ��ʱ�� */
  TIMx_EnableClock(Timer);
                             
  /* ���ù���ģʽ */          
  TIM[Timer]->CR1 |=  (0X1<<7);    //����Ӱ�ӼĴ���
  TIM[Timer]->CR1 &= ~(0X3<<5);
  TIM[Timer]->CR1 |=  (0X0<<5);    //���ض���ģʽ
  TIM[Timer]->CR1 &= ~(0X1<<4);    //���ϼ���
  TIM[Timer]->CR1 &= ~(0X1<<3);    //�ǵ�����ģʽ
  TIM[Timer]->CR1 &= ~(0X1<<2);    //���ø���Դ
  TIM[Timer]->CR1 &= ~(0X1<<1);    //�������

  /* ѡ����ʱ�� */          
  TIM[Timer]->SMCR &= ~(0X7<<0);   //�رմ�ģʽ--�ڲ�ʱ��72M

  /* �����ж� */              
  TIM[Timer]->DIER &= ~(0X1<<14);  //��ֹ����DMA����
  TIM[Timer]->DIER &= ~(0X1<< 0);  //�ر��ж�
  TIM[Timer]->EGR  |=  (0X1<<0);   //���������¼�
  
  #if 0
  if (Def_IntFlag[Timer])  //���ݱ�־λ�����ж�
  {
    TIM[Timer]->DIER |= (Channel<<1);  //�������ж�
    NVIC_Enable(TIMx_IRQn[Timer],2,2); //���ں��ж�
  }
  #endif
  
  /* ����CH1 PWMģʽ */
  TIM[Timer]->CCMR1 &= ~(0X1<<(7+0));  //OC1REF����ETRFӰ��
  TIM[Timer]->CCMR1 &= ~(0X7<<(4+0));  
  TIM[Timer]->CCMR1 |=  (Mode<<(4+0)); //���ģʽ
  TIM[Timer]->CCMR1 |=  (0X1<<(3+0));  //����Ƚ�1Ԥװ��ʹ��
  TIM[Timer]->CCMR1 |=  (0X1<<(2+0));  //����Ƚ�1����ʹ��
  TIM[Timer]->CCMR1 &= ~(0X3<<(0+0));  //CH1����Ϊ���ģʽ

  /* ����CH2 PWMģʽ */
  TIM[Timer]->CCMR1 &= ~(0X1<<(7+8));  //OC2REF����ETRFӰ��
  TIM[Timer]->CCMR1 &= ~(0X7<<(4+8));  
  TIM[Timer]->CCMR1 |=  (Mode<<(4+8)); //���ģʽ
  TIM[Timer]->CCMR1 |=  (0X1<<(3+8));  //����Ƚ�2Ԥװ��ʹ��
  TIM[Timer]->CCMR1 |=  (0X1<<(2+8));  //����Ƚ�2����ʹ��
  TIM[Timer]->CCMR1 &= ~(0X3<<(0+8));  //CH2����Ϊ���ģʽ
  
  /* ����CH3 PWMģʽ */
  TIM[Timer]->CCMR2 &= ~(0X1<<(7+0));  //OC3REF����ETRFӰ��
  TIM[Timer]->CCMR2 &= ~(0X7<<(4+0));  
  TIM[Timer]->CCMR2 |=  (Mode<<(4+0)); //���ģʽ
  TIM[Timer]->CCMR2 |=  (0X1<<(3+0));  //����Ƚ�3Ԥװ��ʹ��
  TIM[Timer]->CCMR2 |=  (0X1<<(2+0));  //����Ƚ�3����ʹ��
  TIM[Timer]->CCMR2 &= ~(0X3<<(0+0));  //CH3����Ϊ���ģʽ

  /* ����CH4 PWMģʽ */
  TIM[Timer]->CCMR2 &= ~(0X1<<(7+8));  //OC4REF����ETRFӰ��
  TIM[Timer]->CCMR2 &= ~(0X7<<(4+8));  
  TIM[Timer]->CCMR2 |=  (Mode<<(4+8)); //���ģʽ
  TIM[Timer]->CCMR2 |=  (0X1<<(3+8));  //����Ƚ�4Ԥװ��ʹ��
  TIM[Timer]->CCMR2 |=  (0X1<<(2+8));  //����Ƚ�4����ʹ��
  TIM[Timer]->CCMR2 &= ~(0X3<<(0+8));  //CH4����Ϊ���ģʽ
  
  /* ������Ч��ƽ */
  TIM[Timer]->CCER  &= ~(0X1<<(1+0) );  //CH1Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  TIM[Timer]->CCER  &= ~(0X1<<(1+4) );  //CH2Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  TIM[Timer]->CCER  &= ~(0X1<<(1+8) );  //CH3Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  TIM[Timer]->CCER  &= ~(0X1<<(1+12));  //CH4Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  
  /* ���ݹؼ���������� */
  TIM[Timer]->CCER |= (Channel & (0X1<<0)) ? (0X1<<(0+0) ) : 0;
  TIM[Timer]->CCER |= (Channel & (0X1<<1)) ? (0X1<<(0+4) ) : 0;
  TIM[Timer]->CCER |= (Channel & (0X1<<2)) ? (0X1<<(0+8) ) : 0;
  TIM[Timer]->CCER |= (Channel & (0X1<<3)) ? (0X1<<(0+12) ) : 0;
  
  /* ����PWM���� */
  TIM[Timer]->PSC  = TIM_GET_PSC(1000, 1000);  //Ƶ������
  TIM[Timer]->ARR  = 1000;   //�ֱ�������(Ĭ��)
  
  if(Timer == TIMx_1)
  {
    TIM[Timer]->BDTR |= (0x1<<15);
    TIM[Timer]->BDTR |= (0x1<<14);
  }
  
  /* ������ʱ�� */
//  TIM[Timer]->CR1  |= (0X1<<0);

}



/* �ӿں���-------------------------------------------------------------- */

/**
  * @brief  ��ʱ��x PWM���ģʽ���ú���
  * @param  Mask ��ʱ��ͨ��������,λֵΪ1ʱ������Ӧ��ͨ��
  * @retval None
  */
void TIMx_Output_Init(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, TIMx_OUTPUT_MODE Mode)
{
  /* �������� */
  TIMx_PortConfig(Timer, Channel, TIMx_Port_Output);

  /* ģʽ���� */
  TIMx_Output_ModeConfig(Timer, Channel, Mode);
  
  /* ʱ������ */
  TIMx_Output_SetPwmDutyRatio(Timer, Channel, 50);
  
  /* ������ʱ�� */
  TIMx_Output_Enable(Timer, 1);
  
}



/**
  * @brief  ��ʱ��x ���ģʽ�ж�ʹ�ܺ���
  * @param  Mask ��ʱ��ͨ��������,λֵΪ1ʱ������Ӧ��ͨ��
  * @retval None
  */
void TIMx_Output_IRQEnable(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->DIER |= (Channel<<1);  //�������ж�
    NVIC_Enable(TIMx_IRQn[Timer], 2, 2); //���ں��ж�
  }
  else 
  {
    NVIC_Disable(TIMx_IRQn[Timer]);
  }
  
}


/**
  * @brief  ��ʱ��xͨ����PWMռ�ձ����ú���
  * @param  Channel ͨ������
  * @param  dDutyRatio PWM����ռ�ձ�,��ΧΪ[0,100]
  * @retval None
  */
void TIMx_Output_SetPwmDutyRatio(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, float dDutyRatio)
{
  uint16_t RegValue = dDutyRatio * (TIM[Timer]->ARR) / 100;
  
  TIM[Timer]->CCR1 = (Channel & 0X01) ? RegValue : TIM[Timer]->CCR1;
  TIM[Timer]->CCR2 = (Channel & 0X02) ? RegValue : TIM[Timer]->CCR2;
  TIM[Timer]->CCR3 = (Channel & 0X04) ? RegValue : TIM[Timer]->CCR3;
  TIM[Timer]->CCR4 = (Channel & 0X08) ? RegValue : TIM[Timer]->CCR4;
  
}



/**
  * @brief  ��ʱ��xͨ����PWMռ�ձ����ú���
  * @param  Channel ͨ������
  * @param  nCompareVal �Ƚ�ֵ,��ֵ��
  * @retval None
  */
void TIMx_Output_SetCompareVal(TIM_TYPE Timer, TIMx_CHANNEL_MASK Channel, uint16_t nCompareVal)
{
  TIM[Timer]->CCR1 = (Channel & 0X01) ? nCompareVal : TIM[Timer]->CCR1;
  TIM[Timer]->CCR2 = (Channel & 0X02) ? nCompareVal : TIM[Timer]->CCR2;
  TIM[Timer]->CCR3 = (Channel & 0X04) ? nCompareVal : TIM[Timer]->CCR3;
  TIM[Timer]->CCR4 = (Channel & 0X08) ? nCompareVal : TIM[Timer]->CCR4;
  
}



/**
  * @brief  ��ʱ��xƵ�����ú���
  * @param  uiFrq  PWM����Ƶ��
  * @retval None
  */
void TIMx_Output_SetPwmFrq(TIM_TYPE Timer, uint32_t iFrq)
{
  TIM[Timer]->PSC  = TIM_GET_PSC(iFrq, TIM[Timer]->ARR);  //Ƶ������
  
}



/**
  * @brief  ��ʱ��x�����Զ��ؼ���ֵ����(����ռ�ձ�)
  * @param  nReloadVal �ؼ���ֵ
  * @retval None
  */
void TIMx_Output_SetAutoReloadReg(TIM_TYPE Timer, uint16_t nReloadVal)
{
  TIM[Timer]->ARR = nReloadVal;
  
}



/**
  * @brief  ��ȡ��ʱ��xARR�Ĵ����е��ؼ���ֵ
  * @param  None
  * @retval ARR�Ĵ����е��ؼ���ֵ
  */
uint16_t TIMx_Output_GetAutoReloadReg(TIM_TYPE Timer)
{
  
  return TIM[Timer]->ARR;
}



/**
  * @brief  ��ʱ��xʹ�ܺ���
  * @param  cFlag 0:�رն�ʱ�� 1:������ʱ��
  * @retval None
  */
void TIMx_Output_Enable(TIM_TYPE Timer, uint8_t isEnable)
{
  if (isEnable)
  {
    TIM[Timer]->CR1 |= (0X1<<0);  //������ʱ��  
  }
  else 
  {
    TIM[Timer]->CR1 &= ~(0X1<<0);   //�رն�ʱ��
  }

}

