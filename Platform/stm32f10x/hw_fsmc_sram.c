/**
  ******************************************************************************
  * @file    fsmc_sram.c
  * @author  �Ź��Ӻ���
  * @version V2.0 
  * @date    2015.12.30
  * @brief   ����FSMC��չSRAM
  ******************************************************************************
  * @attention
  * 
  * ����,��������,50M
  * 
  * FSMC_A(ADDR BUS):             FSMC_D(DATA BUS):
  * A0-A5:    PF0  --- PF5        D0-D1:    PD14 --- PD15
  * A6-A9:    PF12 --- PF15       D2-D3:    PD0  --- PD1
  * A10-A15:  PG0  --- PG5        D4-D12:   PE7  --- PE15
  * A16-A18:  PD11 --- PD13       D13-D15:  PD8  --- PD10
  * A19-A22:  PE3  --- PE6
  * 
  * ע: A23-A25û����
  * 
  * ��������:
  * PD4 --- FSMC_NOE      PE0 --- FSMC_NBL0
  * PD5 --- FSMC_NWE      PE1 --- FSMC_NBL1
  * PD6 --- FSMC_NWAIT
  * 
  * Ƭѡ��: NE3 (PG10)
  * PD7  --- NE1   PG9  --- NE2    PG10 --- NE3    PG12 --- NE4
  *      
  * ��ַ:
  *   BANK1_1 0X6000 0000 ~ 0X63FF FFFF   NOR/PSRAM1
  *   BANK1_2 0X6400 0000 ~ 0X67FF FFFF   NOR/PSRAM2
  *   BANK1_3 0X6800 0000 ~ 0X6CFF FFFF   NOR/PSRAM3
  *   BANK1_4 0X6C00 0000 ~ 0X6FFF FFFF   NOR/PSRAM4
  *   BANK2   0X7000 0000 ~ 0X7FFF FFFF   NAND1
  *   BANK3   0X8000 0000 ~ 0X8FFF FFFF   NAND2
  *   BANK4   0X9000 0000 ~ 0X9FFF FFFF   PCCARD
  * 
  * 
  * ʹ��: �������ĵ�ַ������SRAM�ĵ�ַ��Χ�ڼ���.�����GUN C��__attribute__ʹ��.                
  * ��: 
  *     uint16_t SRamBuff[1024*10] __attribute__((at(FSMC_SRAM_ADDR)));
  *
  * �����Ľ���ǽ� SRamBuff[] �ľ��Ե�ַ�趨ΪFSMC_SRAM_ADDR,�ռ�Ϊ10K.
  * 
  * ע: ��չ��SRAM�ĵ�ַ��������8λ����(8�ı���).�ڷ����ַʱ����ע�����.����
  *     ������ڴ�ͬΪ8�ı���.�����µı���ʱ,��ַ��:(FSMC_SRAM_ADDR+8*N) ����,
  *     ������������²���Ҫ�Ĵ���.
  *
  * __attribute__: ���ڱ�����GNU C(���Ǳ�׼C)�Ļ���,�������ú�������,����������
  *                ���ṹ������.����ɲ鿴��ص�����.
  * 
  * V2.0------------    
  * �޸�����: 1. �޸��ļ�ͷ����,��H�ļ��ж�����غ�,����Ӧ��ͬ��Ƭѡ���������.
  *           2. �޸�����8λ���鸳ֵ�����BUG
  * ����ԭ��: SRAM��ʱ�򲻶�,�����ݱ���/��ַ����/��ַ����ʱ���Ϊ:3/2/2HCLK����
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.07.13
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_fsmc_sram.h"
#include "hw_platform.h"


/**
  * @brief  FSMC_SRAM ��������
  * @param  None
  * @retval None
  */
static void FSMC_SRAM_IOConfig(void)
{
  /* ��ʱ�� */
  RCC->APB2ENR |= (0X1<<5);   //PD
  RCC->APB2ENR |= (0X1<<6);   //PE
  RCC->APB2ENR |= (0X1<<7);   //PF
  RCC->APB2ENR |= (0X1<<8);   //PG
  
  /* IO���� */
  
  //PD����������� 	
  GPIOD->CRL &= (0XFF00FF00);
  GPIOD->CRL |= (0X00BB00BB);
  GPIOD->CRH &= (0X00000000);
  GPIOD->CRH |= (0XBBBBBBBB);
	 
  //PE����������� 
  GPIOE->CRL &= (0X0FFFFF00);
  GPIOE->CRL |= (0XB00000BB);
  GPIOE->CRH &= (0X00000000);
  GPIOE->CRH |= (0XBBBBBBBB);

  //PF�����������
  GPIOF->CRL &= (0XFF000000);
  GPIOF->CRL |= (0X00BBBBBB);
  GPIOF->CRH &= (0X0000FFFF);
  GPIOF->CRH |= (0XBBBB0000);

  //PG�����������
  GPIOG->CRL &= (0XFF000000);
  GPIOG->CRL |= (0X00BBBBBB);
  
  //����Ƭѡ��
  #if(FSMC_SRAM_NES==FSMC_SRAM_NE1)
  GPIOD->CRL &= (0X0FFFFFFF);
  GPIOD->CRL |= (0XB0000000);

  #elif(FSMC_SRAM_NES==FSMC_SRAM_NE2)
  GPIOG->CRH &= (0XFFFFFF0F);
  GPIOG->CRH |= (0X000000B0);

  #elif(FSMC_SRAM_NES==FSMC_SRAM_NE3)
  GPIOG->CRH &= (0XFFFFF0FF);
  GPIOG->CRH |= (0X00000B00);

  #elif(FSMC_SRAM_NES==FSMC_SRAM_NE4)
  GPIOG->CRH &= (0XFFF0FFFF);
  GPIOG->CRH |= (0X000B0000);

  #endif

}


