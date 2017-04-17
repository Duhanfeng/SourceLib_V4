/**
  ******************************************************************************
  * @file    sl_lcd1602.c
  * @author  �Ź��Ӻ���
  * @version V1.0
  * @date    2017.04.10
  * @brief   control lcd1602
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  *     
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "sl_lcd1602.h"
#include "SourceLib.h"

static void (*LCD1602_PORT)(uint8_t State);
static void (*LCD1602_RS)(uint8_t State);
static void (*LCD1602_EN)(uint8_t State);
static void (*LCD1602_RW)(uint8_t State);
static void (*LCD1602_DelayMs)(uint16_t Ms);


/**
  * @brief  NRF24L01 Ӳ���ӿ�
  * @param  ��
  * @retval ��
  */
void LCD1602_HwCtrlInterFaces(void (*LCD_PortInit)(void),
                              void (*LCD_PORT)(uint8_t State),
                              void (*LCD_RS)(uint8_t State),
                              void (*LCD_EN)(uint8_t State),
                              void (*LCD_RW)(uint8_t State),
                              void (*LCD_DelayMs)(uint16_t Ms))
{
  LCD1602_PORT    = LCD_PORT;
  LCD1602_RS      = LCD_RS;
  LCD1602_EN      = LCD_EN;
  LCD1602_RW      = LCD_RW;
  LCD1602_DelayMs = LCD_DelayMs;
  
  LCD_PortInit();
  
  LCD1602_EN(0);
	LCD1602_RW(0);						  //����Ϊд״̬
  
  LCD1602_DelayMs(30);
	LCD1602_write_com(0x38);	  //��ʾģʽ�趨
  LCD1602_DelayMs(5);
	LCD1602_write_com(0x0c);	  //������ʾ������������á������˸����
	LCD1602_write_com(0x06);	  //дһ���ַ���ָ���һ
	LCD1602_write_com(0x01);	  //����ָ��
  
}



//****************************************************
//дָ��
//****************************************************
void LCD1602_write_com(unsigned char com)
{
	LCD1602_RS(0);
	LCD1602_DelayMs(1);
	LCD1602_EN(1);
	LCD1602_PORT(com);
	LCD1602_DelayMs(1);
	LCD1602_EN(0);
}

//****************************************************
//д����
//****************************************************
void LCD1602_write_data(unsigned char dat)
{
	LCD1602_RS(1);
	LCD1602_DelayMs(1);	
	LCD1602_PORT(dat);
	LCD1602_EN(1);
	LCD1602_DelayMs(1);
	LCD1602_EN(0);
}

//****************************************************
//����д�ַ�
//****************************************************
void LCD1602_write_word(unsigned char *s)
{
	while(*s>0)
	{
		LCD1602_write_data(*s);
		s++;
	}
}

