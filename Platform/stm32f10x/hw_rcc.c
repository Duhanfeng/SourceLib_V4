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
#include "hw_rcc.h"
#include "SourceLib.h"


//使能对应模块的时钟
void RCC_EnableClock(void *PERI_ADDR, uint8_t isEnable)
{
  SL_ASSERT(PERI_ADDR);
  
  //开时钟
  if (isEnable)
  {
    switch ((uint32_t) PERI_ADDR)
    {
      /******************  Bit definition for RCC_AHBENR register  ******************/
      case DMA1_Channel1_BASE: 
      case DMA1_Channel2_BASE: 
      case DMA1_Channel3_BASE: 
      case DMA1_Channel4_BASE: 
      case DMA1_Channel5_BASE: 
      case DMA1_Channel6_BASE: 
      case DMA1_Channel7_BASE: 
      case DMA1_BASE         : RCC->AHBENR |= RCC_AHBENR_DMA1EN;   break;
      case CRC_BASE          : RCC->AHBENR |= RCC_AHBENR_CRCEN;    break;
      
      #if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
      case DMA2_Channel1_BASE: 
      case DMA2_Channel2_BASE: 
      case DMA2_Channel3_BASE: 
      case DMA2_Channel4_BASE: 
      case DMA2_Channel5_BASE: 
      case DMA2_BASE         : RCC->AHBENR |= RCC_AHBENR_DMA2EN;   break;
      #endif

      #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      case FSMC_Bank1_R_BASE : 
      case FSMC_Bank1E_R_BASE: 
      case FSMC_Bank2_R_BASE : 
      case FSMC_Bank3_R_BASE : 
      case FSMC_Bank4_R_BASE : RCC->AHBENR |= RCC_AHBENR_FSMCEN;   break;
      case SDIO_BASE         : RCC->AHBENR |= RCC_AHBENR_SDIOEN;   break;
      #endif

      #if defined (STM32F10X_HD_VL)
      case FSMC_Bank1_R_BASE : 
      case FSMC_Bank1E_R_BASE: 
      case FSMC_Bank2_R_BASE : 
      case FSMC_Bank3_R_BASE : 
      case FSMC_Bank4_R_BASE : RCC->AHBENR |= RCC_AHBENR_FSMCEN;   break;
      #endif
      
      #ifdef STM32F10X_CL
      case ETH_MAC_BASE: 
      case ETH_MMC_BASE: 
      case ETH_PTP_BASE: 
      case ETH_DMA_BASE: RCC->AHBENR |= RCC_AHBENR_ETHMACEN 
                                     |  RCC_AHBENR_ETHMACTXEN 
                                     |  RCC_AHBENR_ETHMACRXEN; break;
      #endif /* STM32F10X_CL */

      /******************  Bit definition for RCC_APB2ENR register  *****************/
      case AFIO_BASE  : RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;   break;
      case GPIOA_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   break;
      case GPIOB_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   break;
      case GPIOC_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   break;
      case GPIOD_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;   break;
      case ADC1_BASE  : RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case ADC2_BASE  : RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;   break;
      #endif
      
      case TIM1_BASE  : RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;   break;
      case SPI1_BASE  : RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;   break;
      case USART1_BASE: RCC->APB2ENR |= RCC_APB2ENR_USART1EN; break;
      
      #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
      case TIM15_BASE : RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;  break;
      case TIM16_BASE : RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;  break;
      case TIM17_BASE : RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;  break;
      #endif
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case GPIOE_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */
      
      #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      case GPIOF_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;   break;
      case GPIOG_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;   break;
      case TIM8_BASE  : RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;   break;
      case ADC3_BASE  : RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;   break;
      #endif

      #if defined (STM32F10X_HD_VL)
      case GPIOF_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;   break;
      case GPIOG_BASE : RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;   break;
      #endif
      
      #ifdef STM32F10X_XL
      case TIM9_BASE  : RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;   break;
      case TIM10_BASE : RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;  break;
      case TIM11_BASE : RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;  break;
      #endif
      
      /*****************  Bit definition for RCC_APB1ENR register  ******************/
      case TIM2_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   break;
      case TIM3_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   break;
      case WWDG_BASE  : RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;   break;
      case USART2_BASE: RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
      case I2C1_BASE  : RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case CAN1_BASE  : RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;   break;
      #endif
          
      case BKP_BASE   : RCC->APB1ENR |= RCC_APB1ENR_BKPEN;    break;
      case PWR_BASE   : RCC->APB1ENR |= RCC_APB1ENR_PWREN;    break;
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case TIM4_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   break;
      case SPI2_BASE  : RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;   break;
      case USART3_BASE: RCC->APB1ENR |= RCC_APB1ENR_USART3EN; break;
      case I2C2_BASE  : RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */

      #if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
      #endif
      
      #if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
      case TIM5_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;   break;
      case TIM6_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;   break;
      case TIM7_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;   break;
      case SPI3_BASE  : RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;   break;
      case UART4_BASE : RCC->APB1ENR |= RCC_APB1ENR_UART4EN;  break;
      case UART5_BASE : RCC->APB1ENR |= RCC_APB1ENR_UART5EN;  break;
      case DAC_BASE   : RCC->APB1ENR |= RCC_APB1ENR_DACEN;    break;
      #endif
      
      #if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
      case TIM6_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;   break;
      case TIM7_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;   break;
      case DAC_BASE   : RCC->APB1ENR |= RCC_APB1ENR_DACEN;    break;
      case CEC_BASE   : RCC->APB1ENR |= RCC_APB1ENR_CECEN;    break;
      #endif

      #ifdef STM32F10X_HD_VL
      case TIM5_BASE  : RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;   break;
      case TIM12_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;  break;
      case TIM13_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;  break;
      case TIM14_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;  break;
      case SPI3_BASE  : RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;   break;
      case UART4_BASE : RCC->APB1ENR |= RCC_APB1ENR_UART4EN;  break;
      case UART5_BASE : RCC->APB1ENR |= RCC_APB1ENR_UART5EN;  break;
      #endif /* STM32F10X_HD_VL */

      #ifdef STM32F10X_CL
      case CAN2_BASE  : RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;   break;
      #endif /* STM32F10X_CL */

      #ifdef STM32F10X_XL
      case TIM12_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;  break;
      case TIM13_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;  break;
      case TIM14_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;  break;
      #endif /* STM32F10X_XL */
      
      default: SL_ASSERT(0); break;
    }
  }
  //关时钟
  else 
  {
    switch ((uint32_t) PERI_ADDR)
    {
      /******************  Bit definition for RCC_AHBENR register  ******************/
      case DMA1_Channel1_BASE: 
      case DMA1_Channel2_BASE: 
      case DMA1_Channel3_BASE: 
      case DMA1_Channel4_BASE: 
      case DMA1_Channel5_BASE: 
      case DMA1_Channel6_BASE: 
      case DMA1_Channel7_BASE: 
      case DMA1_BASE         : RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;   break;
      case CRC_BASE          : RCC->AHBENR &= ~RCC_AHBENR_CRCEN;    break;
      
      #if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
      case DMA2_Channel1_BASE: 
      case DMA2_Channel2_BASE: 
      case DMA2_Channel3_BASE: 
      case DMA2_Channel4_BASE: 
      case DMA2_Channel5_BASE: 
      case DMA2_BASE         : RCC->AHBENR &= ~RCC_AHBENR_DMA2EN;   break;
      #endif

      #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      case FSMC_Bank1_R_BASE : 
      case FSMC_Bank1E_R_BASE: 
      case FSMC_Bank2_R_BASE : 
      case FSMC_Bank3_R_BASE : 
      case FSMC_Bank4_R_BASE : RCC->AHBENR &= ~RCC_AHBENR_FSMCEN;   break;
      case SDIO_BASE         : RCC->AHBENR &= ~RCC_AHBENR_SDIOEN;   break;
      #endif

      #if defined (STM32F10X_HD_VL)
      case FSMC_Bank1_R_BASE : 
      case FSMC_Bank1E_R_BASE: 
      case FSMC_Bank2_R_BASE : 
      case FSMC_Bank3_R_BASE : 
      case FSMC_Bank4_R_BASE : RCC->AHBENR &= ~RCC_AHBENR_FSMCEN;   break;
      #endif
      
      #ifdef STM32F10X_CL
      case ETH_MAC_BASE: 
      case ETH_MMC_BASE: 
      case ETH_PTP_BASE: 
      case ETH_DMA_BASE: RCC->AHBENR &= ~(RCC_AHBENR_ETHMACEN 
                                     |    RCC_AHBENR_ETHMACTXEN 
                                     |    RCC_AHBENR_ETHMACRXEN); break;
      #endif /* STM32F10X_CL */

      /******************  Bit definition for RCC_APB2ENR register  *****************/
      case AFIO_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;   break;
      case GPIOA_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPAEN;   break;
      case GPIOB_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPBEN;   break;
      case GPIOC_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPCEN;   break;
      case GPIOD_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPDEN;   break;
      case ADC1_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;   break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case ADC2_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_ADC2EN;   break;
      #endif
      
      case TIM1_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;   break;
      case SPI1_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;   break;
      case USART1_BASE: RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN; break;
      
      #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
      case TIM15_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_TIM15EN;  break;
      case TIM16_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_TIM16EN;  break;
      case TIM17_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_TIM17EN;  break;
      #endif
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case GPIOE_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPEEN;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */
      
      #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      case GPIOF_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPFEN;   break;
      case GPIOG_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPGEN;   break;
      case TIM8_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN;   break;
      case ADC3_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_ADC3EN;   break;
      #endif

      #if defined (STM32F10X_HD_VL)
      case GPIOF_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPFEN;   break;
      case GPIOG_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_IOPGEN;   break;
      #endif
      
      #ifdef STM32F10X_XL
      case TIM9_BASE  : RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;   break;
      case TIM10_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_TIM10EN;  break;
      case TIM11_BASE : RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;  break;
      #endif
      
      /*****************  Bit definition for RCC_APB1ENR register  ******************/
      case TIM2_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;   break;
      case TIM3_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;   break;
      case WWDG_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_WWDGEN;   break;
      case USART2_BASE: RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN; break;
      case I2C1_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN; break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case CAN1_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_CAN1EN;   break;
      #endif
          
      case BKP_BASE   : RCC->APB1ENR &= ~RCC_APB1ENR_BKPEN;    break;
      case PWR_BASE   : RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;    break;
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case TIM4_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;   break;
      case SPI2_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;   break;
      case USART3_BASE: RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN; break;
      case I2C2_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */

      #if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)

      #endif
      
      #if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
      case TIM5_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;   break;
      case TIM6_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;   break;
      case TIM7_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN;   break;
      case SPI3_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;   break;
      case UART4_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;  break;
      case UART5_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;  break;
      case DAC_BASE   : RCC->APB1ENR &= ~RCC_APB1ENR_DACEN;    break;
      #endif
      
      #if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
      case TIM6_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;   break;
      case TIM7_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN;   break;
      case DAC_BASE   : RCC->APB1ENR &= ~RCC_APB1ENR_DACEN;    break;
      case CEC_BASE   : RCC->APB1ENR &= ~RCC_APB1ENR_CECEN;    break;
      #endif

      #ifdef STM32F10X_HD_VL
      case TIM5_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;   break;
      case TIM12_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;  break;
      case TIM13_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_TIM13EN;  break;
      case TIM14_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN;  break;
      case SPI3_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;   break;
      case UART4_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;  break;
      case UART5_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;  break;
      #endif /* STM32F10X_HD_VL */

      #ifdef STM32F10X_CL
      case CAN2_BASE  : RCC->APB1ENR &= ~RCC_APB1ENR_CAN2EN;   break;
      #endif /* STM32F10X_CL */

      #ifdef STM32F10X_XL
      case TIM12_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;  break;
      case TIM13_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_TIM13EN;  break;
      case TIM14_BASE : RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN;  break;
      #endif /* STM32F10X_XL */
      
      default: SL_ASSERT(0); break;
    }
  }
    
}



