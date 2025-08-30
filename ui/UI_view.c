#include "UI_view.h"
#include "TFT_UI.h"
#include "TFT_font.h"
#include "UI_Instance.h"
#include "TFT_ST7735.h"
#include "proj_file.h"
#include "bmp.h"

/*	界面写的乱七八糟....
 *	好多没什么意义的...装饰线条(?)
 *	只有那个蓝粉白旗还算满意....
 *	真可爱呢...
 *			——2025/8/27-21:59.秦羽
 */

/*  UI全局变量  */
extern tft_pointer 	UI_CURSOR;
extern tft_ui 		UI[];
extern tft_page		PAGE[];

/*  BMP全局变量  */
uint16_t index_bmp = 0;
int8_t d_width=0,pix=0;
char file[50];

void Render_View_BackGround(tft_ui* u)
{
//	TFTF_DrawRect(0,0,114,85,InCor_Purple);
	TFTF_DrawRect(0,85,180,50,InCor_Blue);
	TFTF_DrawRect(114,0,50,50,InCor_Pink);
	TFTF_DrawRect(114,50,50,35,InCor_White);
	TFTF_Put01(115,53,InFT_pic_SunRain_2424,InCor_Black,InCor_White);
	//蓝粉白旗
	TFTF_DrawFrame(140,54,18,28,InCor_Red,1);
	TFTF_DrawRect(141,55,5,26,InCor_Pink);
	TFTF_DrawRect(146,55,5,26,InCor_White);
	TFTF_DrawRect(152,55,5,26,InCor_Blue);
	TFTF_DrawRect(115,80,23,2,InCor_Black);
	TFTF_DrawRect(116,32,42,2,InCor_White);	
	//信息测试
	TFTF_ShowString(116,0,"W:",2,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
	TFTF_ShowString(116,15,"H:",2,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
	TFTF_ShowString(116,34,"Fix:",4,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
	//诡异的画点
	uint16_t color_a = InCor_White;
	TFTF_DrawRect(2,88,100,3, color_a);
	TFTF_DrawRect(104,88,3,35,color_a);
	TFTF_DrawRect(107,88,15,3,color_a);
	for(int i=0;i<5;i++)
	{
		TFTF_DrawRect(125+i*6,88,3,3,color_a);
	}
}
void Render_View_Change(tft_ui* u)
{
	TFTF_ShowString(u->x+20,u->y,u->value_text,u->parameter[2],u->InFT,u->ft_color,u->bk_color);
	TFTF_DrawRect(u->x+16,u->y,4,16,u->bk_color);
	TFTF_Put01(u->x,u->y,InFT_pic_UpDown_1616,u->ft_color,u->bk_color);
}
void Event_ToViewChange(tft_ui* u)
{
	UI_Cursor_ChangeUI(&UI[InUI_view_change]);
}
void Render_View_Infor(tft_ui* u)
{
	TFTF_ShowNum(132,0,u->parameter[0],3,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
	TFTF_ShowNum(132,15,u->parameter[1],3,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
}
void Render_View_BmpFix(tft_ui* u)
{
	TFTF_ShowNum(148,34,u->value_num,1,u->InFT,u->ft_color,u->bk_color);
}
void Other_BmpFix(tft_ui* u)
{
	uint8_t param = u->value_num++;
	if(u->value_num>=0x08)
	{
		u->value_num = 0;
	}
	if((param&0x01)!=0)
	{
		d_width = 1;
	}
	else
	{
		d_width = 0;
	}
	if( (param&0x02)!=0 )
	{
		d_width *= -1;
	}
	if( (param&0x04) != 0)
	{
		pix = 1;
	}
	else
	{
		pix = 0;
	}
	UI_AddRender(u);
	TFT_SetRotation(Rota_BMP);
	BMP_AdjustBMP(file,d_width,pix,114,85,TFT_Write16Data,Proj_SetRectLU);
	TFT_SetRotation(Rota_UI);
}
void Other_Infor(uint8_t width,uint8_t height)
{
	UI[InUI_view_infor].parameter[0] = width;
	UI[InUI_view_infor].parameter[1] = height;
}
void Render_View_Button(tft_ui* u)
{
	TFTF_DrawRect(u->x,u->y,100,16,u->bk_color);
	//缺乏前处理
	for(int i=0;i<5;i++)
	{
		TFTF_Put01(u->x+6+i*18,u->y,u->value_text[i],u->ft_color,u->bk_color);
	}
}
void Other_Button(uint8_t InFT)
{
	for(int i=9;i>0;i--)
	{
		UI[InUI_view_button].value_text[i] = UI[InUI_view_button].value_text[i-1];
	}
	UI[InUI_view_button].value_text[0] = InFT;
	UI_AddRender(&UI[InUI_view_button]);
}
void UP_ViewChange(tft_ui* u)
{
	char b[50];
	index_bmp = Proj_bmp(file,b,1);
	//UI重新加载
	Other_StringCpy(u->value_text,b);
	UI_AddRender(u);
	//图片刷新
	TFTF_DrawRect(0,0,114,85,InCor_Purple);
	TFT_SetRotation(Rota_BMP);
	BMP_AdjustBMP(file,d_width,pix,114,85,TFT_Write16Data,Proj_SetRectLU);
	TFT_SetRotation(Rota_UI);
}
void DOWN_ViewChange(tft_ui* u)
{
	char b[50];
	index_bmp = Proj_bmp(file,b,-1);
	//UI重新加载
	Other_StringCpy(u->value_text,b);
	UI_AddRender(u);
	//图片刷新
	TFTF_DrawRect(0,0,114,85,InCor_Purple);
	TFT_SetRotation(Rota_BMP);
	BMP_AdjustBMP(file,d_width,pix,114,85,TFT_Write16Data,Proj_SetRectLU);
	TFT_SetRotation(Rota_UI);
}
void Left_OverView(tft_ui* u)
{
	if(u->value_num!=0)
	{
		u->value_num = 0;
		UI_ChangePage(InPG_View);
		TFTF_DrawRect(0,0,114,85,InCor_Purple);
		TFT_SetRotation(Rota_BMP);
		BMP_AdjustBMP(file,d_width,pix,114,85,TFT_Write16Data,Proj_SetRectLU);
		TFT_SetRotation(Rota_UI);
	}
	else
	{
		Event_ToViewChange(u);
	}
	
}
void RIGHT_OverView(tft_ui* u)
{
	TFTF_DrawRect(0,0,160,128,InCor_Purple);
	char b[50];
	Proj_index(index_bmp,file,b);
	TFT_SetRotation(Rota_BMP);
	BMP_AdjustBMP(file,d_width,pix,160,128,TFT_Write16Data,Proj_SetRectNormal);
	TFT_SetRotation(Rota_UI);
	u->value_num = 1;
	UI[InUI_view_button].is_present = 0;
}





void Page_View(void)
{
	//背景框架
	UI[InUI_view_background] = UI_CreateUI(0,0,InFT_font_Pixel_1608,InCor_White,InCor_Black,Render_View_BackGround);
	//更改bmp文件
	UI[InUI_view_change] = UI_CreateUI(2,94,InFT_font_Pixel_1608,InCor_Black,InCor_White,Render_View_Change);
	INS_Text(&UI[InUI_view_change],"XXOO.bmp",10);
	INS_LeftRight(&UI[InUI_view_change],InUI_view_change,InUI_view_overview);
	UI[InUI_view_change].Func_Event_UP = UP_ViewChange;
	UI[InUI_view_change].Func_Event_DOWN = DOWN_ViewChange;
	//图片上传
	UI[InUI_view_upload] = UI_CreateUI(109,111,InFT_font_Pixel_1608,InCor_Black,InCor_White,Render_Text);
	INS_Text(&UI[InUI_view_upload],"Load >",6);
	INS_UpDown(&UI[InUI_view_upload],InUI_view_overview,InUI_view_upload);
	UI[InUI_view_upload].Func_Event_LEFT = Event_ToViewChange;
	//图片放大
	UI[InUI_view_overview] = UI_CreateUI(109,94,InFT_font_Pixel_1608,InCor_Black,InCor_White,Render_Text);
	INS_Text(&UI[InUI_view_overview],"View >",6);
	INS_UpDown(&UI[InUI_view_overview],InUI_view_bmpfix,InUI_view_upload);
	UI[InUI_view_overview].Func_Event_LEFT = Left_OverView;
	UI[InUI_view_overview].Func_Event_RIGHT = RIGHT_OverView;
	//按键记忆
	UI[InUI_view_button] = UI_CreateUI(2,111,InFT_font_Consolas_1608,InCor_Green,InCor_White,Render_View_Button);
	for(int i=0;i<3;i++)
	{
		UI[InUI_view_button].value_text[i] = InFT_pic_SunRain_1616;
	}
	//BMP信息
	UI[InUI_view_infor] = UI_CreateUI(0,0,InFT_font_Consolas_1608,InCor_Black,InCor_Pink,Render_View_Infor);
	//BMP修正
	UI[InUI_view_bmpfix] = UI_CreateUI(0,0,InFT_font_Pixel_1608,InCor_Black,InCor_Blue,Render_View_BmpFix);
	INS_UpDown(&UI[InUI_view_bmpfix],InUI_view_bmpfix,InUI_view_overview);
	UI[InUI_view_bmpfix].Func_Event_UP = Other_BmpFix;
	UI[InUI_view_bmpfix].Func_Event_LEFT = Other_BmpFix;
	UI[InUI_view_bmpfix].Func_Event_RIGHT = Other_BmpFix;
	
	//创建页面
	uint16_t ui_index[] = {
		InUI_view_background
		,InUI_view_change
		,InUI_view_upload
		,InUI_view_overview
		,InUI_view_button
		,InUI_view_infor
		,InUI_view_bmpfix
	};
	UI_CreatePage(InPG_View,ui_index,sizeof(ui_index)/sizeof(uint16_t),&UI[InUI_view_change]);
}


