/**
  ******************************************************************************
  * @file    fsmc_sram.c
  * @author  杜公子寒枫
  * @version V2.0 
  * @date    2015.12.30
  * @brief   利用FSMC扩展SRAM
  ******************************************************************************
  * @attention
  * 
  * 引脚,复用推挽,50M
  * 
  * FSMC_A(ADDR BUS):             FSMC_D(DATA BUS):
  * A0-A5:    PF0  --- PF5        D0-D1:    PD14 --- PD15
  * A6-A9:    PF12 --- PF15       D2-D3:    PD0  --- PD1
  * A10-A15:  PG0  --- PG5        D4-D12:   PE7  --- PE15
  * A16-A18:  PD11 --- PD13       D13-D15:  PD8  --- PD10
  * A19-A22:  PE3  --- PE6
  * 
  * 注: A23-A25没用上
  * 
  * 功能引脚:
  * PD4 --- FSMC_NOE      PE0 --- FSMC_NBL0
  * PD5 --- FSMC_NWE      PE1 --- FSMC_NBL1
  * PD6 --- FSMC_NWAIT
  * 
  * 片选脚: NE3 (PG10)
  * PD7  --- NE1   PG9  --- NE2    PG10 --- NE3    PG12 --- NE4
  *      
  * 地址:
  *   BANK1_1 0X6000 0000 ~ 0X63FF FFFF   NOR/PSRAM1
  *   BANK1_2 0X6400 0000 ~ 0X67FF FFFF   NOR/PSRAM2
  *   BANK1_3 0X6800 0000 ~ 0X6CFF FFFF   NOR/PSRAM3
  *   BANK1_4 0X6C00 0000 ~ 0X6FFF FFFF   NOR/PSRAM4
  *   BANK2   0X7000 0000 ~ 0X7FFF FFFF   NAND1
  *   BANK3   0X8000 0000 ~ 0X8FFF FFFF   NAND2
  *   BANK4   0X9000 0000 ~ 0X9FFF FFFF   PCCARD
  * 
  * 
  * 使用: 将变量的地址设置在SRAM的地址范围内即可.需配合GUN C的__attribute__使用.                
  * 如: 
  *     uint16_t SRamBuff[1024*10] __attribute__((at(FSMC_SRAM_ADDR)));
  *
  * 上例的结果是将 SRamBuff[] 的绝对地址设定为FSMC_SRAM_ADDR,空间为10K.
  * 
  * 注: 扩展的SRAM的地址必须是以8位对齐(8的倍数).在分配地址时尤须注意这点.建议
  *     分配的内存同为8的倍数.定义新的变量时,地址用:(FSMC_SRAM_ADDR+8*N) 表述,
  *     避免因疏忽导致不必要的错误.
  *
  * __attribute__: 属于编译器GNU C(而非标准C)的机制,可以设置函数属性,变量属性以
  *                及结构的属性.具体可查看相关的资料.
  * 
  * V2.0------------    
  * 修改描述: 1. 修改文件头描述,在H文件中定义相关宏,以适应不同的片选脚连接情况.
  *           2. 修复开辟8位数组赋值错误的BUG
  * 错误原因: SRAM的时序不对,把数据保持/地址保持/地址建立时间改为:3/2/2HCLK即可
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.13
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_fsmc_sram.h"
#include "hw_platform.h"


/**
  * @brief  FSMC_SRAM 引脚配置
  * @param  None
  * @retval None
  */
