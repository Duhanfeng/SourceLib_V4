/**
  ******************************************************************************
  * @file    SPI.c
  * @author  �Ź��Ӻ���
  * @version V2.0 �Ĵ����汾
  * @date    2017.04.18
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
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V3.0
  * �޸�����: 2017.03.10
  *
  * V4.0------------
  * �޸�����: 1.����SPI�����ӻ�ģʽ����
  *           2.����������ģʽ�´ӻ�����������λ��ԭ�򼰽������
  *           3.���ַ��������SPI������ԭ���Լ��������(�����������������Ƶ����)
  *           4.���Ų��ֽӿ�
  *           5.��SPI��д��������ε��������ʹ�8λ�ĳ�16λ(����16λ����)
  * ����ģʽ: ��SPI��ģʽ��,SLK,MISO,MOSI��������IOһһ��Ӧ����,������Ҫ����ʵ�ʵ�
  *           ������������������ģʽ.��ģʽ��,�ӻ����͵�����ֻ�Ǵ����ڷ��ͻ����
  *           ������,�ȴ���������һ������ʱ�ӵĵ�������λ��λ.
  * ������λ: ��SPI��ģʽ������,���յ��������п��ܺ�ʵ���������͵����ݲ�һ��,������
  *           ����λ(��һ��)������,��ʱ����ģʽ��ʱ�Ӳ�ͬ�����µ�.���������������
  *           �ӻ��ڳ�ʼ��������,IO���ȶ���״̬������ӻ���ʶ��Ϊ����յ�������,��
  *           �������ߵ����ݲ�ͬ��.�����ӻ�����ͨ�Ų���Ϊ��,�������Ľ����������
  *           �����ӻ�IO����ģʽ����,�ٶ�SPI���г�ʼ��(���������������ôӻ�),�ô�
  *           ����������ʱ���Ѿ�������������µ�IO���������.
  * ���濨��: �ڷ���ʱ,�����SPI��д�����е� while (!(SPI[Port]->SR & (0X1<<0)));
  *           ��䴦��ϵ�(��δִ��),��ִ�д������ж����,�������޷�ͨ����������
  *           ��,�Ӷ���ϵͳ�����ڴ˴�,����ֱ�ӵ�ִ�оͲ�������������.�����Է���,
  *           ֻҪ�����ж�SPI_RXNE֮ǰ�����˶ϵ������ִֹͣ��,���ᵼ����RXNE��־
  *           ����,ԭ�����ڵ���ģʽ��(������ִ��),���SPI_DR�Ĵ������ж�ȡ,����ȡ
  *           DR�Ĵ����ᵼ����RXNE��־λ����,���յ������жϴ���Ӷ�����.�������
  *           �ڶ��Ĵ������������¶����п��ܳ���,��һ��ֵ��ע��.
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.0
  * �޸�����: 2017.04.18
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
  * @param  Port SPI�˿�
  * @param  Mode SPI�˿�
  * @retval None
  */
void SPIx_IOConfig(SPI_TYPE Port, SPIx_WORK_MODE Mode)
{
  switch (Port)
  {
    case SPIx_1: 
    {
      if (Mode == SPIx_MASTER_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_4, GPIOx_AF_PP_50M);
        GPIOx_FastInit(GPIOx_PA, GPIOx_5, GPIOx_AF_PP_50M);   //CLK
        GPIOx_FastInit(GPIOx_PA, GPIOx_6, GPIOx_PUSH_UP);     //MISO
        GPIOx_FastInit(GPIOx_PA, GPIOx_7, GPIOx_AF_PP_50M);   //MOSI
        
        /*��ʼ�����*/
//        SPI1_NSS = 1;
      }
      else if (Mode == SPIx_SLAVE_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_4, GPIOx_PUSH_UP);
        GPIOx_FastInit(GPIOx_PA, GPIOx_5, GPIOx_FLOAT);     //CLK
        GPIOx_FastInit(GPIOx_PA, GPIOx_6, GPIOx_AF_PP_50M); //MISO
        GPIOx_FastInit(GPIOx_PA, GPIOx_7, GPIOx_FLOAT);     //MOSI
      }

      break; 
    }
    
    case SPIx_2: 
    {
      if (Mode == SPIx_MASTER_MODE)
      {
//        GPIOx_FastInit(GPIOx_PB, GPIOx_12, GPIOx_GP_PP_10M);
        GPIOx_FastInit(GPIOx_PB, GPIOx_13, GPIOx_AF_PP_50M);  //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_14, GPIOx_FLOAT);      //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_15, GPIOx_AF_PP_50M);  //MOSI
        
        /*��ʼ�����*/
//        SPI2_NSS = 1;
      }
      
      else if (Mode == SPIx_SLAVE_MODE)
      {
//        GPIOx_FastInit(GPIOx_PB, GPIOx_12, GPIOx_FLOAT);
        GPIOx_FastInit(GPIOx_PB, GPIOx_13, GPIOx_FLOAT);      //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_14, GPIOx_AF_PP_50M);  //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_15, GPIOx_FLOAT);      //MOSI
      }
      
      break; 
    }
    
    case SPIx_3: 
    {
      if (Mode == SPIx_MASTER_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_15, GPIOx_GP_PP_10M);
        GPIOx_FastInit(GPIOx_PB, GPIOx_3,  GPIOx_AF_PP_50M);  //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_4,  GPIOx_PUSH_UP);    //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_5,  GPIOx_AF_PP_50M);  //MOSI
        
        /*��ʼ�����*/
//        SPI3_NSS = 1;
      }
      else if (Mode == SPIx_SLAVE_MODE)
      {
//        GPIOx_FastInit(GPIOx_PA, GPIOx_15, GPIOx_PUSH_UP);
        GPIOx_FastInit(GPIOx_PB, GPIOx_3,  GPIOx_PUSH_UP);    //CLK
        GPIOx_FastInit(GPIOx_PB, GPIOx_4,  GPIOx_AF_PP_50M);  //MISO
        GPIOx_FastInit(GPIOx_PB, GPIOx_5,  GPIOx_PUSH_UP);    //MOSI
      }
      
      break; 
    }
    
    default: break;
  }
  
}



