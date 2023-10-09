#include <STC12C5A60S2.H>
#include "LCD1602.h"	//包含LCD1602头文件
void main()
{
	LED1 = 0;
	LCD_Init();
	LCD_ShowChar(1,1,'A');
}

