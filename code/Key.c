#include "Key.h"

unsigned char Key_Read(void)
{
	unsigned char key_value = 0;

	if(P12 == 0)   //����/У׼
		key_value = 1;
	if(P13 == 0)   //��
		key_value = 2;
	if(P14 == 0)   //��
		key_value = 3;
	if(P15 == 0)   //��
		key_value = 4;
	if(P16 == 0)   //��
		key_value = 5;
	if(P33 == 0)   //OK
		key_value = 6;
	return key_value;
}
