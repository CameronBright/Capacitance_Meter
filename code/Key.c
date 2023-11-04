#include "Key.h"

unsigned char Key_Read(void)
{
	unsigned char key_value = 0;

	if(P12 == 0)   //±³¹â/Ð£×¼
		key_value = 0;
	if(P13 == 0)   //¡ü
		key_value = 1;
	if(P14 == 0)   //¡ý
		key_value = 2;
	if(P15 == 0)   //¡û
		key_value = 3;
	if(P16 == 0)   //¡ú
		key_value = 4;
	if(P33 == 0)   //OK
		key_value = 5;
	return key_value;
}
