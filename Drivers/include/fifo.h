#ifndef _FIFO_H
#define _FIFO_H

//#include "type.h"


typedef struct __fifo {
	unsigned int	in;     
	unsigned int	out;    
	unsigned int	len;    //����������
  unsigned int  mask;   //����������
	unsigned int	esize;  //������Ա���ֽڳ���
  unsigned char	  data[]; //���ݻ�����(�㳤����)

}SL_FIFO;

struct __fifo * fifo_alloc(unsigned int size, unsigned char esize);


#ifdef __cplusplus
extern "C" {
#endif

  unsigned int fifo_date_len(struct __fifo *fifo);
  void fifo_skip_all(struct __fifo *fifo);
  unsigned int fifo_unused(struct __fifo *fifo);
  struct __fifo * fifo_alloc(unsigned int size, unsigned char esize);
  void fifo_free(struct __fifo *fifo);
  unsigned int fifo_in(struct __fifo *fifo,const void *buf, unsigned int len);
  unsigned int __fifo_out_peek(struct __fifo *fifo,void *buf, unsigned int len);
  unsigned int fifo_out(struct __fifo *fifo,void *buf, unsigned int len);
  void fifo_reset(struct __fifo *fifo);
  void fifo_skip(struct __fifo *fifo,unsigned int len);
  unsigned int  fifo_scan(struct __fifo *fifo,void *buf, unsigned int len);
  
#ifdef __cplusplus
}
#endif


#endif
