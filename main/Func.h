#ifndef __FUNC_H__
#define __FUNC_H__
/*  ST库  */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

void Init_Func(void);
void Task_Func(void* pvParameters);
void Cmd_Func(void);

void Cmd_Botton(void);

#endif
