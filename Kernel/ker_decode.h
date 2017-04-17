#ifndef  __KER_DECODE_H
#define  __KER_DECODE_H


#define FIFO_BUFF_SIZE (512)   /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

#if FIFO_BUFF_SIZE < 2
#error FIFO_BUFF_SIZE is too small.  It must be larger than 1.
#elif ((FIFO_BUFF_SIZE & (FIFO_BUFF_SIZE-1)) != 0)
#error FIFO_BUFF_SIZE must be a power of 2.
#endif

#define MAX_PARAM_COUNT   (16)   //�������Ϊ8��
#define MAX_PARAM_BYTE    (0x5)   //�������Ϊ4λ��(��Ҫ֧��5λ���ϵ��������޸�Դ��)


/* ����FIFO�ṹ�� */
typedef struct
{
  unsigned char   CmdBuff[FIFO_BUFF_SIZE];  //������������ݻ���(���λ������Զ���)
  unsigned int    HeadIndex;        //ָ�������ַ������ַ���ƫ��ֵ
  unsigned int    TailIndex;        //ָ�������ַ����Ľ�����ƫ��ֵ
  unsigned int    StoreIndex;       //ָ����һ���洢��λ�õ�ƫ��ֵ
  unsigned int    Len;              //���鳤��
  unsigned char   CmdFlag;          //�����־,��Ϊ1ʱ��ʾ���Դ�������
  unsigned char   InternalId;       //�ڲ���ݱ�־,Ϊ��ͬ�Ľṹ�����
  
}FIFO_PARAM_TYPE;


/* ������������ṹ�� */
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
