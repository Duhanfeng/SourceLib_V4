#ifndef  __HW_USART_H
#define  __HW_USART_H

#include "stm32f0xx.h"

/*----------------------------------------------------------------------------
    数据缓冲区定义
 *----------------------------------------------------------------------------*/
 
/* ---定义数据缓冲大小--- */
#define TBUF_SIZE   64	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   64      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

/* ---定义接收缓冲区--- */
extern uint8_t Uart1_RecvBuff[RBUF_SIZE];
//extern uint8_t Uart2_RecvBuff[RBUF_SIZE];



/* 串口端口号 */
typedef enum 
{ 
  USARTx_1 = 0, 
  USARTx_2,
  USART_COUNT
}USART_TYPE;


#ifdef __cplusplus
extern "C" {
#endif

  void USARTx_Init(USART_TYPE Port);
  void USARTx_SendData(USART_TYPE Port, uint8_t cSendData);
  void USARTx_SendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint32_t iSize);
  void USARTx_SendStr(USART_TYPE Port,  char *pSendBuff);
  void USARTx_DMASendBuff(USART_TYPE Port, uint8_t *pSendBuff, uint16_t nSize);
  void USARTx_DMASendStr(USART_TYPE Port, char *pSendBuff);
  void USARTx_RxDMAReset(USART_TYPE Port);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
