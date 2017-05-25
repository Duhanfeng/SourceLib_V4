/**
  ******************************************************************************
  * @file    hw_rcc.h
  * @author  杜公子寒枫
  * @version V1.0 寄存器版本
  * @date    2017.05.22
  * @brief   RCC配置函数
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_nvic.h"
#include "SourceLib.h"


/**
  * @brief  外部中断初始化函数
  * @param  IRQn 中断号
  * @param  PreemptPriority  抢占优先级
  * @param  SubPriority  响应优先级
  * @retval None
  */
void NVIC_Enable(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);  
  NVIC_SetPriority((IRQn), NVIC_EncodePriority (NVIC_PRIORITY_GROUP, PreemptPriority, SubPriority));  
  NVIC_EnableIRQ(IRQn); 
  
}



/**
  * @brief  禁止对应的内核中断
  * @param  IRQn 中断号
  * @retval None
  */
void NVIC_Disable(IRQn_Type IRQn)
{ 
  NVIC_DisableIRQ((IRQn));
  
}



/**
  * @brief  根据外设地址进行内核中断配置
  * @param  PERI_ADDR  外设地址
  * @param  PrePri  抢占优先级
  * @param  SubPriority  响应优先级
  * @retval None
  * @note   对于TIM1和TIM8,默认是开启中断号TIMx_CC_IRQn,用于捕获/比较
  */
