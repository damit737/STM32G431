#include "SSD1309.h"

uint8_t SSD1309_Contrast = 0xFF;

SPI_HandleTypeDef *pSSD1309_hspi;

GPIO_TypeDef *pSSD1309_RST_Port = NULL;
uint16_t SSD1309_RST_Pin = 0;

GPIO_TypeDef *pSSD1309_CS_Port = NULL;
uint16_t SSD1309_CS_Pin = 0;

GPIO_TypeDef *pSSD1309_DCX_Port = NULL;
uint16_t SSD1309_DCX_Pin = 0;

static void RST_SetLow( void )
{
	if( pSSD1309_RST_Port != NULL )
		HAL_GPIO_WritePin( pSSD1309_RST_Port, SSD1309_RST_Pin, GPIO_PIN_RESET );
}
static void RST_SetHigh( void )
{
	if( pSSD1309_RST_Port != NULL )
		HAL_GPIO_WritePin( pSSD1309_RST_Port, SSD1309_RST_Pin, GPIO_PIN_SET );
}

static void CS_SetLow( void )
{
	if( pSSD1309_CS_Port != NULL )
		HAL_GPIO_WritePin( pSSD1309_CS_Port, SSD1309_CS_Pin, GPIO_PIN_RESET );
}
static void CS_SetHigh( void )
{
	if( pSSD1309_CS_Port != NULL )
		HAL_GPIO_WritePin( pSSD1309_CS_Port, SSD1309_CS_Pin, GPIO_PIN_SET );
}

static void DCX_SetLow( void )
{
	if( pSSD1309_DCX_Port != NULL )
		HAL_GPIO_WritePin( pSSD1309_DCX_Port, SSD1309_DCX_Pin, GPIO_PIN_RESET );
}

static void DCX_SetHigh( void )
{
	if( pSSD1309_DCX_Port != NULL )
		HAL_GPIO_WritePin( pSSD1309_DCX_Port, SSD1309_DCX_Pin, GPIO_PIN_SET );
}

static void spi_write_nbytes ( uint8_t *d, uint32_t len )
{
	if( pSSD1309_hspi != NULL )
		HAL_SPI_Transmit( pSSD1309_hspi, d, len, 100 );
}

static void spi_write_nbytes_IT ( uint8_t *d, uint32_t len )
{
	if( pSSD1309_hspi != NULL )
		HAL_SPI_Transmit_IT( pSSD1309_hspi, d, len );
}

static void spi_set_prescaler ( uint32_t prescaler )
{
	if( pSSD1309_hspi == NULL )
		return;

	pSSD1309_hspi->Init.BaudRatePrescaler = prescaler;

	HAL_SPI_DeInit( pSSD1309_hspi );
	HAL_SPI_Init( pSSD1309_hspi );
}

static void spi_change_data_size ( uint8_t size )
{
	size = ( size > 16 ) ? 16 : size;
	size = ( size < 4 ) ? 4 : size;
	size--;

	if( pSSD1309_hspi == NULL )
		return;

	pSSD1309_hspi->Init.DataSize = size * 0x100;

	HAL_SPI_DeInit( pSSD1309_hspi );
	HAL_SPI_Init( pSSD1309_hspi );
}

