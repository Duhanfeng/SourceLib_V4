#ifndef  __CMD_APP_H
#define  __CMD_APP_H

#include "cmd_check.h"

#define CMD_ID_COM1      (0xA0)
#define CMD_ID_COM2      (0xA1)
#define CMD_ID_COM3      (0xA2)
#define CMD_ID_COM4      (0xA3)

/* ������� */
typedef enum
{
  Success = 0,      //�ɹ�
  NumberError = 1,  //������������
  RangeError = 2,   //������Χ����
  CommandError = 3, //�������(�޴�����)
  Reserver = 4,     //Ԥ��

}COMMAND_RESULT;


extern CMD_PARAM Uart1Param;
extern CMD_PARAM Uart2Param;
extern CMD_PARAM Uart3Param;
extern CMD_PARAM EthernetParam;

void AT_CmdProcess(CMD_PARAM *CMD_Ptr);



#endif
