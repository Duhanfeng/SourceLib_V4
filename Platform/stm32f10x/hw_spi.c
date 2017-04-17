/**
  ******************************************************************************
  * @file    SPI.c
  * @author  �Ź��Ӻ���
  * @version V2.0 �Ĵ����汾
  * @date    2016.08.11
  * @brief   SPIxʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  *
  * SPIx:   NSS  --- PA4   ͨ������,10M
  *         SCK  --- PA5   ��������,50M
  *         MISO --- PA6   ��������
  *         MOSI --- PA7   ��������,50M
  *
  * SPI2:   NSS  --- PB12  ͨ������,10M
  *         SCK  --- PB13  ��������,50M
  *         MISO --- PB14  ��������
  *         MOSI --- PB15  ��������,50M
  *
  * SPI3:   NSS  --- PA15  ͨ������,10M
  *         SCK  --- PB3   ��������,50M
  *         MISO --- PB4   ��������
  *         MOSI --- PB5   ��������,50M
  * 
  * ����֡: 8λ����λ,��λ��ǰ,ÿ��һλ��ȥ�ӻ����᷵��һλ�����ݻ���
  * 
  * ���ļ��н�SPI��NSSƬѡ��������Ϊ�������ģʽ,����,���SPI�豸ʱ���������ڴ�Ƭѡλ
  * ��ʵ���������(��ֹƬѡ�Ÿ���).
  *
  * V2.0------------
  * �޸�����: �޸������ٶȺ�����Ч�Ĵ���
  * ����ԭ��: Ӧ�������CR1[5:3]λ,ԭ���"SPIx->CR1 &= ~(cSeep<<3)"Ϊ��Ч�Ĵ���
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.07.08
  *
  * V3.0------------
  * �޸�����: �޸ĵײ�ʵ�ֻ���,�����ļ�ƥ��SPI1~3
  * ����ԭ��: Ӧ�������CR1[5:3]λ,ԭ���"SPIx->CR1 &= ~(cSeep<<3)"Ϊ��Ч�Ĵ���
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.07.08
  *
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "hw_spi.h"


/* ---ӳ������Ĵ���--- */
#if(defined(STM32F10X_HD)||defined(STM32F10X_HD_VL)||defined(STM32F10X_XL)||defined(STM32F10X_CL))
static SPI_TypeDef  * const SPI[3] = {SPI1, SPI2, SPI3};
static const uint8_t s_SpiPortNum = 3;

#elif(defined(STM32F10X_MD)||defined(STM32F10X_MD_VL))
static SPI_TypeDef  * const SPI[2] = {SPI1, SPI2};
static const uint8_t s_SpiPortNum = 2;

#elif(defined(STM32F10X_LD)||defined(STM32F10X_LD_VL))
static SPI_TypeDef  * const SPI[1] = {SPI1};
static const uint8_t s_SpiPortNum = 1;

#else
#error Undefine STM32F10x Flash Size

#endif 

/**
  * @brief  SPIx��������
  * @param  None
  * @retval None
  */
static void SPIx_IOConfig(SPI_TYPE Port)
{
  switch (Port)
  {
    case SPIx_1: 
    {
//      GPIOx_FastInit(GPIOx_PA, GPIOx_4, GPIOx_GP_PP_10M);
      GPIOx_FastInit(GPIOx_PA, GPIOx_5, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PA, GPIOx_6, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PA, GPIOx_7, GPIOx_AF_PP_50M);
      
      /*��ʼ�����*/
//      SPI1_NSS = 1;
      break; 
    }
    
    case SPIx_2: 
    {
//      GPIOx_FastInit(GPIOx_PB, GPIOx_12, GPIOx_GP_PP_10M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_13, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_14, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PB, GPIOx_15, GPIOx_AF_PP_50M);
      
      /*��ʼ�����*/
//      SPI2_NSS = 1;
      break; 
    }
    
    case SPIx_3: 
    {
      GPIOx_FastInit(GPIOx_PA, GPIOx_15, GPIOx_GP_PP_10M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_3, GPIOx_AF_PP_50M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_4, GPIOx_PUSH_UP);
      GPIOx_FastInit(GPIOx_PB, GPIOx_5, GPIOx_AF_PP_50M);
      
      /*��ʼ�����*/
//      SPI3_NSS = 1;
      break; 
    }
    
    default: break;
  }
  
}



