#include "proj_file.h"
#include "bmp.h"
#include "TFT_ST7735.h"
#include "U_USART.h"
#include "ff.h"

/*	完全适配当前项目的h/c文件...
 *	写的好烂....写的好烂....写的好烂....写的好烂....
 *			——2025/8/30-12:27.秦羽
 */
const char PROJ_DIR[] = {"0:/B/"}; 
uint16_t FILE_NUM = 0;

void Init_Proj(void)
{
	Init_BMP();
	//初始化获取文件夹内容
	DIR dr;
	if(f_opendir(&dr,PROJ_DIR)!=FR_OK)
	{
		U_Printf("文件夹打开异常 \r\n");
	}
	FILINFO infor;
	f_readdir(&dr,&infor);
	FILE_NUM = 0;
	while(infor.fname[0]!=0)
	{
		U_Printf("读取文件[%d]:%s \r\n",FILE_NUM,infor.fname);
		if(f_readdir(&dr,&infor)!=FR_OK)
		{
			U_Printf("文件读取异常 \r\n");
			break;
		}
		FILE_NUM++;
	}
	FILE_NUM--;
	f_closedir(&dr);
	U_Printf("文件总数:%d \r\n",FILE_NUM);
}
void Proj_index(uint16_t index,char* file,char* file_name)
{
	DIR dr;
	if(f_opendir(&dr,PROJ_DIR)!=FR_OK)
	{
		U_Printf("文件夹打开异常 \r\n");
	}
	FILINFO infor;
	f_readdir(&dr,&infor);
	for(int i=0;i<index;i++)
	{
		if(f_readdir(&dr,&infor)!=FR_OK)
		{
			U_Printf("文件读取异常 \r\n");
			break;
		}
	}
	f_closedir(&dr);
	int i=0;
	for(i=0;infor.fname[i]!='\0';i++)
	{
		file_name[i] = infor.fname[i];
	}
	file_name[i] = '\0';
	i=0;
	for(i=0;PROJ_DIR[i]!='\0';i++)
	{
		file[i] = PROJ_DIR[i];
	}
	for(int j=0;file_name[j]!='\0';j++)
	{
		file[i++] = file_name[j];
	}
	file[i] = '\0';
	for(int i=0;file_name[i]!='\0';i++)
	{
		if(file_name[i]=='.')
		{
			file_name[i]='\0';
			break;
		}
	}
	
//	U_Printf("file:%s \\ file_name:%s \r\n",file,file_name);
}

uint16_t Proj_bmp(char* file,char* file_name,int8_t num_change)
{
	static uint16_t index = 0;
	if(num_change<0)
	{
		if(index>0)
		{
			index--;
		}
		else
		{
			index = FILE_NUM;
		}
	}
	else
	{
		if(index<FILE_NUM)
		{
			index++;
		}
		else
		{
			index = 0;
		}
	}
	Proj_index(index,file,file_name);
	
	return index;
}
#include "TFT_UI.h"
#include "UI_view.h"
extern tft_ui UI[];
void Proj_SetRectLU(uint16_t width,uint16_t height)
{
	//更改数据
	UI[InUI_view_infor].parameter[0] = width;
	UI[InUI_view_infor].parameter[1] = height; 
	UI_AddRender(&UI[InUI_view_infor]);
	TFT_SetCursor(43+(85-height)/2,(114-width)/2,height,width);
}
void Proj_SetRectNormal(uint16_t width,uint16_t height)
{
	TFT_SetCursor((128-height)/2,(160-width)/2,height,width);
}
void Test_Proj(void)
{
	//初始化
	Init_Proj();
	/*
	DIR dr;
	if(f_opendir(&dr,"0:/B")!=FR_OK)
	{
		U_Printf("文件夹打开异常 \r\n");
	}
	FILINFO infor;
	f_readdir(&dr,&infor);
	while(infor.fname[0]!= 0)
	{	
		U_Printf("读取:%s \r\n",infor.fname);
		if(f_readdir(&dr,&infor)!=FR_OK)
		{
			U_Printf("文件夹读取异常 \r\n");
			break;
		}
	}
	f_closedir(&dr);
	
	
	char a[50],b[50];
	Proj_index(5,a,b);
	
	TFT_SetRotation(Rota_BMP);
	vTaskDelay(500);
	BMP_AdjustBMP(a,1,0,114,85,TFT_Write16Data,Proj_SetRectLU);
//	BMP_AdjustBMP(a,1,0,160,128,TFT_Write16Data,Proj_SetRectNormal);
	TFT_SetRotation(Rota_UI);
	vTaskDelay(500);
	
	U_Printf("Proj测试区 \r\n");
	*/
}

/*
	BMP_AdjustBMP("0:/B.bmp",0,0,114,85,TFT_Write16Data,SetRect);

SetRect
	TFT_SetCursor(43+(85-height)/2,(114-width)/2,height,width);
*/


