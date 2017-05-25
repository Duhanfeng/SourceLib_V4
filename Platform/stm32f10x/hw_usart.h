#ifndef  __HW_USART_H
#define  __HW_USART_H

#include "stm32f10x.h"

/* ---定义数据缓冲大小--- */
#define RBUF_SIZE   64      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

/* ---定义接收缓冲区--- */
extern uint8_t Uart1_RecvBuff[RBUF_SIZE];
extern uint8_t Uart2_RecvBuff[RBUF_SIZE];
extern uint8_t Uart3_RecvBuff[RBUF_SIZE];
extern uint8_t Uart4_RecvBuff[RBUF_SIZE];

#ifdef __cplusplus
extern "C" {
#endif
  
  void USARTx_Init(USART_TypeDef *USARTx, uint32_t iBaudRate);
  uint16_t USARTx_GetRxDMACount(USART_TypeDef *USARTx);
  void USARTx_RxDMAReset(USART_TypeDef *USARTx);
  void USARTx_DMASendBuff(USART_TypeDef *USARTx, uint8_t *pSendBuff, uint16_t nSize);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_USART_H */
