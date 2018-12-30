/******************************************************************************
 *	spi.c								      *
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

	#include "spi.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	SPI_TIMEOUT	(500)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	SPI_HandleTypeDef	spi;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	spi_gpio_init		(void);
static	int	spi_peripherial_init	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize SPI
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	spi_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	__SPI2_CLK_ENABLE();
	spi_gpio_init();
	if (spi_peripherial_init()) {
		prj_error	|= ERROR_SPI_HAL_SPI_INIT;
		prj_error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Transmit the message in data through SPI
	 *		Sets global variable 'error'
	 * @param	data:	data to transmit
	 * @return	Error
	 */
int	spi_msg_write	(uint16_t data)
{
	uint8_t	spi_data [sizeof(uint16_t) / sizeof(uint8_t)];

	if (init_pending) {
		if (spi_init()) {
			prj_error	|= ERROR_SPI_INIT;
			prj_error_handle();
			return	ERROR_NOK;
		}
	}

	spi_data[0]	= data / (UINT8_MAX + 1u);
	spi_data[1]	= data % (UINT8_MAX + 1u);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

	if (HAL_SPI_Transmit(&spi, spi_data, 1, SPI_TIMEOUT)) {
		prj_error	|= ERROR_SPI_HAL_SPI_TRANSMIT;
		prj_error_handle();
		return	ERROR_NOK;
	}
	while (HAL_SPI_GetState(&spi) != HAL_SPI_STATE_READY) {
		/* Empty loop */
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	spi_gpio_init		(void)
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOB_CLK_ENABLE ();

	gpio.Pin	= GPIO_PIN_13;
	gpio.Mode	= GPIO_MODE_AF_PP;
	gpio.Speed	= GPIO_SPEED_FREQ_HIGH;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Alternate	= GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &gpio);

	__HAL_RCC_GPIOC_CLK_ENABLE ();

	gpio.Pin	= GPIO_PIN_3;
	gpio.Mode	= GPIO_MODE_AF_PP;
	gpio.Speed	= GPIO_SPEED_FREQ_HIGH;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Alternate	= GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &gpio);

	gpio.Pin	= GPIO_PIN_2;
	gpio.Mode	= GPIO_MODE_OUTPUT_PP;
	gpio.Speed	= GPIO_SPEED_FREQ_HIGH;
	gpio.Pull	= GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}

static	int	spi_peripherial_init	(void)
{
	spi.Instance		= SPI2;
	spi.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_32;
	spi.Init.Direction		= SPI_DIRECTION_2LINES;
	spi.Init.CLKPhase		= SPI_PHASE_1EDGE;
	spi.Init.CLKPolarity		= SPI_POLARITY_LOW;
	spi.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;
	spi.Init.DataSize		= SPI_DATASIZE_16BIT;
	spi.Init.FirstBit		= SPI_FIRSTBIT_MSB;
	spi.Init.NSS			= SPI_NSS_SOFT;
	spi.Init.TIMode			= SPI_TIMODE_DISABLE;
	spi.Init.Mode			= SPI_MODE_MASTER;

	return	HAL_SPI_Init(&spi);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
