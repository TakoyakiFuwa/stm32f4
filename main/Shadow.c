#include "Shadow.h"
/*  架构库  */
//#include "Func.h"
#include "BaseFunc.h"
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  外设库  */
#include "U_USART.h"

/*	希望我这次重新写模板可以用的久一点...
 *	想开始做一些很有趣的项目....
 *	以及...很想mo....
 *		——2025/5/20-14:41
 */

/**@brief  用于main中的接口
  */
void Main_Start(void* pvParameters)
{
	//基本功能函数
	BF_Start();
	//初始化 建议格式:Init_XXX()
	Init_USART();
	
	//线程	 建议格式:Task_XXX()
		//进入临界区
	taskENTER_CRITICAL();
	
		//退出临界区
	taskEXIT_CRITICAL();	
	//删除自身线程
	vTaskDelete(NULL);
}
