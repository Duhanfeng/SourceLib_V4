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
  * ���濨��: �ڷ���ʱ,�����SPI��д�����е� while (!(SPIx->SR & (0X1<<0)));
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
  * V4.1------------
  * �޸�����: 1.�޸���SPI���û���,�����ָ������������ĳ�ֱ�ӵ�����ָ�����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V4.1
  * �޸�����: 2017.05.23
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "hw_spi.h"

/* ------SPIͨ������------ */
#define SPI1_NSS_PORT     GPIOx_PA
#define SPI1_NSS_PIN      GPIOx_4
#define SPI1_CLK_PORT     GPIOx_PA
#define SPI1_CLK_PIN      GPIOx_5
#define SPI1_MISO_PORT    GPIOx_PA
#define SPI1_MISO_PIN     GPIOx_6
#define SPI1_MOSI_PORT    GPIOx_PA
#define SPI1_MOSI_PIN     GPIOx_7

#define SPI2_NSS_PORT     GPIOx_PB
#define SPI2_NSS_PIN      GPIOx_12
#define SPI2_CLK_PORT     GPIOx_PB
#define SPI2_CLK_PIN      GPIOx_13
#define SPI2_MISO_PORT    GPIOx_PB
#define SPI2_MISO_PIN     GPIOx_14
#define SPI2_MOSI_PORT    GPIOx_PB
#define SPI2_MOSI_PIN     GPIOx_15

#define SPI3_NSS_PORT     GPIOx_PA
#define SPI3_NSS_PIN      GPIOx_15
#define SPI3_CLK_PORT     GPIOx_PB
#define SPI3_CLK_PIN      GPIOx_3
#define SPI3_MISO_PORT    GPIOx_PB
#define SPI3_MISO_PIN     GPIOx_4
#define SPI3_MOSI_PORT    GPIOx_PB
#define SPI3_MOSI_PIN     GPIOx_5



/**
  * @brief  SPIx��������
  * @param  Port SPI�˿�
  * @param  Mode SPI�˿�
  * @retval None
  */
