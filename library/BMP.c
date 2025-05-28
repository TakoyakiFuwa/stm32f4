#include "BMP.h"
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
/*  外设库  */
#include "U_USART.h"
/*  TFT库  */
#include "TFT_ST7735.h"
/*  FATFS  */
#include "ff.h"

/*	是说...最近在做一个关于TFT屏幕软排线的项目...
 *	也不确定能不能行...
 *	毕竟和软排线有关的项目已经做了两个月...
 *	完全没有成功点亮过...
 *	实在不行我还是去做MC的Mod开发吧...
 *			——2025/5/27-18:40
 */
/*	就在刚刚 我在梦里
 *	写了好长一段这个解码库...
 *	:)
 *			——2025/5/27-20:44
 */

/**@brief  初始化 关于TFT和SD卡
  *@param  void
  *@retval void
  */
FATFS FS;
void Init_BMP(void)
{
	//文件系统初始化->文件系统挂载
	if(f_mount(&FS,"0:",1)!=FR_OK)
	{
		U_Printf("BMP.c:文件系统挂载异常:%d \r\n",f_mount(&FS,"0:",1));
		return;
	}
	//TFT屏幕初始化
	Init_TFT();
		//TFT关于xy轴应该实践去尝试...
		//	y/128px
		//	^
		//	|
		//	| 蓝 绿
		//	| 红 黄
		//	+——————————> x/160px
		//当前配置为xy不反转 xy轴交换
	TFT_SetXY(0,0,1);
	U_Printf("BMP.c:初始化完成，包含TFT屏幕和FATFS \r\n");
	BMP_BMP("0:/bmp10.bmp",0,0);
}
/**@brief  打印文件头信息
  *@param  bmp_infor 要打印的文件信息
  *@retval void
  */
static void BMP_PrintfInfor(bmp_head infor)
{
	U_Printf("现在打印文件信息: \r\n");
	U_Printf("文件标识:%h \r\n",infor.bmp_sign);
	U_Printf("文件大小:%d字节 \r\n",infor.file_size);
	U_Printf("保留位(0):%d \r\n",infor.reserved);
	U_Printf("数据偏移位置:%h \r\n",infor.data_offset);
	U_Printf("信息头长度:%d \r\n",infor.header_size);
	U_Printf("位图宽度:%dpx\t位图高度:%dpx \r\n",infor.width,infor.height);
	U_Printf("位面数(1):%d \r\n",infor.planes);
	U_Printf("像素位数:%d \r\n",infor.bits_per_px);
	U_Printf("位图数据大小:%d字节 \r\n",infor.data_size);
	U_Printf("水平分辨率:%dpx\t垂直分辨率:%dpx \r\n",infor.HResolution,infor.VResolution);
	U_Printf("用到的颜色数:%d \r\n",infor.colors);
	U_Printf("重要颜色数:%d \r\n",infor.important_cor);
}
/**@brief  读取文件头信息
  *@param  path 读取文件路径
  *@retval 文件头信息
  */
bmp_head BMP_ReadInfor(const char* path)
{
	bmp_head infor;
	FIL fp;
	//打开文件
	if(f_open(&fp,path,FA_READ)!=FR_OK)
	{
		U_Printf("%s打开失败，状态:%d \r\n",f_open(&fp,path,FA_READ));
	}
	//读取文件信息
	f_read(&fp,&infor.bmp_sign,sizeof(uint16_t),0);
	f_read(&fp,&infor.file_size,sizeof(uint32_t),0);
	f_read(&fp,&infor.reserved,sizeof(uint32_t),0);
	f_read(&fp,&infor.data_offset,sizeof(uint32_t),0);
	f_read(&fp,&infor.header_size,sizeof(uint32_t),0);
	f_read(&fp,&infor.width,sizeof(uint32_t),0);
	f_read(&fp,&infor.height,sizeof(uint32_t),0);
	f_read(&fp,&infor.planes,sizeof(uint16_t),0);
	f_read(&fp,&infor.bits_per_px,sizeof(uint16_t),0);
	f_read(&fp,&infor.compression,sizeof(uint32_t),0);
	f_read(&fp,&infor.data_size,sizeof(uint32_t),0);
	f_read(&fp,&infor.HResolution,sizeof(uint32_t),0);
	f_read(&fp,&infor.VResolution,sizeof(uint32_t),0);
	f_read(&fp,&infor.colors,sizeof(uint32_t),0);
	f_read(&fp,&infor.important_cor,sizeof(uint32_t),0);
	//关闭文件
	f_close(&fp);
	//打印文件信息(仅测试时)
	BMP_PrintfInfor(infor);
	return infor;
}
/**@brief  在TFT屏幕上显示一个BMP图片
  *@param  path		SD卡内bmp存储位置
  *@param  d_width	宽度补正		一般不需要，给0
  *@param  px_fix	缺失像素修复	一般不需要，给0
  *@retval void
  */
