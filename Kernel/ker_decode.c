/**
  ******************************************************************************
  * @file    ker_decode.c
  * @author  �Ź��Ӻ���
  * @version V4.0
  * @date    2017.03.03
  * @brief   ָ������㷨
  ******************************************************************************
  * @attention
  * 
  * V2.0------------
  * �޸�����: 1. ����16���Ʋ����ļ���
  *           2. �޸ĺ������, ɾ�������ִ������
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.11.03
  * 
  * V4.0------------
  * �޸�����: 1. �޸ĵײ�ʵ�ֻ���,���ַ�������ֵ��ת������ascii.c������
  *           2. �Ż��㷨,ɾ���������
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V2.0
  * �޸�����: 2016.11.03
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
	EndIndicator1     = '.', //�������ָʾ��
	EndIndicator2     = '!', //�������ָʾ��
	separateIndicator = ',', //�����ָ�ָʾ��

}AtIndicator_t;  



//��������
unsigned char SLDecode_Check_xCmdy_Command(FIFO_PARAM_TYPE *FifoParamPtr, const unsigned char *pCmd, DECODE_PARAM_TPYE *AT_Parameter)
{
  unsigned char       i               =  0;
  unsigned char       cLen            =  0;
  unsigned int        CmdHeadIndex    =  0;     //�������ַ���ƫ��
  unsigned int        ParamYIndex     =  0;     //����Y���ַ���ƫ��
  
  unsigned int        nHeadIndex      =  FifoParamPtr->HeadIndex;  //���λ�����βָ������
  unsigned int        nTailIndex      =  FifoParamPtr->TailIndex;  //���λ�������ָ������
  unsigned char       *pBuff          =  FifoParamPtr->CmdBuff;   //ָ�򻺳���
  

  unsigned char TempBuff[32] = {0};
  unsigned int  InputArray[16] = {0};
  unsigned short int InputNumber = 0;

  unsigned int  OutputArray[16] = {0};
  unsigned short int OutputNumber = 0; 

  cLen = strlen((char *)pCmd);

  /*----------------------------------------------------------------------------
      У���������Ч��,����ȡ�������ַ�,Y�������ַ���λ��
   *----------------------------------------------------------------------------*/

  /* ָ��ƫ�Ƶ�command�� */
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

  CmdHeadIndex = nHeadIndex;  //�洢�������������ַ������ַ���ַ
  
  /* ��黺�����������Ƿ���pCmd[]�е�һ�� */
  for (i= 0; i < cLen; i++)
  {
    if (pBuff[nHeadIndex & ADDR_MASK] != pCmd[i])
    {
      return 0;
    }
    nHeadIndex++;
  }
  
  ParamYIndex = nHeadIndex;  //�洢����Y�����ַ���ַ

  /*----------------------------------------------------------------------------
      ��ȡ����X
   *----------------------------------------------------------------------------*/
  nHeadIndex = FifoParamPtr->HeadIndex;  //�ص����ַ���

  //����û�о���ԭ��
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
      ��ȡ����Y
   *----------------------------------------------------------------------------*/
  nHeadIndex = ParamYIndex;
  memset(TempBuff, 0, 32);

  //����û�о���ԭ��
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
      ���ݳ���
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



//��Ѱ����(��Tail��λ�ڽ�����'.'��)
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


//�����λ������д洢���յ�������
void SLDecode_StoreCommand(FIFO_PARAM_TYPE *FifoParamPtr, unsigned char *pBuff, unsigned short int nSize)
{
  unsigned short int i = 0;
  
  for (i = 0; i < nSize; i++)
  {
    FifoParamPtr->CmdBuff[(FifoParamPtr->StoreIndex++) & ADDR_MASK] = pBuff[i];
  }
  
}


//�����λ������ж�ȡ���յ�����(HeadIndex��TailIndex������)
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


//��������
//��HeadIndex��TailIndex����λ����һ����������ַ���
//��ִ����"CmdProcess"��,Ӧ���ô˺���,��ָ���ƫ��
void SLDecode_EndProcess(FIFO_PARAM_TYPE *FifoParamPtr)
{
  FifoParamPtr->HeadIndex = ++FifoParamPtr->TailIndex;
  FifoParamPtr->CmdFlag = 0;
}


