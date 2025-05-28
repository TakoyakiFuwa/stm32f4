#ifndef __TFT_ST7735_H__
#define __TFT_ST7735_H__
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

void Init_TFT(void);
void TFT_Clear(uint16_t RGB);
void TFT_SetRect(uint16_t x,uint16_t y,uint16_t width,uint16_t hight);
void TFT_WriteData16(uint16_t data);
uint16_t TFT_RGB888To565(uint32_t RGB888);
void Cmd_TFT_XYTest(void);
void TFT_SetXY(uint8_t x,uint8_t y,uint8_t x_y);

#endif
