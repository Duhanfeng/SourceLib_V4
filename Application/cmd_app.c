/**
  ******************************************************************************
  * @file    cmd_app.c
  * @author  杜公子寒枫
  * @version V1.0
  * @date    2016.08.28
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 命令解析/应用文件
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

static const uint8_t CMD_RESET[]        =   "RESET";        //复位



//调试指令
COMMAND_RESULT DebugCommand_Handler(CMD_PARAM *CMD_Ptr)
{
  AT_PARAM_TPYE AT_Parameter = {0};   //参数结构体
  COMMAND_RESULT CmdResult;     //命令处理结果
  
  /* 命令匹配.. */

  // 复位
  if (AT_Check_xCmdy_Command(CMD_Ptr, CMD_RESET, &AT_Parameter))
  {
    
    //参数数量判断
    if ((AT_Parameter.InputCount != 0) || (AT_Parameter.OutputCount != 0))
    {
      CmdResult = NumberError;
    }
    
    //命令处理
    else
    {
      SCB->AIRCR = ((uint32_t)0x05FA0004);  //复位命令
      CmdResult = Success;
    }
    
  }
  else;

  return CmdResult;
}

  

//命令处理(放在main函数中轮询)
void AT_CmdProcess(CMD_PARAM *CMD_Ptr)
{
  
  COMMAND_RESULT CmdResult;     //命令处理结果
  (void)CmdResult;
  
  //搜寻命令
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
    
    //结束处理
    AT_EndProcess(CMD_Ptr);
  }
  
}
  
