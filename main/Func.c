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

FATFS fs;

/**@brief  Func初始化
  */
void Init_Func(void)
{
	if(f_mount(&fs,"0:",1)!=FR_OK)
	{
		U_Printf("SD卡挂载异常,代码:%d \r\n",f_mount(&fs,"0:",1));
	}
	else
	{
		U_Printf("SD卡正常挂载 \r\n");
	}
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
/**@brief  Func命令行接口
  */
void Cmd_Func(void)
{
	char example_words[] = {"你好呢w Hello there!"};
	
	uint8_t length;

	FIL fp;
	f_open(&fp,"0:/bca.txt",FA_WRITE);
	f_write(&fp,example_words,sizeof(example_words),&length);
	U_Printf("写入完成%d \r\n",length);
	f_close(&fp);
	f_open(&fp,"0:/bca.txt",FA_READ);
	char words[255];
	f_read(&fp,(void*)words,255,(UINT*)&length);
	U_Printf("读取到%d个字符 输出:%s \r\n",length,words);
	U_Printf("这里是Func命令行测试 \r\n");
}








