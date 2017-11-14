/**
  ******************************************************************************
  * @file    fifo.c
  * @author  杜公子寒枫
  * @version V1.0 
  * @date    2017.05.04
  * @brief   FIFO的应用
  ******************************************************************************
  * @attention
  * 
  * 本文件依赖于"SourceLib.h"的SL_ASSERT(x),以及"ker_typedef.h"的MIN(x,y)
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"
#include "SourceLib.h"



//get fifo buffer lenght that unused
//获取未使用的FIFO缓冲区长度
unsigned int fifo_unused (struct __fifo *fifo)
{
  SL_ASSERT(fifo);

  return (fifo->len) - (fifo->in - fifo->out);
}


//get fifo buffer lenght that has been used
//获取已使用的FIFO长度
unsigned int fifo_date_len(struct __fifo *fifo)
{
  SL_ASSERT(fifo);

//  return (fifo->in - fifo->out)&(fifo->len-1);
  return (fifo->in - fifo->out);
}


//alloc fifo
//开辟fifp
struct __fifo * fifo_alloc(unsigned int size, unsigned char esize)
{
  struct __fifo *fifo = NULL; 

  //size must be greater than 2 and power of 2
  SL_ASSERT(size >= 2);
  SL_ASSERT((size & (size-1)) == 0);

  fifo = (struct __fifo *)malloc(sizeof(struct __fifo) + size * esize);

  memset(fifo, 0, sizeof(struct __fifo) + size * esize);
  
  //if alloc success, initialization
  if (fifo != NULL)
  {
    fifo->len = size;
    fifo->esize = esize;
    fifo->in = 0;
    fifo->out = 0;
  }

  return fifo;
}


//free fifo
void fifo_free(struct __fifo *fifo)
{
  SL_ASSERT(fifo);

  free(fifo);

}


static void fifo_copy_in(struct __fifo *fifo, const unsigned char *src,
    unsigned int len, unsigned int off)
{
  unsigned int size = fifo->len;
  unsigned int esize = fifo->esize;
  unsigned int l = 0;

  SL_ASSERT(fifo);

  off &= (fifo->len-1);
  if (esize != 1) 
  {
    off *= esize;
    size *= esize;
    len *= esize;
  }
  l = MIN(len, size - off);

  memcpy(fifo->data + off, src, l);
  memcpy(fifo->data, src + l, len - l);

}


unsigned int fifo_in(struct __fifo *fifo, const void *buf, unsigned int len)
{
  unsigned int l = 0;

  SL_ASSERT(fifo);
  
  l = fifo_unused(fifo);
  
  #if 0
  if (len > l)
    len = l;
  #endif
  len = MIN(len, l);

  fifo_copy_in(fifo, (const unsigned char *)buf, len, fifo->in);
  fifo->in += len;

  return len;
}


void fifo_copy_out(struct __fifo *fifo, unsigned char *dst,
    unsigned int len, unsigned int off)
{
  unsigned int size = fifo->len;
  unsigned int esize = fifo->esize;
  unsigned int l = 0;

  SL_ASSERT(fifo);

  off &= (fifo->len-1);
  if (esize != 1) 
  {
    off *= esize;
    size *= esize;
    len *= esize;
  }
  l = MIN(len, size - off);

  memcpy(dst, fifo->data + off, l);
  memcpy(dst + l, fifo->data, len - l);

}


unsigned int fifo_out_peek(struct __fifo *fifo,
    void *buf, unsigned int len)
{
  unsigned int l = 0;

  SL_ASSERT(fifo);

  l = fifo->in - fifo->out;

#if 0
  if (len > l)
    len = l;
#endif
  len = MIN(len, l);

  fifo_copy_out(fifo, (unsigned char *)buf, len, fifo->out);

  return len;
}


//提取len个长度的数据
unsigned int fifo_out(struct __fifo *fifo,
    void *buf, unsigned int len)
{
  SL_ASSERT(fifo);

  len = fifo_out_peek(fifo, buf, len);
  fifo->out += len;

  return len;
}


//复位fifo
void fifo_reset(struct __fifo *fifo)
{
  SL_ASSERT(fifo);

  fifo->in = 0;
  fifo->out = 0;

}

//将out跳过len长度的缓冲区
void fifo_skip(struct __fifo *fifo, unsigned int len)
{
  SL_ASSERT(fifo);

  fifo->out+=len;

}


//将out跳到in处
void fifo_skip_all(struct __fifo *fifo)
{
//  unsigned int l = 0;

  SL_ASSERT(fifo);

#if 0
  l = fifo->in - fifo->out;
  fifo_skip(fifo, l);
#endif

  fifo->out = fifo->in;

}


unsigned int fifo_scan(struct __fifo *fifo, void *buf, unsigned int len)
{
    unsigned int l = 0;
    unsigned int tmp = 0;
    tmp = len;
    l = fifo->in - fifo->out;

    if (len > l) 
  {
    return 0;
  }

    fifo_copy_out(fifo, (unsigned char *)buf, len, fifo->out);

    return (len==tmp);
}


unsigned int fifo_search(struct __fifo *fifo, unsigned char data)
{
  unsigned int i = 0;
//  unsigned int search = 0;
  unsigned int len = fifo_date_len(fifo);
  unsigned char *buff = NULL;
  unsigned char result = 0;

  do
  {
    if (!len) break;
    
    //动态申请一个空间用于存储当前缓冲区的数据,如果申请失败则退出
    if ((buff = (unsigned char *)malloc(len * sizeof(unsigned char)))==NULL) break;

    //将当前的fifo数据提取出来,如果提取失败则退出
    if (!fifo_scan(fifo, buff, len)) break;

    //比较数据
    for (i = 0; i < len; i++)
    {
      if (data == buff[i])
      {
        fifo->search = fifo->out + i;
        break;
      }
    }

    //如果i==len,表明没有搜寻到
    if (i == len) break;
    
    result = 1;
  }while(0);

  if (result == 0)
  {
    fifo->search = fifo->out;
  }
  
  free(buff);

  return result;
}

