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
static	SPI_HandleTypeDef	spi_handle;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	void	spi_clk_enable		(void);
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
		error	|= ERROR_SPI_INIT;
		return	ERROR_OK;
	}

	spi_clk_enable();
	spi_gpio_init();
	if (spi_peripherial_init()) {
		error	|= ERROR_SPI_HAL_SPI_INIT;
		error_handle();
		return	ERROR_GENERIC;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Transmit the message in data through SPI
	 *		Sets global variable 'error'
	 * @param	data:	data to transmit
	 * @return	None
	 */
int	spi_msg_write	(uint16_t data)
{
	uint8_t	spi_data [sizeof(uint16_t) / sizeof(uint8_t)];
	int	i;

	if (init_pending) {
		error	|= ERROR_SPI_INIT;
		error_handle();
		return	ERROR_GENERIC;
	}

	spi_data[0]	= data / (UINT8_MAX + 1u);
	spi_data[1]	= data % (UINT8_MAX + 1u);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

	if (HAL_SPI_Transmit(&spi_handle, spi_data, 1, SPI_TIMEOUT)) {
		error	|= ERROR_SPI_HAL_SPI_TRANSMIT;
		error_handle();
		return	ERROR_GENERIC;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		/* Empty loop */
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	spi_clk_enable		(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE ();
	__HAL_RCC_GPIOC_CLK_ENABLE ();
	__SPI2_CLK_ENABLE();
}

static	void	spi_gpio_init		(void)
{
	GPIO_InitTypeDef gpio_init_values;

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

static	int	spi_peripherial_init	(void)
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


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
