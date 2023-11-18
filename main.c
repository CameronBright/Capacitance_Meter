/*
program versions : 3.2.1

已经能检测电容，但是校验还没有写

modification: 2023/11/18 19:47

modifier: Cameron Bright

*/

#include "main.h"
#include <string.h>
#include "LCD1602.h"	//包含LCD1602头文件
#include "Key.h"      //按键扫描函数
#include "ADC.h"
#include <stdio.h>

sbit V0 = P1^2;

sbit K1 = P3^7;//继电器 低电平闭合
sbit K2 = P3^6;
sbit K3 = P3^5;
sbit K4 = P3^4;

sbit Buzzer = P2^3;//蜂鸣器 低电平工作

void Timer0_Init(void);	//1毫秒@11.0592MHz
void Delay(unsigned int delay); //定时器延时 

void Key_Proc(void);   	   //Keystroke process function
void Lcd_Proc(void);  	   //LCD Dsiplay process function
void Detection_Proc(void); //cap detection process function

unsigned int key_slow_down = 0; //按键刷新计数
unsigned int lcd_slow_down = 0; //LCD刷新计数
unsigned int Det_slow_down = 0; //adc检测刷新计数

unsigned int timer_tick = 0;
unsigned int buzzer_tick = 0;//用于开机计数500ms

unsigned char key_value; //按键处理变量
unsigned char key_down;
unsigned char key_up;
unsigned char key_old;

unsigned int led1_tick; //状态灯计数
unsigned int key_tick; //long key press count
unsigned int delay_tick;//定时器延时计数
unsigned int cap_tick;  //电容测量计计时

unsigned char dispbuf[4] = {'0','0','0','0'};
unsigned char page = 0;//lcd 显示界面
unsigned char cursor = 5; //光标



unsigned char password[6] = {'0','0','0','0','0','0'};
//unsigned char password_true[6] = {'8','7','6','5','4','3'};

unsigned char password_true[6] = {'1','0','0','0','0','0'}; //正确密码
//unsigned char password_true[6] = {'8','7','6','5','4','3'}; //正确密码
unsigned char password_for = 0; 

unsigned char capvalue_char;
float capvalue_float;


void main()
{
	Timer0_Init();//定时器初始化
	LCD_Init(); //LCD函数初始化
	ADC_Init(); //ADC函数初始化
	
	//Buzzer = 0;//蜂鸣器初始化
	
	K1 = 1;
	K2 = 1;
	K3 = 1;
	K4 = 1;
	
	while(1)
	{	
		Key_Proc();
		Lcd_Proc();
		Detection_Proc();
	}
	
}

//================LCD=======================

void Lcd_Proc(void)     //LCD Dsiplay process function
{
	if(lcd_slow_down) return;   //10ms更新一次
		lcd_slow_down = 1;
	
	if(page == 0)            //测量界面 初始界面
	{	
		sprintf((char *)dispbuf,"%3.2f",capvalue_float);
		
		LCD_WriteCommand(0x0C);//关光标
		
		LCD_ShowString(1,1,"Press OK Start!");
		
		LCD_ShowChar(2,1,dispbuf[0]);
		LCD_ShowChar(2,2,dispbuf[1]);
		LCD_ShowChar(2,3,dispbuf[2]);
		LCD_ShowChar(2,4,dispbuf[3]);
		
		//LCD_ShowNum(2,8,key_tick,4); //测试按键长按
	}
	else if(page == 1)       //输密码页
	{
		LCD_WriteCommand(0x0f);//开光标
		
		LCD_ShowString(1,2,"Input Password");
		LCD_ShowString(2,6,password);
	}
	else if(page == 2)       //密码错误页
	{
		LCD_WriteCommand(0x0C);//关光标
		LCD_ShowString(1,6,"ERROR");
	}
	else if(page == 3)       //密码正确页
	{
		LCD_WriteCommand(0x0C);//关光标
		LCD_ShowString(1,6,"RIGHT");
	}
	
	if(page == 2 || page == 3) //闪一下ERROR和RIGHT的页面
	{
		Delay(2000); //两秒后切换页面
		Lcd_Clear();
		page = 0;//密码输完后切换到校准页面		
	}
	
	//LCD_WriteCommand(0x80+cursor); //第一行光标
	LCD_WriteCommand(0xc0+cursor); //第二行光标 
}

//================Key=======================

void Key_Proc(void)
{
	if(key_slow_down) return;   //10ms更新一次
		key_slow_down = 1;
	
	key_value = Key_Read();
	key_down = key_value & (key_value ^ key_old);
	key_up = ~key_value & (key_value ^ key_old);
	key_old = key_value;

	if(key_down)       //长按五秒
		key_tick = 2000;
	
	if(key_old)
	{
		if(key_tick == 0)
			{
				Lcd_Clear();
				page = 1;
				
				//清空密码字符串
				for(password_for = 0;password_for <= 5; password_for++)
				{
					password[password_for] = '0';
				}
			}
	}
	
	if(key_tick)
	{
		switch(key_up)
		{
			case 1:        //背光/校准按键
			{
				key_tick = 0;
				break;
			}
			case 2:        //↑ 
			{
				password[cursor-5] += 1;
				if(password[cursor-5] > '9')
					password[cursor-5] = '9';
			
				key_tick = 0;
				break;
			}
			case 3:        //↓
			{
				password[cursor-5] -= 1;
				if(password[cursor-5] == '/')
					password[cursor-5] = '0';
				
				key_tick = 0;
				break;
			}
			case 4:        //←
			{
				if(--cursor <= 5)
					cursor = 5;
				key_tick = 0;
				break;
			}
			case 5:        //→
			{
				if(++cursor >= 10)
					cursor = 10;
				key_tick = 0;
				break;
			}
			case 6:        //OK
			{
				Lcd_Clear();
				if(strncmp(password,password_true,6) == 0)
					page = 3;
				else
					page = 2;
				key_tick = 0;
				break;
			}
			
			default:
				break;
		}		
	}
	
}

void Detection_Proc(void)
{
	if(Det_slow_down) return;   //10ms更新一次
	Det_slow_down = 1;
	
	capvalue_char = GetADCResult(0); //测量P10 ADC
	capvalue_float = (float)capvalue_char/51;//转换成电压值
	
}

//================中断函数=======================
void Timer0_Isr(void) interrupt 1
{
	if(++key_slow_down == 10) key_slow_down = 0;
	if(++lcd_slow_down == 200) lcd_slow_down = 0;
	if(++Det_slow_down == 100) Det_slow_down = 0;
	
	if(delay_tick > 0) delay_tick--;//延时函数 会卡住当前函数
	if(key_tick > 0) key_tick--;    //按键计时
	
	if(++cap_tick >= 1000)
	{
		//K1 ^= 1;
//		K2 ^= 1;
//		K3 ^= 1;
//		K4 ^= 1;
		cap_tick = 0;
	}
	
	//呼吸灯 用于测试中断
	if(++led1_tick >= 1000)
	{
		LED1 ^= 1;
		led1_tick = 0;
	}
		
	
//	if(Buzzer == 0)
//	{
//		if(++buzzer_tick >= 500)
//			{
//				Buzzer = 1;
//				buzzer_tick = 0;
//			}
//	}		
}

void Delay(unsigned int delay) //定时器延时 会卡住当前函数
{
	delay_tick = delay;
	while(delay_tick > 0);
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
	EA = 1;         //总中断
}
 