bool SSD1309_Init ( SPI_HandleTypeDef *hspi,
					GPIO_TypeDef *RST_PORT, uint16_t RST_PIN,
					GPIO_TypeDef *CS_PORT, uint16_t CS_PIN,
					GPIO_TypeDef *DCX_PORT, uint16_t DCX_PIN )
{

	if( (hspi == NULL) || (RST_PORT == NULL) || (CS_PORT == NULL) || (DCX_PORT == NULL) )
		return false;

	pSSD1309_hspi = hspi;
	pSSD1309_RST_Port = RST_PORT;
	SSD1309_RST_Pin = RST_PIN;

	pSSD1309_CS_Port = CS_PORT;
	SSD1309_CS_Pin = CS_PIN;

	pSSD1309_DCX_Port = DCX_PORT;
	SSD1309_DCX_Pin = DCX_PIN;

	/* Reset SSD1309 IC by hardware */
//	SSD1309_hardware_reset( );

//	SSD1309_WrCmd( 0xAE ); // Display Off
//
//	SSD1309_WrCmd( 0xA8 ); // Select Multiplex Ratio
//	SSD1309_WrCmd( 0x3F ); // Default => 0x3F (1/64 Duty)	0x1F(1/32 Duty)
//
//	SSD1309_WrCmd( 0xD3 ); //Setting Display Offset
//	SSD1309_WrCmd( 0x00 ); //00H Reset
//
//	SSD1309_WrCmd( 0x20 ); //;Set Memory Addressing Mode Default
//	SSD1309_WrCmd( 0x02 ); //Page Addressing Mode
//
//	SSD1309_WrCmd( 0x00 ); //Set Column Address LSB
//
//	SSD1309_WrCmd( 0x10 ); //Set Column Address MSB
//
//	SSD1309_WrCmd( 0x40 ); //Set Display Start Line
//
//	SSD1309_WrCmd( 0xA6 ); //Set Normal Display
//
//	SSD1309_WrCmd( 0xDB ); //Set Deselect Vcomh level
//	SSD1309_WrCmd( 0x3C ); //~0.83xVCC
//
//	SSD1309_WrCmd( 0xA4 ); //Entire Display ON
//
//	SSD1309_Set_Contrast( 0xFF ); //Set Contrast Control for Bank 0
//
//	SSD1309_WrCmd( 0xD5 ); //SET DISPLAY CLOCK
//	SSD1309_WrCmd( 0xC0 ); //105HZ
//
//	SSD1309_WrCmd( 0xA1 ); //Set Segment Re-Map Default => 0xA0
//						 //0xA1 (0x01) => Column Address 0 Mapped to SEG131
//	SSD1309_WrCmd( 0xC8 ); //Set COM Output Scan Direction Default => 0xC0
//						 //0xC8 (0x08) => Scan from COM63 to 0
//	SSD1309_WrCmd( 0xDA ); //Set COM Hardware Configuration
//	SSD1309_WrCmd( 0x12 ); //Alternative COM Pin
//
//	SSD1309_WrCmd( 0xD9 ); //Set Pre-Charge period
//	SSD1309_WrCmd( 0x22 ); //Refer to SPEC 34PAGE
//
//	SSD1309_WrCmd( 0xAF ); // Display ON

	SSD1309_WrCmd( 0xFD ); //Set Command Lock
	SSD1309_WrCmd( 0x12 ); //(12H=Unlock,16H=Lock)

	SSD1309_WrCmd( 0xAE ); // Display Off

	SSD1309_WrCmd( 0x15 ); //Set column Address
	SSD1309_WrCmd( 0x00 ); //Start column Address
	SSD1309_WrCmd( 0x7F ); //End column Address

	SSD1309_WrCmd( 0x75 ); //Set Row Address
	SSD1309_WrCmd( 0x00 ); //Start Row Address
	SSD1309_WrCmd( 0x3F ); //End Row Address

	SSD1309_WrCmd(0x81); //Set contrast
	SSD1309_WrCmd(0x2F);

	SSD1309_WrCmd(0xA0); //Set Remap
	SSD1309_WrCmd(0xC3);

	SSD1309_WrCmd(0xA1); //Set Display Start Line
	SSD1309_WrCmd(0x00);

	SSD1309_WrCmd(0xA2); //Set Display Offset
	SSD1309_WrCmd(0x00);

	SSD1309_WrCmd(0xA4); //Normal Display

	SSD1309_WrCmd(0xA8); //Set Multiplex Ratio
	SSD1309_WrCmd(0x3F);

	SSD1309_WrCmd(0xAB); //Set VDD regulator
	SSD1309_WrCmd(0x01); //Regulator Enable

	SSD1309_WrCmd(0xAD); //External /Internal IREF Selection
	SSD1309_WrCmd(0x8E);

	SSD1309_WrCmd(0xB1); //Set Phase Length
	SSD1309_WrCmd(0x22);

	SSD1309_WrCmd(0xB3); //Display clock Divider
	SSD1309_WrCmd(0xA0);

	SSD1309_WrCmd(0xB6); //Set Second precharge Period
	SSD1309_WrCmd(0x04);

	SSD1309_WrCmd(0xB9); //Set Linear LUT

	SSD1309_WrCmd(0xBC); //Set pre-charge voltage level
	SSD1309_WrCmd(0x10); //0.5*Vcc

	SSD1309_WrCmd(0xBD); //Pre-charge voltage capacitor Selection
	SSD1309_WrCmd(0x01);

	SSD1309_WrCmd(0xBE); //Set cOM deselect voltage level
	SSD1309_WrCmd(0x07); //0.82*Vcc

	SSD1309_WrCmd(0xAF); //Display ON

	SSD1309_full_screen( 0x00 );

	return true;
}

