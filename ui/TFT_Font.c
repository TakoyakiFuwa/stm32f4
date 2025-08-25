#include "TFT_Font.h"

/*	正在做f4_ui，（用于单片机间传输数据配置）
 *	关于字体的很多想法都来自于在公司实习时的考虑
 *	在屏幕上取模显示图片，显示文本等等...
 *	但这个屏幕分辨率不高...显示文本有够占空间....
 *			——2025/8/25-15:48.秦羽
 */

/*  字体宏  */
tft_font FONT[128];

/*  接口部分  */
	//屏幕驱动库
#include "TFT_ST7735.h"
	//主要实现的两个内部用函数
/**@brief  设定显示位置
  *@add    这里宽和高和X/Yend大概是Xend=width+x-1，Yend=height+y-1
  */
static void TFTF_SetRect(uint16_t x,uint16_t y,uint16_t width,uint16_t height)
{
	TFT_SetCursor(x,y,width,height);
}
/**@brief  发送单个像素
  */
static void TFTF_Pixel(uint16_t rgb565)
{
	TFT_Write16Data(rgb565);
}
	//字库
#include "font.h"
	//字体初始化
static tft_font TFTF_CreateFont(const char* font,uint8_t height,uint8_t width);
/**@brief  包含屏幕初始化和字体绑定
  */
void Init_TFTF(void)
{
	//屏幕初始化
	Init_TFT();
	//字体绑定
	FONT[InFT_font_Consolas_1608] = TFTF_CreateFont((const char*)font_consola_1608,16,8);
	FONT[InFT_font_Pixel_1608] = TFTF_CreateFont((const char*)font_piexl_1608,16,8);
	FONT[InFT_font_Pixel_2412] = TFTF_CreateFont((const char*)font_piexl_2412,24,12);
	FONT[InFT_RGB_Cat_6364] = TFTF_CreateFont((const char*)rgb_cat_6364,63,64);
	FONT[InFT_pic_SunRain_2424] = TFTF_CreateFont((const char*)pic_sunrain_2424,24,24);
	FONT[InFT_RGB_CatCake_6464] = TFTF_CreateFont((const char*)rgb_catcake_6464,64,64);
	
}



/*  函数实现部分  */

/*  关于形状的部分  */
/**@brief  填充矩形
  *@param  -
  *@param  color 颜色
  *@retval void
  */
void TFTF_DrawRect(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color)
{
	TFTF_SetRect(x,y,width,height);
	for(int i=0;i<width*height;i++)
	{
		TFTF_Pixel(color);
	}
}
/**@brief  绘制矩形框
  *@param  x y width height
  *@param  color 框颜色
  *@param  thick 框线宽
  *@retval void
  *@add    注：框架线是向内收缩的 x y width height是最大外边框
  */
void TFTF_DrawFrame(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color,int8_t thick)
{
	if(thick<=0)
	{
		return;
	}
	TFTF_DrawRect(x,y,width,thick,color);
	TFTF_DrawRect(x,y+height-thick,width,thick,color);
	TFTF_DrawRect(x,y+thick,thick,height-thick*2,color);
	TFTF_DrawRect(x+width-thick,y+thick,thick,height-thick*2,color);
}

/*  关于取模显示的部分  */
/**@brief  创建一个字体/图片
  *@param  font				字库/图库
  *@param  height/width		高度/宽度
  *@retval 创建好的字体/图片
  */
static tft_font TFTF_CreateFont(const char* font,uint8_t height,uint8_t width)
{
	tft_font f;
	f.font = font;
	f.height = height;
	f.width = width;
	return f;
}
/**@brief  放置单色图片
  *@param  x/y 		位置
  *@param  InFT		字体宏定义
  *@param  ft_color/bk_color	前景色/背景色 
  *@retval void
  */
void TFTF_Put01(uint16_t x,uint16_t y,uint8_t InFT,uint16_t ft_color,uint16_t bk_color)
{
	tft_font f = FONT[InFT];
	TFTF_SetRect(x,y,f.width,f.height);
	const char* font = f.font;
	uint16_t size = f.width*f.height/8;
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<8;j++)
		{
			if( (font[i] & (0x01<<j) ) != 0 )
			{
				TFTF_Pixel(ft_color);
			}
			else
			{
				TFTF_Pixel(bk_color);
			}
		}
	}
}
/**@brief  放置RGB565(16位真彩色)图片
  *@param  x/y		位置
  *@param  InFT		图库宏定义
  *@retval void
  */
