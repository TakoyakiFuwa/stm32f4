#ifndef __FUNC_H__
#define __FUNC_H__
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

void Init_Func(void);
void U_RGB(uint16_t rgb565);
void Task_Func(void* pvParameters);
void Cmd_Func(void);

#endif
