/**
  ******************************************************************************
  * @file    tim_output.c
  * @author  �Ź��Ӻ���
  * @version V4.1 �Ĵ����汾
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
  * ��Ч��ƽ: Ĭ�ϸߵ�ƽΪ��Ч��ƽ  TIMx->CCER ������
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
  * V4.1------------
  * �޸�����: 1.�޸������û���,�����ָ������������ĳ�ֱ�ӵ�����ָ�����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.1
  * �޸�����: 2017.05.23
  * 
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "hw_tim_output.h"
#include "SourceLib.h"



/* �ڲ�����-------------------------------------------------------------- */


/**
  * @brief  ��ʱ��x ģʽ����
  * @param  TIMx ��ʱ��ָ��
  * @param  ChMask ��ʱ��ͨ������,λֵΪ1ʱ������Ӧ��ͨ��
  * @param  Mode ����ģʽ
  * @retval None
  */
static void TIMx_Output_ModeConfig(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_OUTPUT_MODE Mode)
{
  //��ʱ��
  RCC_EnableClock(TIMx, 1);
                             
  //���ù���ģʽ
  TIMx->CR1 |=  TIM_CR1_ARPE; //����Ӱ�ӼĴ���
  TIMx->CR1 &= ~TIM_CR1_CMS;  //���ض���ģʽ
  TIMx->CR1 &= ~TIM_CR1_DIR;  //���ϼ���
  TIMx->CR1 &= ~TIM_CR1_OPM;  //�ǵ�����ģʽ
  TIMx->CR1 &= ~TIM_CR1_URS;  //���ø���Դ
  TIMx->CR1 &= ~TIM_CR1_UDIS; //�������

  //ѡ����ʱ��
  TIMx->SMCR &= ~TIM_SMCR_SMS;   //�رմ�ģʽ--�ڲ�ʱ��72M

  //�����ж�             
  TIMx->DIER &= ~TIM_DIER_UDE;  //��ֹ����DMA����
  TIMx->DIER &= ~TIM_DIER_UIE;  //�رո����ж�
  TIMx->EGR  |=  TIM_EGR_UG;    //���������¼�
  
  #if 0
  if (Def_IntFlag[Timer])  //���ݱ�־λ�����ж�
  {
    TIMx->DIER |= (ChMask<<1);  //�������ж�
    NVIC_Enable(TIMx_IRQn[Timer],2,2); //���ں��ж�
  }
  #endif
  
  //����CH1 PWMģʽ
  TIMx->CCMR1 &= ~TIM_CCMR1_OC1CE;  //OC1REF����ETRFӰ��
  TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;  
  TIMx->CCMR1 |=  (Mode<<(4+0));    //���ģʽ
  TIMx->CCMR1 |=  TIM_CCMR1_OC1PE;  //����Ƚ�1Ԥװ��ʹ��
  TIMx->CCMR1 |=  TIM_CCMR1_OC1FE;  //����Ƚ�1����ʹ��
  TIMx->CCMR1 &= ~TIM_CCMR1_CC1S;   //CH1����Ϊ���ģʽ

  //����CH2 PWMģʽ
  TIMx->CCMR1 &= ~TIM_CCMR1_OC2CE;  //OC2REF����ETRFӰ��
  TIMx->CCMR1 &= ~TIM_CCMR1_OC2M;  
  TIMx->CCMR1 |=  (Mode<<(4+8));    //���ģʽ
  TIMx->CCMR1 |=  TIM_CCMR1_OC2PE;  //����Ƚ�2Ԥװ��ʹ��
  TIMx->CCMR1 |=  TIM_CCMR1_OC2FE;  //����Ƚ�2����ʹ��
  TIMx->CCMR1 &= ~TIM_CCMR1_CC2S;   //CH2����Ϊ���ģʽ
  
  
  //����CH3 PWMģʽ
  TIMx->CCMR2 &= ~TIM_CCMR2_OC3CE;  //OC3REF����ETRFӰ��
  TIMx->CCMR2 &= ~TIM_CCMR2_OC3M;  
  TIMx->CCMR2 |=  (Mode<<(4+0));    //���ģʽ
  TIMx->CCMR2 |=  TIM_CCMR2_OC3PE;  //����Ƚ�3Ԥװ��ʹ��
  TIMx->CCMR2 |=  TIM_CCMR2_OC3FE;  //����Ƚ�3����ʹ��
  TIMx->CCMR2 &= ~TIM_CCMR2_CC3S;   //CH3����Ϊ���ģʽ
  
  //����CH4 PWMģʽ
  TIMx->CCMR2 &= ~TIM_CCMR2_OC4CE;  //OC4REF����ETRFӰ��
  TIMx->CCMR2 &= ~TIM_CCMR2_OC4M;   
  TIMx->CCMR2 |=  (Mode<<(4+8));    //���ģʽ
  TIMx->CCMR2 |=  TIM_CCMR2_OC4PE;  //����Ƚ�4Ԥװ��ʹ��
  TIMx->CCMR2 |=  TIM_CCMR2_OC4FE;  //����Ƚ�4����ʹ��
  TIMx->CCMR2 &= ~TIM_CCMR2_CC4S;   //CH4����Ϊ���ģʽ
  
  //������Ч��ƽ
  TIMx->CCER  &= ~TIM_CCER_CC1P;    //CH1Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  TIMx->CCER  &= ~TIM_CCER_CC2P;    //CH2Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  TIMx->CCER  &= ~TIM_CCER_CC3P;    //CH3Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  TIMx->CCER  &= ~TIM_CCER_CC4P;    //CH4Ĭ�ϸߵ�ƽΪ��Ч��ƽ
  
  //���ݹؼ����������
  TIMx->CCER |= (ChMask & (0X1<<0)) ? TIM_CCER_CC1E : 0;
  TIMx->CCER |= (ChMask & (0X1<<1)) ? TIM_CCER_CC2E : 0;
  TIMx->CCER |= (ChMask & (0X1<<2)) ? TIM_CCER_CC3E : 0;
  TIMx->CCER |= (ChMask & (0X1<<3)) ? TIM_CCER_CC4E : 0;
  
  //����PWM����
  TIMx->PSC  = TIM_GET_PSC_BY_OP_FRE(1000, 1000);  //Ƶ������
  TIMx->ARR  = 1000;   //�ֱ�������(Ĭ��)
  
  //ʹ�����
  if((TIMx == TIM1) || ((TIMx == TIM8)))
  {
    TIMx->BDTR |= TIM_BDTR_MOE; //�����ʹ��
    TIMx->BDTR |= TIM_BDTR_AOE; //�Զ����ʹ��
  }
  
  //������ʱ��
//  TIMx->CR1  |= TIM_CR1_CEN;

}