/**
  * @brief  SPIxģʽ����
  * @param  Port SPI�˿�
  * @retval None
  */
static void SPIx_ModeConfig(SPI_TYPE Port, SPIx_WORK_MODE Mode)
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
  
  //ģʽ����
  SPI[Port]->CR1 &= ~(0x1<<15);   //˫��˫��ģʽ(ȫ˫��)
  SPI[Port]->CR1 &= ~(0x1<<13);   //Ӳ��CRCУ��,���ǿ���ֲ��,������
  SPI[Port]->CR1 &= ~(0x1<<11);   //����֡��ʽ:8λ
  SPI[Port]->CR1 &= ~(0x1<<10);   //ȫ˫��ģʽ����
  SPI[Port]->CR1 |=  (0x1<<9);    //�������ģʽ
  SPI[Port]->CR1 |=  (0x1<<8);
  SPI[Port]->CR1 &= ~(0x1<<7);    //����֡:��λ�ȷ�
  SPI[Port]->CR1 &= ~(0x7<<3);
  SPI[Port]->CR1 |=  (0x7<<3);    //SPIx��Ƶ: 256��Ƶ(������Ϊ���,�ٸ���ʵ���޸�)
  SPI[Port]->CR1 |=  (0x1<<2);    //����Ϊ����

  if (Mode == SPIx_SLAVE_MODE)
  {
    IRQn_Type SPIx_IRQ[3] = {SPI1_IRQn, SPI2_IRQn, SPI3_IRQn};
    
    SPI[Port]->CR1 &= ~(0x1<<8);  //�ڲ�NSS����
    SPI[Port]->CR1 &= ~(0x1<<2);  //����Ϊ�ӻ�
    
    SPI[Port]->CR2 |=  (0x1<<6);  //���������ж�
    NVIC_Enable(SPIx_IRQ[Port], 1, 1); //���ں��ж�
    
    SPI[Port]->DR = 0xFF;
  }
  
  /* ����ͨ��ʱ�� */
  SPI[Port]->CR1 |=  (0x0<<0);    //ģʽx
  
  /* ����SPI */
  SPI[Port]->CR1 |=  (0x1<<6);
  
}



/**
  * @brief  SPIx��ʼ��
  * @param  Port SPI�˿�
  * @retval None
  */
void SPIx_Init(SPI_TYPE Port, SPIx_WORK_MODE Mode)
{
  /* �ж���� */
  if (Port >= s_SpiPortNum)
  {
    return;
  }
  
  /* ���ù���ģʽ */
  SPIx_ModeConfig(Port, Mode);
  
  /* �������� */
//  SPIx_IOConfig(Port, Mode);
  
  /* ���ù����ٶ� */
  SPIx_SetSpeed(Port, SPIx_SPEED_DIV16);
  
}



/**----------------------------------------------/
  * @attention SPI1��ʱ��Ϊ72M,SPI2,3��ʱ��Ϊ36M
  *   SPI1������2��Ƶ(���18M)
  *----------------------------------------------*/

/**
  * @brief  SPIx�ٶ�����
  * @param  Port SPI�˿�
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
  * @brief  SPIx״̬�Ĵ���д��
  * @param  Port SPI�˿�
  * @retval SPI[Port]->SR SPI״̬�Ĵ�����ȡ
  */
uint16_t SPIx_GetStatus(SPI_TYPE Port)
{
  
  return SPI[Port]->SR;
}


/**
  * @brief  SPIx���ݼĴ���д��
  * @param  Port SPI�˿�
  * @param  cWriteData Ҫд�������
  * @retval Note
  */
void SPIx_WriteDataReg(SPI_TYPE Port, uint16_t cWriteData)
{
  SPI[Port]->DR = cWriteData;
  
}



/**
  * @brief  SPIx���ݼĴ�����ȡ
  * @param  Port SPI�˿�
  * @retval cReadData  ��ȡ��������
  */
uint16_t SPIx_ReadDataReg(SPI_TYPE Port)
{
  
  return SPI[Port]->DR;
}



/**
  * @brief  SPIx���ݶ�д
  * @param  cWriteData Ҫд�������
  * @retval Note
  */
uint16_t SPIx_ReadWriteByte(SPI_TYPE Port, uint16_t cWriteData)
{
  uint8_t cReadData = 0;

  /* �ȴ�������� */
  while (!(SPI[Port]->SR & (0X1<<1)));
  SPI[Port]->DR = cWriteData;
  
  /* �ȴ�������� */
  //ֵ��ע�����,�ڷ����ʱ������������ϵ�(��RXNE״̬λ֮ǰ),��ϵͳ�Ὣ
  //����,��Ϊ���������ȡDR�Ĵ�����ֵ,�Ӷ�RXNE״̬λ����
  while (!(SPI[Port]->SR & (0X1<<0)));
  cReadData = SPI[Port]->DR;
  
  return cReadData;
}



