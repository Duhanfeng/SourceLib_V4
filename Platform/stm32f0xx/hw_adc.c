/** 
  ******************************************************************************
  * @file    adc1.c
  * @author  �Ź��Ӻ���
  * @version V1.0 �Ĵ����汾
  * @date    2016.02.25
  * @brief   ADC1����ת��
  ******************************************************************************
  * @attention
  *
  * ����,ģ������ 
  *      PA0 --- ADC12_CH0 
  *      PA1 --- ADC12_CH1 
  *      PA2 --- ADC12_CH2 
  *      PA3 --- ADC12_CH3 
  * 
  * ��������: ���������� ADC_SMPR ��������
  * ͨ������: �������ͨ����ADC_SQRx������,�������ת����ͨ������˳��.
  * 
  * TCONV = ����ʱ��+ 12.5������
  * ģ���� = ������*(3.3V/4096)
  * 
  * �޸�,
  *      1. �޸�ͨ�������ĺ�
  *      2. ����ͨ����������
  *      3. ����AD�ɼ�����(ADC_SMPRx)
  *      4. ����ADCͨ��ת����˳��(ADC_SQRx)
  *      5. ����ʵ������޸� ADC_GetDigVal() ����
  * 
  * 
  * ע2: ADC������ʱ�Ӳ��ó���14MHz(ʵ�ʵ�ʱ��Ϊ12M)
  * ע3: ������ֻ��һ��ת�����ݼĴ���������Ҫ��ʱ��ȡת������(ʹ��DMA���Ա�������⣩
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "SourceLib.h"
#include "hw_adc.h"


uint16_t g_FilterValue = 0;


// 0: 1.167us
// 1: 1.667us
// 2: 2.167us
// 3: 3.417us
// 4: 4.500us
// 5: 5.667us
// 6: 7.000us
// 7: 21.00us


//����IO
static void ADC_IOConfig(void)
{
  #if 0
  /* ��ʱ�� */
  RCC->AHBENR |=  (0X1<<17);  //PA
  
  /* �������� */
  GPIOA->MODER &= ~(0XF<<(0*2));
  GPIOA->MODER |=  (0XF<<(0*2));   //PA0-PA3, ģ��ģʽ
  #endif
  
  GPIOx_FastInit(GPIOx_PA, GPIOx_1, GPIOx_ANALOG);
  
}



//����ģʽ
static void ADC_ModeConfig(void)
{
  /* ��ʱ�� */
  RCC->APB2ENR |= (0X1<<9);  //ADC1
  
  /* ʱ�ӷ�Ƶ */
  ADC1->CFGR2 &= ~(0X3U<<30);
  ADC1->CFGR2 |=  (0X2U<<30);   //4��Ƶ(ADC��ʱ��Ƶ�ʲ��ó���14M,�˷�Ƶ��Ϊ12M)
    
  /* ģʽ���� */
  ADC1->CFGR1 &= ~(0X1<<16);  //�Ǽ��ģʽ
  ADC1->CFGR1 |=  (0X1<<14);  //�ȴ�ģʽ
  ADC1->CFGR1 |=  (0X1<<13);  //����ת��ģʽ  ADC_CFGR1_CONT
  ADC1->CFGR1 &= ~(0X3<<10);  //�������
  ADC1->CFGR1 &= ~(0X1<<5);   //�Ҷ���
  ADC1->CFGR1 &= ~(0X3<<3); 
  ADC1->CFGR1 |=  (0X0<<3);   //12λ�ֱ���
  ADC1->CFGR1 &= ~(0X1<<2);   //������ɨ�跽��
//  ADC1->CFGR1 |=  (0X1<<0);   //DMAѭ��ģʽ
//  ADC1->CFGR1 |=  (0X1<<0);   //ʹ��DMA
  
  /* �������� */
  ADC1->CFGR1 &= ~(0X3<<10);
//  ADC1->CFGR1 |=  (0X3<<10);  //�ⲿ����ģʽ�������ؼ��½���
  ADC1->CFGR1 &= ~(0X7<<6);   //�ر��ⲿ����ģʽ
//  ADC1->CFGR1 |=  (0X3<<6);   //��ʱ��3_TRGO�źŴ���
  
  /* ʱ������ */
  ADC1->SMPR &= ~(0X7<<1);
  ADC1->SMPR |=  (0X7<<1);    //239.5+12.5������,��21us�����һ�β���
  
  /* ͨ��ѡ�� */
  ADC1->CHSELR |= (0X1<<1);   //ͨ��1
  
  /* �����ж� */
  ADC1->IER |= (0X1<<2);  //����ת���ж�
  ADC1->ISR &= ~(0x1<<2); //���־λ
  
  /* ���ú˼��ж� */
  NVIC_SetPriority(ADC1_IRQn, 2);
  NVIC_EnableIRQ(ADC1_IRQn);
  
  /* У׼ADC */
  ADC1->CR |=  (0X1U<<31);       //��ʼУ׼
  while (ADC1->CR & (0X1U<<31));  //�ȴ�У׼���
  
  /*����ADC1*/
  ADC1->CR |=  (0X1<<0);
  
}



/**
  * @brief  ������
  * @param  None
  * @retval None
  */
void ADC_Init(void)
{
  /* ����IOģʽ */
  ADC_IOConfig();
  
  /* ���ù���ģʽ */
  ADC_ModeConfig();
  
}
  


/**
  * @brief  ����ADת��
  * @param  None
  * @retval None
  * @note   ������TROG������,����ADCת����������������,ֻ�н��յ�TRGO�ź�
  *         ��Ż�����ת��
  */
void ADC_StartConversion(void)
{
  while (ADC1->CR & (0X1<<2));
  
  /* ����ADCת�� */
  ADC1->CR |=  (0X1<<2);
  
}



/**
  * @brief  һ�׵ݹ��˲��㷨
  * @param  Vib_Value: ����ֵ
  * @retval Vib_Val: �˲����
  */
static uint16_t Vib_Filter(uint16_t Vib_Value)
{
  static uint16_t Vib_Fil_Val = 0;
  static uint8_t First_Flag = 1;
 
  if(First_Flag)
  {
    Vib_Fil_Val = Vib_Value;
    First_Flag = 0;
  }
  else
  {
    Vib_Fil_Val = (Vib_Fil_Val>>1) + (Vib_Value>>1);
  }
   
  return Vib_Fil_Val;
}



uint8_t g_AdcFinishFlag = 0;


//�ж�ʱ��: 2us/3us
void ADC1_IRQHandler(void)
{
  static uint32_t s_Sum = 0;
  static uint8_t s_Number = 0;
  
  /* �ж�����ж� */
  if (ADC1->ISR & (0x1<<2)) //����ת���ж�
  {
    ADC1->ISR &= ~(0x1<<2); //���־λ
    
    s_Sum += ADC1->DR;
    s_Number++;
    
    //ÿ8��������һ��ƽ��ֵ
    if (s_Number == 8)
    {
      g_FilterValue = Vib_Filter(s_Sum>>3);
      g_FilterValue = (g_FilterValue == 0xFFE) ? 0xFFF : g_FilterValue;
      g_AdcFinishFlag = 1;
      s_Sum = 0;
      s_Number = 0;
    }
    
  }
  
}
