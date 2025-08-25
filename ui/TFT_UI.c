#include "TFT_UI.h"
/*  uint库  */
#include "stdint.h"

/*  接口  */

/*  光标颜色宏定义  */
#define COLOR_UI_CURSOR_FT	0xF20D
#define COLOR_UI_CURSOR_BK	0xFFB4
/*	在创建页面时需要用到来自这个文件的全局变量
extern tft_pointer 	UI_CURSOR;
extern tft_ui 		UI[];
extern tft_page		PAGE[];
 */
/**@brief  页面-UI初始化接口
  *@param  void
  *@retval 初始化完成后进入的第一个页面(下标)
  */
static uint8_t INS_Init_Page(void)
{
	//INS_Test(); //页面函数中应该包含有 UI初始化/页面初始化
	return InPG_Test;
}



/*  以下是实现内容  */

/*  全局变量  */
tft_pointer UI_CURSOR;				//光标
tft_ui 		UI[200];				//UI
tft_page 	PAGE[8];				//页面
/*  渲染队列  */
tft_ui* 	QUEUE_RENDER_UI[100];		//用is_present=100作为尾标志
										//更改容量时记得该CircleRender_UI中的for(int i=0;i<xxxx;i++);
uint16_t	queue_render_ui_index = 0;
tft_ui		null_ui;

/*  函数  */
/**@brief  用于充当函数指针的空指针
  *@add    避免程序调用的函数是空的导致异常
  *		   NULL
  */
void NULL_UI_Func(struct tft_ui* none)
{
	
}
void NULL_VOID_Func(void)
{

}
/**@brief  UI初始化，需要放在循环前(常规初始化位置就可以)
  *@param  void
  *@retval void
  *@add	   包括:UI创建接口/Page创建接口/光标创建/渲染队列创建
  */
void Init_UI(void)
{
	//创建充当"空指针"的内容
	null_ui = UI_CreateUI(0,0,0,0,0,NULL_UI_Func);
	null_ui.is_present = 100;	//作为渲染队列尾的标志位
	for(int i=0;i<sizeof(UI)/sizeof(UI[0]);i++)
	{
		UI[i] = null_ui;
	}
	//页面创建接口
	UI_CURSOR.InPG = INS_Init_Page();
	//创建光标
	UI_CURSOR.ui = PAGE[UI_CURSOR.InPG].start_ui;
	UI_CURSOR.ui_color[0] = UI_CURSOR.ui->ft_color;
	UI_CURSOR.ui_color[1] = UI_CURSOR.ui->bk_color;
	UI_CURSOR.parameter = 0;
	UI_CURSOR.ptr_color[0] = COLOR_UI_CURSOR_FT;
	UI_CURSOR.ptr_color[1] = COLOR_UI_CURSOR_BK;
	//创建渲染队列
		//UI渲染
	for(int i=0;i<sizeof(QUEUE_RENDER_UI)/sizeof(tft_ui*);i++)
	{
		QUEUE_RENDER_UI[i] = &null_ui;
	}
	//第一次渲染
	UI_ChangePage(UI_CURSOR.InPG);
}
/**@brief  具体的渲染函数，用于向屏幕输出
  *@param  void
  *@retval void
  *@add    放在循环中或者定时器定时调用，tft_ui.Func_Render_N只在这里直接调用
  */
void CircleRender_UI(void)
{
	//遍历UI渲染队列
	for(int i=0;i<100;i++)
	{
		if(QUEUE_RENDER_UI[i]->is_present==100)//遍历到队列尾，退出
		{
			break;
		}
		QUEUE_RENDER_UI[i]->Func_Render_N(QUEUE_RENDER_UI[i]);
		QUEUE_RENDER_UI[i]->is_present &= 0xFD;
		
		QUEUE_RENDER_UI[i] = &null_ui;
	}
	queue_render_ui_index = 0;
}
/**@brief  将一个UI添加到渲染队列中
  *@param  u	要渲染的UI
  *@retval void
  *@add	   这个架构里面不直接使用UI的Render方法，而是用这个配合队列渲染
  */
void UI_AddRender(tft_ui* u)
{
	if(u->is_present==1)
	{
		QUEUE_RENDER_UI[queue_render_ui_index] = u;	//将该UI添加到渲染队列
		u->is_present |= 0x02;						//UI的准备渲染标志位换成1
													//再次对该UI调用"添加到队列"函数时无效，可避免重复渲染

		queue_render_ui_index++;					//队尾向后移动
	}
}
/**@brief  创建一个UI
  *@param  x,y 	该UI的坐标
  *@param  font 该UI的字体，应绑定全局变量
  *@param  Func_Render	渲染函数
  *@retval tft_ui	应该用全局变量来接收
  */
