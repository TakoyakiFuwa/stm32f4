#include "U1.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t U1_buff[50];
uint8_t U1_Signal_Cmd = 0;

void Init_U1(void)
{
	//外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//外设初始化
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 4800;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	//引脚初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//中断初始化
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 10;
	NVIC_Init(&NVIC_InitStruct);
	//开启外设
	USART_Cmd(USART1,ENABLE);
}

void U1_Putchar(char _char)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=1);
	USART_SendData(USART1,_char);
}
void U1_SendWords(const char* words)
{
	for(int i=0;words[i]!='\0';i++)
	{
		U1_Putchar(words[i]);
	}
}
int8_t U1_Command(const char* words)
{
	for(int i=0;words[i]!='\0';i++)
	{
		if(words[i]!=U1_buff[i])
		{
			return 0;
		}
	}
	U1_Signal_Cmd = 0;
	return 1;
}
#include "bmp.h"
#include "proj_file.h"
#include "TFT_ST7735.h"
char bmp_filename[20];
void BMP_FileName(const char* words)
{
	for(int i=0;words[i]!='\0';i++)
	{
		bmp_filename[i] = words[i];
	}
}
void Render_WQ(uint16_t data)
{
	U1_Putchar( (data>>8) );
	U1_Putchar(data);
	TFT_Write16Data(data);
}
void SetRect_WQ(uint16_t width,uint16_t height)
{
	Proj_SetRectNormal(width,height);
	uint8_t count = 0;
	for(;bmp_filename[count]!='\0';count++)
	{
		if(count>=9)
		{
			break;
		}
		U1_Putchar(bmp_filename[count]);
	}
	while(count<9)
	{
		count++;
		U1_Putchar(' ');
	}
	//发送尺寸数据
	U1_Putchar(width);
	U1_Putchar(height);
}
char f1_filename[50];
char f1_filepath[50];
uint8_t f1_dwidth = 0;
uint8_t f1_fixpx = 0;
void DataToF1(void)
{
	BMP_FileName(f1_filename);
	TFT_SetRotation(Rota_BMP);
	BMP_AdjustBMP(f1_filepath,f1_dwidth,f1_fixpx,160,128,Render_WQ,SetRect_WQ);
	TFT_SetRotation(Rota_UI);
}
#include "U_USART.h"
#include "TFT_UI.h"
void Task_U1Command(void* pvParameters)
{
	while(1)
	{
		vTaskDelay(50);
		if(U1_Signal_Cmd==0)
		{
			continue;
		}
		else if(U1_Command("DataIn"))
		{
			vTaskDelay(100);
			U_Printf("准备处理...");
			vTaskDelay(200);
			DataToF1();
		}
		else if(U1_Command("DataEnd"))
		{
			U_Printf("处理结束(?) \r\n");
			UI_ChangePage(InPG_View);
		}
		
	}
}
#include "U_USART.h"
void USART1_IRQHandler(void)
{
	static uint16_t u1_index = 0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		U1_buff[u1_index++] = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE)==SET)
	{
		U1_Signal_Cmd = 1;
		U1_buff[u1_index] = '\0';
		u1_index = 0;
		U_Printf("接收到数据:%s \r\n",U1_buff);
		//清除空闲标志位
		USART1->SR;USART1->DR;
	}
}

