/**
  ******************************************************************************
  * @file    hw_dma.h
  * @author  �Ź��Ӻ���
  * @version V1.0 �Ĵ����汾
  * @date    2017.05.22
  * @brief   DMA������ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */
  
  
/***********************************<INCLUDES>**********************************/
#include "hw_dma.h"
#include "SourceLib.h"
#include "hw_rcc.h"


/**
  * @brief  ���贫��ģʽ����
  * @param  DMA_Channelx DMAͨ�����
  * @param  PeriAddr �����ַ
  * @param  MemAddr �ڴ��ַ
  * @param  cDir ����(���赽�ڴ�/�ڴ浽����)
  * @retval None
  */
void DMAx_PeriTransferConfig(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr, void *MemAddr, uint8_t cDir)
{
  //��ʱ��
  RCC_EnableClock(DMA_Channelx, 1);
  
  //����ģʽ
  DMA_Channelx->CCR &= ~DMA_CCR1_MEM2MEM; //���ڴ�-�ڴ�ģʽ
  DMA_Channelx->CCR &= ~DMA_CCR1_PL; 
  DMA_Channelx->CCR |=  DMA_CCR1_PL_0;    //�е����ȼ�
  
  DMA_Channelx->CCR &= ~DMA_CCR1_MSIZE;   //�ڴ�:8λ����֡��ʽ
  DMA_Channelx->CCR &= ~DMA_CCR1_PSIZE;   //����:8λ����֡��ʽ
  
  DMA_Channelx->CCR |=  DMA_CCR1_MINC;    //�ڴ�����ģʽ
  DMA_Channelx->CCR &= ~DMA_CCR1_PINC;    //���������ģʽ
  DMA_Channelx->CCR &= ~DMA_CCR1_CIRC;    //��ѭ��ģʽ
  
  //���ô��䷽��
  DMA_Channelx->CCR &= ~DMA_CCR1_DIR;
  DMA_Channelx->CCR |=  (cDir ? DMA_CCR1_DIR : 0);  //cDir: 0--���赽�ڴ� 1--�ڴ浽����

  //���������ж�
  DMA_Channelx->CCR &= ~DMA_CCR1_TEIE;    //�رմ����ж�
  DMA_Channelx->CCR &= ~DMA_CCR1_HTIE;    //�رհ봫���ж�
  DMA_Channelx->CCR &= ~DMA_CCR1_TCIE;    //�رմ�������ж�
  
  //���������ַ
  DMA_Channelx->CPAR = (uint32_t)PeriAddr;
  
  //�����ڴ��ַ
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
  //�ر�DMA
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
}



/**
  * @brief  ���贫��ģʽ����
  * @param  DMA_Channelx DMAͨ�����
  * @param  isEnable 1-����DMA,0-�ر�DMA
  * @retval None
  */
void DMAx_Enable(DMA_Channel_TypeDef *DMA_Channelx, uint8_t isEnable)
{
  if (isEnable)
  {
    DMA_Channelx->CCR |=  DMA_CCR1_EN;
  }
  else 
  {
    DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  }
  
}



/**
  * @brief  ��λ��Ӧ��ͨ���ļ�����
  * @param  DMA_Channelx DMAͨ�����
  * @param  nCount �������ĸ�λֵ
  * @retval None
  */
void DMAx_ResetCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  //�ر�ͨ��
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
  //���÷�������
  DMA_Channelx->CNDTR = nCount;
    
  //����ͨ��
  DMA_Channelx->CCR |=  DMA_CCR1_EN;
  
}



/**
  * @brief  ��λ��Ӧ��ͨ�����ڴ��ַ�Լ�������
  * @param  DMA_Channelx DMAͨ�����
  * @param  MemAddr �ڴ��ַ�ĸ�λֵ
  * @param  nCount �������ĸ�λֵ
  * @retval None
  */
void DMAx_ResetMemAddrAndCounter(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr, uint16_t nCount)
{
  //�ر�ͨ��
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
  //�����ڴ��ַ
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
  //���÷�������
  DMA_Channelx->CNDTR = nCount;
    
  //����ͨ��
  DMA_Channelx->CCR |=  DMA_CCR1_EN;
  
}



/**
  * @brief  �����ж�����
  * @param  DMA_Channelx DMAͨ�����
  * @param  IRQ_Mask ��Ӧ�жϵ�����
  * @retval None
  */
void DMAx_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint8_t IRQ_Mask)
{
  //�ر������ж�
  DMA_Channelx->CCR &= ~(DMA_CCR1_TCIE | DMA_CCR1_HTIE | DMA_CCR1_TEIE);
  
  //������Ӧ���ж�
  DMA_Channelx->CCR |= (IRQ_Mask);
  
}



/**
  * @brief  ���ô�������
  * @param  DMA_Channelx DMAͨ�����
  * @param  nCount Ҫ���õļ�������ֵ
  * @retval None
  */
void DMAx_SetTransferCount(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  DMA_Channelx->CNDTR = nCount;
  
}



/**
  * @brief  ��ȡ��ǰ��������ֵ
  * @param  DMA_Channelx DMAͨ�����
  * @retval ��ǰ��������ֵ
  */
uint16_t DMAx_GetCurrentCount(DMA_Channel_TypeDef *DMA_Channelx)
{
  
  return DMA_Channelx->CNDTR;
}



/**
  * @brief  ���������ַ
  * @param  DMA_Channelx DMAͨ�����
  * @param  PeriAddr �����ַ
  * @retval None
  */
void DMAx_SetPeriAddr(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr)
{
  DMA_Channelx->CPAR = (uint32_t)PeriAddr;
  
}



/**
  * @brief  �����ڴ��ַ
  * @param  DMA_Channelx DMAͨ�����
  * @param  MemAddr �ڴ��ַ
  * @retval None
  */
void DMAx_SetMemAddr(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr)
{
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
}



/**
  * @brief  ��ȡDMA��״̬
  * @param  DMAx DMA���
  * @retval ��ǰDMA��״̬
  */
uint32_t DMAx_GetITStatus(DMA_TypeDef *DMAx)
{
  
  return DMAx->ISR;
}



/**
  * @brief  �����Ӧ�ı�־λ
  * @param  DMAx DMA���
  * @param  iMask Ҫ�����״̬λ������
  * @retval None
  */
void DMAx_ClearITStatus(DMA_TypeDef *DMAx, uint32_t iMask)
{
  DMAx->IFCR = iMask;
  
}

