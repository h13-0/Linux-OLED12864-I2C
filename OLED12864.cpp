#include "i2c_control.h"
#include "OLED12864_I2C.h"
#include "stdio.h"
#include "stdlib.h"

const Oled12864_i2c_t Oled12864_i2c =
{
	.FileID = open("/dev/i2c-0", TIMEOUT, RETRY),
	.Address = 0x3c,
};

int main()
{
	while (1)
	{
		OLED_Init(Oled12864_i2c);
		OLED_CLS(Oled12864_i2c);//����
		delayms(1000);
		OLED_Fill(Oled12864_i2c, 0xFF);//ȫ������
		delayms(1000);
		OLED_Fill(Oled12864_i2c, 0x00);//ȫ����
		delayms(1000);
		OLED_ShowStr(Oled12864_i2c, 0, 3, (unsigned char*)"Hello,World!", SIZE_6X8);//����6*8�ַ�
		delayms(1000);
		OLED_ShowStr(Oled12864_i2c, 0, 3, (unsigned char*)"Hello,World!", SIZE_8X16);//����6*8�ַ�
		delayms(1000);
	}
}