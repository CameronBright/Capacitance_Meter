/*
program versions : 1.1

此版本已能显示数字，但亮度不够

modification: 2023/10/9 23:55

modifier: Cameron Bright

*/

#include <STC12C5A60S2.H>
#include "LCD1602.h"	//包含LCD1602头文件

unsigned char text;

void main()
{
	LED1 = 0;
	LCD_Init();
	
	text = 2;
	
	while(1)
	{
		//Lcd_WriteChar(1, 2, distance/10%10 + '0');
		Lcd_WriteChar(1, 1, '2');
		
	}
	
}

 