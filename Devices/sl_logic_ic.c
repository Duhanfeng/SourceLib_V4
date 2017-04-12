/**
  ******************************************************************************
  * @file    sl_logic_ic.c
  * @author  �Ź��Ӻ���
  * @version V1.1
  * @date    2017.04.12
  * @brief   about some logic ic drivers
  ******************************************************************************
  * @attention
  * 
  * 74HC164:  ��ת��
  * 74HC4052: ˫բģ�⿪��  PORT_INH  PORT_SEL0  PORT_SEL1
  * 
  * V1.0------------
  * �޸�����: ����74HC4052ģ�⿪�ص������ļ�
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.0
  * �޸�����: 2017.04.11
  * 
  * V1.1------------
  * �޸�����: ����74HC595ģ�⿪�ص������ļ�
  * �޸�����: �Ź��Ӻ���
  * ��ǰ�汾: V1.1
  * �޸�����: 2017.04.12
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_logic_ic.h"
#include "SourceLib.h"



/*----------------------------------------------------------------------------
    74HC405x ��·Xͨ��ģ�⿪��
 *----------------------------------------------------------------------------*/
 
static void    *HC405x_PrivateData;
static void    (*HC405x_SetINH)  (void *Data, uint8_t State);
static void    (*HC405x_SetSEL0) (void *Data, uint8_t State);
static void    (*HC405x_SetSEL1) (void *Data, uint8_t State);
static void    (*HC405x_SetSEL2) (void *Data, uint8_t State); //��4052����Ҫ
static HC405x_TYPE HC405x_Type;

#define HC405x_INH(x)   HC405x_SetINH (HC405x_PrivateData, (x))
#define HC405x_SEL0(x)  HC405x_SetSEL0(HC405x_PrivateData, (x))
#define HC405x_SEL1(x)  HC405x_SetSEL1(HC405x_PrivateData, (x))
#define HC405x_SEL2(x)  HC405x_SetSEL2(HC405x_PrivateData, (x))


//Ӳ�����ƽӿ�
void HC405x_HwCtrlInterFaces(void    (*HC_PortInit)(void),
                             void    *Data,
                             void    (*SetINH) (void *Data, uint8_t State),
                             void    (*SetSEL0)(void *Data, uint8_t State),
                             void    (*SetSEL1)(void *Data, uint8_t State),
                             void    (*SetSEL2)(void *Data, uint8_t State),
                             HC405x_TYPE DevType)
{
  HC405x_PrivateData = Data;
  HC405x_SetINH  = SetINH;
  HC405x_SetSEL0 = SetSEL0;
  HC405x_SetSEL1 = SetSEL1;
  HC405x_SetSEL2 = SetSEL2;
  
  HC_PortInit();  //��ʼ��Ӳ���˿�
  
}
  

//INH�ŵ͵�ƽ��Ч
void HC405x_Enable(uint8_t isEnable)
{
  HC405x_INH(!getBOOL(isEnable));
  
}


//ѡ��ͨ��
void HC405x_SetChannel(uint8_t cChannel)
{
  HC405x_Enable(cChannel != 0xFF);  //���ͨ��Ϊ0xFF,��ر�
  
  if (HC405x_Type != SWITCH_HC4052)
  {
    HC405x_SEL2(getBOOL(cChannel & 0x04));
  }
  HC405x_SEL1(getBOOL(cChannel & 0x02));
  HC405x_SEL0(getBOOL(cChannel & 0x01));
  
}



/*----------------------------------------------------------------------------
    74HC595 ��ת��оƬ
 *----------------------------------------------------------------------------*/

static void    *HC595_PrivateData;
static void    (*HC595_SetSHCP) (void *Data, uint8_t State);
static void    (*HC595_SetSTCP) (void *Data, uint8_t State);
static void    (*HC595_SetDATA) (void *Data, uint8_t State);
static void    (*HC595_DelayUs) (uint16_t Us);
static uint8_t HC595_ParBitNum = 0;

#define HC595_SHCP(x)   HC595_SetSHCP(HC595_PrivateData, (x))
#define HC595_STCP(x)   HC595_SetSTCP(HC595_PrivateData, (x))
#define HC595_DATA(x)   HC595_SetDATA(HC595_PrivateData, (x))

typedef enum
{
  HC595_8_BIT   = 8,
  HC595_16_BIT  = 16,
  HC595_24_BIT  = 16,
  HC595_32_BIT  = 16,
  
}HC595_PARALLEL_BIT;




//Ӳ�����ƽӿ�
void HC595_HwCtrlInterFaces(void    (*HC_PortInit)(void),
                            void    *Data,
                            void    (*SetSHCP)(void *Data, uint8_t State),
                            void    (*SetSTCP)(void *Data, uint8_t State),
                            void    (*SetDATA)(void *Data, uint8_t State),
                            void    (*HC_DelayUs)(uint16_t Us),
                            uint8_t ParBitNum)
{
  HC595_PrivateData = Data; //˽������
  HC595_SetSHCP = SetSHCP;
  HC595_SetSTCP = SetSTCP;
  HC595_SetDATA = SetDATA;
  HC595_DelayUs = HC_DelayUs;
  HC595_ParBitNum = ParBitNum;
  
  HC_PortInit();  //��ʼ��Ӳ���˿�
  
}



//�����������
void HC595_OutputParallelData(uint32_t iSerialData)
{
  uint8_t i = 0;
  
  //������λ
  for (i = 0; i < HC595_ParBitNum; i++)
  {
    HC595_SHCP(1);
    HC595_DelayUs(5);
    
    HC595_SHCP(0);
    
    HC595_DATA((iSerialData & (0x1<<((HC595_ParBitNum - 1)-i))) ? 1 : 0);  //�Ӹ�λ����λɨ��
    
    HC595_DelayUs(5);
    
    HC595_SHCP(1);
  }
  
  //��������
  HC595_STCP(0);
  HC595_DelayUs(5);
  HC595_STCP(1);
  
}




