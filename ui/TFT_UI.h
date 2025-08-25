#ifndef __TFT_UI_H__
#define __TFT_UI_H__
#include "stdint.h"

/*  定义页面  */
#define InPG_Test		0

/*  UI结构体  */
typedef struct tft_ui{
	//基础属性
	uint16_t 	x;
	uint16_t 	y;
	uint8_t 	InFT;
	uint16_t 	ft_color;
	uint16_t	bk_color;
	int8_t 		is_present;		//0不显示 1显示 3正在队列中 100渲染队列标志尾
	
	//值
	uint32_t 	value_num;
	char 	 	value_text[16];
	
	//参数
	uint16_t parameter[3];		//建议0->前件 1->后件 2->其他参数
	
	//绑定的函数方法
	void (*Func_Render_N)(struct tft_ui* self);
	void (*Func_Event_UP)(struct tft_ui* self);
	void (*Func_Event_DOWN)(struct tft_ui* self);
	void (*Func_Event_LEFT)(struct tft_ui* self);
	void (*Func_Event_RIGHT)(struct tft_ui* self);
}tft_ui;
/*  页面结构体  */
typedef struct tft_page{
	uint16_t ui_index[100];			//该页面的UI下标 999为不绑定
	struct tft_ui* start_ui;		//切换到该页面时的第一个UI
}tft_page;
/*  光标结构体  */
typedef struct tft_pointer{
	uint8_t 	InPG;
	tft_ui* 	ui;
	uint16_t	ui_color[2];
	uint16_t 	ptr_color[2];
	uint16_t 	parameter;
}tft_pointer;

/*  架构接口  */
void Init_UI(void);
void CircleRender_UI(void);
/*  具体操作函数  */
void UI_AddRender(tft_ui* u);
tft_ui UI_CreateUI(uint16_t x,uint16_t y,uint8_t InFT,uint16_t ft_color,uint16_t bk_color,void (*Func_Render)(struct tft_ui* u));
void UI_Cursor_ChangeUI(tft_ui* new_ui);
void UI_CreatePage(uint8_t InPG,uint16_t* ui_index,uint16_t number_ofUI,tft_ui* start_ui);
void UI_ChangePage(uint8_t InPG);
/*  辅助函数  */
void Other_StringCpy(char* target,const char* _string);

#endif









