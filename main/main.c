#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "FreeRTOS.h"
#include "task.h"

/*	所以...今天想创建并整理github库...
 *	所以把代码以UTF8的格式...
 *	以及当前觉得很规范的方式...
 *	重新写一遍...
 *			——2025/5/20-14:14
 *	啊...今天是520 偷偷在代码里夹带悄悄话...
 *	很爱moran w 🥰
 */

uint32_t RCC_AHB1Periph_GPIO_LED = RCC_AHB1Periph_GPIOE;
GPIO_TypeDef* GPIO_LED 				 = GPIOE;
uint16_t GPIO_Pin_LED			 = GPIO_Pin_12;	

void Task_LED(void* pvParameters);

/**@brief  主函数 程序入口说是
  */
int main(void)
{
	//引脚初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIO_LED,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_LED;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIO_LED,&GPIO_InitStruct);
	//创建线程
	TaskHandle_t TASK_LED_Handler;
	xTaskCreate(Task_LED,"LED",32,NULL,3,&TASK_LED_Handler);
	vTaskStartScheduler();
}

/**@breif  线程函数 指示灯
  */
void Task_LED(void* pvParameters)
{
	while(1)
	{
		GPIO_WriteBit(GPIO_LED,GPIO_Pin_LED,Bit_SET);
		vTaskDelay(300);
		GPIO_WriteBit(GPIO_LED,GPIO_Pin_LED,Bit_RESET);
		vTaskDelay(600);
	}
}
