#ifndef __SSD1309_H
#define __SSD1309_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"

#define ResolutionWidth 128
#define ResolutionHeight 64

bool SSD1309_Init ( SPI_HandleTypeDef *hspi, GPIO_TypeDef *RST_PORT, uint16_t RST_PIN, GPIO_TypeDef *CS_PORT, uint16_t CS_PIN, GPIO_TypeDef *DCX_PORT, uint16_t DCX_PIN );
void SSD1309_hardware_reset ( void );
void SSD1309_WrCmd ( uint8_t d );
void SSD1309_WrData ( uint8_t d );
void SSD1309_full_screen ( uint8_t color );
void SSD1309_full_screen_pointer ( bool convert, uint8_t *addr );
void SSD1309_Set_Contrast ( uint8_t Contrast );
uint8_t SSD1309_Get_Contrast ( void );

void Home_Addr(void);

#ifdef __cplusplus
}
#endif

#endif /* __SSD1309_H */
