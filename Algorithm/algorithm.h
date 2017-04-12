#ifndef  __ALGORITHM_H
#define  __ALGORITHM_H


/*----------------------------------------------------------------------------
    功能裁剪
 *----------------------------------------------------------------------------*/
#define USING_ASCII     (1)   //使用/禁止ASCII算法
#define USING_CRC8      (0)   //使用/禁止CRC8
#define USING_CRC16     (1)   //使用/禁止CRC16

  
  
#ifdef __cplusplus
extern "C" {
#endif
  
  /* ASCII */
  unsigned char Ascii_CharToDec(unsigned char Char);
  unsigned int Ascii_StringToNum(unsigned char *pParamBuff, unsigned int *pValue);
  unsigned short int Ascii_StringToArray(unsigned char *pStringBuff, unsigned int *pArray);
  
  /* CRC8 */
  unsigned char GetCRC8(unsigned char *pBuf, unsigned long int iCount);
  
  /* CRC16 */
  unsigned short int GetCRC16(unsigned char *pBuf, unsigned long int iCount);

#ifdef __cplusplus
}
#endif


#endif /* __ALGORITHM_H */
