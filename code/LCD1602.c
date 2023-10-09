#include"LCD1602.h"

void LCD_Init()
{
    Lcd_WriteCmd(0x38);
    Delay5ms();
    Lcd_WriteCmd(0x38);
    Delay5ms();
    Lcd_WriteCmd(0x38);
    Delay5ms();
    Lcd_WriteCmd(0x38);

    Lcd_WriteCmd(0x08);
    Lcd_WriteCmd(0x01);
    Lcd_WriteCmd(0x06);
    Delay5ms();
    Lcd_WriteCmd(0x0C);
}
void Lcd_WriteCmd(unsigned char cmd)
{
    //while(Lcd_Check_Busy());
    //用延时替代
    Delay1ms();
    Delay1ms();
    Delay1ms();

    //指令数据，0指令，1数据
    RS = 0;
    RW = 0;
    DATA = cmd;
    EN = 1; 
    EN = 0;
}
void Lcd_WriteData(unsigned char data1)
{
    //仿真中不能检测忙函数
    //用延时替代
    while(Lcd_Check_Busy());
//    Delay1ms();
//    Delay1ms();
//    Delay1ms();
//    Delay1ms();


    //指令数据，0指令，1数据
    RS = 1;
    RW = 0;
    DATA = data1;
    EN = 1;
    EN = 0;

}
void Lcd_WriteChar(unsigned char x, unsigned char y, unsigned char char1)
{
    if(x == 1)
    {
        Lcd_WriteCmd(0x80|(y-1));  
    }
    else{
        Lcd_WriteCmd(0x80|(y-1)+ 0x40);  
    }
    Lcd_WriteData(char1);
}
bit Lcd_Check_Busy()
{
    RS = 0;
    RW = 1;
    EN = 0;
    Delay1ms();
    _nop_();
    EN = 1;
    Delay1ms();
    return (bit)(DATA & 0x80);
}