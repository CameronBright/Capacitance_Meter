/*
program versions : 3.2.2

此分支[branch2_1]用于编写校验代码

Descrription: 修复了校验界面显示不正确的bug

updata record:
修复了校验界面显示不正确的bug

modification: 2023/12/15 20:40

modifier: Cameron Bright

*/

#include "main.h"
#include <string.h>
#include "LCD1602.h"	//包含LCD1602头文件
#include "Key.h"      //按键扫描函数
#include "ADC.h"
#include "Relay.h"    //继电器头文件
#include <stdio.h>

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
unsigned char key_down;	 //按键下降沿
unsigned char key_up;    //上升沿
unsigned char key_old;   //上次的按键值

unsigned int led1_tick = 1; //状态灯计数
unsigned int key_tick; //long key press count
unsigned int delay_tick;//定时器延时计数
unsigned int cap_tick;  //电容测量计计时
unsigned int relay_tick = 0;//继电器计时

unsigned char dispbuf[6] = {'0','0','0','0','0','0'};
unsigned char page = 0;//lcd 显示界面
unsigned char cursor = 5; //光标

unsigned char relay_index;  //继电器选择

unsigned char password[6] = {'0','0','0','0','0','0'};
//unsigned char password_true[6] = {'8','7','6','5','4','3'};

unsigned char password_true[6] = {'1','0','0','0','0','0'}; //正确密码
//unsigned char password_true[6] = {'8','7','6','5','4','3'}; //正确密码
unsigned char password_for = 0; //index

unsigned char adc_char;			//adc检测返回的char类型值
float adc_float;						//adc检测返回的float类型值，就是具体的电压值

xdata float cap_value_k1 = 0;         
xdata float cap_value_k2 = 0;
xdata float cap_value_k3 = 0;
xdata float cap_value_k4 = 0;

xdata float cap_calibrations_value[6];
xdata float cap_calibrations_buf = 0; //测量校验值的缓冲区
xdata float increments[] = {100, 10, 1, 0, 0.1, 0.01};//个位数+1、百位数+1....

unsigned char calibrations = 0; //校准

float cap_value;						//存放电容的容值
char cap_units;    					//电容的单位0:uF、1:nF、 2:pF

void main()
{
	Timer0_Init();//定时器初始化
	LCD_Init(); //LCD函数初始化
	ADC_Init(); //ADC函数初始化
	
	//Buzzer = 0;//蜂鸣器初始化
	
	Relay_Control(0, 0);//继电器全关
	
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
	if(lcd_slow_down) return;   //200ms刷新一次
		lcd_slow_down = 1;
	
	if(page == 0)            //测量界面 初始界面
	{	
		sprintf((char *)dispbuf,"%06.2f",cap_value);
		
		LCD_WriteCommand(0x0C);//关光标
		
		LCD_ShowString(1,1,"Press OK Start!");
		
		LCD_ShowChar(2,1,dispbuf[0]);
		LCD_ShowChar(2,2,dispbuf[1]);
		LCD_ShowChar(2,3,dispbuf[2]);
		LCD_ShowChar(2,4,dispbuf[3]);
		LCD_ShowChar(2,5,dispbuf[4]);
		LCD_ShowChar(2,6,dispbuf[5]);
		
		switch(cap_units)
		{
			case 0:
				LCD_ShowString(2,7,"uF");
				break;
			case 1:
				LCD_ShowString(2,7,"nF");
				break;
			case 2:
				LCD_ShowString(2,7,"pF");
				break;
			default:
				break;
		}
		
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
	else if(page == 4)
	{
		LCD_ShowString(1,1,"Wait...");
		LCD_ShowString(2,1,"Press OK End");
	}
	else if(page == 5)       //校准输入页
	{
		LCD_WriteCommand(0x0f);//开光标
		sprintf((char *)dispbuf,"%06.2f",cap_calibrations_buf);
		
		LCD_ShowChar(2,1,dispbuf[0]);
		LCD_ShowChar(2,2,dispbuf[1]);
		LCD_ShowChar(2,3,dispbuf[2]);
		LCD_ShowChar(2,4,dispbuf[3]);
		LCD_ShowChar(2,5,dispbuf[4]);
		LCD_ShowChar(2,6,dispbuf[5]);
		
		LCD_ShowChar(1,1,calibrations+0x30); //左上角显示校准挡位 +0x30转换成ASC11码输出
	}
	else if(page == 6) //校准完成页
	{
		LCD_ShowChar(1,1,'A');
		Delay(2000);
		Lcd_Clear();
		page = 0;
	}
	
	if(page == 2 || page == 3) //闪一下ERROR和RIGHT的页面
	{
		Delay(2000); //两秒后切换页面
		Lcd_Clear();
		if(page == 2)
			page = 0;//密码错误输完后切换回测量界面		
		if(page == 3)
		{
			page = 5;//密码正确输完后切换到校准界面
			cursor = 0;//初始化一下光标位置
		}	
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
				if(page == 1) 	//输密码页
				{
					password[cursor-5] += 1;  //按照光标位置加
					if(password[cursor-5] > '9')
						password[cursor-5] = '9';
				}
				else if(page == 5) //校准页
				{
					cap_calibrations_buf += increments[cursor];
					if(cap_calibrations_buf >= 1000)
						cap_calibrations_buf -= 1000;
				}
				key_tick = 0;
				break;
			}
			case 3:        //↓
			{
				if(page == 1)		//输密码页
				{
					password[cursor-5] -= 1; //按照光标位置减
					if(password[cursor-5] == '/')
						password[cursor-5] = '0';
				}else if(page == 5)	//校准页
				{
					cap_calibrations_buf -= increments[cursor];
					if(cap_calibrations_buf < 0)
						cap_calibrations_buf = 0;
				}
				key_tick = 0;
				break;
			}
			case 4:        //←
			{
				if(page == 1)	  //输密码页
				{
					if(--cursor <= 5)
						cursor = 5;
				}else if(page == 5)	//校准页
				{
					if(--cursor == 255) //光标限位unsigned char类型0-1=255
						cursor = 0;
				}
				key_tick = 0;
				break;
			}
			case 5:        //→
			{
				if(page == 1)  //输密码页
				{
					if(++cursor >= 10)
						cursor = 10;
				}else if(page == 5)	//校准页
				{
					if(++cursor > 5)  //光标限位
						cursor = 5;
				}
				key_tick = 0;
				break;
			}
			case 6:        //OK
			{
				if(page == 0)			 //如果在测量页，按下OK键开始测量
				{
					Lcd_Clear();
					page = 4;
				}
				else if(page == 4) //如果正在测量，按下OK键停止测量
				{
					Lcd_Clear();
					page = 0;
					cap_value = 0;//清零
					
					if(cap_value_k1 > 5.00)//如果被测电容超过了量程(50uF - 220pF)，就限制在50uF
					{
						cap_value = 50.0;
						cap_units = 0;           //单位换成uF
					}
					if(cap_value_k1 <= 5.00 && cap_value_k1 > 0.50)    //测量50uF-5uF
					{
						cap_value = cap_value_k1 * 10;
						cap_units = 0;           //单位换成uF
					}
					else if(cap_value_k1 <= 0.50 && cap_value_k2 > 0.50)//测量5uF-500nF
					{
						cap_value = cap_value_k2;
						cap_units = 0;           //单位换成uF
					}
					else if(cap_value_k2 <= 0.50 && cap_value_k3 > 0.50)//测量500nF-5nF
					{
						cap_value = cap_value_k3 * 100;
						cap_units = 1;           //单位换成nF
					}
					else if(cap_value_k3 <= 0.50 && cap_value_k4 > 0.00)//测量5000pF-0pF
					{
						cap_value = cap_value_k4 * 10000;
						if(cap_value >= 10000)
						{
							cap_value = cap_value / 1000.0;
							cap_units = 1;           //单位换成nF
						}
						else 
							cap_units = 2;           //单位换成pF
					}
				}
				else if(page == 1) //如果在输密码页，按下OK键确认密码
				{
					Lcd_Clear();
					if(strncmp(password,password_true,6) == 0)
						page = 3;
					else
						page = 2;
				}
				else if(page == 5) //如果在校验页面
				{
					cap_calibrations_value[calibrations] = cap_calibrations_buf;//将每一次的校验值储存起来
					cap_calibrations_buf = 0;//缓冲区清零
					calibrations += 1;
					
					if(calibrations > 6 )
					{
						calibrations = 0;
						page = 6; //跳转到校准完成页
					}
					
				}		
				key_tick = 0;
				break;
			}
			
			default:
				break;
		}		
	}
}

