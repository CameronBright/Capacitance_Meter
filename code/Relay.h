#ifndef _RELAY_H_
#define _RELAY_H_

#include <STC12C5A60S2.H>

sbit K1 = P3^7;//�̵��� �͵�ƽ�պ�
sbit K2 = P3^6;
sbit K3 = P3^5;
sbit K4 = P3^4;

void Relay_Control(unsigned char relay_num, bit status);

#endif
