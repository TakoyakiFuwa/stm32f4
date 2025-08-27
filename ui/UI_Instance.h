#ifndef __UI_INSTANCE_H__
#define __UI_INSTANCE_H__
#include "TFT_UI.h"

void Page_Test(void);


void INS_Num(tft_ui* u,uint32_t num,uint8_t length);
void INS_Text(tft_ui* u,const char* text,uint8_t length);

void INS_LeftRight(tft_ui* u,uint16_t front_ui,uint16_t next_ui);
void INS_UpDown(tft_ui* u,uint16_t front_ui,uint16_t next_ui);
void Event_NumMul(tft_ui* u);
void Event_NumAdd(tft_ui* u);
void Event_Next(tft_ui* u);
void Event_Front(tft_ui* u);
void Render_Num(tft_ui* u);
void Render_Text(tft_ui* u);

#endif