//================电容检测函数======================
void Detection_Proc(void)
{
	if(Det_slow_down) return;   //10ms更新一次
	Det_slow_down = 1;
	
	if(relay_index == 1)
	{
		adc_char = GetADCResult(0); //测量P10 ADC
		cap_value_k1 = (float)adc_char/51;//转换成电压值
	}
	else if(relay_index == 2)
	{
		adc_char = GetADCResult(0); //测量P10 ADC
		cap_value_k2 = (float)adc_char/51;//转换成电压值
	}
	else if(relay_index == 3)
	{
		adc_char = GetADCResult(0); //测量P10 ADC
		cap_value_k3 = (float)adc_char/51;//转换成电压值
	}
	else if(relay_index == 4)
	{
		adc_char = GetADCResult(0); //测量P10 ADC
		cap_value_k4 = (float)adc_char/51;//转换成电压值
	}
	
	Relay_Control(relay_index, 1);//继电器刷新
}

//================中断函数=======================
void Timer0_Isr(void) interrupt 1
{
	if(++key_slow_down == 10) key_slow_down = 0;
	if(++lcd_slow_down == 200) lcd_slow_down = 0;
	if(++Det_slow_down == 50) Det_slow_down = 0;
	
	if(delay_tick > 0) delay_tick--;//延时函数 会卡住当前函数
	if(key_tick > 0) key_tick--;    //按键计时
	
	//呼吸灯 用于测试中断
//	if(++led1_tick >= 1000)
//	{
//		LED1 ^= 1;
//		led1_tick = 0;
//	}
		
	
//	if(Buzzer == 0)
//	{
//		if(++buzzer_tick >= 500)
//			{
//				Buzzer = 1;
//				buzzer_tick = 0;
//			}
//	}		

//---------继电器切换控制----------------------
	if(page ==  4)
	{
		if(++relay_tick > 1000)
		{
			if(++relay_index > 4)//relay 继电器控制index1=继电器1
				relay_index = 1;
			relay_tick = 0;
		}
	}
	
	else if(page ==  5)
	{
		if(calibrations == 0)
		{
//			K1 = 0;
//			K2 = K3 = K4 = 1;
			relay_index = 1;
		}	
		else if(calibrations == 1)
		{
//			K2 = 0;
//			K1 = K3 = K4 = 1;
			relay_index = 2;
		}		
		else if(calibrations == 2)
		{
//			K3 = 0;
//			K1 = K2 = K4 = 1;
			relay_index = 3;
		}
		else if(calibrations == 3)
		{
//			K4 = 0;
//			K1 = K2 = K3 = 1;
			relay_index = 4;
		}
		
		
	}	
		
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
 