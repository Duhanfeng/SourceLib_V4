#ifndef  __HW_USART_H
#define  __HW_USART_H

#include "stm32f10x.h"

/* ---�������ݻ����С--- */
#define RBUF_SIZE   128      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

/* ---������ջ�����--- */
extern uint8_t Uart1_RecvBuff[RBUF_SIZE];
extern uint8_t Uart2_RecvBuff[RBUF_SIZE];
extern uint8_t Uart3_RecvBuff[RBUF_SIZE];
extern uint8_t Uart4_RecvBuff[RBUF_SIZE];

#ifdef __cplusplus
extern "C" {
#endif
  
  //USART��ʼ��
  void USARTx_Init(USART_TypeDef *USARTx, uint32_t iBaudRate);
  
  //��ͨ����
  void USARTx_SendData(USART_TypeDef *USARTx, uint8_t cSendData);
  void USARTx_SendBuff(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint32_t iSize);
  void USARTx_SendStr(USART_TypeDef *USARTx, char *pSendBuff);
  
  //ͨ��DMA����
  void USARTx_DMASendBuff(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint16_t nSize);
  void USARTx_DMASendStr(USART_TypeDef *USARTx, char *pSendBuff);
  
  //DMA���
  void USARTx_RxDMAReset(USART_TypeDef *USARTx);
  uint16_t USARTx_GetRxDMACount(USART_TypeDef *USARTx);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_USART_H */
