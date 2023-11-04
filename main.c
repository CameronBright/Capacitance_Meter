/*
program versions : 2.2.1

增加按键函数 此Version暂未实现功能，但是下班了

modification: 2023/11/4 23:02

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
void Key_Proc(void);    //按键处理函数

unsigned int timer_tick = 0;
unsigned int buzzer_tick = 0;//用于开机计数500ms

unsigned char key_value = 0; //按键处理变量
unsigned char key_Dowm = 0;
unsigned char key_old = 0;
unsigned int Key_Slow_Down = 0;

unsigned int dispbuf;

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
		LCD_ShowNum(1,1,dispbuf,4);
	}
	
}

void Key_Proc(void)
{
	if(Key_Slow_Down) return;   //10ms更新一次
		Key_Slow_Down = 1;
	
	key_value = Key_Read();
	key_Dowm = key_value & (key_value ^ key_old);
	key_old = key_value;
	
	switch(key_Dowm)
	{
		case 0:
			dispbuf += 1;          //S6 UP
		break;
		case 1: 
			dispbuf -= 1;         //S5 DOWN
 		break;
//		case 3: 
//			key_flag = 1;         //S3 L
//		break;
//		case 4:
//			Send_Message(0x00ff);  //S7 TEST
//		break;
//		case 5:
//			Send_Message(0x66AA);  //S2 OFF\ON
//		break;
//		case 6:
//			key_flag = 3;         //S4 R
//		break;
	}
}

void Timer0_Isr(void) interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	
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

 