#pragma once
#ifndef __OLED12864_I2C_H
#define __OLED12864_I2C_H			  	   	
#define uint8_t __uint8_t

#include "OLED12864_I2C.h"
#include "i2c_control.h"
#include "unistd.h"
#include <fcntl.h>

#define SIZE_6X8  1
#define SIZE_8X16 2


#define timelimit 200
#define uint8_t __uint8_t

#define TIMEOUT	3
#define RETRY	3

typedef struct
{
	int FileID;
	uint8_t Address;
}Oled12864_i2c_t;

void delayms(long t);

void I2C_WriteByte(Oled12864_i2c_t Oled12864_i2c, unsigned char reg, unsigned char val);

//--------------------------------------------------------------
// Prototype      : void I2C_WriteData(uint8_t addr,uint8_t *data,unsigned char length);
// Calls          : 
// Parameters     : addr,�Ĵ�����ַ����ʾ��������������ݰ�;*data,���ݰ�����;length,���ݰ�����
// Description    : ����д����
//--------------------------------------------------------------

void I2C_WriteData(Oled12864_i2c_t Oled12864_i2c,unsigned char reg, unsigned char* val, int len);


void WriteCmd(Oled12864_i2c_t Oled12864_i2c, unsigned char reg, unsigned char val);

void OLED_Init(Oled12864_i2c_t Oled12864_i2c);

void OLED_SetPos(Oled12864_i2c_t Oled12864_i2c,unsigned char x, unsigned char y);

void OLED_Fill(Oled12864_i2c_t Oled12864_i2c, unsigned char fill_Data);

void OLED_CLS(Oled12864_i2c_t Oled12864_i2c);

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(Oled12864_i2c_t Oled12864_i2c);

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
//--------------------------------------------------------------
void OLED_OFF(Oled12864_i2c_t Oled12864_i2c);

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[],unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_ShowStr(Oled12864_i2c_t Oled12864_i2c, unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
// Description    : ��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------
void OLED_ShowCN(Oled12864_i2c_t Oled12864_i2c, unsigned char x, unsigned char y, unsigned char N);

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_DrawBMP(Oled12864_i2c_t Oled12864_i2c, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

//--------------------------------------------------------------
// Prototype      : void errfunction();
// Calls          : 
// Parameters     : 
// Description    : ��⵽��ʾ��������Զ�������ʾ��(��������Ƭ��)����ʹ��Ļ�����ٲ���Ҳ��������ʹ��
//--------------------------------------------------------------
void errfunction(Oled12864_i2c_t Oled12864_i2c);

#endif  