void TFTF_Put565(uint16_t x,uint16_t y,uint8_t InFT)
{
	uint16_t size = FONT[InFT].height*FONT[InFT].width*2;
	const char* f = FONT[InFT].font;
	TFTF_SetRect(x,y,FONT[InFT].width,FONT[InFT].height);
	uint16_t rgb565;
	for(uint16_t i=0;i<size;i++)
	{
		rgb565 = f[i];
		rgb565 <<= 8 ;
		rgb565 |= f[++i];
		TFTF_Pixel( rgb565 );
	}
}
/**@brief  放置单个字符
  *@param  x/y 		位置
  *@param  InFT		字体宏定义
  *@param  _char	要放置的字符
  *@param  ft_color/bk_color	前景色/背景色
  */
void TFTF_PutChar(uint16_t x,uint16_t y,uint8_t InFT,char _char,uint16_t ft_color,uint16_t bk_color)
{
	tft_font f = FONT[InFT];
	TFTF_SetRect(x,y,f.width,f.height);
	uint32_t d_ft = _char - ' ';
	uint16_t size = f.width*f.height/8;
	const char* font = &f.font[d_ft*size];
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<8;j++)
		{
			if( (font[i] & (0x01<<j) ) != 0 )
			{
				TFTF_Pixel(ft_color);
			}
			else
			{
				TFTF_Pixel(bk_color);
			}
		}
	}
}

/*  关于文本部分  */
/**@brief  显示一个数字
  *@param  -
  *@param  digits  显示的位数，超过位数会使高位输出成字符
  *@retval void
  */
void TFTF_ShowNum(uint16_t x,uint16_t y,uint32_t num,int8_t digits,uint8_t InFT,uint16_t ft_color,uint16_t bk_color)
{
	tft_font f = FONT[InFT];
	uint32_t num_length = 1;
	for(;digits>0;digits--)
	{	
		num_length*=10;
	}
	//从高位开始显示
	int8_t i=0;
	for(num_length/=10;num_length>=1;num_length/=10)
	{
		TFTF_PutChar(x+f.width*(i++),y,InFT,num/num_length+'0',ft_color,bk_color);
		//减去最高位
		num -= (num - (num%num_length));
	}
}
/**@brief  字符串
  *@param  -
  *@param  NumOfChar  显示的数量
  */
void TFTF_ShowString(uint16_t x,uint16_t y,const char* text,int8_t NumOfChar,uint8_t InFT,uint16_t ft_color,uint16_t bk_color)
{
	tft_font f=FONT[InFT];
	int i=0;
	for(;text[i]!='\0';i++)
	{
		if(--NumOfChar<0)
		{
			TFTF_PutChar(x+f.width*(--i),y,InFT,'-',ft_color,bk_color);
			return;
		}
		TFTF_PutChar(x+f.width*(i),y,InFT,text[i],ft_color,bk_color);
	}
	for(;NumOfChar>0;NumOfChar--)
	{
		TFTF_PutChar(x+f.width*(i++),y,InFT,' ',ft_color,bk_color);
	}
}


/*  测试接口  */
void TFTF_Test(void)
{
	TFTF_PutChar(0,0,InFT_font_Pixel_1608,'F',0,0xFFFF);
	TFTF_Put565(40,40,InFT_RGB_Cat_6364);
	TFTF_Put565(40,40,InFT_RGB_CatCake_6464);
	TFTF_ShowNum(10,10,123,6,InFT_font_Pixel_1608,0,0xFFFF);
	TFTF_ShowString(10,40,"Hello",7,InFT_font_Pixel_1608,InCor_Black,InCor_White);
	TFTF_Put01(100,10,InFT_pic_SunRain_2424,0,0xFFFF);
	uint8_t i=0;
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Black);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Blue);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Brown);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Green);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Orange);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Pink);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Purple);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Red);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_White);
	TFTF_DrawRect(20+i++*10,20,10,30,InCor_Yellow);
	TFTF_DrawFrame(50,100,40,20,InCor_Green,3);
	TFTF_DrawLine(10,10,50,100,InCor_Purple,2);
	
}

/*  额外内容  */
/**@brief  两点之间连线
  *@param  x1,y1 起始点
  *@param  x2,y2 终止点
  *@param  color 颜色
  *@param  thick 厚度
  *@add    注:这个真是我ctrl cv别人的了
  */
void TFTF_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color,int8_t thick)
{
	if(thick<=0)
	{
		return;
	}
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	int d_thick = thick/2;
	
	delta_x = x2 - x1; // 计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; // 设置单步方向
	else if (delta_x == 0)
		incx = 0; // 垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; // 水平线
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; // 选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) // 画线输出
	{ // 画点
		TFTF_SetRect(uRow-d_thick,uCol-d_thick,thick,thick);
		for(int i=0;i<thick*thick;i++)
		{
			TFTF_Pixel(color);
		}
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