/**
  * @brief  FSMC_SRAM ģʽ
  * @param  None
  * @retval None
  * @note   FSMC_SRAM_NE1 -- BTCR[0] BTCR[1]
  *         FSMC_SRAM_NE2 -- BTCR[2] BTCR[3]
  *         FSMC_SRAM_NE3 -- BTCR[4] BTCR[5]
  *         FSMC_SRAM_NE4 -- BTCR[5] BTCR[7]
  */
static void FSMC_SRAM_ModeConfig(void)
{
  /* ��ʱ�� */
  RCC->AHBENR |= (0X1<<8);  //FSMC
  
  /* ���ù���ģʽ */
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] = 0; 
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X1<<19);  //д���������첽ģʽ
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X1<<14);  //��дͬʱ��
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X1<<12);  //дʹ��
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X3<<4);
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X1<<4);   //16λ���ݿ��(���������SRAM����)
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X3<<2);
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X0<<2);   //�洢������:SRAM

  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] = 0;
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] &= ~(0X3<<28);  //����ģʽA(��������չģʽ����Ч)
  
  /* ����ʱ����� */
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X1<<20);  //CLK����=2��HCLK����
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X3<<8);   //���ݱ���ʱ�䣨DATAST��-- 3��HCLKʱ������
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X3<<4);   //��ַ����ʱ�䣨ADDHLD��-- 2��HCLKʱ������
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X3<<0);   //��ַ����ʱ�䣨ADDSET��-- 2��HCLKʱ������
  
  /* ����FSMC */
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X1<<0);   //ʹ�ܴ洢����
  
}


void sram_init(void)
{
	RCC->APB2ENR |= 1<<5;//ʹ��PDʱ��
	RCC->APB2ENR |= 1<<6;//ʹ��PEʱ��
 	RCC->APB2ENR |= 1<<7;//ʹ��PFʱ��
	RCC->APB2ENR |= 1<<8;//ʹ��PGʱ��	   

	//PD����������� 	
	GPIOD->CRH &= 0X00000000;
	GPIOD->CRH |= 0XBBBBBBBB; 
	GPIOD->CRL &= 0XFF00FF00;
	GPIOD->CRL |= 0X00BB00BB;   	 
	//PE������ ����� 	
	GPIOE->CRH &= 0X00000000;
	GPIOE->CRH |= 0XBBBBBBBB; 
	GPIOE->CRL &= 0X0FFFFF00;
	GPIOE->CRL |= 0XB00000BB; 
	//PF�����������
	GPIOF->CRH &= 0X0000FFFF;
	GPIOF->CRH |= 0XBBBB0000;  	    	 											 
	GPIOF->CRL &= 0XFF000000;
	GPIOF->CRL |= 0X00BBBBBB; 
	//PG����������� PG10->NE3 -- BANK1����3   	 											 
	GPIOG->CRH &= 0XFFFFF0FF;
	GPIOG->CRH |= 0X00000B00;  
 	GPIOG->CRL &= 0XFF000000;
	GPIOG->CRL |= 0X00BBBBBB;
  
  RCC->AHBENR |= (1<<8);//����FSMC��ʱ��
  
  FSMC_Bank1->BTCR[4] = 0;
  FSMC_Bank1->BTCR[4] |= (1<<12) | (1<<4);
  FSMC_Bank1->BTCR[5] = 0;
  /* 
     ���ݱ���ʱ�䣺2��HCLK
     ��ַ����ʱ�䣺1��HCLK
     ��ַ�ı���ʱ�䣺1��HCLK 
  */
  FSMC_Bank1->BTCR[5] |= (0<<28) | (1<<20) | (2<<8) | (0<<4) | (0<<0);
  
  FSMC_Bank1->BTCR[4] |= (1<<0);//������Ӧ����
}



/**
  * @brief  ͨ��FSMC��չSRAM
  * @param  None
  * @retval None
  */
void FSMC_SRAM_Init(void)
{

  /* �������� */
  FSMC_SRAM_IOConfig();
  
  /* ���ù���ģʽ */
  FSMC_SRAM_ModeConfig();
  
}


