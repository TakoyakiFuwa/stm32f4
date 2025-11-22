#ifndef __OV7670_H__
#define __OV7670_H__
#include "stdint.h"

void Init_OV(uint32_t* data_addr);
void Init_OV_DoubleBuffer(uint32_t* addr);
void OV_GetPixels(void);

void OV_Pause(void);
void OV_Continue(void);

#endif
