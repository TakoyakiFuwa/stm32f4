#include "bmp.h"
#include "ff.h"
#include "U_USART.h"
#include "sdio_sd.h"

FATFS fs;

void Test_BMP(void)
{
	if(f_mount(&fs,"0:",1)!=FR_OK)
	{
		U_Printf("文件系统挂载异常 \r\n");
	}
	else
	{
		U_Printf("文件系统初始化完成 \r\n");
	}
}
