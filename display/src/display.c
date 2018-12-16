/******************************************************************************
 *	display.c							      *
 *	2018/dec/16							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdbool.h>
	#include <stdint.h>

/* Drivers -------------------------------------------------------------------*/
	#include "stm32l4xx_hal.h"

/* libalx --------------------------------------------------------------------*/
/* STM32L4 modules -----------------------------------------------------------*/
	#include "errors.h"

	#include "display.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	SPI_TIMEOUT	(500)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	SPI_HandleTypeDef	spi_handle;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	int	display_spi_init	(void);
static	void	display_gpio_init	(void);
static	void	display_display_init	(void);
static	void	display_data_set	(char ch, uint16_t data [8]);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init LED matrix display using SPI
	 *		PB13 -> SCK
	 *		PC3 -> MOSI
	 *		PC2 -> SELECT
	 *		Sets global variable 'error'
	 * @return	None
	 */
void	display_init	(void)
{
	GPIO_InitTypeDef gpio_init_values;

	__HAL_RCC_GPIOB_CLK_ENABLE ();
	__HAL_RCC_GPIOC_CLK_ENABLE ();
	__SPI2_CLK_ENABLE();

	if (display_spi_init()) {
		error	|= ;
		error_handle();
		return;
	}
	__HAL_SPI_ENABLE(&spi_handle);

	display_gpio_init();
	display_display_init();
}

	/**
	 * @brief	Show @param ch on the display
	 *		Sets global variable 'error'
	 * @param	ch:	The character to be displayed
	 * @return	None
	 */
void	display_set	(char ch)
{
	uint16_t	data [8];
	int		i;

	display_data_set(ch, data);

	for (i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&(data[i])),
							1, SPI_TIMEOUT)) {
			error	|= ;
			error_handle();
			return;
		}
		while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
			;
		}
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	display_spi_init	(void)
{
	spi_handle.Instance		= SPI2;
	spi_handle.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_32;
	spi_handle.Init.Direction		= SPI_DIRECTION_2LINES;
	spi_handle.Init.CLKPhase		= SPI_PHASE_1EDGE;
	spi_handle.Init.CLKPolarity		= SPI_POLARITY_LOW;
	spi_handle.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;
	spi_handle.Init.DataSize		= SPI_DATASIZE_16BIT;
	spi_handle.Init.FirstBit		= SPI_FIRSTBIT_MSB;
	spi_handle.Init.NSS			= SPI_NSS_SOFT;
	spi_handle.Init.TIMode			= SPI_TIMODE_DISABLE;
	spi_handle.Init.Mode			= SPI_MODE_MASTER;

	return	HAL_SPI_Init(&spi_handle);
}

static	void	display_gpio_init	(void)
{
	gpio_init_values.Pin		= GPIO_PIN_13;                
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_HIGH;
	gpio_init_values.Pull		= GPIO_NOPULL;
	gpio_init_values.Alternate	= GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &gpio_init_values);

	gpio_init_values.Pin		= GPIO_PIN_3;                
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_HIGH;
	gpio_init_values.Pull		= GPIO_NOPULL;
	gpio_init_values.Alternate	= GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &gpio_init_values);

	gpio_init_values.Pin		= GPIO_PIN_2;                
	gpio_init_values.Mode		= GPIO_MODE_OUTPUT_PP;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_HIGH;
	gpio_init_values.Pull		= GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio_init_values);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}

static	void	display_display_init	(void)
{
	uint16_t data;

	//Disable MAX7219
	data	= 0x0C00;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&data), 1, SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	//Disable Test Mode
	data	= 0x0F00;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&data), 1, SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	//Enable 8 Digits
	data	= 0x0BFF;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&data), 1, SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	//Set highest brightness
	data	= 0x0A0F;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&data), 1, SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	//Disable Mode BCD
	data	= 0x0900;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&data), 1, SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	//Enable MAX7219
	data	= 0x0C01;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&data), 1, SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	Set_Number_Matrix(10);
}

static	void	display_data_set	(char ch, uint16_t data [8])
{
	switch (ch) {
	case '0':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0344u;
		data[3]	= 0x0444u;
		data[4]	= 0x0544u;
		data[5]	= 0x0644u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '1':
		data[0]	= 0x0110u;
		data[1]	= 0x0230u;
		data[2]	= 0x0310u;
		data[3]	= 0x0410u;
		data[4]	= 0x0510u;
		data[5]	= 0x0610u;
		data[6]	= 0x0710u;
		data[7]	= 0x0838u;
		break;
	case '2':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0304u;
		data[3]	= 0x0404u;
		data[4]	= 0x0508u;
		data[5]	= 0x0610u;
		data[6]	= 0x0720u;
		data[7]	= 0x087Cu;
		break;
	case '3':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0304u;
		data[3]	= 0x0418u;
		data[4]	= 0x0504u;
		data[5]	= 0x0604u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '4':
		data[0]	= 0x0104u;
		data[1]	= 0x020Cu;
		data[2]	= 0x0314u;
		data[3]	= 0x0424u;
		data[4]	= 0x0544u;
		data[5]	= 0x067Cu;
		data[6]	= 0x0704u;
		data[7]	= 0x0804u;
		break;
	case '5':
		data[0]	= 0x017Cu;
		data[1]	= 0x0240u;
		data[2]	= 0x0340u;
		data[3]	= 0x0478u;
		data[4]	= 0x0504u;
		data[5]	= 0x0604u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '6':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0340u;
		data[3]	= 0x0478u;
		data[4]	= 0x0544u;
		data[5]	= 0x0644u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '7':
		data[0]	= 0x017Cu;
		data[1]	= 0x0204u;
		data[2]	= 0x0304u;
		data[3]	= 0x0408u;
		data[4]	= 0x0510u;
		data[5]	= 0x0620u;
		data[6]	= 0x0720u;
		data[7]	= 0x0820u;
		break;
	case '8':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0344u;
		data[3]	= 0x0438u;
		data[4]	= 0x0544u;
		data[5]	= 0x0644u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '9':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0344u;
		data[3]	= 0x0444u;
		data[4]	= 0x053Cu;
		data[5]	= 0x0604u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	default:
		data[0]	= 0x0100u;
		data[1]	= 0x0200u;
		data[2]	= 0x0300u;
		data[3]	= 0x0400u;
		data[4]	= 0x0500u;
		data[5]	= 0x0600u;
		data[6]	= 0x0700u;
		data[7]	= 0x0800u;
		error	|= ;
		break;
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
