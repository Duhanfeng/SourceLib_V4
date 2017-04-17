/**
  ******************************************************************************
  * @file    i2c_bit_ops.c
  * @author  �Ź��Ӻ���
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C���:λ����ʵ���ļ�
  ******************************************************************************
  * @attention
  * 
  * ����: SL_I2C_BIT_OPS_TYPE
  * 
  * typedef struct sl_i2c_bit_ops
  * {
  *   void    (*Data);    //˽������ 
  *   void    (*SetSDA)(void *Data, uint8_t State);
  *   void    (*SetSCL)(void *Data, uint8_t State);
  *   uint8_t (*GetSDA)(void *Data);
  *   uint8_t (*GetSCL)(void *Data);
  * 
  *   void    (*BitDelayUs)(uint16_t Us);
  *   
  *   uint16_t DelayTime; //��λ: ΢�� 
  *   uint32_t iTimeOut;  //��λ: ����
  *   
  * }SL_I2C_BIT_OPS_TYPE;
  * 
  * ���ļ���Ҫ��ͨ��BIT_OPS��ʵ�ֻ���IICʱ��: 
  * I2C_Start   
  * I2C_Stop    
  * I2C_SendAckOrNack 
  * I2C_WaitAck  
  * I2C_SendByte 
  * I2C_RecvByte 
  * 
  * �����ʱ��ӿں����ɷ�װ�ɽṹ��SL_I2C_BUS_BASE_OPS_TYPE,������I2C�����豸�ṹ��
  * ��������.
  * 
  * 
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "SourceLib.h"
#include "i2c.h"

#if USING_I2C

#define SET_SDA(Ops, Value)   Ops->SetSDA(Ops->Data, Value)
#define SET_SCL(Ops, Value)   Ops->SetSCL(Ops->Data, Value)
#define GET_SDA(Ops)          Ops->GetSDA(Ops->Data)
#define GET_SCL(Ops)          Ops->GetSCL(Ops->Data)
  
#define SDA_L(Ops)            SET_SDA(Ops, 0)
#define SDA_H(Ops)            SET_SDA(Ops, 1)
#define SCL_L(Ops)            SET_SCL(Ops, 0)
#define SCL_H(Ops)            SET_SCL(Ops, 1)


Sl_inline void I2C_DelayUs(SL_I2C_BIT_OPS_TYPE *Ops)
{
    Ops->BitDelayUs(Ops->DelayTime);
  
}

/*----------------------------------------------------------------------------
    �ײ��������,��ʵ���������I2Cʱ���߼�,ֱ�Ӷ�IO���п���
    ������Ķ����� SL_I2C_BIT_OPS_TYPE 
 *----------------------------------------------------------------------------*/

/**
  * @brief  I2C��ʼ��־(��SCL�ߵ�ƽʱ����SDA)
  * @param  ��
  * @retval ��
  */
static void I2C_Bit_Start(SL_I2C_BIT_OPS_TYPE *Ops)
{
  SDA_H(Ops);
  SCL_H(Ops);
  I2C_DelayUs(Ops);
  SDA_L(Ops);
  I2C_DelayUs(Ops);
  SCL_L(Ops);
}



/**
  * @brief  I2C������־(��SCL�ߵ�ƽʱ����SDA)
  * @param  ��
  * @retval ��
  */
static void I2C_Bit_Stop(SL_I2C_BIT_OPS_TYPE *Ops)
{
  SCL_L(Ops);
  SDA_L(Ops);
  
  SCL_H(Ops);
  I2C_DelayUs(Ops);
  SDA_H(Ops);
  I2C_DelayUs(Ops);
}



/**
  * @brief  I2C�ȴ�Ӧ���ź�(��SCLΪ��ʱ�ȴ�Ӧ��,SDA���ΪӦ��)
  * @param  ��
  * @retval isAck �Ƿ���Ӧ���ź�,���򷵻�1,���򷵻�0
  */
