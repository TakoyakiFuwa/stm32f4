#include "Func.h"
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  外设库  */
#include "U_USART.h"
/*  FATFS  */
#include "ff.h"


/**@brief  Func初始化
  */
void Init_Func(void)
{

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
/**@brief  Func命令行接口
  */
void Cmd_Func(void)
{

	U_Printf("这里是Func命令行测试 \r\n");
}

/**@brief  用串口模拟软件按键
  */
extern uint8_t USART_Buff[512];	
extern int8_t 	USART_RX_Signal;
void Cmd_Botton(void)
{
	while(USART_Buff[0]!='q' && USART_Buff[0]!='Q')
	{
		vTaskDelay(10);
		if(USART_RX_Signal!=0)
		{
			USART_RX_Signal=0;
			switch(USART_Buff[0])
			{
			case 'w':case 'W':U_Printf("w\r\n");break;
			case 'a':case 'A':U_Printf("a\r\n");break;
			case 's':case 'S':U_Printf("s\r\n");break;
			case 'd':case 'D':U_Printf("d\r\n");break;
			default:U_Printf("None \r\n");
			}
		}
	}
}
/**@brief  渲染
  */
#include "TFT_UI.h"
void Task_Render(void* pvParameters)
{
	while(1)
	{
		vTaskDelay(100);
		CircleRender_UI();
	}
}