//复位时钟
//注: 当进入复位模式下,对寄存器的操作无效,必须要退出复位模式能对其寄存器进行配置
void RCC_ResetClock(void *PERI_ADDR, uint8_t isEnable)
{
  SL_ASSERT(PERI_ADDR);
  
  //开时钟
  if (isEnable)
  {
    switch ((uint32_t) PERI_ADDR)
    {
      /******************  Bit definition for RCC_APB2RSTR register  *****************/
      case AFIO_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_AFIORST;   break;
      case GPIOA_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPARST;   break;
      case GPIOB_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPBRST;   break;
      case GPIOC_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPCRST;   break;
      case GPIOD_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPDRST;   break;
      case ADC1_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST;   break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case ADC2_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_ADC2RST;   break;
      #endif
      
      case TIM1_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;   break;
      case SPI1_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;   break;
      case USART1_BASE: RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST; break;
      
      #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
      case TIM15_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_TIM15RST;  break;
      case TIM16_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_TIM16RST;  break;
      case TIM17_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_TIM17RST;  break;
      #endif
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case GPIOE_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPERST;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */
      
      #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      case GPIOF_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPFRST;   break;
      case GPIOG_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPGRST;   break;
      case TIM8_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_TIM8RST;   break;
      case ADC3_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_ADC3RST;   break;
      #endif

      #if defined (STM32F10X_HD_VL)
      case GPIOF_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPFRST;   break;
      case GPIOG_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_IOPGRST;   break;
      #endif
      
      #ifdef STM32F10X_XL
      case TIM9_BASE  : RCC->APB2RSTR |= RCC_APB2RSTR_TIM9RST;   break;
      case TIM10_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_TIM10RST;  break;
      case TIM11_BASE : RCC->APB2RSTR |= RCC_APB2RSTR_TIM11RST;  break;
      #endif
      
      /*****************  Bit definition for RCC_APB1RSTR register  ******************/
      case TIM2_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;   break;
      case TIM3_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;   break;
      case WWDG_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_WWDGRST;   break;
      case USART2_BASE: RCC->APB1RSTR |= RCC_APB1RSTR_USART2RST; break;
      case I2C1_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_USART2RST; break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case CAN1_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_CAN1RST;   break;
      #endif
          
      case BKP_BASE   : RCC->APB1RSTR |= RCC_APB1RSTR_BKPRST;    break;
      case PWR_BASE   : RCC->APB1RSTR |= RCC_APB1RSTR_PWRRST;    break;
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case TIM4_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM4RST;   break;
      case SPI2_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;   break;
      case USART3_BASE: RCC->APB1RSTR |= RCC_APB1RSTR_USART3RST; break;
      case I2C2_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */

      #if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
      #endif
      
      #if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
      case TIM5_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM5RST;   break;
      case TIM6_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;   break;
      case TIM7_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST;   break;
      case SPI3_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;   break;
      case UART4_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_UART4RST;  break;
      case UART5_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_UART5RST;  break;
      case DAC_BASE   : RCC->APB1RSTR |= RCC_APB1RSTR_DACRST;    break;
      #endif
      
      #if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
      case TIM6_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;   break;
      case TIM7_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM7RST;   break;
      case DAC_BASE   : RCC->APB1RSTR |= RCC_APB1RSTR_DACRST;    break;
      case CEC_BASE   : RCC->APB1RSTR |= RCC_APB1RSTR_CECRST;    break;
      #endif

      #ifdef STM32F10X_HD_VL
      case TIM5_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_TIM5RST;   break;
      case TIM12_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_TIM12RST;  break;
      case TIM13_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_TIM13RST;  break;
      case TIM14_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_TIM14RST;  break;
      case SPI3_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;   break;
      case UART4_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_UART4RST;  break;
      case UART5_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_UART5RST;  break;
      #endif /* STM32F10X_HD_VL */

      #ifdef STM32F10X_CL
      case CAN2_BASE  : RCC->APB1RSTR |= RCC_APB1RSTR_CAN2RST;   break;
      #endif /* STM32F10X_CL */

      #ifdef STM32F10X_XL
      case TIM12_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_TIM12RST;  break;
      case TIM13_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_TIM13RST;  break;
      case TIM14_BASE : RCC->APB1RSTR |= RCC_APB1RSTR_TIM14RST;  break;
      #endif /* STM32F10X_XL */
      
      default: SL_ASSERT(0); break;
    }
  }
  //关时钟
  else 
  {
    switch ((uint32_t) PERI_ADDR)
    {
      /******************  Bit definition for RCC_APB2RSTR register  *****************/
      case AFIO_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_AFIORST;   break;
      case GPIOA_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPARST;   break;
      case GPIOB_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPBRST;   break;
      case GPIOC_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPCRST;   break;
      case GPIOD_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPDRST;   break;
      case ADC1_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_ADC1RST;   break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case ADC2_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_ADC2RST;   break;
      #endif
      
      case TIM1_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;   break;
      case SPI1_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;   break;
      case USART1_BASE: RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST; break;
      
      #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
      case TIM15_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM15RST;  break;
      case TIM16_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM16RST;  break;
      case TIM17_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM17RST;  break;
      #endif
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case GPIOE_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPERST;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */
      
      #if defined (STM32F10X_HD) || defined (STM32F10X_XL)
      case GPIOF_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPFRST;   break;
      case GPIOG_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPGRST;   break;
      case TIM8_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM8RST;   break;
      case ADC3_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_ADC3RST;   break;
      #endif

      #if defined (STM32F10X_HD_VL)
      case GPIOF_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPFRST;   break;
      case GPIOG_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPGRST;   break;
      #endif
      
      #ifdef STM32F10X_XL
      case TIM9_BASE  : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM9RST;   break;
      case TIM10_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM10RST;  break;
      case TIM11_BASE : RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM11RST;  break;
      #endif
      
      /*****************  Bit definition for RCC_APB1RSTR register  ******************/
      case TIM2_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;   break;
      case TIM3_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;   break;
      case WWDG_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_WWDGRST;   break;
      case USART2_BASE: RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST; break;
      case I2C1_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST; break;
      
      #if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
      case CAN1_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST;   break;
      #endif
          
      case BKP_BASE   : RCC->APB1RSTR &= ~RCC_APB1RSTR_BKPRST;    break;
      case PWR_BASE   : RCC->APB1RSTR &= ~RCC_APB1RSTR_PWRRST;    break;
      
      #if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
      case TIM4_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM4RST;   break;
      case SPI2_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;   break;
      case USART3_BASE: RCC->APB1RSTR &= ~RCC_APB1RSTR_USART3RST; break;
      case I2C2_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;   break;
      #endif /* STM32F10X_LD && STM32F10X_LD_VL */

      #if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)

      #endif
      
      #if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
      case TIM5_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM5RST;   break;
      case TIM6_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;   break;
      case TIM7_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;   break;
      case SPI3_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;   break;
      case UART4_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_UART4RST;  break;
      case UART5_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_UART5RST;  break;
      case DAC_BASE   : RCC->APB1RSTR &= ~RCC_APB1RSTR_DACRST;    break;
      #endif
      
      #if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
      case TIM6_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;   break;
      case TIM7_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;   break;
      case DAC_BASE   : RCC->APB1RSTR &= ~RCC_APB1RSTR_DACRST;    break;
      case CEC_BASE   : RCC->APB1RSTR &= ~RCC_APB1RSTR_CECRST;    break;
      #endif

      #ifdef STM32F10X_HD_VL
      case TIM5_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM5RST;   break;
      case TIM12_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM12RST;  break;
      case TIM13_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM13RST;  break;
      case TIM14_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM14RST;  break;
      case SPI3_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;   break;
      case UART4_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_UART4RST;  break;
      case UART5_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_UART5RST;  break;
      #endif /* STM32F10X_HD_VL */

      #ifdef STM32F10X_CL
      case CAN2_BASE  : RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN2RST;   break;
      #endif /* STM32F10X_CL */

      #ifdef STM32F10X_XL
      case TIM12_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM12RST;  break;
      case TIM13_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM13RST;  break;
      case TIM14_BASE : RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM14RST;  break;
      #endif /* STM32F10X_XL */
      
      default: SL_ASSERT(0); break;
    }
  }
    
}

