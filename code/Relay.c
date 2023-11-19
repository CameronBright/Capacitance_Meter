#include "Relay.h"

/*
继电器控制函数
relay_num: K1-K4选择，1=K1
status: 0=断开，1=闭合
*/
void Relay_Control(unsigned char relay_num, bit status)
{
	switch(relay_num)
	{
		case 0:
		{
			if(status)
				K1 = K2 = K3 = K4 = 0;
			else 
				K1 = K2 = K3 = K4 = 1;
			
			break;
		}
		case 1:
		{
			if(status)
			{
				K1 = 0;
				K2 = K3 = K4 = 1;
			}	
			else 
				K1 = 1;
			
			break;
		}
		case 2:
		{
			if(status)
			{
				K2 = 0;
				K1 = K3 = K4 = 1;
			}
			else 
				K2 = 1;
			
			break;
		}
		case 3:
		{
			if(status)
			{
				K3 = 0;
				K1 = K2 = K4 = 1;
			}
			else 
				K3 = 1;
			
			break;
		}
		case 4:
		{
			if(status)
			{
				K4 = 0;
				K1 = K2 = K3 = 1;
			}		
			else 
				K4 = 1;
			
			break;
		}

	}
}
