#include <STC8G.H>
#include "delay.h"
#include "oled.h"
#include "uart.h"
#include <string.h>

void main()
{
	unsigned char i = 0;
	
	
	OLED_Init();
	OLED_Clear();
	Uart_Init();
	

	Delay(10);
	clrStruct();	//清空缓存数组

	OLED_ShowString(1,1,"Initiating...");
	OLED_ShowString(30,20,"By Eric");
	OLED_ShowString(30,5,"Fireware:v1.0");
	Delay(400);
	OLED_Clear();
	
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();
	}
}


void errorLog(int num)
{
	
	while (1)
	{
	  	UartPrintf("ERROR");
		UartPrintASCII(num+0x30);
		UartPrintf("\r\n");
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		UartPrintf("**************\r\n");
		UartPrintf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void printGpsBuffer()
{
	OLED_Clear();
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		/*OLED显示时间*/
		OLED_ShowString(1,1,strcat("UTC Time:",Save_Data.UTCTime));
		
		UartPrintf("Save_Data.UTCTime = ");
		UartPrintf(Save_Data.UTCTime);
		UartPrintf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			
			//OLED_ShowString(4,1,strcat("Latitude:",Save_Data.latitude));
			
			UartPrintf("Save_Data.latitude = ");
			UartPrintf(Save_Data.latitude);
			UartPrintf("\r\n");


			UartPrintf("Save_Data.N_S = ");
			UartPrintf(Save_Data.N_S);
			UartPrintf("\r\n");

			OLED_ShowString(4,1,strcat("Latitude: ",strcat(Save_Data.latitude,Save_Data.N_S)));

			
			UartPrintf("Save_Data.longitude = ");
			UartPrintf(Save_Data.longitude);
			UartPrintf("\r\n");

			UartPrintf("Save_Data.E_W = ");
			UartPrintf(Save_Data.E_W);
			UartPrintf("\r\n");
			
			OLED_ShowString(7,1,strcat("Longitude: ",strcat(Save_Data.longitude,Save_Data.E_W)));
		}
		else
		{
			UartPrintf("GPS DATA is not usefull!\r\n");
			OLED_ShowString(10,1,"GPS DATA is not usefull!");
		}
		
	}
}
