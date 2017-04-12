/**
  ******************************************************************************
  * @file    ker_decode.c
  * @author  杜公子寒枫
  * @version V4.0
  * @date    2017.03.03
  * @brief   指令解码算法
  ******************************************************************************
  * @attention
  * 
  * V2.0------------
  * 修改描述: 1. 增加16进制参数的兼容
  *           2. 修改函数框架, 删除冗余的执行流程
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.11.03
  * 
  * V4.0------------
  * 修改描述: 1. 修改底层实现机制,将字符串到数值的转换放在ascii.c的里面
  *           2. 优化算法,删除冗余代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.11.03
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "ker_decode.h"
#include "algorithm.h"
#include <string.h>

#define ADDR_MASK     (FIFO_BUFF_SIZE - 1)

typedef enum
{
	EndIndicator1     = '.', //命令结束指示符
	EndIndicator2     = '!', //命令结束指示符
	separateIndicator = ',', //参数分隔指示符

}AtIndicator_t;  



//解析命令
unsigned char SLDecode_Check_xCmdy_Command(FIFO_PARAM_TYPE *FifoParamPtr, const unsigned char *pCmd, DECODE_PARAM_TPYE *AT_Parameter)
{
  unsigned char       i               =  0;
  unsigned char       cLen            =  0;
  unsigned int        CmdHeadIndex    =  0;     //命令首字符的偏移
  unsigned int        ParamYIndex     =  0;     //参数Y首字符的偏移
  
  unsigned int        nHeadIndex      =  FifoParamPtr->HeadIndex;  //环形缓冲区尾指针索引
  unsigned int        nTailIndex      =  FifoParamPtr->TailIndex;  //环形缓冲区首指针索引
  unsigned char       *pBuff          =  FifoParamPtr->CmdBuff;   //指向缓冲区
  

  unsigned char TempBuff[32] = {0};
  unsigned int  InputArray[16] = {0};
  unsigned short int InputNumber = 0;

  unsigned int  OutputArray[16] = {0};
  unsigned short int OutputNumber = 0; 

  cLen = strlen((char *)pCmd);

  /*----------------------------------------------------------------------------
      校验命令的有效性,并获取命令首字符,Y参数首字符的位置
   *----------------------------------------------------------------------------*/

  /* 指针偏移到command处 */
  while (((pBuff[nHeadIndex & ADDR_MASK] >= '0') && 
        (pBuff[nHeadIndex & ADDR_MASK] <= '9')) || (pBuff[nHeadIndex & ADDR_MASK] == separateIndicator))
  {
    if ((pBuff[nHeadIndex & ADDR_MASK] == '0') && 
      ((pBuff[(nHeadIndex+1) & ADDR_MASK] == 'x') || (pBuff[(nHeadIndex+1) & ADDR_MASK] == 'X')))
    {
      nHeadIndex+=3;
    }
    nHeadIndex++;
    
  }

  CmdHeadIndex = nHeadIndex;  //存储缓冲区中命令字符的首字符地址
  
  /* 检查缓冲区的命令是否与pCmd[]中的一致 */
  for (i= 0; i < cLen; i++)
  {
    if (pBuff[nHeadIndex & ADDR_MASK] != pCmd[i])
    {
      return 0;
    }
    nHeadIndex++;
  }
  
  ParamYIndex = nHeadIndex;  //存储参数Y的首字符地址

  /*----------------------------------------------------------------------------
      提取参数X
   *----------------------------------------------------------------------------*/
  nHeadIndex = FifoParamPtr->HeadIndex;  //回到首字符处

  //假如没有经过原点
  if ((nHeadIndex & ADDR_MASK) < (CmdHeadIndex & ADDR_MASK))
  {
    memcpy(TempBuff, &pBuff[nHeadIndex & ADDR_MASK], (CmdHeadIndex & ADDR_MASK) - (nHeadIndex & ADDR_MASK));
  }
  else 
  {
    unsigned short int TempNumber = FIFO_BUFF_SIZE - (nHeadIndex & ADDR_MASK);
    memcpy(TempBuff, &pBuff[nHeadIndex & ADDR_MASK], TempNumber);
    memcpy(&TempBuff[TempNumber], pBuff, (CmdHeadIndex & ADDR_MASK));
  }
  
  InputNumber = Ascii_StringToArray(TempBuff, InputArray);

  /*----------------------------------------------------------------------------
      提取参数Y
   *----------------------------------------------------------------------------*/
  nHeadIndex = ParamYIndex;
  memset(TempBuff, 0, 32);

  //假如没有经过原点
  if ((nHeadIndex & ADDR_MASK) < (nTailIndex & ADDR_MASK))
  {
    memcpy(TempBuff, &pBuff[nHeadIndex & ADDR_MASK], (nTailIndex & ADDR_MASK) - (nHeadIndex & ADDR_MASK));
  }
  else 
  {
    unsigned short int TempNumber = FIFO_BUFF_SIZE - (nHeadIndex & ADDR_MASK);
    memcpy(TempBuff, &pBuff[nHeadIndex & ADDR_MASK], TempNumber);
    memcpy(&TempBuff[TempNumber], pBuff, (nTailIndex & ADDR_MASK));
  }

  OutputNumber = Ascii_StringToArray(TempBuff, OutputArray);
  
  /*----------------------------------------------------------------------------
      数据出参
   *----------------------------------------------------------------------------*/
  AT_Parameter->InputCount = InputNumber;
  
  for (i = 0 ; i < InputNumber; i++)
  {
    AT_Parameter->InputParam[i] = InputArray[i];
  }
  
  AT_Parameter->OutputCount = OutputNumber;
  
  for (i = 0 ; i < OutputNumber; i++)
  {
    AT_Parameter->OutputParam[i] = OutputArray[i];
  }
  
  return 1;
}



