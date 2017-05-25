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


//��λ��Ӧ��ͨ���ļ�����
void DMAx_ResetCounter(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  //�ر�ͨ��
  DMA_Channelx->CCR &= ~DMA_CCR1_EN;
  
  //���÷�������
  DMA_Channelx->CNDTR = nCount;
    
  //����ͨ��
  DMA_Channelx->CCR |=  DMA_CCR1_EN;
  
}


//��λ��Ӧ��ͨ�����ڴ��ַ�Լ�������
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



//�����ж�����
void DMAx_ITConfig(DMA_Channel_TypeDef *DMA_Channelx, uint8_t IRQ_Mask)
{
  //�ر������ж�
  DMA_Channelx->CCR &= ~(DMA_CCR1_TCIE | DMA_CCR1_HTIE | DMA_CCR1_TEIE);
  
  //������Ӧ���ж�
  DMA_Channelx->CCR |= (IRQ_Mask);
  
}



//���贫��ģʽ����
//����<--->�ڴ�
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


//��ʼ/�ر�DMA
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


//���ô�������
void DMAx_SetTransferCount(DMA_Channel_TypeDef *DMA_Channelx, uint16_t nCount)
{
  DMA_Channelx->CNDTR = nCount;
  
}


//��ȡ���������
uint16_t DMAx_GetTransferCount(DMA_Channel_TypeDef *DMA_Channelx)
{
  
  return DMA_Channelx->CNDTR;
}


//���������ַ
void DMAx_SetPeriAddr(DMA_Channel_TypeDef *DMA_Channelx, void *PeriAddr)
{
  DMA_Channelx->CPAR = (uint32_t)PeriAddr;
  
}


//�����ڴ��ַ
void DMAx_SetMemAddr(DMA_Channel_TypeDef *DMA_Channelx, void *MemAddr)
{
  DMA_Channelx->CMAR = (uint32_t)MemAddr;
  
}


//��ȡDMA��״̬
uint32_t DMAx_GetITStatus(DMA_TypeDef *DMAx)
{
  
  return DMAx->ISR;
}


//�����Ӧ�ı�־λ
void DMAx_ClearITStatus(DMA_TypeDef *DMAx, uint32_t iMask)
{
  DMAx->IFCR = iMask;
  
}





