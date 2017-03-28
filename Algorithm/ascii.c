/**
  ******************************************************************************
  * @file    ascii.c
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
  *             
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.11.03
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include <string.h>
#include <stdio.h>
#include "algorithm.h"

#if USING_ASCII

#define SUPPORT_NONE_PARAM  (1)
#define MAX_PARAM_NUMBER  (16)

typedef enum
{
  SeparateChar1 = ',',
  SeparateChar2 = ':',
  SeparateChar3 = '\'',

}ASCII_SEPARATE_CHAR;


//字符数转数字
//'7'-->7, 'C'-->12
//return: 0xFF: invalid value 
//        other:  valid value
unsigned char Ascii_CharToDec(unsigned char Char)
{
  unsigned char ResultValue = 0xFF;

  if ((Char >= 'A') && (Char <= 'F'))
  {
     ResultValue = Char - 'A' + 10;
  }
  else if ((Char >= 'a') && (Char <= 'f'))
  {
    ResultValue = Char - 'a' + 10;
  }
  else if ((Char >= '0') && (Char <= '9'))
  {
    ResultValue = Char - '0';
  }

  return ResultValue;
}


//字符串数字转数字
//"0x7C" --> 0x7C
//return:  1:   valid
//         0: invalid
unsigned int Ascii_StringToNum(unsigned char *pParamBuff, unsigned int *pValue)
{
  unsigned char isHex = 0;
  unsigned char *BuffPtr = pParamBuff;
  unsigned char cParamByte = strlen((char *)pParamBuff);
  unsigned char i = 0;
  unsigned int  iValue = 0;
  unsigned char cTemp = 0;
  
#if !SUPPORT_NONE_PARAM

  if (cParamByte == 0)
  {
    return 0;
  }
#endif

  if ((BuffPtr[0] == '0') && ((BuffPtr[1] == 'x')||(BuffPtr[1] == 'X')))
  {
    isHex = 1;
    BuffPtr += 2;
    cParamByte -= 2;
  }
  
  for (i = 0; i < cParamByte; i++)
  {
    iValue *= (isHex?16:10);

    cTemp = Ascii_CharToDec(BuffPtr[i]);

    if (cTemp != 0xFF)
    {
      iValue += cTemp;
    }
    else 
    {
      return 0;
    }
  }
  
  *pValue = iValue;
  return 1;
}




//将一段字符串中提取出其中的数据(以数组的形式出参)
//"1,2,,0xaA,0x1C"-->0x01,0x02,0x00,0xAA,0x1C
//'' is 0x00 or invalid value, Determine by SUPPORT_NONE_PARAM value
//return:  other:   valid parameter number
//             0: invalid
unsigned short int Ascii_StringToArray(unsigned char *pStringBuff, unsigned int *pArray)
{
  unsigned char cStringByte = 0;
  unsigned char i = 0;
  unsigned char TempBuff[16] = {0};
  unsigned char Index = 0;
  unsigned short int Number = 0;
  unsigned int  TempArray[MAX_PARAM_NUMBER] = {0};

  if (pArray == NULL)
  {
    return 0;
  }

  cStringByte = strlen((char *)pStringBuff);

  for (Index = 0; Index < cStringByte+1; Index++)
  {
    if ((pStringBuff[Index] != SeparateChar1) && (pStringBuff[Index] != SeparateChar2) 
      && (pStringBuff[Index] != SeparateChar3) && (Index != cStringByte))
    {
      TempBuff[i++] = pStringBuff[Index];
    }
    else 
    {
      i = 0;
      
      if (Number < MAX_PARAM_NUMBER)
      {
        if (!Ascii_StringToNum(TempBuff, &TempArray[Number++]))
        {
          return 0;
        }
      }

      memset(TempBuff, 0, MAX_PARAM_NUMBER);
    }
  }

  memcpy(pArray, TempArray, Number*sizeof(unsigned int));

  return Number;
}


#endif /* USING_ASCII */
