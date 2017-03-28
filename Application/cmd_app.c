/**
  ******************************************************************************
  * @file    cmd_app.c
  * @author  �Ź��Ӻ���
  * @version V1.0
  * @date    2016.08.28
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * �������/Ӧ���ļ�
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "cmd_app.h"
#include "common.h"
#include "cmd_check.h"
#include <stdio.h>
#include "stm32f10x.h"



CMD_PARAM Uart1Param   = {{0}, 0, 0, 0, CMD_BUFF_SIZE, 0, CMD_ID_COM1};

static const uint8_t CMD_RESET[]        =   "RESET";        //��λ



//����ָ��
COMMAND_RESULT DebugCommand_Handler(CMD_PARAM *CMD_Ptr)
{
  AT_PARAM_TPYE AT_Parameter = {0};   //�����ṹ��
  COMMAND_RESULT CmdResult;     //�������
  
  /* ����ƥ��.. */

  // ��λ
  if (AT_Check_xCmdy_Command(CMD_Ptr, CMD_RESET, &AT_Parameter))
  {
    
    //���������ж�
    if ((AT_Parameter.InputCount != 0) || (AT_Parameter.OutputCount != 0))
    {
      CmdResult = NumberError;
    }
    
    //�����
    else
    {
      SCB->AIRCR = ((uint32_t)0x05FA0004);  //��λ����
      CmdResult = Success;
    }
    
  }
  else;

  return CmdResult;
}

  

//�����(����main��������ѯ)
void AT_CmdProcess(CMD_PARAM *CMD_Ptr)
{
  
  COMMAND_RESULT CmdResult;     //�������
  (void)CmdResult;
  
  //��Ѱ����
  AT_SearchingCommand(CMD_Ptr); 
  
  
   if (CMD_Ptr->CmdFlag)
  {
    switch (CMD_Ptr->InternalId)
    {
      case CMD_ID_COM1: 
      {
        CmdResult = DebugCommand_Handler(CMD_Ptr); 
        break;
      }
      
      case CMD_ID_COM2:
      {
        
        break;
      }
      
      case CMD_ID_COM3:
      {
        
        break;
      }
      
      case CMD_ID_COM4:
      {
        
        break;
      }
      default: break;
      
    }
    
    //��������
    AT_EndProcess(CMD_Ptr);
  }
  
}
  
