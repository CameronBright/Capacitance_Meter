#include"LCD1602.h"
#include "intrins.h"

void LCD_WriteCommand(unsigned char Command)
{
		LCD_RS = 0;
		LCD_RW = 0;
		LCD_DataPort = Command;
		LCD_E  = 1;
		Delay1ms();
		LCD_E  = 0;
		Delay1ms();
		
}

void LCD_WriteData(unsigned char Data)
{
		LCD_RS = 1;
		LCD_RW = 0;
		LCD_DataPort = Data;
		LCD_E  = 1;
		Delay1ms();
		LCD_E  = 0;
		Delay1ms();
		
}

void LCD_Init()
{
	//LCD_WriteCommand(0x01);//0000 0001 清屏 
	LCD_WriteCommand(0x38);//0011 1000 数据总线8位 16X2显示 5*7点阵
	LCD_WriteCommand(0x0F);// 0000 1110 显示开，游标开，闪烁关
	LCD_WriteCommand(0x06);// 0000 0110 写入数据后光标自动右移 整屏不移动
	
}

void LCD_SetCursor(unsigned char Line,unsigned char Columu)
{
	if(Line == 1)
	{
		LCD_WriteCommand( 0x80 | (Columu-1) );
	}
	else
	{
		LCD_WriteCommand( 0x80 | ( Columu-1 )+0x40 );
	}
}

void LCD_ShowChar(unsigned char Line , unsigned char Columu , unsigned char Char)
{
		LCD_SetCursor(Line,Columu);
	
		LCD_WriteData(Char);
}

void LCD_ShowString(unsigned char Line , unsigned char Columu , unsigned char* String)
{
		unsigned char i = 0;
	
		LCD_SetCursor(Line,Columu);
	
		for(i=0;String[i] != '\0';i++)
		{
				LCD_WriteData(String[i]);
		}
}

int LCD_Pow(unsigned char x,unsigned char y)
{
		int result = 1;
		unsigned char i = 0;
		
		for(i=0;i<y;i++)
		{
				result *= x;
		}
		
		return result;
}

void LCD_ShowNum(unsigned char Line,unsigned char Columu,unsigned int Num,unsigned int Length)
{
		unsigned char i = 0;
		
		LCD_SetCursor(Line,Columu);
		
		for(i=Length;i>0;i--)
		{
				LCD_WriteData('0'+Num/LCD_Pow(10,i-1)%10);
		}
}

void Lcd_Clear(void)
{
	LCD_WriteCommand(0x01);//0000 0001 清屏 
}

