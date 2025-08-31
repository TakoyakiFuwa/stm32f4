#ifndef __U1_H__
#define __U1_H__
#include "stdint.h"

void Init_U1(void);
void Task_U1Command(void* pvParameters);

void U1_SendNum(uint32_t num);
void U1_Putchar(char _char);
void U1_SendWords(const char* words);

#endif
