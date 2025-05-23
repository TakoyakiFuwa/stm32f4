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
/*  FATFS  */
#include "ff.h"

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
	Init_Func();
	
	//线程	 建议格式:Task_XXX()
		//进入临界区
	taskENTER_CRITICAL();
		//Func测试
	TaskHandle_t TASK_FUNC_Handler;
	xTaskCreate(Task_Func,"Func",64,NULL,1,&TASK_FUNC_Handler);
	
		//退出临界区
	taskEXIT_CRITICAL();	
	//打印各线程栈
//	BF_Stack();
	//删除自身线程
	vTaskDelete(NULL);
}
/**@brief  命令行创建接口
  *@param  1有匹配 0没匹配转到BaseFunc
  */
uint8_t words_write[] = "HEllo Why cant use Chinese...(?";
UINT words_length = 0;
uint8_t words_read[255];
TCHAR fs_path[] = "0:/fatfs_test.txt";
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
		//FATFS提交测试
	//	有关FATFS 移植时需要注意
	//	->更改SD_GetState中的引脚检测
	//	一直返回存在状态即可
	//	->更改config文件中#define FF_USE_LFN		2
	else if(Command("WRITE"))
	{
		FIL fp;
		U_Printf("这里是关于SD卡的写测试 \r\n");
		if(f_open(&fp,fs_path,FA_WRITE)!=FR_OK)
		{
			U_Printf("未找到文件 正在重新创建 \r\n");
			f_open(&fp,fs_path,FA_CREATE_NEW|FA_WRITE);
		}
		f_write(&fp,words_write,sizeof(words_write),&words_length);
		f_close(&fp);
		U_Printf("写入%d个字符 \r\n",words_length);
	}
	else if(Command("READ"))
	{
		FIL fp;
		U_Printf("这里是关于SD卡的读测试 \r\n");
		f_open(&fp,fs_path,FA_READ);
		f_read(&fp,(void*)words_read,255,&words_length);
		f_close(&fp);
		U_Printf("读取到%d个字符: \r\n%s \r\n",words_length,words_read);
	}
	
	
	//CLI :>
	else if(Command("HELP"))
	{
		U_Printf("HELP	  : 获取可用命令行 \r\n");
		U_Printf("COMMAND : 查看当前程序信息 \r\n");
		U_Printf("RESET	  : 系统重启 \r\n");
		U_Printf("STACK	  : 获取各线程剩余栈 \r\n");
		U_Printf("FUNC	  : 架构库Func.h测试命令行 \r\n");
	}
	else
	{
		return 0;
	}
	return 1;
} 











