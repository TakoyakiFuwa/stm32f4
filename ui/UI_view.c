#include "UI_view.h"
#include "TFT_UI.h"
#include "TFT_font.h"
#include "UI_Instance.h"
#include "TFT_ST7735.h"

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

void Render_View_BackGround(tft_ui* u)
{
//	TFTF_DrawRect(0,0,114,85,InCor_Purple);
	TFTF_DrawRect(0,85,180,50,InCor_Blue);
	TFTF_DrawRect(114,0,50,50,InCor_Pink);
	TFTF_DrawRect(114,50,50,35,InCor_White);
	TFTF_Put01(115,53,InFT_pic_SunRain_2424,InCor_Black,InCor_White);
	TFTF_DrawFrame(140,54,18,28,InCor_Red,1);
	TFTF_DrawRect(141,55,5,26,InCor_Pink);
	TFTF_DrawRect(146,55,5,26,InCor_White);
	TFTF_DrawRect(152,55,5,26,InCor_Blue);
	TFTF_DrawRect(115,80,23,2,InCor_Black);
	TFTF_DrawRect(116,42,42,2,InCor_White);
	
	//信息测试
	TFTF_ShowString(116,3,"W:",2,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
	TFTF_ShowString(116,20,"H:",2,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
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
	TFTF_ShowNum(132,3,u->parameter[0],3,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
	TFTF_ShowNum(132,20,u->parameter[1],3,InFT_font_Consolas_1608,InCor_Black,InCor_Pink);
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
void Page_View(void)
{
	//背景框架
	UI[InUI_view_background] = UI_CreateUI(0,0,InFT_font_Pixel_1608,InCor_White,InCor_Black,Render_View_BackGround);
	//更改bmp文件
	UI[InUI_view_change] = UI_CreateUI(2,94,InFT_font_Pixel_1608,InCor_Black,InCor_White,Render_View_Change);
	INS_Text(&UI[InUI_view_change],"XXOO.bmp",10);
	INS_LeftRight(&UI[InUI_view_change],InUI_view_change,InUI_view_overview);
	//图片上传
	UI[InUI_view_upload] = UI_CreateUI(109,111,InFT_font_Pixel_1608,InCor_Black,InCor_White,Render_Text);
	INS_Text(&UI[InUI_view_upload],"Load >",6);
	INS_UpDown(&UI[InUI_view_upload],InUI_view_overview,InUI_view_upload);
	UI[InUI_view_upload].Func_Event_LEFT = Event_ToViewChange;
	//图片放大
	UI[InUI_view_overview] = UI_CreateUI(109,94,InFT_font_Pixel_1608,InCor_Black,InCor_White,Render_Text);
	INS_Text(&UI[InUI_view_overview],"View >",6);
	INS_UpDown(&UI[InUI_view_overview],InUI_view_overview,InUI_view_upload);
	UI[InUI_view_overview].Func_Event_LEFT = Event_ToViewChange;
	//按键记忆
	UI[InUI_view_button] = UI_CreateUI(2,111,InFT_font_Consolas_1608,InCor_Green,InCor_White,Render_View_Button);
	for(int i=0;i<3;i++)
	{
		UI[InUI_view_button].value_text[i] = InFT_pic_SunRain_1616;
	}
	//BMP信息
	UI[InUI_view_infor] = UI_CreateUI(0,0,InFT_font_Pixel_1608,InCor_Black,InCor_Pink,Render_View_Infor);
	
	
	//创建页面
	uint16_t ui_index[] = {
		InUI_view_background
		,InUI_view_change
		,InUI_view_upload
		,InUI_view_overview
		,InUI_view_button
		,InUI_view_infor
	};
	UI_CreatePage(InPG_View,ui_index,sizeof(ui_index)/sizeof(uint16_t),&UI[InUI_view_change]);
}