static void FSMC_SRAM_IOConfig(void)
{
  /* 开时钟 */
  RCC->APB2ENR |= (0X1<<5);   //PD
  RCC->APB2ENR |= (0X1<<6);   //PE
  RCC->APB2ENR |= (0X1<<7);   //PF
  RCC->APB2ENR |= (0X1<<8);   //PG
  
  /* IO配置 */
  
  //PD复用推挽输出 	
  GPIOD->CRL &= (0XFF00FF00);
  GPIOD->CRL |= (0X00BB00BB);
  GPIOD->CRH &= (0X00000000);
  GPIOD->CRH |= (0XBBBBBBBB);
	 
  //PE复用推挽输出 
  GPIOE->CRL &= (0X0FFFFF00);
  GPIOE->CRL |= (0XB00000BB);
  GPIOE->CRH &= (0X00000000);
  GPIOE->CRH |= (0XBBBBBBBB);

  //PF复用推挽输出
  GPIOF->CRL &= (0XFF000000);
  GPIOF->CRL |= (0X00BBBBBB);
  GPIOF->CRH &= (0X0000FFFF);
  GPIOF->CRH |= (0XBBBB0000);

  //PG复用推挽输出
  GPIOG->CRL &= (0XFF000000);
  GPIOG->CRL |= (0X00BBBBBB);
  
  //配置片选脚
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
  * @brief  FSMC_SRAM 模式
  * @param  None
  * @retval None
  * @note   FSMC_SRAM_NE1 -- BTCR[0] BTCR[1]
  *         FSMC_SRAM_NE2 -- BTCR[2] BTCR[3]
  *         FSMC_SRAM_NE3 -- BTCR[4] BTCR[5]
  *         FSMC_SRAM_NE4 -- BTCR[5] BTCR[7]
  */
static void FSMC_SRAM_ModeConfig(void)
{
  /* 开时钟 */
  RCC->AHBENR |= (0X1<<8);  //FSMC
  
  /* 配置工作模式 */
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] = 0; 
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X1<<19);  //写操作出于异步模式
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X1<<14);  //读写同时序
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X1<<12);  //写使能
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X3<<4);
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X1<<4);   //16位数据宽度(针对于外扩SRAM而言)
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] &= ~(0X3<<2);
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X0<<2);   //存储器类型:SRAM

  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] = 0;
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] &= ~(0X3<<28);  //访问模式A(仅开启扩展模式下有效)
  
  /* 配置时序参数 */
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X1<<20);  //CLK周期=2个HCLK周期
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X3<<8);   //数据保持时间（DATAST）-- 3个HCLK时钟周期
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X3<<4);   //地址保持时间（ADDHLD）-- 2个HCLK时钟周期
  FSMC_Bank1->BTCR[FSMC_SRAM_NES+1] |=  (0X3<<0);   //地址建立时间（ADDSET）-- 2个HCLK时钟周期
  
  /* 开启FSMC */
  FSMC_Bank1->BTCR[FSMC_SRAM_NES] |=  (0X1<<0);   //使能存储器块
  
}


void sram_init(void)
{
	RCC->APB2ENR |= 1<<5;//使能PD时钟
	RCC->APB2ENR |= 1<<6;//使能PE时钟
 	RCC->APB2ENR |= 1<<7;//使能PF时钟
	RCC->APB2ENR |= 1<<8;//使能PG时钟	   

	//PD复用推挽输出 	
	GPIOD->CRH &= 0X00000000;
	GPIOD->CRH |= 0XBBBBBBBB; 
	GPIOD->CRL &= 0XFF00FF00;
	GPIOD->CRL |= 0X00BB00BB;   	 
	//PE复用推 挽输出 	
	GPIOE->CRH &= 0X00000000;
	GPIOE->CRH |= 0XBBBBBBBB; 
	GPIOE->CRL &= 0X0FFFFF00;
	GPIOE->CRL |= 0XB00000BB; 
	//PF复用推挽输出
	GPIOF->CRH &= 0X0000FFFF;
	GPIOF->CRH |= 0XBBBB0000;  	    	 											 
	GPIOF->CRL &= 0XFF000000;
	GPIOF->CRL |= 0X00BBBBBB; 
	//PG复用推挽输出 PG10->NE3 -- BANK1区域3   	 											 
	GPIOG->CRH &= 0XFFFFF0FF;
	GPIOG->CRH |= 0X00000B00;  
 	GPIOG->CRL &= 0XFF000000;
	GPIOG->CRL |= 0X00BBBBBB;
  
  RCC->AHBENR |= (1<<8);//开启FSMC的时钟
  
  FSMC_Bank1->BTCR[4] = 0;
  FSMC_Bank1->BTCR[4] |= (1<<12) | (1<<4);
  FSMC_Bank1->BTCR[5] = 0;
  /* 
     数据保持时间：2个HCLK
     地址建立时间：1个HCLK
     地址的保持时间：1个HCLK 
  */
  FSMC_Bank1->BTCR[5] |= (0<<28) | (1<<20) | (2<<8) | (0<<4) | (0<<0);
  
  FSMC_Bank1->BTCR[4] |= (1<<0);//开启相应的区
}



/**
  * @brief  通过FSMC扩展SRAM
  * @param  None
  * @retval None
  */
void FSMC_SRAM_Init(void)
{

  /* 配置引脚 */
  FSMC_SRAM_IOConfig();
  
  /* 配置工作模式 */
  FSMC_SRAM_ModeConfig();
  
}