void NVIC_Config(void *PERI_ADDR, uint32_t PreemptPriority, uint32_t SubPriority)
{
  SL_ASSERT(PERI_ADDR);

  switch ((uint32_t) PERI_ADDR)
  {
    /******************  Bit definition for RCC_AHBENR register  ******************/
    case DMA1_Channel1_BASE: NVIC_Enable(DMA1_Channel1_IRQn, PreemptPriority, SubPriority); break;
    case DMA1_Channel2_BASE: NVIC_Enable(DMA1_Channel2_IRQn, PreemptPriority, SubPriority); break;
    case DMA1_Channel3_BASE: NVIC_Enable(DMA1_Channel3_IRQn, PreemptPriority, SubPriority); break;
    case DMA1_Channel4_BASE: NVIC_Enable(DMA1_Channel4_IRQn, PreemptPriority, SubPriority); break;
    case DMA1_Channel5_BASE: NVIC_Enable(DMA1_Channel5_IRQn, PreemptPriority, SubPriority); break;
    case DMA1_Channel6_BASE: NVIC_Enable(DMA1_Channel6_IRQn, PreemptPriority, SubPriority); break;
    case DMA1_Channel7_BASE: NVIC_Enable(DMA1_Channel7_IRQn, PreemptPriority, SubPriority); break;
    case CRC_BASE          : break;
    
    #if defined (STM32F10X_HD) || defined  (STM32F10X_HD_VL)
    case DMA2_Channel1_BASE: NVIC_Enable(DMA2_Channel1_IRQn, PreemptPriority, SubPriority);   break;
    case DMA2_Channel2_BASE: NVIC_Enable(DMA2_Channel2_IRQn, PreemptPriority, SubPriority);   break;
    case DMA2_Channel3_BASE: NVIC_Enable(DMA2_Channel3_IRQn, PreemptPriority, SubPriority);   break;
    case DMA2_Channel4_BASE: NVIC_Enable(DMA2_Channel4_5_IRQn, PreemptPriority, SubPriority); break;
    case DMA2_Channel5_BASE: NVIC_Enable(DMA2_Channel4_5_IRQn, PreemptPriority, SubPriority); break;
    case DMA2_BASE         : break;
    #endif
    
    #if defined  (STM32F10X_CL)
    case DMA2_Channel4_BASE: NVIC_Enable(DMA2_Channel4_IRQn, PreemptPriority, SubPriority);   break;
    case DMA2_Channel5_BASE: NVIC_Enable(DMA2_Channel5_IRQn, PreemptPriority, SubPriority);   break;
    #endif

    #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case FSMC_Bank1_R_BASE : 
    case FSMC_Bank1E_R_BASE: 
    case FSMC_Bank2_R_BASE : 
    case FSMC_Bank3_R_BASE : 
    case FSMC_Bank4_R_BASE : NVIC_Enable(FSMC_IRQn, PreemptPriority, SubPriority); break;
    case SDIO_BASE         : NVIC_Enable(SDIO_IRQn, PreemptPriority, SubPriority); break;
    #endif
    
    #ifdef STM32F10X_CL
    case ETH_MAC_BASE: NVIC_Enable(ETH_IRQn, PreemptPriority, SubPriority); break;
    case ETH_MMC_BASE: NVIC_Enable(ETH_IRQn, PreemptPriority, SubPriority); break;
    case ETH_PTP_BASE: NVIC_Enable(ETH_IRQn, PreemptPriority, SubPriority); break;
    case ETH_DMA_BASE: NVIC_Enable(ETH_IRQn, PreemptPriority, SubPriority); break;
    #endif /* STM32F10X_CL */

    /******************  Bit definition for RCC_APB2ENR register  *****************/
    #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
    case ADC1_BASE  : NVIC_Enable(ADC1_2_IRQn, PreemptPriority, SubPriority); break;
    case ADC2_BASE  : NVIC_Enable(ADC1_2_IRQn, PreemptPriority, SubPriority); break;
    #else 
    case ADC1_BASE  : NVIC_Enable(ADC1_IRQn, PreemptPriority, SubPriority);   break;
    #endif
    
    case TIM1_BASE  : NVIC_Enable(TIM1_CC_IRQn, PreemptPriority, SubPriority);break;
    case SPI1_BASE  : NVIC_Enable(SPI1_IRQn, PreemptPriority, SubPriority);   break;
    case USART1_BASE: NVIC_Enable(USART1_IRQn, PreemptPriority, SubPriority); break;
    
    #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
    case TIM15_BASE : NVIC_Enable(TIM1_BRK_TIM15_IRQn, PreemptPriority, SubPriority); break;
    case TIM16_BASE : NVIC_Enable(TIM1_UP_TIM16_IRQn, PreemptPriority, SubPriority);  break;
    case TIM17_BASE : NVIC_Enable(TIM1_TRG_COM_TIM17_IRQn, PreemptPriority, SubPriority); break;
    #endif
    
    #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case TIM8_BASE  : NVIC_Enable(TIM8_CC_IRQn, PreemptPriority, SubPriority);break;
    case ADC3_BASE  : NVIC_Enable(ADC3_IRQn, PreemptPriority, SubPriority);   break;
    #endif
    
    #ifdef STM32F10X_XL
    case TIM9_BASE  : NVIC_Enable(TIM1_BRK_TIM9_IRQn, PreemptPriority, SubPriority);  break;
    case TIM10_BASE : NVIC_Enable(TIM1_UP_TIM10_IRQn, PreemptPriority, SubPriority);  break;
    case TIM11_BASE : NVIC_Enable(TIM1_TRG_COM_TIM11_IRQn, PreemptPriority, SubPriority); break;
    #endif
    
    /*****************  Bit definition for RCC_APB1ENR register  ******************/
    case TIM2_BASE  : NVIC_Enable(TIM2_IRQn, PreemptPriority, SubPriority);   break;
    case TIM3_BASE  : NVIC_Enable(TIM3_IRQn, PreemptPriority, SubPriority);   break;
    case WWDG_BASE  : NVIC_Enable(WWDG_IRQn, PreemptPriority, SubPriority);   break;
    case USART2_BASE: NVIC_Enable(USART2_IRQn, PreemptPriority, SubPriority); break;
//    case I2C1_BASE  : break;
    
    #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
//    case CAN1_BASE  : 
    #endif
        
//    case BKP_BASE   :   break;
//    case PWR_BASE   :   break;
    
    #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
    case TIM4_BASE  : NVIC_Enable(TIM4_IRQn, PreemptPriority, SubPriority);   break;
    case SPI2_BASE  : NVIC_Enable(SPI2_IRQn, PreemptPriority, SubPriority);   break;
    case USART3_BASE: NVIC_Enable(USART3_IRQn, PreemptPriority, SubPriority); break;
//    case I2C2_BASE  :   break;
    #endif /* STM32F10X_LD && STM32F10X_LD_VL */

    #if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
    #endif
    
    #if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
    case TIM5_BASE  : NVIC_Enable(TIM5_IRQn, PreemptPriority, SubPriority);   break;
    case TIM6_BASE  : NVIC_Enable(TIM6_IRQn, PreemptPriority, SubPriority);   break;
    case TIM7_BASE  : NVIC_Enable(TIM7_IRQn, PreemptPriority, SubPriority);   break;
    case SPI3_BASE  : NVIC_Enable(SPI3_IRQn, PreemptPriority, SubPriority);   break;
    case UART4_BASE : NVIC_Enable(UART4_IRQn, PreemptPriority, SubPriority);  break;
    case UART5_BASE : NVIC_Enable(UART5_IRQn, PreemptPriority, SubPriority);  break;
//    case DAC_BASE   : break;
    #endif
    
    #if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
    case TIM6_BASE  : NVIC_Enable(TIM6_DAC_IRQn, PreemptPriority, SubPriority); break;
    case TIM7_BASE  : NVIC_Enable(TIM7_IRQn, PreemptPriority, SubPriority);     break;
    case DAC_BASE   : NVIC_Enable(TIM6_DAC_IRQn, PreemptPriority, SubPriority); break;
//    case CEC_BASE   :   break;
    #endif

    #ifdef STM32F10X_HD_VL
    case TIM5_BASE  : NVIC_Enable(TIM5_IRQn,  PreemptPriority, SubPriority);  break;
    case TIM12_BASE : NVIC_Enable(TIM12_IRQn, PreemptPriority, SubPriority);  break;
    case TIM13_BASE : NVIC_Enable(TIM13_IRQn, PreemptPriority, SubPriority);  break;
    case TIM14_BASE : NVIC_Enable(TIM14_IRQn, PreemptPriority, SubPriority);  break;
    case SPI3_BASE  : NVIC_Enable(SPI3_IRQn,  PreemptPriority, SubPriority);  break;
    case UART4_BASE : NVIC_Enable(UART4_IRQn, PreemptPriority, SubPriority);  break;
    case UART5_BASE : NVIC_Enable(UART5_IRQn, PreemptPriority, SubPriority);  break;
    #endif /* STM32F10X_HD_VL */

    #ifdef STM32F10X_CL
//    case CAN2_BASE  :    break;
    #endif /* STM32F10X_CL */

    #ifdef STM32F10X_XL
    case TIM12_BASE : NVIC_Enable(TIM8_BRK_TIM12_IRQn,     PreemptPriority, SubPriority); break;
    case TIM13_BASE : NVIC_Enable(TIM8_UP_TIM13_IRQn,      PreemptPriority, SubPriority); break;
    case TIM14_BASE : NVIC_Enable(TIM8_TRG_COM_TIM14_IRQn, PreemptPriority, SubPriority); break;
    #endif /* STM32F10X_XL */
    
    default: SL_ASSERT(0); break;
  }
  
}