/**
  * @brief  SPIxģʽ����
  * @param  None
  * @retval None
  */
static void SPIx_ModeConfig(SPI_TYPE Port)
{
  /* ��ʱ�� */
  if (Port == SPIx_1)
  {
    RCC->APB2ENR |= (0X1<<12);
  }
  else 
  {
    RCC->APB1ENR |= (0X1<<(13+Port));
  }
  
  /* ���ù���ģʽ */
  SPI[Port]->CR1 &= ~(0X1<<15);   //˫��˫��ģʽ(ȫ˫��)
  SPI[Port]->CR1 &= ~(0X1<<13);   //Ӳ��CRCУ��,���ǿ���ֲ��,������
  SPI[Port]->CR1 &= ~(0X1<<11);   //����֡��ʽ:8λ
  SPI[Port]->CR1 &= ~(0X1<<10);   //ȫ˫��ģʽ����
  SPI[Port]->CR1 |=  (0X1<<9);    //�������ģʽ
  SPI[Port]->CR1 |=  (0X1<<8);
  SPI[Port]->CR1 &= ~(0X1<<7);    //����֡:��λ�ȷ�
  SPI[Port]->CR1 &= ~(0X7<<3);
  SPI[Port]->CR1 |=  (0X7<<3);    //SPIx��Ƶ: 256��Ƶ(������Ϊ���,�ٸ���ʵ���޸�)
  SPI[Port]->CR1 |=  (0X1<<2);    //����Ϊ����
  
  /* ����ͨ��ʱ�� */
  SPI[Port]->CR1 |=  (0X0<<0);    //ģʽ0
  
  /* ����SPI */
  SPI[Port]->CR1 |=  (0X1<<6);
  
}



/**
  * @brief  SPIx��ʼ��
  * @param  None
  * @retval None
  */
void SPIx_Init(SPI_TYPE Port)
{
  /* �ж���� */
  if (Port >= s_SpiPortNum)
  {
    return;
  }
  
  /* �������� */
  SPIx_IOConfig(Port);
  
  /* ���ù���ģʽ */
  SPIx_ModeConfig(Port);
  
  /* ���ù����ٶ� */
  SPIx_SetSpeed(Port, SPIx_SPEED_DIV8);
  
}



/**----------------------------------------------/
  * @attention SPI1��ʱ��Ϊ72M,SPI2,3��ʱ��Ϊ36M
  *   SPI1������2��Ƶ(���18M)
  *----------------------------------------------*/

/**
  * @brief  SPIx�ٶ�����
  * @param  cSeep Ҫ���õ��ٶ�,�Ѷ�����صĺ�
  * @retval None
  */
void SPIx_SetSpeed(SPI_TYPE Port, SPIx_SPEED_DVI SpeedDvi)
{
  /* �ر�SPIx */
  SPI[Port]->CR1 &= ~(0X1<<6);
  
  /* ����SPIx�ٶ� */
  SPI[Port]->CR1 &= ~(0X7<<3);
  SPI[Port]->CR1 |=  (SpeedDvi<<3);   //SPIx��Ƶ
  
  /* ����SPIx */
  SPI[Port]->CR1 |=  (0X1<<6);
  
}



/**
  * @brief  SPIx���ݶ�д
  * @param  cWriteData Ҫд�������
  * @retval cReadData  ��ȡ��������
  */
uint8_t SPIx_ReadWriteByte(SPI_TYPE Port, uint8_t cWriteData)
{
  uint8_t cReadData = 0;

  /* �ȴ�������� */
  while (!(SPI[Port]->SR & (0X1<<1)));
  SPI[Port]->DR = cWriteData;
  
  /* �ȴ�������� */
  while (!(SPI[Port]->SR & (0X1<<0)));
  cReadData = SPI[Port]->DR;
  
  return cReadData;
}

