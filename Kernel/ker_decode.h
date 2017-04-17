#ifndef  __KER_DECODE_H
#define  __KER_DECODE_H


#define FIFO_BUFF_SIZE (512)   /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

#if FIFO_BUFF_SIZE < 2
#error FIFO_BUFF_SIZE is too small.  It must be larger than 1.
#elif ((FIFO_BUFF_SIZE & (FIFO_BUFF_SIZE-1)) != 0)
#error FIFO_BUFF_SIZE must be a power of 2.
#endif

#define MAX_PARAM_COUNT   (16)   //参数最多为8个
#define MAX_PARAM_BYTE    (0x5)   //参数最大为4位数(若要支持5位以上的需自行修改源码)


/* 定义FIFO结构体 */
typedef struct
{
  unsigned char   CmdBuff[FIFO_BUFF_SIZE];  //包含命令的数据缓冲(环形或者线性队列)
  unsigned int    HeadIndex;        //指向命令字符串首字符的偏移值
  unsigned int    TailIndex;        //指向命令字符串的结束的偏移值
  unsigned int    StoreIndex;       //指向下一个存储的位置的偏移值
  unsigned int    Len;              //数组长度
  unsigned char   CmdFlag;          //命令标志,当为1时表示可以处理命令
  unsigned char   InternalId;       //内部身份标志,为不同的结构体变量
  
}FIFO_PARAM_TYPE;


/* 定义命令参数结构体 */
typedef struct
{
  unsigned int InputParam [MAX_PARAM_COUNT];
  unsigned short int InputCount;
  
  unsigned int OutputParam[MAX_PARAM_COUNT];
  unsigned short int OutputCount;
  
}DECODE_PARAM_TPYE;


#ifdef __cplusplus
extern "C" {
#endif
  
  unsigned char SLDecode_Check_xCmdy_Command(FIFO_PARAM_TYPE *FifoParamPtr, const unsigned char *pCmd, DECODE_PARAM_TPYE *AT_Parameter);
  void SLDecode_SearchingCommand(FIFO_PARAM_TYPE *FifoParamPtr);
  void SLDecode_StoreCommand(FIFO_PARAM_TYPE *FifoParamPtr, unsigned char *pBuff, unsigned short int nSize);
  void SLDecode_ReadCommand (FIFO_PARAM_TYPE *FifoParamPtr, unsigned char *pBuff, unsigned short int *pSize);
  void SLDecode_ReadCommand (FIFO_PARAM_TYPE *FifoParamPtr, unsigned char *pBuff, unsigned short int *pSize);
  void SLDecode_EndProcess  (FIFO_PARAM_TYPE *FifoParamPtr);

#ifdef __cplusplus
}
#endif


#endif /* __KER_DECODE_H */
