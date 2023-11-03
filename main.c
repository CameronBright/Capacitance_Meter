/*
program versions : 2.1

更换LCD驱动函数并添加了继电器功能

modification: 2023/11/3 17:27

modifier: Cameron Bright

*/

#include <STC12C5A60S2.H>
#include "LCD1602.h"	//包含LCD1602头文件

sbit V0 = P1^2;

sbit K1 = P3^7;//继电器 低电平闭合
sbit K2 = P3^6;
sbit K3 = P3^5;
sbit K4 = P3^4;

void main()
{
	LED1 = 0;
	LCD_Init();
	K1 = 0;
	
	while(1)
	{
		LCD_ShowString(2,2,"Hello!");
		V0 = 0;
	}
	
}

 