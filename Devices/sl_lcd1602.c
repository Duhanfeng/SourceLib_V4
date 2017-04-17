/**
  ******************************************************************************
  * @file    sl_lcd1602.c
  * @author  杜公子寒枫
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
  * @brief  NRF24L01 硬件接口
  * @param  无
  * @retval 无
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
	LCD1602_RW(0);						  //设置为写状态
  
  LCD1602_DelayMs(30);
	LCD1602_write_com(0x38);	  //显示模式设定
  LCD1602_DelayMs(5);
	LCD1602_write_com(0x0c);	  //开关显示、光标有无设置、光标闪烁设置
	LCD1602_write_com(0x06);	  //写一个字符后指针加一
	LCD1602_write_com(0x01);	  //清屏指令
  
}



//****************************************************
//写指令
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
//写数据
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
//连续写字符
//****************************************************
void LCD1602_write_word(unsigned char *s)
{
	while(*s>0)
	{
		LCD1602_write_data(*s);
		s++;
	}
}

