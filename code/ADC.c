#include "ADC.h"
#include "Delay.h"
#include "intrins.h"
#include <STC12C5A60S2.H>
//#include "main.h"

/*声明ADC相关寄存器 */
//sfr ADC_CONTR   =   0xBC;           //ADC 控制寄存器
//sfr ADC_RES     =   0xBD;           //ADC 高8位结果寄存器
sfr ADC_LOW2    =   0xBE;           //ADC 低2位结果寄存器
//sfr P1ASF       =   0x9D;           //P1 辅助控制寄存器

/*ADC操作的一些宏定义*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

void ADC_Init(void)
{
    P1ASF = 0x01;                   //开启8路ADC功能
    ADC_RES = 0;                    //清楚之前的结果
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay1ms();                       //ADC上电和延时
		Delay1ms();
}

unsigned char GetADCResult(unsigned char ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
	_nop_();						//查询前必须等待
	_nop_();
	_nop_();
	_nop_();
	
	while (!(ADC_CONTR & ADC_FLAG));//等待完成标志
	ADC_CONTR &= ~ADC_FLAG;         //关闭 ADC
	return ADC_RES;                 //返回ADC结果
}
