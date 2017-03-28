#ifndef  __HW_IIC_H
#define  __HW_IIC_H

//定义IIC通道
typedef enum
{
  IIC_PORT1 = 0,
  IIC_PORT2,
  IIC_PORT3,
  IIC_PORT4,
  IIC_PORT5,
  IIC_PORT6,
  
}IIC_PORT;



#ifdef __cplusplus
extern "C" {
#endif

  void    IICx_Init       (IIC_PORT Port);   //IICx初始化
  void    IICx_Start      (IIC_PORT Port);   //IICx开始标志
  void    IICx_Stop       (IIC_PORT Port);   //IICx结束标志
  void    IICx_GiveAck    (IIC_PORT Port);   //IICx给应答信号
  void    IICx_GiveNoAck  (IIC_PORT Port);   //IICx给不应答信号
  void    IICx_WaitAck    (IIC_PORT Port);   //IICx等待应答信号
  void    IICx_SendByte   (IIC_PORT Port, unsigned char cSendData);  //IICx单字节发送
  unsigned char IICx_RecvByte   (IIC_PORT Port);   //IICx单字节接收
  
#ifdef __cplusplus
}
#endif




#endif /* __IIC_H */
