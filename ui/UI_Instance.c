#include "UI_Instance.h"
#include "TFT_UI.h"
#include "TFT_Font.h"
#include "TFT_ST7735.h"

/*  UI全局变量  */
extern tft_pointer 	UI_CURSOR;
extern tft_ui 		UI[];
extern tft_page		PAGE[];

/*  渲染函数  */
void Render_Text(tft_ui* u)
{
	TFTF_ShowString(u->x,u->y,u->value_text,u->parameter[2],u->InFT,u->ft_color,u->bk_color);
}
void Render_Num(tft_ui* u)
{
	TFTF_ShowNum(u->x,u->y,u->value_num,u->parameter[2],u->InFT,u->ft_color,u->bk_color);
}

/*  常用事件函数  */
void Event_Front(tft_ui* u)
{
	UI_Cursor_ChangeUI(&UI[u->parameter[0]]);
}
void Event_Next(tft_ui* u)
{
	UI_Cursor_ChangeUI(&UI[u->parameter[1]]);
}
void Event_NumAdd(tft_ui* u)
{
	u->value_num++;
	UI_AddRender(u);
}
void Event_NumMul(tft_ui* u)
{
	if(u->value_num>0)
	{
		u->value_num--;
	}
	UI_AddRender(u);
}

/*  前后件绑定  */
void INS_UpDown(tft_ui* u,uint16_t front_ui,uint16_t next_ui)
{
	u->parameter[0] = front_ui;
	u->parameter[1] = next_ui;
	u->Func_Event_UP = Event_Front;
	u->Func_Event_DOWN = Event_Next;
}
void INS_LeftRight(tft_ui* u,uint16_t front_ui,uint16_t next_ui)
{
	u->parameter[0] = front_ui;
	u->parameter[1] = next_ui;
	u->Func_Event_LEFT = Event_Front;
	u->Func_Event_RIGHT = Event_Next;
}

/*  固定内容创建  */
void INS_Text(tft_ui* u,const char* text,uint8_t length)
{
	u->parameter[2] = length;
	Other_StringCpy(u->value_text,text);
}
void INS_Num(tft_ui* u,uint32_t num,uint8_t length)
{
	u->parameter[2] = length;
	u->value_num = num;
	u->Func_Event_UP = Event_NumAdd;
	u->Func_Event_DOWN = Event_NumMul;
}



/*  测试页面  */
void Page_Test(void)
{
	//?
	UI[0] = UI_CreateUI(10,20,InFT_font_Consolas_1608,InCor_Blue,InCor_Brown,Render_Text);
	INS_LeftRight(&UI[0],1,1);
	INS_Text(&UI[0],"Hello!",8);

	UI[1] = UI_CreateUI(100,20,InFT_font_Pixel_1608,InCor_Red,InCor_Pink,Render_Num);
	INS_LeftRight(&UI[1],0,0);
	INS_Num(&UI[1],12,4);
	
	uint16_t ui_index[] = {0,1};
	UI_CreatePage(InPG_Test,ui_index,sizeof(ui_index)/sizeof(uint16_t),&UI[0]);
}



