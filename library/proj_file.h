#ifndef __PROJ_FILE_H__
#define __PROJ_FILE_H__
#include "stdint.h"

void Init_Proj(void);
void Test_Proj(void);
void Proj_SetRectNormal(uint16_t width,uint16_t height);
void Proj_SetRectLU(uint16_t width,uint16_t height);
uint16_t Proj_bmp(char* file,char* file_name,int8_t num_change);
void Proj_index(uint16_t index,char* file,char* file_name);

#endif
