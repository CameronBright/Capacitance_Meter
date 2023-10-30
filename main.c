/*
program versions : 1.2.1

更换LCD1602驱动函数前的version

modification: 2023/10/30 19:50

modifier: Cameron Bright

*/

#include <STC12C5A60S2.H>
#include "LCD1602.h"	//包含LCD1602头文件

char lcd_dispbuff[4] = {}

void main()
{
	LED1 = 0;
	LCD_Init();
	
	text = 2;
	
	while(1)
	{
		//Lcd_WriteChar(1, 2, distance/10%10 + '0');
		Lcd_WriteChar(2, 1, '3');
		
	}
	
}

 