#include "OLED12864_I2C.h"
#include "codetab.h"
#include "unistd.h"

#define timelimit 200
#define uint8_t __uint8_t

//--------------------------------------------------------------
// Prototype      : void delayms(long t);
// Calls          : 
// Parameters     : 
// Description    : �ȴ�����
//--------------------------------------------------------------
void delayms(long t)
{
	usleep(t*1000);
}

//--------------------------------------------------------------
// Prototype      : void I2C_WriteByte(Oled12864_i2c_t Oled12864_i2c, unsigned char reg, unsigned char val);
// Calls          : 
// Parameters     : 
// Description    : �ȴ�����
//--------------------------------------------------------------
void I2C_WriteByte(Oled12864_i2c_t Oled12864_i2c, unsigned char reg, unsigned char val)
{
	i2c_byte_write(Oled12864_i2c.FileID, Oled12864_i2c.Address, reg, val);
}


//--------------------------------------------------------------
// Prototype      : void I2C_WriteData(uint8_t addr,uint8_t *data,unsigned char length);
// Calls          : 
// Parameters     : addr,�Ĵ�����ַ����ʾ��������������ݰ�;*data,���ݰ�����;length,���ݰ�����
// Description    : ����д����
//--------------------------------------------------------------

void I2C_WriteData(Oled12864_i2c_t Oled12864_i2c, unsigned char reg, unsigned char* val, int len)
{
	i2c_nbytes_write(Oled12864_i2c.FileID, Oled12864_i2c.Address, reg,val,len);
}



void WriteCmd(Oled12864_i2c_t Oled12864_i2c, unsigned char val)//д����
{
	I2C_WriteByte(Oled12864_i2c, 0x00,  val);     //0x00��Ӧ������Ϊ0000000 ��д����ʱ ��9λΪ0
}

