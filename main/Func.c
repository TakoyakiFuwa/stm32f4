#include "Func.h"
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  外设库  */
#include "U_USART.h"
#include "BMP.h"
#include "TFT_ST7735.h"
/*  FATFS  */
//#include "ff.h"
#include "UR_USART.h"

/**@brief  Func初始化
  */
void Init_Func(void)
{
	U_Printf("Func初始化完成 \r\n");
}
/**@brief  Func线程示例
  */
void Task_Func(void* pvParameters)
{
	while(1)
	{
		vTaskDelay(200);
	}
}
void U_RGB(uint16_t rgb565)
{
	UR_Putchar((rgb565>>8));
	UR_Putchar((rgb565&0xFF));
}
/**@brief  Func命令行接口
  */
void Cmd_Func(void)
{
	char tt_a = 'a';
	uint16_t tt = tt_a<<8;
	tt_a = 'b';
	tt |= tt_a;
	
	U_RGB(tt);//0011 1010 0100 1100
	U_Printf("\r\n\r\n这里是Func命令行测试 \r\n");
}