void SSD1309_hardware_reset ( void )
{
	HAL_Delay( 5 );
	RST_SetLow( ); // RST Low
	HAL_Delay( 50 ); // RSE low level more than 3us
	RST_SetHigh( ); // RST High
	HAL_Delay( 100 );
}

void SSD1309_WrCmd ( uint8_t d )
{
	CS_SetLow( );
	DCX_SetLow( );
	spi_write_nbytes( &d, 1 );
	CS_SetHigh( );
}

void SSD1309_WrData ( uint8_t d )
{
	CS_SetLow( );
    DCX_SetHigh( );
    spi_write_nbytes( &d, 1 );
    CS_SetHigh( );
}

void SSD1309_full_screen ( uint8_t color )
{
	unsigned int x,y;

	SSD1309_WrCmd( 0x15 );
	SSD1309_WrCmd( 0x00 );
	SSD1309_WrCmd( 0x7F );

	SSD1309_WrCmd( 0x75 );
	SSD1309_WrCmd( 0x00 );
	SSD1309_WrCmd( 0x3F );

	for(y = 0;y < 64;y ++)
	{
    	for(x = 0;x < 128;x ++)
		{
    		SSD1309_WrData( 0xFF );
		}
	}
//	color = ( color > 0 ) ? 0xFF : 0x00;
//
//	for( uint8_t i = 0 ; i < (ResolutionHeight / 8) ; i++ )
//	{
//		SSD1309_WrCmd( 0xB0 + i );
//		SSD1309_WrCmd( 0x10 );
//		SSD1309_WrCmd( 0x00 );
//
//		for( uint8_t j = 0 ; j < ResolutionWidth ; j++ )
//			SSD1309_WrData( 0x00 );
//	}
}

void SSD1309_full_screen_pointer ( bool convert, uint8_t *addr )
{
	if( convert == false )
	{
		for( uint8_t i = 0 ; i < (ResolutionHeight / 8) ; i++ )
		{
			SSD1309_WrCmd( 0xB0 + i );
			SSD1309_WrCmd( 0x10 );
			SSD1309_WrCmd( 0x00 );

			for( uint8_t j = 0 ; j < ResolutionWidth ; j++ )
				SSD1309_WrData( *addr++ );
		}
	}
	else
	{
		for( uint8_t i = 0 ; i < (ResolutionHeight / 8) ; i++ )
		{
			SSD1309_WrCmd( 0xB0 + i );
			SSD1309_WrCmd( 0x10 );
			SSD1309_WrCmd( 0x00 );

			for( uint8_t j = 0 ; j < ResolutionWidth ; j++ )
			{
				uint16_t Byte = ((uint16_t)i * 8 * ResolutionWidth + j) / 8;
				uint8_t bit = ((uint16_t)i * 8 * ResolutionWidth + j) % 8;
				uint8_t color = 0;

				for( uint8_t k = 0 ; k < 8 ; k++ )
				{
					uint8_t mask = 0x80 >> bit;

					color |= ( addr[Byte] & mask ) ? (0x01 << k) : 0x00;

					Byte += ResolutionWidth / 8;
				}

				SSD1309_WrData( color );//set screen pixel value
			}
		}
	}
}

void SSD1309_Set_Contrast ( uint8_t Contrast )
{
	SSD1309_WrCmd( 0x81 ); //Set Contrast Control for Bank 0
	SSD1309_WrCmd( Contrast );
	SSD1309_Contrast = Contrast;
}

uint8_t SSD1309_Get_Contrast ( void )
{
	return SSD1309_Contrast;
}
