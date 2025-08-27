#ifndef __TFT_FONT_H__
#define __TFT_FONT_H__
#include "stdint.h"

/*  颜色宏定义  */
#define InCor_Black 	TFT_RGB888To565(0x000000)
#define InCor_White		TFT_RGB888To565(0xFFFFFF)
#define InCor_Pink		TFT_RGB888To565(0xffc7c7)
#define InCor_Blue		TFT_RGB888To565(0x71c9ce)
#define InCor_Red		TFT_RGB888To565(0xf6416c)
#define InCor_Green		TFT_RGB888To565(0x62d2a2)
#define InCor_Yellow	TFT_RGB888To565(0xfff5a5)
#define InCor_Purple	TFT_RGB888To565(0x6a2c70)
#define InCor_Brown		TFT_RGB888To565(0x8d6262)
#define InCor_Orange	TFT_RGB888To565(0xff9a3c)

/*  字体宏定义  */
//命名 : InFT(index font)_font/pic/RGB_名称_高度宽度
#define InFT_font_Consolas_1608			0
#define InFT_font_Pixel_1608			1
#define InFT_font_Pixel_2412			2
#define InFT_RGB_Cat_6364				3
#define InFT_pic_SunRain_2424			4
#define InFT_RGB_CatCake_6464			5
#define InFT_pic_UpDown_1616			6
#define InFT_pic_SunRain_1616			7
#define InFT_pic_up_1616				8
#define InFT_pic_down_1616				9
#define InFT_pic_left_1616				10
#define InFT_pic_right_1616				11
#define InFT_pic_ok_1616				12

/*  字体结构体  */
typedef struct tft_font{
	const char* font;
	uint8_t width;
	uint8_t height;
}tft_font;

/*  内容函数  */
//初始化
void Init_TFTF(void);
//图案
void TFTF_DrawRect(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color);
void TFTF_DrawFrame(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color,int8_t thick);
void TFTF_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color,int8_t thick);
//图片
void TFTF_Put01(uint16_t x,uint16_t y,uint8_t InFT,uint16_t ft_color,uint16_t bk_color);
void TFTF_Put565(uint16_t x,uint16_t y,uint8_t InFT);
//数字-文本
void TFTF_PutChar(uint16_t x,uint16_t y,uint8_t InFT,char _char,uint16_t ft_color,uint16_t bk_color);
void TFTF_ShowNum(uint16_t x,uint16_t y,uint32_t num,int8_t digits,uint8_t InFT,uint16_t ft_color,uint16_t bk_color);
void TFTF_ShowString(uint16_t x,uint16_t y,const char* text,int8_t NumOfChar,uint8_t InFT,uint16_t ft_color,uint16_t bk_color);

/*  测试接口  */
void TFTF_Test(void);



#endif
