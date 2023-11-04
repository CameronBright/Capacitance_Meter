/*
program versions : 2.2

新增蜂鸣器的功能 并完善了Timer函数

modification: 2023/11/4 22:10

modifier: Cameron Bright

*/

#include <STC12C5A60S2.H>
#include "LCD1602.h"	//包含LCD1602头文件

void Timer0_Init(void);	//1毫秒@11.0592MHz

sbit V0 = P1^2;

sbit K1 = P3^7;//继电器 低电平闭合
sbit K2 = P3^6;
sbit K3 = P3^5;
sbit K4 = P3^4;

sbit Buzzer = P2^3;//蜂鸣器 低电平工作

unsigned int timer_tick = 0;
unsigned int buzzer_tick = 0;

void main()
{
	Timer0_Init();//定时器初始化
	
	Buzzer = 0;
	LCD_Init();
	K2 = 0;
	
	EA = 1;
	
	while(1)
	{
		LCD_ShowString(2,2,"Hello!");
		LCD_ShowNum(1,1,buzzer_tick,4);
	}
	
}

void Timer0_Isr(void) interrupt 1
{
//if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(Buzzer == 0)
	{
		if(++buzzer_tick >= 500)
			{
				Buzzer = 1;
				buzzer_tick = 0;
			}
	}
	
				
//	TL0 = 0x20;				//设置定时初始值
//	TH0 = 0xD1;				//设置定时初始值
		
}

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

 