#ifndef _LCD1602_H_
#define _LCD1602_H_

//#include<reg52.h>
#include <STC12C5A60S2.H>
#include<intrins.h>

#include"Delay.h"

#define DATA P0

sbit LED1 = P2^2;

sbit RS = P2^7;
sbit RW = P2^6;
sbit EN = P2^5;

void LCD_Init();

void Lcd_WriteCmd(unsigned char cmd);
void Lcd_WriteData(unsigned char data1);
void Lcd_WriteChar(unsigned char y, unsigned char x, unsigned char char1);
void Lcd_WriteStr(unsigned char x, unsigned char y, unsigned char* string);	

bit Lcd_Check_Busy();
#endif