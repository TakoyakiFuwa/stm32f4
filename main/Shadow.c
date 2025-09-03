#include "Shadow.h"
/*  架构库  */
#include "Func.h"
#include "BaseFunc.h"
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  外设库  */
#include "U_USART.h"
#include "TFT_Font.h"
#include "TFT_UI.h"
#include "U1.h"
/*  FATFS  */
#include "ff.h"
/*  BMP  */
#include "proj_file.h"


/*	希望我这次重新写模板可以用的久一点...
 *	想开始做一些很有趣的项目....
 *	以及...很想mo....
 *		——2025/5/20-14:41
 */

extern tft_pointer UI_CURSOR;
/**@brief  用于main中的接口
  */
void Main_Start(void* pvParameters)
{
	//基本功能函数
	BF_Start();
	//初始化 建议格式:Init_XXX()
	Init_Func();
		//TFT_Font初始化(字体)
	Init_TFTF();
		//TFT_UI测试
	Init_UI();
		//BMP测试
	Init_Proj();
		//接口初始化
	Init_U1();
	//线程	 建议格式:Task_XXX()
		//进入临界区
	taskENTER_CRITICAL();
		//Func测试
	TaskHandle_t TASK_FUNC_Handler;
	xTaskCreate(Task_Func,"Func",64,NULL,1,&TASK_FUNC_Handler);
		//渲染
	TaskHandle_t TASK_RENDER_Handler;
	xTaskCreate(Task_Render,"Render",128,NULL,1,&TASK_RENDER_Handler);
		//下位机接口
	xTaskCreate(Task_U1Command,"U1_Command",512,NULL,3,NULL);
		//按键
	xTaskCreate(Task_Button,"Button",512,NULL,4,NULL);
		//退出临界区
	taskEXIT_CRITICAL();
		//加载第一张图片
	UI_CURSOR.ui->Func_Event_DOWN(UI_CURSOR.ui);
	//打印各线程栈
	BF_Stack();
	//删除自身线程
	vTaskDelete(NULL);
}
/**@brief  命令行创建接口
  *@param  1有匹配 0没匹配转到BaseFunc
  */
extern uint8_t USART_Buff[512];
int8_t Cmd(void)
{
	//COMMAND
	if(Command("COMMAND"))
	{
		U_Printf("这里是F407VE的模板程序，关于FATFS的测试示例文件 \r\n");
	}
		//FUNC测试
	else if(Command("FUNC"))
	{
		Cmd_Func();
	}
	else if(Command("B"))
	{
		U_Printf("进入模拟按键 \r\n");
		Cmd_Botton();
		U_Printf("退出模拟按键 \r\n");
	}
	else if(Command("U1"))
	{
		U1_SendWords((const char*)&USART_Buff[3]);
	}
	
	//CLI :>
	else if(Command("HELP"))
	{
		U_Printf("HELP	  : 获取可用命令行 \r\n");
		U_Printf("COMMAND : 查看当前程序信息 \r\n");
		U_Printf("RESET	  : 系统重启 \r\n");
		U_Printf("STACK	  : 获取各线程剩余栈 \r\n");
		U_Printf("FUNC	  : 架构库Func.h测试命令行 \r\n");
		U_Printf("B		  : 进入wasd模拟按键模式 \r\n");
	}
	else
	{
		return 0;
	}
	return 1;
} 











