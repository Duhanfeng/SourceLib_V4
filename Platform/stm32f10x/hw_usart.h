#ifndef  __HW_USART_H
#define  __HW_USART_H

#include "stm32f10x.h"

/* 串口端口号 */
typedef enum 
{ 
  USARTx_1 = 0, 
  USARTx_2,
  USARTx_3,
  USARTx_4,
  USARTx_COUNT
}USART_TYPE;


#ifdef __cplusplus
extern "C" {
#endif

  void USARTx_Init(USART_TYPE Port, uint32_t iBaudRate);
  void USARTx_SendData(USART_TYPE Port, uint8_t cSendData);
  void USARTx_SendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint32_t iSize);
  void USARTx_SendStr(USART_TYPE Port,  char *pSendBuff);
  void USARTx_DMASendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint16_t nSize);
  void USARTx_DMASendStr(USART_TYPE Port, char *pSendBuff);
  void USARTx_RxDMAReset(USART_TYPE Port);
  void USARTx_ReadRxBuff(USART_TYPE Port, uint8_t *pReadBuff, uint16_t nSize);
  
#ifdef __cplusplus
}
#endif

#endif /* __HW_USART_H */