void OLED_Init(Oled12864_i2c_t Oled12864_i2c)
{
	delayms(100); //�������ʱ����Ҫ
	WriteCmd(Oled12864_i2c,0xAE); //display off
	WriteCmd(Oled12864_i2c, 0x20);	//Set Memory Addressing Mode	
	WriteCmd(Oled12864_i2c, 0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(Oled12864_i2c, 0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(Oled12864_i2c, 0xc8);	//Set COM Output Scan Direction
	WriteCmd(Oled12864_i2c, 0x00); //---set low column address
	WriteCmd(Oled12864_i2c, 0x10); //---set high column address
	WriteCmd(Oled12864_i2c, 0x40); //--set start line address
	WriteCmd(Oled12864_i2c, 0x81); //--set contrast control register
	WriteCmd(Oled12864_i2c, 0xff); //���ȵ��� 0x00~0xff
	WriteCmd(Oled12864_i2c, 0xa1); //--set segment re-map 0 to 127
	WriteCmd(Oled12864_i2c, 0xa6); //--set normal display
	WriteCmd(Oled12864_i2c, 0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(Oled12864_i2c, 0x3F); //
	WriteCmd(Oled12864_i2c, 0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(Oled12864_i2c, 0xd3); //-set display offset
	WriteCmd(Oled12864_i2c, 0x00); //-not offset
	WriteCmd(Oled12864_i2c, 0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(Oled12864_i2c, 0xf0); //--set divide ratio
	WriteCmd(Oled12864_i2c, 0xd9); //--set pre-charge period
	WriteCmd(Oled12864_i2c, 0x22); //
	WriteCmd(Oled12864_i2c, 0xda); //--set com pins hardware configuration
	WriteCmd(Oled12864_i2c, 0x12);
	WriteCmd(Oled12864_i2c, 0xdb); //--set vcomh
	WriteCmd(Oled12864_i2c, 0x20); //0x20,0.77xVcc
	WriteCmd(Oled12864_i2c, 0x8d); //--set DC-DC enable
	WriteCmd(Oled12864_i2c, 0x14); //
	WriteCmd(Oled12864_i2c, 0xaf); //--turn on oled panel
}

void OLED_SetPos(Oled12864_i2c_t Oled12864_i2c,unsigned char x, unsigned char y) //������ʼ������
{
	WriteCmd(Oled12864_i2c, 0xb0 + y);
	WriteCmd(Oled12864_i2c, ((x & 0xf0) >> 4) | 0x10);
	WriteCmd(Oled12864_i2c, (x & 0x0f) | 0x01);
}

void OLED_Fill(Oled12864_i2c_t Oled12864_i2c, unsigned char fill_Data)//ȫ�����
{
	unsigned char n, fill[128];
	for (n = 0; n < 128; n++)
		fill[n] = fill_Data;
	for (n = 0; n < 8; n++)
	{
		WriteCmd(Oled12864_i2c, 0xb0 + n);		//page0-page1
		WriteCmd(Oled12864_i2c, 0x00);		//low column start address
		WriteCmd(Oled12864_i2c, 0x10);		//high column start address
		OLED_SetPos(Oled12864_i2c, 0, n);
		I2C_WriteData(Oled12864_i2c, 0x40, fill, 128);
	}
}

void OLED_CLS(Oled12864_i2c_t Oled12864_i2c)//����
{
	OLED_Fill(Oled12864_i2c, 0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(Oled12864_i2c_t Oled12864_i2c)
{
	WriteCmd(Oled12864_i2c, 0X8D);  //���õ�ɱ�
	WriteCmd(Oled12864_i2c, 0X14);  //������ɱ�
	WriteCmd(Oled12864_i2c, 0XAF);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
//--------------------------------------------------------------
void OLED_OFF(Oled12864_i2c_t Oled12864_i2c)
{
	WriteCmd(Oled12864_i2c, 0X8D);  //���õ�ɱ�
	WriteCmd(Oled12864_i2c, 0X10);  //�رյ�ɱ�
	WriteCmd(Oled12864_i2c, 0XAE);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[],unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_ShowStr(Oled12864_i2c_t Oled12864_i2c, unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0, i = 0, j0 = 0, j1 = 0, x0 = x, x1 = x, y0, t0 = 0, str0[128];
	switch (TextSize)
	{
	case 1:                                      //6*8��6���ֽ�
	{
		while (1)
		{
			if (x0 > 122)                        //����Ƿ���
			{
				OLED_SetPos(Oled12864_i2c, x, y);
				I2C_WriteData(Oled12864_i2c,0x40, str0, t0);
				t0 = 0;
				x = 0;
				x0 = 0;
				y++;
				if (y > 7)
					return;                      //������Ļ���� �˳�����
			}
			if (ch[j0] == '\0')                  //����Ƿ�������
			{
				if (t0 != 0)
				{
					OLED_SetPos(Oled12864_i2c, x, y);
					I2C_WriteData(Oled12864_i2c,0x40, str0, t0);
				}
				return;                        //������ �˳�����
			}
			c = ch[j0] - 32;
			for (i = 0; i < 6; i++)
			{
				str0[t0++] = F6x8[c][i];
			}
			x0 += 6;
			j0++;
		}
	}
	case 2:                    //16*8,����16*8�ģ������һ����������Ҳ����ٶഴ��һ������ ֻ���������������ˡ�д��һ��case�Ĵ����ʱ������̬Ҳ�Ǳ�����...
	{
		/*�ȼ���Ƿ���Ҫ����*/
		if (x > 118)
		{
			y += 2;
			x = 0;
			x0 = 0;
			x1 = 0;
			if (y > 6)
				return;                    //������Ļ��Χ ��������
		}

		y0 = y + 1;
		while (1)
		{
			/*�ȴ����ϰ벿��*/
			c = ch[j0] - 32;
			for (i = 0; i < 8; i++)
				str0[t0++] = F8x16[c][i];     //�������ϰ벿�ִ��
			j0++;
			x0 += 8;

			if (x0 > 118)                      //����Ƿ���Ҫ����
			{
				OLED_SetPos(Oled12864_i2c, x, y);
				I2C_WriteData(Oled12864_i2c,0x40,str0, t0);
				t0 = 0;
				x0 = 0;
				y += 2;
				if (y > 6)
					goto a;
			}

			if (ch[j0] == '\0')               //����ϰ벿���Ƿ�������
			{
			a:
				if (t0 != 0)
				{
					OLED_SetPos(Oled12864_i2c, x, y);
					I2C_WriteData(Oled12864_i2c,0x40, str0, t0);
					t0 = 0;
				}

				/*��ʼ����°벿��*/
				while (1)
				{
					c = ch[j1] - 32;
					for (i = 0; i < 8; i++)
						str0[t0++] = F8x16[c][i + 8];     //�������°벿�ִ��
					j1++;
					x1 += 8;

					if (x1 > 118)                      //����Ƿ���Ҫ����
					{
						OLED_SetPos(Oled12864_i2c, x, y0);
						I2C_WriteData(Oled12864_i2c,0x40, str0, t0);
						t0 = 0;
						x1 = 0;
						y0 += 2;
						if (y0 > 7)
							return;
					}

					if (ch[j1] == '\0')
					{
						if (t0 != 0)
						{
							OLED_SetPos(Oled12864_i2c, x, y0);
							I2C_WriteData(Oled12864_i2c,0x40, str0, t0);
						}
						return;
					}
				}
			}
		}
	}
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
// Description    : ��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------
void OLED_ShowCN(Oled12864_i2c_t Oled12864_i2c, unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm = 0, t = 0, str0[16];
	unsigned int  adder = 32 * N;

	for (wm = 0; wm < 16; wm++)
	{
		str0[t++] = F16x16[adder];
		adder += 1;
	}
	OLED_SetPos(Oled12864_i2c, x, y);
	I2C_WriteData(Oled12864_i2c,0x40, str0, t);

	t = 0;
	for (wm = 0; wm < 16; wm++)
	{
		str0[t++] = F16x16[adder];
		adder += 1;
	}
	OLED_SetPos(Oled12864_i2c, x, y + 1);
	I2C_WriteData(Oled12864_i2c,0x40, str0, t);
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(Oled12864_i2c_t Oled12864_i2c,unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_DrawBMP(Oled12864_i2c_t Oled12864_i2c,unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned char x, y;
	x = x1 - x0;
	for (y = y0; y < y1; y++)
	{
		OLED_SetPos(Oled12864_i2c, x0, y);
		I2C_WriteData(Oled12864_i2c,0x40, BMP, x);
		BMP += x;
	}
}

//--------------------------------------------------------------
// Prototype      : void errfunction();
// Calls          : 
// Parameters     : 
// Description    : ��⵽��ʾ��������Զ�������ʾ��(��������Ƭ��)����ʹ��Ļ�����ٲ���Ҳ��������ʹ��
//--------------------------------------------------------------
void errfunction(Oled12864_i2c_t Oled12864_i2c)
{
	delayms(500);
	OLED_CLS(Oled12864_i2c);
}