/**
  * @brief  根据外设地址获取其外设中断号
  * @param  PERI_ADDR  外设地址
  * @retval None
  */
IRQn_Type NVIC_GetIRQType(void *PERI_ADDR)
{
  IRQn_Type Type;
  
  SL_ASSERT(PERI_ADDR);
  
  switch ((uint32_t) PERI_ADDR)
  {
    /******************  Bit definition for RCC_AHBENR register  ******************/
    case DMA1_Channel1_BASE: Type = DMA1_Channel1_IRQn; break;
    case DMA1_Channel2_BASE: Type = DMA1_Channel2_IRQn; break;
    case DMA1_Channel3_BASE: Type = DMA1_Channel3_IRQn; break;
    case DMA1_Channel4_BASE: Type = DMA1_Channel4_IRQn; break;
    case DMA1_Channel5_BASE: Type = DMA1_Channel5_IRQn; break;
    case DMA1_Channel6_BASE: Type = DMA1_Channel6_IRQn; break;
    case DMA1_Channel7_BASE: Type = DMA1_Channel7_IRQn; break;
    case CRC_BASE          : break;
    
    #if defined (STM32F10X_HD) || defined  (STM32F10X_HD_VL)
    case DMA2_Channel1_BASE: Type = DMA2_Channel1_IRQn;   break;
    case DMA2_Channel2_BASE: Type = DMA2_Channel2_IRQn;   break;
    case DMA2_Channel3_BASE: Type = DMA2_Channel3_IRQn;   break;
    case DMA2_Channel4_BASE: Type = DMA2_Channel4_5_IRQn; break;
    case DMA2_Channel5_BASE: Type = DMA2_Channel4_5_IRQn; break;
    case DMA2_BASE         : break;
    #endif
    
    #if defined  (STM32F10X_CL)
    case DMA2_Channel4_BASE: Type = DMA2_Channel4_IRQn;   break;
    case DMA2_Channel5_BASE: Type = DMA2_Channel5_IRQn;   break;
    #endif

    #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case FSMC_Bank1_R_BASE : 
    case FSMC_Bank1E_R_BASE: 
    case FSMC_Bank2_R_BASE : 
    case FSMC_Bank3_R_BASE : 
    case FSMC_Bank4_R_BASE : Type = FSMC_IRQn; break;
    case SDIO_BASE         : Type = SDIO_IRQn; break;
    #endif
    
    #ifdef STM32F10X_CL
    case ETH_MAC_BASE: Type = ETH_IRQn; break;
    case ETH_MMC_BASE: Type = ETH_IRQn; break;
    case ETH_PTP_BASE: Type = ETH_IRQn; break;
    case ETH_DMA_BASE: Type = ETH_IRQn; break;
    #endif /* STM32F10X_CL */

    /******************  Bit definition for RCC_APB2ENR register  *****************/
    #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
    case ADC1_BASE  : Type = ADC1_2_IRQn; break;
    case ADC2_BASE  : Type = ADC1_2_IRQn; break;
    #else 
    case ADC1_BASE  : Type = ADC1_IRQn;   break;
    #endif
    
    case TIM1_BASE  : Type = TIM1_CC_IRQn;break;
    case SPI1_BASE  : Type = SPI1_IRQn;   break;
    case USART1_BASE: Type = USART1_IRQn; break;
    
    #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
    case TIM15_BASE : Type = TIM1_BRK_TIM15_IRQn; break;
    case TIM16_BASE : Type = TIM1_UP_TIM16_IRQn;  break;
    case TIM17_BASE : Type = TIM1_TRG_COM_TIM17_IRQn; break;
    #endif
    
    #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
    case TIM8_BASE  : Type = TIM8_CC_IRQn;break;
    case ADC3_BASE  : Type = ADC3_IRQn;   break;
    #endif
    
    #ifdef STM32F10X_XL
    case TIM9_BASE  : Type = TIM1_BRK_TIM9_IRQn;  break;
    case TIM10_BASE : Type = TIM1_UP_TIM10_IRQn;  break;
    case TIM11_BASE : Type = TIM1_TRG_COM_TIM11_IRQn; break;
    #endif
    
    /*****************  Bit definition for RCC_APB1ENR register  ******************/
    case TIM2_BASE  : Type = TIM2_IRQn;   break;
    case TIM3_BASE  : Type = TIM3_IRQn;   break;
    case WWDG_BASE  : Type = WWDG_IRQn;   break;
    case USART2_BASE: Type = USART2_IRQn; break;
//    case I2C1_BASE  : break;
    
    #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
//    case CAN1_BASE  : 
    #endif
        
//    case BKP_BASE   :   break;
//    case PWR_BASE   :   break;
    
    #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
    case TIM4_BASE  : Type = TIM4_IRQn;   break;
    case SPI2_BASE  : Type = SPI2_IRQn;   break;
    case USART3_BASE: Type = USART3_IRQn; break;
//    case I2C2_BASE  :   break;
    #endif /* STM32F10X_LD && STM32F10X_LD_VL */

    #if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
    #endif
    
    #if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
    case TIM5_BASE  : Type = TIM5_IRQn;   break;
    case TIM6_BASE  : Type = TIM6_IRQn;   break;
    case TIM7_BASE  : Type = TIM7_IRQn;   break;
    case SPI3_BASE  : Type = SPI3_IRQn;   break;
    case UART4_BASE : Type = UART4_IRQn;  break;
    case UART5_BASE : Type = UART5_IRQn;  break;
//    case DAC_BASE   : break;
    #endif
    
    #if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
    case TIM6_BASE  : Type = TIM6_DAC_IRQn; break;
    case TIM7_BASE  : Type = TIM7_IRQn;     break;
    case DAC_BASE   : Type = TIM6_DAC_IRQn; break;
//    case CEC_BASE   :   break;
    #endif

    #ifdef STM32F10X_HD_VL
    case TIM5_BASE  : Type = TIM5_IRQn,  PreemptPriority, SubPriority);  break;
    case TIM12_BASE : Type = TIM12_IRQn;  break;
    case TIM13_BASE : Type = TIM13_IRQn;  break;
    case TIM14_BASE : Type = TIM14_IRQn;  break;
    case SPI3_BASE  : Type = SPI3_IRQn,  PreemptPriority, SubPriority);  break;
    case UART4_BASE : Type = UART4_IRQn;  break;
    case UART5_BASE : Type = UART5_IRQn;  break;
    #endif /* STM32F10X_HD_VL */

    #ifdef STM32F10X_CL
//    case CAN2_BASE  :    break;
    #endif /* STM32F10X_CL */

    #ifdef STM32F10X_XL
    case TIM12_BASE : Type = TIM8_BRK_TIM12_IRQn,     PreemptPriority, SubPriority); break;
    case TIM13_BASE : Type = TIM8_UP_TIM13_IRQn,      PreemptPriority, SubPriority); break;
    case TIM14_BASE : Type = TIM8_TRG_COM_TIM14_IRQn; break;
    #endif /* STM32F10X_XL */
    
    default: SL_ASSERT(0); break;
  }
  
  return Type;
}




