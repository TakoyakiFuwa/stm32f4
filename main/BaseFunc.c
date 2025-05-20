#include "BaseFunc.h"
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"

/*	用来存放系统启动的基本功能
 *	包括指示灯/看门狗/命令行等
 *		:)
 *		——2025/5/20-14:35
 */

/*  指示灯  */
#define BF_LED_R	RCC_AHB1Periph_GPIOE
#define BF_LED_G	GPIOE
#define BF_LED_P	GPIO_Pin_12

static void BF_Init_LED(void);
static void BF_Task_LED(void* pvParameters);

/**@brief  初始化函数
  */
void BF_Start(void)
{
	//初始化
		//指示灯
	BF_Init_LED();
	//进入临界区
	taskENTER_CRITICAL();
		//指示灯&看门狗复位
	TaskHandle_t BF_TASK_LED_Handler;
	xTaskCreate(BF_Task_LED,"BF_LED",16,NULL,8,&BF_TASK_LED_Handler);
	//退出临界区
	taskEXIT_CRITICAL();
}


/**@brief  看门狗初始化
  */
static void BF_Init_WDG(void)
{
	
}
/**@brief  指示灯引脚初始化
  */
static void BF_Init_LED(void)
{
	//引脚初始化
	RCC_AHB1PeriphClockCmd(BF_LED_R,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = BF_LED_P;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BF_LED_G,&GPIO_InitStruct);
	GPIO_WriteBit(BF_LED_G,BF_LED_P,Bit_SET);
}
/**@breif  线程函数 指示灯
  *@add	   因为含有看门狗复位 所以优先级应该高些
  */
static void BF_Task_LED(void* pvParameters)
{
	while(1)
	{
		GPIO_WriteBit(BF_LED_G,BF_LED_P,Bit_SET);
		vTaskDelay(300);
		GPIO_WriteBit(BF_LED_G,BF_LED_P,Bit_RESET);
		vTaskDelay(600);
	}
}


