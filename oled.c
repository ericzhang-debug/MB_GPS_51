#include <STC8G.H>
#include "OLED_Font.h"

#define uint8_t unsigned char 
#define uint16_t unsigned int
#define uint32_t unsigned long  	  
	
//----------------------------------------------------------------
sbit OLED_SCL=P2^5;//SCL
sbit OLED_SDIN=P2^4;//SDA
//----------------------------------------------------------------


//开始
void IIC_Start()
{
	OLED_SCL = 1;
	OLED_SDIN = 1;
	OLED_SDIN = 0;
	OLED_SCL = 0;
}

//停止
void IIC_Stop()
{
	OLED_SCL = 1 ;
	OLED_SDIN = 0;
	OLED_SDIN = 1;

}

void IIC_Wait_Ack()
{
	OLED_SCL = 1 ;
	OLED_SCL = 0;
}

//发送一个字节
void Write_IIC_Byte(uint8_t IIC_Byte)
{
	uint8_t i;
	uint8_t m, da;
	da = IIC_Byte;
	OLED_SCL = 0;
	for(i = 0; i < 8; i++)
	{
		m = da;
		m = m & 0x80;
		if(m == 0x80)
		{
			OLED_SDIN = 1;
		}
		else OLED_SDIN = 0;
		da = da << 1;
		OLED_SCL = 1;
		OLED_SCL = 0;
	}
}

//发送命令
void Write_IIC_Command(uint8_t IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00);
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

//发送数据
void Write_IIC_Data(uint8_t IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40);
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

//设置坐标
void OLED_Set_Pos(uint8_t Y, uint8_t X)
{
	Write_IIC_Command(0xB0 | Y);
	Write_IIC_Command(0x10 | ((X & 0xF0) >> 4));
	Write_IIC_Command(0x00 | (X & 0x0F));
}
//清屏
void OLED_Clear()
{
	uint8_t i, j;
	for(i = 0; i < 8; i++)
	{
		OLED_Set_Pos(i,0);
		for(j = 0; j < 128; j++)
		{
			Write_IIC_Data(0x00);
		}
	}
}
//平方
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

// OLED显示字符
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
	uint8_t i;
	OLED_Set_Pos((Line - 1) * 2, (Column - 1) * 8);
	for (i = 0; i < 8; i++)
	{
		Write_IIC_Data(F8x16[Char - ' '][i]);
	}
	OLED_Set_Pos((Line - 1) * 2 + 1, (Column - 1) * 8);
	for (i = 0; i < 8; i++)
	{
		Write_IIC_Data(F8x16[Char - ' '][i + 8]);
	}
}

// OLED显示字符串
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}
// OLED显示数字
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

//OLED显示数字（十进制，带符号数）
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

//OLED显示数字（十六进制，正数）
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}
//OLED显示数字（二进制，正数）
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}


//void OLED_Init(void)
void OLED_Init()
{
	Write_IIC_Command(0xAE); //--display off
	Write_IIC_Command(0x00); //---set low column address
	Write_IIC_Command(0x10); //---set high column address
	Write_IIC_Command(0x40); //--set start line address
	Write_IIC_Command(0xB0); //--set page address
	Write_IIC_Command(0x81); // contract control
	Write_IIC_Command(0xFF); //--128
	Write_IIC_Command(0xA1); //set segment remap
	Write_IIC_Command(0xA6); //--normal / reverse
	Write_IIC_Command(0xA8); //--set multiplex ratio(1 to 64)
	Write_IIC_Command(0x3F); //--1/32 duty
}