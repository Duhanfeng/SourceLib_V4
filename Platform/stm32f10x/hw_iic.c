/**
  ******************************************************************************
  * @file    multi_iic.c
  * @author  �Ź��Ӻ���
  * @version V3.0 �Ĵ����汾
  * @date    2016.08.05
  * @brief   IICЭ������(ģ�ⷽʽ)
  ******************************************************************************
  * @attention
  * 
  * ����,ͨ�ÿ�©���,2M
  * 
  * ����֡: ��λ�ȷ�
  * 
  * ע1: ���Ƕ�ȡ����ǰ,��Ҫ�ͷ�����(�øߵ�ƽ),����ӻ��޷���������!
  * ע2: ��Ӳ��ʵ����,���û���ڴ��䵱ǰ�ֽ�֮ǰ����EV7��EV7_1��EV6_1��EV2��
  *      EV8��EV3�¼�,�п��ܲ�������,���յ�һ��������ֽڡ����ζ�����ͬ������
  *      ��ʧ���ݡ���<<�����ֲ�>>.   Ӳ����ʹ�ô���BUG,�ʶ��������ʽʵ��
  * 
  * Ϊ���һЩ��Ҫ��IICͨ�����Ҳ��ǹ���ͬһ���ߵĳ���,�ش˱�д�������ļ�.
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "hw_iic.h"
#include "SourceLib.h"
#include "stm32f10x.h"

#define IIC_RESERVER      *((volatile unsigned long  *)0x00000000)
  
//SII9575
#define IIC1_SCL      (PBout(9))
#define IIC1_SDA      (PBout(8))
#define IIC1_R_SDA    (PBin (8))

//M21452_2
#define IIC2_SCL      (PDout(2))
#define IIC2_SDA      (PDout(1))
#define IIC2_R_SDA    (PDin (1))

//IIC_RESERVER
#define IIC3_SCL      (IIC_RESERVER)
#define IIC3_SDA      (IIC_RESERVER)
#define IIC3_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC4_SCL      (IIC_RESERVER)
#define IIC4_SDA      (IIC_RESERVER)
#define IIC4_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC5_SCL      (IIC_RESERVER)
#define IIC5_SDA      (IIC_RESERVER)
#define IIC5_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC6_SCL      (IIC_RESERVER)
#define IIC6_SDA      (IIC_RESERVER)
#define IIC6_R_SDA    (IIC_RESERVER)

//IIC_RESERVER
#define IIC7_SCL      (IIC_RESERVER)
#define IIC7_SDA      (IIC_RESERVER)
#define IIC7_R_SDA    (IIC_RESERVER)


static volatile unsigned long * const pSDA_Port[7]  = {&IIC1_SDA, &IIC2_SDA, &IIC3_SDA, &IIC4_SDA, &IIC5_SDA, &IIC6_SDA};
static volatile unsigned long * const pSCL_Port[7]  = {&IIC1_SCL, &IIC2_SCL, &IIC3_SCL, &IIC4_SCL, &IIC5_SCL, &IIC6_SCL};
static volatile unsigned long * const pRead_Port[7] = {&IIC1_R_SDA, &IIC2_R_SDA, &IIC3_R_SDA, &IIC4_R_SDA, &IIC5_R_SDA, &IIC6_R_SDA};

#define IICx_SDA    *pSDA_Port
#define IICx_SCL    *pSCL_Port
#define IICx_R_SDA  *pRead_Port

#define IIC_HIGH_SPEED    ( 1)  //����,Լ150KHz  H:3us  L:4us    
#define IIC_MEDIUM_SPEED  ( 5)  //����,Լ70KHz   H:7us  L:8us    
#define IIC_LOW_SPEED     (10)  //����,Լ40KHz   H:12us L:13us     

#define IIC_Delay()  IICx_Delay(Port)

void IICx_Delay(IIC_PORT Port)
{
  const unsigned short int nTime[7] = { 
    IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED,
    IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED, IIC_MEDIUM_SPEED};
  
  DelayUs(nTime[Port]);
  
}



//IIC��������
void IICx_IOConfig(IIC_PORT Port)
{
  switch (Port)
  {
    case IIC_PORT1:
    {
      GPIOx_FastInit(GPIOx_PB, GPIOx_8, GPIOx_GP_OD_10M);
      GPIOx_FastInit(GPIOx_PB, GPIOx_9, GPIOx_GP_OD_10M);
      break;
    }
    case IIC_PORT2:
    {
      
      break;
    }
    case IIC_PORT3:
    {
      
      break;
    }
    case IIC_PORT4:
    {
      
      break;
    }
    case IIC_PORT5:
    {
      
      break;
    }
    case IIC_PORT6:
    {
      
      break;
    }
    default: break;
  }
  
  IICx_SDA[Port] = 1;
  IICx_SCL[Port] = 1;
}


//IIC��ʼ��
void IICx_Init(IIC_PORT Port)
{
  IICx_IOConfig(Port);
  
}



/**
  * @brief  IIC1��ʼ��־(��SCL�ߵ�ƽʱ����SDA)
  * @param  ��
  * @retval ��
  */