tft_ui UI_CreateUI(uint16_t x,uint16_t y,uint8_t InFT,uint16_t ft_color,uint16_t bk_color,void (*Func_Render)(struct tft_ui* self))
{
	tft_ui u;
	u.x = x;
	u.y = y;
	u.InFT = InFT;
	u.ft_color = ft_color;
	u.bk_color = bk_color;
	u.is_present = 0;
	u.Func_Render_N = Func_Render;
	//赋值
	u.value_num = 0;
	for(int i=0;i<16;i++)
	{
		u.value_text[i] = '\0';
	}
	//绑定空操作函数
	u.Func_Event_UP 	= NULL_UI_Func;
	u.Func_Event_DOWN 	= NULL_UI_Func;
	u.Func_Event_LEFT 	= NULL_UI_Func;
	u.Func_Event_RIGHT 	= NULL_UI_Func;
	//不一定会用上，后续补充内容
	for(int i=0;i<=3;i++)
	{
		u.parameter[i] = 0;
	}
	
	return u;
}
/**@brief  改变光标Cursor绑定的UI
  *@param  new_ui 要绑定的新UI
  *@retval void
  *@add	   这里的光标Cursor提示方式是用背景色来作为提示
  *		   也可以通过修改其他属性来实现光标提示的功能
  */
void UI_Cursor_ChangeUI(tft_ui* new_ui)
{
	//把当前UI颜色改回来
	UI_CURSOR.ui->ft_color = UI_CURSOR.ui_color[0];
	UI_CURSOR.ui->bk_color = UI_CURSOR.ui_color[1];
	UI_AddRender(UI_CURSOR.ui);
	//记录新指向的UI
	UI_CURSOR.ui_color[0] = new_ui->ft_color;
	UI_CURSOR.ui_color[1] = new_ui->bk_color;
	//更改新指向的UI颜色
	new_ui->ft_color = UI_CURSOR.ptr_color[0];
	new_ui->bk_color = UI_CURSOR.ptr_color[1];
	//光标指向新的UI
	UI_CURSOR.ui = new_ui;
	UI_AddRender(new_ui);
}
/**@brief  创捷页面
  *@add	   关于页面创建和UI_CraeteUI函数格式不同，
  *		   tft_page中含有数组ui_index[100]，不可以用 数组=数组
  *		   而UI_CreateUI函数中并没有对tft_ui的数组(notes[20]/value_text[20]进行操作)
  *		   所以UI_CreateUI可以用返回值的写法，而UI_CreatePage不可以
  *@add	   总之出问题首先考虑结构体中的数组问题
  */
void UI_CreatePage(uint8_t InPG,uint16_t* ui_index,uint16_t number_ofUI,tft_ui* start_ui)
{
	tft_page* page = &PAGE[InPG];
	uint16_t i=0;
	for(;i<number_ofUI;i++)
	{
		page->ui_index[i] = ui_index[i];
	}
	for(;i<100;i++)
	{
		page->ui_index[i] = 999;
	}
	page->start_ui = start_ui;
}
/**@brief  更改当前显示的页面
  *@param  new_page 要更改绑定的位置
  *@retval void
  */
void UI_ChangePage(uint8_t InPG)
{
	//上一个页面的ui.ispresent=0
	tft_page* p = &PAGE[UI_CURSOR.InPG];
	tft_page* new_page = &PAGE[InPG];
	for(int i=0;p->ui_index[i]!=999;i++)
	{
		if(i>=100)
		{//异常
//			U_Printf("页面加载异常! \r\n");
			break;
		}
		UI[p->ui_index[i]].is_present = 0;
	}
	//将本页面ui.ispresent=1
	for(int i=0;i<100;i++)
	{
		if(new_page->ui_index[i]==999)
		{
			break;
		}
		UI[new_page->ui_index[i]].is_present = 1;
		UI_AddRender(&UI[new_page->ui_index[i]]);
	}
	//绑定指针
	UI_Cursor_ChangeUI(new_page->start_ui);
	UI_CURSOR.InPG = InPG;
}

/*  使用率较高，可能对创建页面有帮助的函数  */
/**@brief  字符串，把_string赋给target
  */
void Other_StringCpy(char* target,const char* _string)
{
	int i=0;
	for(;_string[i]!='\0';i++)
	{
		target[i] = _string[i];
	}
	target[i] = '\0';
}


