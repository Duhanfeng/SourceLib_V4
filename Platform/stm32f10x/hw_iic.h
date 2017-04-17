#ifndef  __HW_IIC_H
#define  __HW_IIC_H

//����IICͨ��
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

  void    IICx_Init       (IIC_PORT Port);   //IICx��ʼ��
  void    IICx_Start      (IIC_PORT Port);   //IICx��ʼ��־
  void    IICx_Stop       (IIC_PORT Port);   //IICx������־
  void    IICx_GiveAck    (IIC_PORT Port);   //IICx��Ӧ���ź�
  void    IICx_GiveNoAck  (IIC_PORT Port);   //IICx����Ӧ���ź�
  void    IICx_WaitAck    (IIC_PORT Port);   //IICx�ȴ�Ӧ���ź�
  void    IICx_SendByte   (IIC_PORT Port, unsigned char cSendData);  //IICx���ֽڷ���
  unsigned char IICx_RecvByte   (IIC_PORT Port);   //IICx���ֽڽ���
  
#ifdef __cplusplus
}
#endif




#endif /* __IIC_H */
