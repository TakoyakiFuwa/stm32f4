#ifndef __TFT_ST7735_H__
#define __TFT_ST7735_H__
#include <stdint.h>

#define Rota_BMP	0x20
#define Rota_UI		0x40

//工具函数
uint16_t TFT_RGB888To565(uint32_t RGB_888);
void TFT_Clear(uint32_t RGB_888);
//主要的接口
void Init_TFT(void);
void TFT_SetCursor(uint8_t x,uint8_t y,uint8_t weight,uint8_t height);
void TFT_Write16Data(uint16_t RGB_565);
void TFT_SetRotation(uint8_t rotation);
//测试
void TFT_Test(void);

#endif
