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
#include "TFT_UI.h"
#include "TFT_Font.h"
#include "UI_view.h"
extern tft_pointer 	UI_CURSOR;
void Cmd_Botton(void)
{
	int16_t sunrain = 0;
	uint8_t is_sunrain = 0;
	while(USART_Buff[0]!='q' && USART_Buff[0]!='Q')
	{
		vTaskDelay(30);
		if(USART_RX_Signal!=0)
		{
			sunrain = 0;
			is_sunrain = 0;
			USART_RX_Signal=0;
			switch(USART_Buff[0])
			{
			case 'w':case 'W':U_Printf("w\r\n");Other_Button(InFT_pic_up_1616);UI_CURSOR.ui->Func_Event_UP(UI_CURSOR.ui);break;
			case 'a':case 'A':U_Printf("a\r\n");Other_Button(InFT_pic_left_1616);UI_CURSOR.ui->Func_Event_LEFT(UI_CURSOR.ui);break;
			case 's':case 'S':U_Printf("s\r\n");Other_Button(InFT_pic_down_1616);UI_CURSOR.ui->Func_Event_DOWN(UI_CURSOR.ui);break;
			case 'd':case 'D':U_Printf("d\r\n");Other_Button(InFT_pic_right_1616);UI_CURSOR.ui->Func_Event_RIGHT(UI_CURSOR.ui);break;
			case 'e':case 'E':U_Printf("e\r\n");Other_Button(InFT_pic_ok_1616);UI_CURSOR.ui->Func_Event_Other(UI_CURSOR.ui);break;
			case 'r':case 'R':U_Printf("1s之后重启 \r\n");NVIC_SystemReset();break;
			default:U_Printf("None \r\n");
			}
		}
		if(sunrain++>400)
		{
			sunrain=-800;
			if(is_sunrain<2)
			{
				Other_Button(InFT_pic_SunRain_1616);
			}
			is_sunrain++;
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






