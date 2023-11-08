#include "ADC.h"
#include "Delay.h"
#include "intrins.h"
#include <STC12C5A60S2.H>
//#include "main.h"

/*����ADC��ؼĴ��� */
//sfr ADC_CONTR   =   0xBC;           //ADC ���ƼĴ���
//sfr ADC_RES     =   0xBD;           //ADC ��8λ����Ĵ���
sfr ADC_LOW2    =   0xBE;           //ADC ��2λ����Ĵ���
//sfr P1ASF       =   0x9D;           //P1 �������ƼĴ���

/*ADC������һЩ�궨��*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

void ADC_Init(void)
{
    P1ASF = 0x01;                   //����8·ADC����
    ADC_RES = 0;                    //���֮ǰ�Ľ��
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay1ms();                       //ADC�ϵ����ʱ
		Delay1ms();
}

unsigned char GetADCResult(unsigned char ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
	_nop_();						//��ѯǰ����ȴ�
	_nop_();
	_nop_();
	_nop_();
	
	while (!(ADC_CONTR & ADC_FLAG));//�ȴ���ɱ�־
	ADC_CONTR &= ~ADC_FLAG;         //�ر� ADC
	return ADC_RES;                 //����ADC���
}
