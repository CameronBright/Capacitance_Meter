#include "Relay.h"

/*
�̵������ƺ���
relay_num: K1-K4ѡ��0=ȫ�� 1=K1 2=K2 3=K3 4=K4
status: 0=�Ͽ���1=�պ�
*/
void Relay_Control(unsigned char relay_num, bit status)
{
	switch(relay_num)
	{
		case 0: //ȫ���
		{
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
		case 5: //ȫ�����
		{
			if(status)
				K1 = K2 = K3 = K4 = 0;
			else 
				K1 = K2 = K3 = K4 = 1;
			
			break;
		}

	}
}
