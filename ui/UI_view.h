#ifndef __UI_VIEW_H__
#define __UI_VIEW_H__
#include "stdint.h"

/*  预览页面  */
#define		InUI_view_background		10		//背景，整个框架
#define		InUI_view_change			11		//bmp的名字
#define		InUI_view_upload			12		//烧录
#define		InUI_view_overview			13		//图片放大
#define		InUI_view_button			14		//按键记忆显示
#define		InUI_view_infor				15		//bmp图片信息

void Page_View(void);
void Other_Button(uint8_t InFT);
void Other_Infor(uint8_t width,uint8_t height);

#endif