static uint8_t I2C_Bit_WaitAck(SL_I2C_BIT_OPS_TYPE *Ops)
{
  uint8_t isAck = 1;
  SLTimerType I2C_Timer = {0};
  
  SDA_H(Ops); //�ͷ�����
  SCL_L(Ops);
  I2C_DelayUs(Ops);
  SCL_H(Ops);
  
  SLTimer_Start(&I2C_Timer, Ops->iTimeOut);  //���ó�ʱʱ��
  while (GET_SDA(Ops)) //�ȴ�SDA���
  {
    if (SLTimer_GetExpiredState(&I2C_Timer))
    {
      isAck = 0;
      break;
    }
    I2C_DelayUs(Ops);
  }
  
  I2C_DelayUs(Ops);
  SCL_L(Ops);
  
  return isAck;
}



/**
  * @brief  I2C��Ӧ��/��Ӧ���ź�(��SCLΪ��ʱSDA�����͵�ƽ��ΪӦ���ź�)
  * @param  ��
  * @retval ��
  */
static void I2C_Bit_SendAckOrNack(SL_I2C_BIT_OPS_TYPE *Ops, uint8_t ack)
{
  SCL_L(Ops);
  
  if (ack) 
  {
    SDA_L(Ops);  //Ӧ���ź�
  }
  else 
  {
    SDA_H(Ops);  //Ӧ���ź�
  }
  
  I2C_DelayUs(Ops);
  
  SCL_H(Ops);
  I2C_DelayUs(Ops);
  
  SCL_L(Ops);
  
}



/**
  * @brief  I2C�ֽڷ��ͺ���(��SCL�͵�ƽʱ����/����SDA)
  * @param  cSendData Ҫ���͵��ֽ�
  * @retval ��
  */
static uint8_t I2C_Bit_SendByte(SL_I2C_BIT_OPS_TYPE *Ops, uint8_t cByte)
{
  uint8_t i = 0;
  
  for (i = 0; i < 8; i++)
  {
    SCL_L(Ops);
    
    if (cByte & (0X1<<(7-i)))  //��������
    {
      SDA_H(Ops);
    }
    else
    {
      SDA_L(Ops);
    }
    
    I2C_DelayUs(Ops);
    
    SCL_H(Ops);
    I2C_DelayUs(Ops);
    
  }
  SCL_L(Ops);
  
  return I2C_Bit_WaitAck(Ops);
}


/**
  * @brief  I2C�ֽڽ��պ���(��SCL�ߵ�ƽʱ��SDA��ƽ״̬)
  * @param  ��
  * @retval cRecvData ���յ����ֽ�
  */
static uint8_t I2C_Bit_RecvByte(SL_I2C_BIT_OPS_TYPE *Ops)
{
  uint8_t i = 0;
  uint8_t cRecvData = 0;

  SDA_H(Ops);   //���Ƕ�ȡ����ǰ,��Ҫ�ͷ�����,����ӻ��޷���������
  
  for (i = 0; i < 8; i++)
  {
    SCL_L(Ops);
    I2C_DelayUs(Ops);
    
    SCL_H(Ops);
    
    if (GET_SDA(Ops))
    {
      cRecvData |=  (0X1<<(7-i));
    }
      
    I2C_DelayUs(Ops);
  }
  SCL_L(Ops);
  
  return cRecvData;
}



//��װ�Ϸ���I2C��������ʱ��Ľӿں���
static SL_I2C_BUS_BASE_OPS_TYPE I2C_BusBaseOps = 
{
  I2C_Bit_Start,
  I2C_Bit_Stop,
  I2C_Bit_WaitAck,
  I2C_Bit_SendAckOrNack,
  I2C_Bit_SendByte,
  I2C_Bit_RecvByte,
  0,
};



//��ȡI2C����ʱ�����Ľӿ�(�����豸�����)
sl_err_t SL_I2C_Bit_GetBitOpsInterfaces(SL_I2C_BUS_DEV_TYPE *Bus)
{
  Bus->Priv_BaseOps = &I2C_BusBaseOps;  //��ȡ��λ����ָ��
  
  return SL_EOK;
}

#endif /* USING_I2C */