/* �ӿں���-------------------------------------------------------------- */

/**
  * @brief  ��ʱ��x PWM���ģʽ���ú���
  * @param  TIMx ��ʱ��ָ��
  * @param  ChMask ��ʱ��ͨ������,λֵΪ1ʱ������Ӧ��ͨ��
  * @retval None
  */
void TIMx_Output_Init(TIM_TypeDef *TIMx, uint8_t ChMask, TIMx_OUTPUT_MODE Mode)
{
  //��������
  TIMx_PortConfig(TIMx, ChMask, TIMx_Port_Output);

  //ģʽ����
  TIMx_Output_ModeConfig(TIMx, ChMask, Mode);
  
  //ʱ������
  TIMx_Output_SetPwmDutyRatio(TIMx, ChMask, 50);
  
  //������ʱ��
  TIMx_Output_Enable(TIMx, 1);
  
}



/**
  * @brief  ��ʱ��x ���ģʽ�ж�ʹ�ܺ���
  * @param  TIMx ��ʱ��ָ��
  * @param  ChMask ��ʱ��ͨ������,λֵΪ1ʱ������Ӧ��ͨ��
  * @retval None
  */
void TIMx_Output_IRQEnable(TIM_TypeDef *TIMx, uint8_t ChMask, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->DIER |= (ChMask<<1);  //�������ж�
    NVIC_Config(TIMx, 2, 2);    //���ں��ж�
  }
  else 
  {
    NVIC_Disable(NVIC_GetIRQType(TIMx));
  }
  
}


