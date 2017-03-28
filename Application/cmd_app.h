#ifndef  __CMD_APP_H
#define  __CMD_APP_H

#include "cmd_check.h"

#define CMD_ID_COM1      (0xA0)
#define CMD_ID_COM2      (0xA1)
#define CMD_ID_COM3      (0xA2)
#define CMD_ID_COM4      (0xA3)

/* 命令处理结果 */
typedef enum
{
  Success = 0,      //成功
  NumberError = 1,  //参数数量错误
  RangeError = 2,   //参数范围错误
  CommandError = 3, //命令错误(无此命令)
  Reserver = 4,     //预留

}COMMAND_RESULT;


extern CMD_PARAM Uart1Param;
extern CMD_PARAM Uart2Param;
extern CMD_PARAM Uart3Param;
extern CMD_PARAM EthernetParam;

void AT_CmdProcess(CMD_PARAM *CMD_Ptr);



#endif
