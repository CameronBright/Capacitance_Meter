#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <STC12C5A60S2.H>
#include <intrins.h>
#include "Delay.h"

#define LCD_DataPort P0

sbit LCD_RS = P2^7;
sbit LCD_RW = P2^6;
sbit LCD_E = P2^5;
//sbit LED_V0 = P1^0;

sbit LED1 = P2^2;

void LCD_WriteCommand(unsigned char Command);
void LCD_WriteData(unsigned char Data);
void LCD_Init(void);
void LCD_SetCursor(unsigned char Line,unsigned char Columu);
void LCD_ShowChar(unsigned char Line,unsigned char Columu,unsigned char Char);
void LCD_ShowString(unsigned char Line,unsigned char Columu,unsigned char* String);
void LCD_ShowNum(unsigned char Line,unsigned char Columu,unsigned int Num,unsigned int Length);

#endif