void SPIx_IOConfig(SPI_TypeDef *SPIx, SPIx_WORK_MODE Mode)
{
  switch ((uint32_t)SPIx)
  {
    case SPI1_BASE: 
    {
//    GPIOx_FastInit(SPI1_NSS_PORT,  SPI1_NSS_PIN,  (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      GPIOx_FastInit(SPI1_CLK_PORT,  SPI1_CLK_PIN,  (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      GPIOx_FastInit(SPI1_MISO_PORT, SPI1_MISO_PIN, (Mode == SPIx_MASTER_MODE) ? GPIOx_PUSH_UP   : GPIOx_AF_PP_50M);
      GPIOx_FastInit(SPI1_MOSI_PORT, SPI1_MOSI_PIN, (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      break;
    }
    
    case SPI2_BASE: 
    {
//    GPIOx_FastInit(SPI2_NSS_PORT,  SPI2_NSS_PIN,  (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      GPIOx_FastInit(SPI2_CLK_PORT,  SPI2_CLK_PIN,  (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      GPIOx_FastInit(SPI2_MISO_PORT, SPI2_MISO_PIN, (Mode == SPIx_MASTER_MODE) ? GPIOx_PUSH_UP   : GPIOx_AF_PP_50M);
      GPIOx_FastInit(SPI2_MOSI_PORT, SPI2_MOSI_PIN, (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      break;
    }
    
    case SPI3_BASE: 
    {
//    GPIOx_FastInit(SPI3_NSS_PORT,  SPI3_NSS_PIN,  (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      GPIOx_FastInit(SPI3_CLK_PORT,  SPI3_CLK_PIN,  (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
      GPIOx_FastInit(SPI3_MISO_PORT, SPI3_MISO_PIN, (Mode == SPIx_MASTER_MODE) ? GPIOx_PUSH_UP   : GPIOx_AF_PP_50M);
      GPIOx_FastInit(SPI3_MOSI_PORT, SPI3_MOSI_PIN, (Mode == SPIx_MASTER_MODE) ? GPIOx_AF_PP_50M : GPIOx_FLOAT);
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
static void SPIx_ModeConfig(SPI_TypeDef *SPIx, SPIx_WORK_MODE Mode)
{
  //��ʱ��
  RCC_EnableClock(SPIx, 1);
  
  //ģʽ����
  SPIx->CR1 &= ~SPI_CR1_BIDIMODE; //˫��˫��ģʽ(ȫ˫��)
  SPIx->CR1 &= ~SPI_CR1_CRCEN;    //Ӳ��CRCУ��,���ǿ���ֲ��,������
  SPIx->CR1 &= ~SPI_CR1_DFF;      //����֡��ʽ:8λ
  SPIx->CR1 &= ~SPI_CR1_RXONLY;   //ȫ˫��ģʽ����
  SPIx->CR1 |=  SPI_CR1_SSM;      //�������ģʽ
  SPIx->CR1 |=  SPI_CR1_SSI;
  SPIx->CR1 &= ~SPI_CR1_LSBFIRST; //����֡:��λ�ȷ�
  SPIx->CR1 &= ~SPI_CR1_BR;
  SPIx->CR1 |=  SPI_CR1_BR;       //SPIx��Ƶ: 256��Ƶ(������Ϊ���,�ٸ���ʵ���޸�)
  SPIx->CR1 |=  SPI_CR1_MSTR;     //����Ϊ����

  //��ģʽ����
  if (Mode == SPIx_SLAVE_MODE)
  {
    SPIx->CR1 &= ~SPI_CR1_SSI;    //�ڲ�NSS����
    SPIx->CR1 &= ~SPI_CR1_MSTR;   //����Ϊ�ӻ�
    
    SPIx->CR2 |=  SPI_CR2_RXNEIE; //���������ж�
    NVIC_Config(SPIx, 2, 2);      //���ں��ж�
    
    SPIx->DR = SPI_DR_DR;
  }
  
  //����ͨ��ʱ��
  SPIx->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
  
  //����SPI
  SPIx->CR1 |=  SPI_CR1_SPE;
  
}



/**
  * @brief  SPIx��ʼ��
  * @param  Port SPI�˿�
  * @retval None
  */
void SPIx_Init(SPI_TypeDef *SPIx, SPIx_WORK_MODE Mode)
{
  //���ù���ģʽ
  SPIx_ModeConfig(SPIx, Mode);
  
  //��������
  SPIx_IOConfig(SPIx, Mode);
  
  //���ù����ٶ�
  SPIx_SetSpeed(SPIx, SPIx_SPEED_DIV16);
  
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
void SPIx_SetSpeed(SPI_TypeDef *SPIx, SPIx_SPEED_DVI SpeedDvi)
{
  //�ر�SPIx
  SPIx->CR1 &= ~SPI_CR1_SPE;
  
  //����SPIx�ٶ�
  SPIx->CR1 &= ~SPI_CR1_BR;
  SPIx->CR1 |=  SpeedDvi;   //SPIx��Ƶ
  
  //����SPIx
  SPIx->CR1 |=  SPI_CR1_SPE;
  
}



/**
  * @brief  SPIx״̬�Ĵ���д��
  * @param  Port SPI�˿�
  * @retval SPIx->SR SPI״̬�Ĵ�����ȡ
  */
uint16_t SPIx_GetStatus(SPI_TypeDef *SPIx)
{
  
  return SPIx->SR;
}



/**
  * @brief  SPIx���ݼĴ���д��
  * @param  Port SPI�˿�
  * @param  cWriteData Ҫд�������
  * @retval Note
  */
void SPIx_WriteDataReg(SPI_TypeDef *SPIx, uint16_t cWriteData)
{
  SPIx->DR = cWriteData;
  
}



/**
  * @brief  SPIx���ݼĴ�����ȡ
  * @param  Port SPI�˿�
  * @retval cReadData  ��ȡ��������
  */
uint16_t SPIx_ReadDataReg(SPI_TypeDef *SPIx)
{
  
  return SPIx->DR;
}



/**
  * @brief  SPIx���ݶ�д
  * @param  cWriteData Ҫд�������
  * @retval Note
  */
uint16_t SPIx_ReadWriteByte(SPI_TypeDef *SPIx, uint16_t cWriteData)
{
  uint8_t cReadData = 0;

  //�ȴ��������
  while (!(SPIx->SR & SPI_SR_TXE));
  SPIx->DR = cWriteData;
  
  //�ȴ��������
  //ֵ��ע�����,�ڷ����ʱ������������ϵ�(��RXNE״̬λ֮ǰ),��ϵͳ�Ὣ
  //����,��Ϊ���������ȡDR�Ĵ�����ֵ,�Ӷ��RXNE״̬λ����
  while (!(SPIx->SR & SPI_SR_RXNE));
  cReadData = SPIx->DR;
  
  return cReadData;
}