//搜寻命令(将Tail定位在结束符'.'中)
void SLDecode_SearchingCommand(FIFO_PARAM_TYPE *FifoParamPtr)
{
  while (FifoParamPtr->TailIndex != FifoParamPtr->StoreIndex)
  {
    if ((FifoParamPtr->CmdBuff[FifoParamPtr->TailIndex & ADDR_MASK] == EndIndicator1) || 
        (FifoParamPtr->CmdBuff[FifoParamPtr->TailIndex & ADDR_MASK] == EndIndicator2))
    {
      FifoParamPtr->CmdFlag++;
      return;
    }
    FifoParamPtr->TailIndex++;
  }
  
}


//往环形缓冲区中存储接收到的命令
void SLDecode_StoreCommand(FIFO_PARAM_TYPE *FifoParamPtr, unsigned char *pBuff, unsigned short int nSize)
{
  unsigned short int i = 0;
  
  for (i = 0; i < nSize; i++)
  {
    FifoParamPtr->CmdBuff[(FifoParamPtr->StoreIndex++) & ADDR_MASK] = pBuff[i];
  }
  
}


//往环形缓冲区中读取接收到命令(HeadIndex到TailIndex的数据)
void SLDecode_ReadCommand(FIFO_PARAM_TYPE *FifoParamPtr, unsigned char *pBuff, unsigned short int *pSize)
{
  unsigned short int i = 0;
  unsigned short int nLen = FifoParamPtr->TailIndex - FifoParamPtr->HeadIndex + 1;
  
  for (i = 0; i < nLen; i++)
  {
    pBuff[i] = FifoParamPtr->CmdBuff[(FifoParamPtr->HeadIndex + i) & ADDR_MASK];
  }
  
  *pSize = nLen;
  
}


//结束处理
//将HeadIndex和TailIndex都定位在下一条命令的首字符处
//在执行完"CmdProcess"后,应调用此函数,作指针的偏移
void SLDecode_EndProcess(FIFO_PARAM_TYPE *FifoParamPtr)
{
  FifoParamPtr->HeadIndex = ++FifoParamPtr->TailIndex;
  FifoParamPtr->CmdFlag = 0;
}


