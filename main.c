/*
program versions : 1.0

此版本为修改前初始版本 充其量只能点个灯

modification: 2023/10/9 22:09

modifier: Cameron Bright

*/

#include <STC12C5A60S2.H>
#include "LCD1602.h"	//包含LCD1602头文件
void main()
{
	LED1 = 0;
	LCD_Init();
	LCD_ShowChar(1,1,'A');
}

