#ifndef __SPI_SF_H__
#define __SPI_SF_H__
#include <stdint.h>

#define SPI_SF_CS_L()		GPIOD->BSRRH = GPIO_Pin_8
#define SPI_SF_CS_H()		GPIOD->BSRRL = GPIO_Pin_8

void SPI_SF_Init(void);
void SPI_SF_Test(void);
void SPI_SF_Send(uint8_t data);
uint8_t SPI_SF_Receive(void);

#endif
