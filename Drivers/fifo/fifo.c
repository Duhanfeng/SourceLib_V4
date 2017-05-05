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
unsigned int fifo_unused (struct __fifo *fifo)
{
  SL_ASSERT(fifo);

  return (fifo->len) - (fifo->in - fifo->out);
}


//get fifo buffer lenght that has been used
unsigned int fifo_date_len(struct __fifo *fifo)
{
  SL_ASSERT(fifo);

  return (fifo->in - fifo->out)&(fifo->mask);
}


//alloc fifo
struct __fifo * fifo_alloc(unsigned int size, unsigned char esize)
{
  struct __fifo *fifo = NULL; 

  //size must be greater than 2 and power of 2
  SL_ASSERT(size >= 2);
  SL_ASSERT((size & (size-1)) == 0);

  fifo = (struct __fifo *)malloc(sizeof(struct __fifo) + size * esize);
  
  //if alloc success, initialization
  if (fifo != NULL)
  {
    fifo->len = size;
    fifo->mask = size - 1;
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
  unsigned int size = fifo->mask;
  unsigned int esize = fifo->esize;
  unsigned int l = 0;

  SL_ASSERT(fifo);

  off &= fifo->mask;
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

  fifo_copy_in(fifo, buf, len, fifo->in);
  fifo->in += len;

  return len;
}


static void fifo_copy_out(struct __fifo *fifo, unsigned char *dst,
    unsigned int len, unsigned int off)
{
  unsigned int size = fifo->mask + 1;
  unsigned int esize = fifo->esize;
  unsigned int l = 0;

  SL_ASSERT(fifo);

  off &= fifo->mask;
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


static unsigned int fifo_out_peek(struct __fifo *fifo,
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

  fifo_copy_out(fifo, buf, len, fifo->out);

  return len;
}


unsigned int fifo_out(struct __fifo *fifo,
    void *buf, unsigned int len)
{
  SL_ASSERT(fifo);

  len = fifo_out_peek(fifo, buf, len);
  fifo->out += len;

  return len;
}


void fifo_reset(struct __fifo *fifo)
{
  SL_ASSERT(fifo);

  fifo->in = 0;
  fifo->out = 0;

}


void fifo_skip(struct __fifo *fifo,unsigned int len)
{
  SL_ASSERT(fifo);

  fifo->out+=len;

}


void fifo_skip_all(struct __fifo *fifo)
{
  unsigned int l = 0;

  SL_ASSERT(fifo);

  l = fifo->in - fifo->out;
  fifo_skip(fifo, l);

}


unsigned int fifo_scan(struct __fifo *fifo,void *buf, unsigned int len)
{
	unsigned int l = 0;
	unsigned int tmp = 0;
	tmp = len;
	l = fifo->in - fifo->out;

	if (len > l) 
  {
    return 0;
  }

	fifo_copy_out(fifo, buf, len, fifo->out);

	return (len==tmp);
}