void BMP_BMP(const char* path,int8_t d_width,int8_t px_fix)
{
	uint32_t data_offset;
	uint32_t width,height;
	FIL fp;
	//打开文件
	if(f_open(&fp,path,FA_READ)!=FR_OK)
	{
		U_Printf("%s打开失败，状态:%d \r\n",f_open(&fp,path,FA_READ));	
		return;
	}
	//获取文件基本信息
	f_lseek(&fp,0x0A);
	f_read(&fp,&data_offset,sizeof(uint32_t),0);
	f_lseek(&fp,0x12);
	f_read(&fp,&width,sizeof(uint32_t),0);
	f_read(&fp,&height,sizeof(uint32_t),0);
	U_Printf("data_offset:%h \r\nsize:%d*%d \r\n",data_offset,width,height);
		//图像尺寸过大
	if(width>180 || height>150)
	{
		U_Printf("图像尺寸过大，建议用自适应函数 \r\n");
			//关闭文件
		f_close(&fp);
		BMP_AdjustBMP(path,1,d_width,px_fix);
		return;
	}
	//读取并打印像素
	TFT_SetRect((160-width)/2,(128-height)/2,width+d_width,height);
	f_lseek(&fp,data_offset);
		//计算是否需要像素补全
	uint32_t temp_px_fix = 0;
		//读取单个像素
	bmp_rgb px_rgb;
	uint16_t px_rgb565 = 0;
	for(int i=0;i<width*height;i++)
	{
		f_read(&fp,(void*)&px_rgb,sizeof(bmp_rgb),0);
			//数据处理
		px_rgb565 = (px_rgb.red&0xF8)<<8;//6+5-3
		px_rgb565 |= (px_rgb.green&0xFC)<<3;//5-2
		px_rgb565 |= (px_rgb.blue&0xF8)>>3;
		TFT_WriteData16(px_rgb565);
		if(px_fix!=0 && i%width==0)
		{
			if(temp_px_fix++%px_fix==0)
			{
				TFT_WriteData16(px_rgb565);
			}
		}
	}
	//关闭文件
	f_close(&fp);
}
void BMP_AdjustBMP(const char* path,int8_t d_scale,int8_t d_width,int8_t px_fix)
{
	uint32_t data_offset;
	uint32_t width,height;
	FIL fp;
	//打开文件
	if(f_open(&fp,path,FA_READ)!=FR_OK)
	{
		U_Printf("%s打开失败，状态:%d \r\n",f_open(&fp,path,FA_READ));	
		return;
	}
	//获取文件基本信息
	f_lseek(&fp,0x0A);
	f_read(&fp,&data_offset,sizeof(uint32_t),0);
	f_lseek(&fp,0x12);
	f_read(&fp,&width,sizeof(uint32_t),0);
	f_read(&fp,&height,sizeof(uint32_t),0);
	U_Printf("data_offset:%h \r\nsize:%d * %d \r\n",data_offset,width,height);
	//像素缩放处理
	uint8_t scale = width/160>height/128 ? width/160:height/128;
	scale+=d_scale;
		//s_ 缩放后的尺寸数据
	uint8_t s_width=width/scale,s_height=height/scale;
	U_Printf("scale:%d \r\nresize:%d * %d \r\n",scale,s_width,s_height);
	f_lseek(&fp,data_offset);
		//设置显示位置
	TFT_SetRect((160-s_width)/2,(128-s_height)/2,s_width+d_width,s_height);
		//缩放实现
	uint32_t scale_offset = data_offset;
		//计算是否需要像素补全
	uint32_t temp_px_fix = 0;
		//读取单个像素
	bmp_rgb px_rgb;
	uint16_t px_rgb565 = 0;
	uint16_t count = 0;
	for(int i=0;i<s_height;i++)//高度
	{
		for(int j=0;j<s_width;j++)//宽度
		{
			scale_offset=data_offset+sizeof(bmp_rgb)*scale*(width*i+j);
			f_lseek(&fp,scale_offset);
			f_read(&fp,(void*)&px_rgb,sizeof(bmp_rgb),0);
				//数据处理
			px_rgb565 = (px_rgb.red&0xF8)<<8;//6+5-3
			px_rgb565 |= (px_rgb.green&0xFC)<<3;//5-2
			px_rgb565 |= (px_rgb.blue&0xF8)>>3;
			TFT_WriteData16(px_rgb565);
		}
		//像素补全
		if(px_fix!=0)
		{
			temp_px_fix++;
			if(temp_px_fix%px_fix==0)
			{
				count++;
				TFT_WriteData16(px_rgb565);
			}
		}
	}
	U_Printf("进行%d此补全 \r\n",count);
	f_close(&fp);
}






















