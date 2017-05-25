#ifndef  __HW_DMA_H
#define  __HW_DMA_H

#include "SourceLib.h"

//DMA�ж�ģʽ
#define DMA_IT_TEIE   (0x08)  //DMA��������ж�
#define DMA_IT_HTIE   (0x04)  //DMA�봫���ж�
#define DMA_IT_TCIE   (0x02)  //DMA��������ж�

//DMA���䷽��
#define DMA_DIR_PERI_TO_MEM   (0x00)  //���赽�ڴ����
#define DMA_DIR_MEM_TO_PERI   (0x01)  //�ڴ浽�������

#ifdef __cplusplus
extern "C" {
#endif
  
  void DMAx_PeriTransferConfig(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr, void *MemAddr, uint8_t cDir);
  void DMAx_Enable(DMA_Channel_TypeDef *DMA_Channelx, uint8_t isEnable);
  void DMAx_Enable(DMA_Channel_TypeDef *DMA_Channelx, uint8_t isEnable);
  void DMAx_ResetCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount);
  void DMAx_ResetMemAddrAndCounter(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr, uint16_t nCount);
  void DMAx_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint8_t IRQ_Mask);
  void DMAx_SetTransferCount(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount);
  uint16_t DMAx_GetCurrentCount(DMA_Channel_TypeDef *DMA_Channelx);
  void DMAx_SetPeriAddr(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr);
  void DMAx_SetMemAddr(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr);
  uint32_t DMAx_GetITStatus(DMA_TypeDef *DMAx);
  void DMAx_ClearITStatus(DMA_TypeDef *DMAx, uint32_t iMask);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_DMA_H */
