#include "SPI_SF.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  stm库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  串口  */
#include "U_USART.h"

/*
 *	spi_software 通过软件实现的SPI通信
 *			2025/2/16-16:58
 */

/*	在写f4_ui的内容
 *	SCL	->	PB13
 *	SDA	->	PB15
 *	PB12	->	CS
 */

/*  接口处理  */
/*  引脚处理  */
#define SPI_SW_SCK_L()		GPIOB->BSRRH = GPIO_Pin_13
#define SPI_SW_SCK_H()		GPIOB->BSRRL = GPIO_Pin_13
#define SPI_SW_MOSI_L()		GPIOB->BSRRH = GPIO_Pin_15
#define SPI_SW_MOSI_H()		GPIOB->BSRRL = GPIO_Pin_15

/**@brief  接口 SPI初始化
  *@param  void
  *@retval void
  */
void SPI_SF_Init(void)
{	
	//引脚时钟初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//常规电平置位
		//GND	->	PD12/PD14
		//VCC	->	PB14/PD9/PD13
	GPIO_WriteBit(GPIOD,GPIO_Pin_12|GPIO_Pin_14,Bit_RESET);
	GPIO_WriteBit(GPIOD,GPIO_Pin_9|GPIO_Pin_13,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
	
		//引脚初始状态
	SPI_SF_CS_H();
	SPI_SW_SCK_L();
	U_Printf("SPI_SF初始化完成 \r\n");
}
/**@brief  发送一个数据
  *@param  data 要发送的数据
  *@retval void
  */
void SPI_SF_Send(uint8_t data)
{
	for(int i=0;i<8;i++)
	{
		if( (data&(0x80>>i))==0 )
		{
			SPI_SW_MOSI_L();
		}
		else 
		{
			SPI_SW_MOSI_H();
		}
		SPI_SW_SCK_H();
		SPI_SW_SCK_L();
	}
}
/**@brief  接收一个数据
  *@param  void
  *@retval 接收到的数据
  */
uint8_t SPI_SF_Receive(void)
{
//	uint8_t data = 0;
//	for(int i=0;i<8;i++)
//	{
//		SPI_SW_SCK_H();
//		if(SPI_SW_MISO()!=0)
//		{
//			data |= (0x80>>i);
//		}
//		SPI_SW_SCK_L();
//	}
//	return data;
	return 0;
}
void SPI_SF_Test(void)
{
	U_Printf("SPI测试 \r\n");
}






