/*
program versions : 2.5

Added long and short key press function;

modification: 2023/11/5 21:11

modifier: Cameron Bright

*/

#include "main.h"
#include "LCD1602.h"	//包含LCD1602头文件
#include "Key.h"      //按键扫描函数

sbit V0 = P1^2;

sbit K1 = P3^7;//继电器 低电平闭合
sbit K2 = P3^6;
sbit K3 = P3^5;
sbit K4 = P3^4;

sbit Buzzer = P2^3;//蜂鸣器 低电平工作

void Timer0_Init(void);	//1毫秒@11.0592MHz

void Key_Proc(void);    //Keystroke process function
void Lcd_Proc(void);    //LCD Dsiplay process function

unsigned int timer_tick = 0;
unsigned int buzzer_tick = 0;//用于开机计数500ms

unsigned char key_value; //按键处理变量
unsigned char key_down;
unsigned char key_up;
unsigned char key_old;

unsigned int key_tick; //long key press count

unsigned char page = 0;

unsigned int key_slow_down = 0;
unsigned int lcd_slow_down = 0;

unsigned int dispbuf = 0;

void main()
{
	Timer0_Init();//定时器初始化
	
	//Buzzer = 0;
	LCD_Init();
	K2 = 0;
	
	EA = 1;
	
	while(1)
	{	
		Key_Proc();
		Lcd_Proc();
	}
	
}

void Lcd_Proc(void)     //LCD Dsiplay process function
{
	if(lcd_slow_down) return;   //10ms更新一次
		lcd_slow_down = 1;
	
	if(page == 0)
	{
		LCD_ShowString(2,2,"Hello!");
	
		LCD_ShowNum(1,1,dispbuf,4);
		LCD_ShowNum(1,8,key_tick,4);
	}
	else if(page == 1)
	{
		LCD_ShowString(2,2,"ctrl");
	}
	
	
}

void Key_Proc(void)
{
	if(key_slow_down) return;   //10ms更新一次
		key_slow_down = 1;
	
	key_value = Key_Read();
	key_down = key_value & (key_value ^ key_old);
	key_up = ~key_value & (key_value ^ key_old);
	key_old = key_value;

	if(key_down)       //长按五秒
		key_tick = 5000;
	
	if(key_old)
	{
		if(key_tick == 0)
			{
				Lcd_Clear();
				page = 1;
			}
	}
	
	switch(key_up)
	{
		case 1:
		{
			LED1 = 1;
			break;
		}
		case 2: 
		{
			if(--dispbuf == 0)          
				dispbuf = 10;
			key_tick = 0;
			break;
		}
		case 3:
		{
			if(++dispbuf == 10)          
				dispbuf = 0;
			key_tick = 0;
			break;
		}
		case 4:
		{
			LED1 = 0;
			break;
		}
		case 5:
		{
			LED1 ^= 1;
			break;
		}
		case 6:
		{
			LED1 ^= 1;
			break;
		}
		default:
			break;
 	}		
}

void Timer0_Isr(void) interrupt 1
{
	if(++key_slow_down == 10) key_slow_down = 0;
	if(++lcd_slow_down == 200) lcd_slow_down = 0;
	
	if(key_tick > 0) key_tick--;
	
//	if(Buzzer == 0)
//	{
//		if(++buzzer_tick >= 500)
//			{
//				Buzzer = 1;
//				buzzer_tick = 0;
//			}
//	}
	
	
				
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

 