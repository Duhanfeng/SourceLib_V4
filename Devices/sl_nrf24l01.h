#ifndef  __SL_NRF24L01_H
#define  __SL_NRF24L01_H

#include "SourceLib.h"


#define TX_ADR_WIDTH 	5  	  //�����ַ���
#define TX_PLOAD_WIDTH  4   //��������ͨ����Ч���ݿ��0~32Byte

#define RX_ADR_WIDTH    5   //���յ�ַ���
#define RX_PLOAD_WIDTH  4   //��������ͨ����Ч���ݿ��0~32Byte

#define CHANAL 40	//Ƶ��ѡ�� 

/*NRF24L01�Ĵ����������� */
#define NRF_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 

/* NRF24L01 ��ؼĴ�����ַ�ĺ궨�� */
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;

#define STA_MARK_MX     (0x10)  //�ﵽ����ط������жϱ�־λ
#define STA_MARK_TX		  (0x20)  //��������жϱ�־λ
#define STA_MARK_RX		  (0x40)  //���յ������жϱ�־λ

#define MAX_RT          (0x10)  //�ﵽ����ط������жϱ�־λ
#define TX_DS		        (0x20)  //��������жϱ�־λ	 
#define RX_DR		        (0x40)  //���յ������жϱ�־λ


#ifdef __cplusplus
extern "C" {
#endif

  
  extern uint8_t NRF_RX_BUF[RX_PLOAD_WIDTH];   //�������ݻ���
  extern uint8_t NRF_TX_BUF[TX_PLOAD_WIDTH];   //�������ݻ���
//  extern uint8_t NRF_TX_ADDRESS[TX_ADR_WIDTH];  // ����һ����̬���͵�ַ
//  extern uint8_t NRF_RX_ADDRESS[RX_ADR_WIDTH];


  void nRF24L01_HwCtrlInterFaces(void    (*NRF_PortInit)(void),
                                 void    *Data,
                                 uint8_t (*SPI_ReadWriteByte)(void *Data, uint8_t cWriteData),
                                 void    (*SetCSN)(void *Data, uint8_t State),
                                 void    (*SetCE)(void *Data, uint8_t State),
                                 void    (*BitDelayUs)(uint16_t Us));
  void nRF24L01_RxMode(void);
  void nRF24L01_TxMode(void);
  uint8_t nRF24L01_ConnectCheck(void);
  uint8_t nRF24L01_GetState(void);
                                 
  void nRF24L01_SetAddress(uint8_t *nRFAddr);
  uint8_t nRF24L01_SendBuff(uint8_t *Tx_Buff);
  uint8_t nRF24L01_SendData(uint32_t Data);

#ifdef __cplusplus
}
#endif
                       
#endif /* __SL_NRF24L01_H */
