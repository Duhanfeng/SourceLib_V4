#ifndef _FIFO_H
#define _FIFO_H

//#include "type.h"


typedef struct __fifo {
    unsigned int    in;     //入缓冲指针
    unsigned int    out;    //出缓冲指针
  unsigned int  search; //搜寻指针,可以用于搜寻in~out之间的数据
    unsigned int    len;    //缓冲区长度
//  unsigned int  mask;   //缓冲区掩码
    unsigned int    esize;  //单个成员的字节长度
  void         *priv;   //预留
  unsigned char data[]; //数据缓冲区(零长数组)

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
  unsigned int fifo_search(struct __fifo *fifo, unsigned char data);
  
#ifdef __cplusplus
}
#endif


#endif