void IICx_Start(IIC_PORT Port)
{
  IICx_SDA[Port] = 1;
  IICx_SCL[Port] = 1;
  IIC_Delay();
  IICx_SDA[Port] = 0;
  IIC_Delay();
  IICx_SCL[Port] = 0;
  
}



/**
  * @brief  IIC1������־(��SCL�ߵ�ƽʱ����SDA)
  * @param  ��
  * @retval ��
  */
void IICx_Stop(IIC_PORT Port)
{
  IICx_SCL[Port] = 0;
  IICx_SDA[Port] = 0;
  
  IICx_SCL[Port] = 1;
  IIC_Delay();
  IICx_SDA[Port] = 1;
  IIC_Delay();

}



/**
  * @brief  IIC1�ֽڷ��ͺ���(��SCL�͵�ƽʱ����/����SDA)
  * @param  cSendData Ҫ���͵��ֽ�
  * @retval ��
  */
void IICx_SendByte(IIC_PORT Port, uint8_t cSendData)
{
  uint8_t i = 0;
  
  for (i = 0; i < 8; i++)
  {
    IICx_SCL[Port] = 0;
    
    if (cSendData & (0X1<<(7-i)))  //��������
    {
      IICx_SDA[Port] = 1;      
    }
    else
    {
      IICx_SDA[Port] = 0;
    }
    
    IIC_Delay();
    
    IICx_SCL[Port] = 1;
    IIC_Delay();
    
    IICx_SCL[Port] = 0;
  }

}



/**
  * @brief  IIC1�ֽڽ��պ���(��SCL�ߵ�ƽʱ��SDA��ƽ״̬)
  * @param  ��
  * @retval cRecvData ���յ����ֽ�
  */
uint8_t IICx_RecvByte(IIC_PORT Port)
{
  uint8_t i = 0;
  uint8_t cRecvData = 0;

  IICx_SDA[Port] = 1;   //���Ƕ�ȡ����ǰ,��Ҫ�ͷ�����,����ӻ��޷���������
  
  for (i = 0; i < 8; i++)
  {
    IICx_SCL[Port] = 0;
    IIC_Delay();
    
    IICx_SCL[Port] = 1;
    
    if (IICx_R_SDA[Port])
    {
      cRecvData |=  (0X1<<(7-i));
    }
      
    IIC_Delay();
    
    IICx_SCL[Port] = 0;
  }
  
  return cRecvData;
}



/**
  * @brief  IIC1�ȴ�Ӧ���ź�(��SCLΪ��ʱ�ȴ�Ӧ��,SDA���ΪӦ��)
  * @param  ��
  * @retval ��
  */
void IICx_WaitAck(IIC_PORT Port)
{
  uint16_t nTime = 0;
  
  IICx_SDA[Port] = 1;  //�ͷ�����
  
  IICx_SCL[Port] = 0;
  IIC_Delay();
  
  IICx_SCL[Port] = 1;
  
  while (IICx_R_SDA[Port])  //�ȴ�SDA���
  {
    DelayUs(10);
    nTime++;
    if (nTime == 300)   //��ʱ����,�������ʱ������ѭ��
    {
      break;
    }
  }
  
  IIC_Delay();
  
  IICx_SCL[Port] = 0;
  
}



/**
  * @brief  IIC1��Ӧ���ź�(��SCLΪ��ʱSDA�����͵�ƽ)
  * @param  ��
  * @retval ��
  */
void IICx_GiveAck(IIC_PORT Port)
{
  IICx_SCL[Port] = 0;
  IICx_SDA[Port] = 0;  //Ӧ���ź�
  IIC_Delay();
  
  IICx_SCL[Port] = 1;
  IIC_Delay();
  
  IICx_SCL[Port] = 0;
  
}



/**
  * @brief  IIC1����Ӧ���ź�(��SCLΪ��ʱSDA�����ߵ�ƽ)
  * @param  ��
  * @retval ��
  */
void IICx_GiveNoAck(IIC_PORT Port)
{
  IICx_SCL[Port] = 0;
  IICx_SDA[Port] = 1;  //��Ӧ���ź�
  IIC_Delay();
  
  IICx_SCL[Port] = 1;
  IIC_Delay();
  
  IICx_SCL[Port] = 0;
  
}





