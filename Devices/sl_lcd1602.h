#ifndef __SL_LCD1602_H
#define __SL_LCD1602_H

#include "SourceLib.h"

//函数或者变量声明
void LCD1602_HwCtrlInterFaces(void (*LCD_PortInit)(void),
                              void (*LCD_PORT)(uint8_t State),
                              void (*LCD_RS)(uint8_t State),
                              void (*LCD_EN)(uint8_t State),
                              void (*LCD_RW)(uint8_t State),
                              void (*LCD_DelayMs)(uint16_t Ms));

extern void LCD1602_write_com(unsigned char com);
extern void LCD1602_write_data(unsigned char dat);
extern void LCD1602_write_word(unsigned char *s);
extern void Init_LCD1602(void);

#endif /* __SL_LCD1602_H */