/**
  * @brief  ��ʱ��xͨ����PWMռ�ձ����ú���
  * @param  TIMx ��ʱ��ָ��
  * @param  ChMask ��ʱ��ͨ������,λֵΪ1ʱ������Ӧ��ͨ��
  * @param  dDutyRatio PWM����ռ�ձ�,��ΧΪ[0,100]
  * @retval None
  */
void TIMx_Output_SetPwmDutyRatio(TIM_TypeDef *TIMx, uint8_t ChMask, float dDutyRatio)
{
  uint16_t RegValue = dDutyRatio * (TIMx->ARR) / 100;
  
  TIMx->CCR1 = (ChMask & 0X01) ? RegValue : TIMx->CCR1;
  TIMx->CCR2 = (ChMask & 0X02) ? RegValue : TIMx->CCR2;
  TIMx->CCR3 = (ChMask & 0X04) ? RegValue : TIMx->CCR3;
  TIMx->CCR4 = (ChMask & 0X08) ? RegValue : TIMx->CCR4;
  
}



/**
  * @brief  ��ʱ��xͨ����PWMռ�ձ����ú���
  * @param  TIMx ��ʱ��ָ��
  * @param  ChMask ͨ������
  * @param  nCompareVal �Ƚ�ֵ,��ֵ��
  * @retval None
  */
void TIMx_Output_SetCompareVal(TIM_TypeDef *TIMx, uint8_t ChMask, uint16_t nCompareVal)
{
  TIMx->CCR1 = (ChMask & 0X01) ? nCompareVal : TIMx->CCR1;
  TIMx->CCR2 = (ChMask & 0X02) ? nCompareVal : TIMx->CCR2;
  TIMx->CCR3 = (ChMask & 0X04) ? nCompareVal : TIMx->CCR3;
  TIMx->CCR4 = (ChMask & 0X08) ? nCompareVal : TIMx->CCR4;
  
}



/**
  * @brief  ��ʱ��xƵ�����ú���
  * @param  TIMx ��ʱ��ָ��
  * @param  iFrq  PWM����Ƶ��
  * @retval None
  */
void TIMx_Output_SetPwmFrq(TIM_TypeDef *TIMx, uint32_t iFrq)
{
  TIMx->PSC  = TIM_GET_PSC_BY_OP_FRE(iFrq, TIMx->ARR);  //Ƶ������
  
}



/**
  * @brief  ��ʱ��x�����Զ��ؼ���ֵ����(����ռ�ձ�)
  * @param  nReloadVal �ؼ���ֵ
  * @retval None
  */
void TIMx_Output_SetAutoReloadReg(TIM_TypeDef *TIMx, uint16_t nReloadVal)
{
  TIMx->ARR = nReloadVal;
  
}



/**
  * @brief  ��ȡ��ʱ��xARR�Ĵ����е��ؼ���ֵ
  * @param  TIMx ��ʱ��ָ��
  * @retval ARR�Ĵ����е��ؼ���ֵ
  */
uint16_t TIMx_Output_GetAutoReloadReg(TIM_TypeDef *TIMx)
{
  
  return TIMx->ARR;
}



/**
  * @brief  ��ʱ��xʹ�ܺ���
  * @param  TIMx ��ʱ��ָ��
  * @param  isEnable 0:�رն�ʱ�� 1:������ʱ��
  * @retval None
  */
void TIMx_Output_Enable(TIM_TypeDef *TIMx, uint8_t isEnable)
{
  if (isEnable)
  {
    TIMx->CR1 |=  TIM_CR1_CEN;  //������ʱ��  
  }
  else 
  {
    TIMx->CR1 &= ~TIM_CR1_CEN;  //�رն�ʱ��
  }

}

