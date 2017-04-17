/** 
  ******************************************************************************
  * @file    adc1.c
  * @author  杜公子寒枫
  * @version V1.0 寄存器版本
  * @date    2016.02.25
  * @brief   ADC1采样转换
  ******************************************************************************
  * @attention
  *
  * 引脚,模拟输入 
  *      PA0 --- ADC12_CH0 
  *      PA1 --- ADC12_CH1 
  *      PA2 --- ADC12_CH2 
  *      PA3 --- ADC12_CH3 
  * 
  * 采样周期: 采样周期由 ADC_SMPR 进行配置
  * 通道配置: 规则组的通道在ADC_SQRx中配置,这决定了转换的通道数和顺序.
  * 
  * TCONV = 采样时间+ 12.5个周期
  * 模拟量 = 数字量*(3.3V/4096)
  * 
  * 修改,
  *      1. 修改通道数量的宏
  *      2. 根据通道配置引脚
  *      3. 配置AD采集周期(ADC_SMPRx)
  *      4. 配置ADC通道转换的顺序(ADC_SQRx)
  *      5. 根据实际情况修改 ADC_GetDigVal() 函数
  * 
  * 
  * 注2: ADC的输入时钟不得超过14MHz(实际的时钟为12M)
  * 注3: 规则组只有一个转换数据寄存器，必须要及时读取转换数据(使用DMA可以避免此问题）
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


//配置IO
static void ADC_IOConfig(void)
{
  #if 0
  /* 开时钟 */
  RCC->AHBENR |=  (0X1<<17);  //PA
  
  /* 配置引脚 */
  GPIOA->MODER &= ~(0XF<<(0*2));
  GPIOA->MODER |=  (0XF<<(0*2));   //PA0-PA3, 模拟模式
  #endif
  
  GPIOx_FastInit(GPIOx_PA, GPIOx_1, GPIOx_ANALOG);
  
}



//配置模式
static void ADC_ModeConfig(void)
{
  /* 开时钟 */
  RCC->APB2ENR |= (0X1<<9);  //ADC1
  
  /* 时钟分频 */
  ADC1->CFGR2 &= ~(0X3U<<30);
  ADC1->CFGR2 |=  (0X2U<<30);   //4分频(ADC的时钟频率不得超过14M,此分频后为12M)
    
  /* 模式配置 */
  ADC1->CFGR1 &= ~(0X1<<16);  //非间断模式
  ADC1->CFGR1 |=  (0X1<<14);  //等待模式
  ADC1->CFGR1 |=  (0X1<<13);  //连续转换模式  ADC_CFGR1_CONT
  ADC1->CFGR1 &= ~(0X3<<10);  //软件触发
  ADC1->CFGR1 &= ~(0X1<<5);   //右对齐
  ADC1->CFGR1 &= ~(0X3<<3); 
  ADC1->CFGR1 |=  (0X0<<3);   //12位分辨率
  ADC1->CFGR1 &= ~(0X1<<2);   //序列正扫描方向
//  ADC1->CFGR1 |=  (0X1<<0);   //DMA循环模式
//  ADC1->CFGR1 |=  (0X1<<0);   //使能DMA
  
  /* 触发配置 */
  ADC1->CFGR1 &= ~(0X3<<10);
//  ADC1->CFGR1 |=  (0X3<<10);  //外部触发模式：上升沿及下降沿
  ADC1->CFGR1 &= ~(0X7<<6);   //关闭外部触发模式
//  ADC1->CFGR1 |=  (0X3<<6);   //定时器3_TRGO信号触发
  
  /* 时序配置 */
  ADC1->SMPR &= ~(0X7<<1);
  ADC1->SMPR |=  (0X7<<1);    //239.5+12.5个周期,即21us可完成一次采样
  
  /* 通道选择 */
  ADC1->CHSELR |= (0X1<<1);   //通道1
  
  /* 配置中断 */
  ADC1->IER |= (0X1<<2);  //结束转换中断
  ADC1->ISR &= ~(0x1<<2); //清标志位
  
  /* 配置核级中断 */
  NVIC_SetPriority(ADC1_IRQn, 2);
  NVIC_EnableIRQ(ADC1_IRQn);
  
  /* 校准ADC */
  ADC1->CR |=  (0X1U<<31);       //开始校准
  while (ADC1->CR & (0X1U<<31));  //等待校准完成
  
  /*启动ADC1*/
  ADC1->CR |=  (0X1<<0);
  
}



/**
  * @brief  主函数
  * @param  None
  * @retval None
  */
void ADC_Init(void)
{
  /* 配置IO模式 */
  ADC_IOConfig();
  
  /* 配置工作模式 */
  ADC_ModeConfig();
  
}
  


/**
  * @brief  启动AD转换
  * @param  None
  * @retval None
  * @note   在配置TROG触发后,启动ADC转换并不会立即触发,只有接收到TRGO信号
  *         后才会启动转换
  */
void ADC_StartConversion(void)
{
  while (ADC1->CR & (0X1<<2));
  
  /* 启动ADC转换 */
  ADC1->CR |=  (0X1<<2);
  
}



/**
  * @brief  一阶递归滤波算法
  * @param  Vib_Value: 传入值
  * @retval Vib_Val: 滤波结果
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


//中断时间: 2us/3us
void ADC1_IRQHandler(void)
{
  static uint32_t s_Sum = 0;
  static uint8_t s_Number = 0;
  
  /* 中断入口判断 */
  if (ADC1->ISR & (0x1<<2)) //结束转换中断
  {
    ADC1->ISR &= ~(0x1<<2); //清标志位
    
    s_Sum += ADC1->DR;
    s_Number++;
    
    //每8个数据求一次平均值
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
