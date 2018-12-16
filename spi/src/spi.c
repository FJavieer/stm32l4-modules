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
	 * @return	None
	 */
void	spi_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_SPI_INIT;
		return;
	}

	spi_clk_enable();
	spi_gpio_init();
	if (spi_peripherial_init()) {
		error	|= ERROR_SPI_HAL_SPI_INIT;
		error_handle();
		return;
	}
}

	/**
	 * @brief	Transmit the message in data through SPI
	 *		Sets global variable 'error'
	 * @param	data:	data to transmit
	 * @return	None
	 */
void	spi_msg_write	(uint8_t data_len, uint8_t data [data_len])
{
	uint8_t	spi_data [data_len];
	int	i;

	if (init_pending) {
		error	|= ERROR_CAN_INIT;
		error_handle();
		return;
	}

	for (i = 0; i < data_len; i++) {
		spi_data[i]	= data[i];
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

	if (HAL_SPI_Transmit(&spi_handle, (uint8_t *)(&spi_data[0]), data_len,
								SPI_TIMEOUT)) {
		error	|= ;
		error_handle();
		return;
	}
	while (HAL_SPI_GetState(&spi_handle) != HAL_SPI_STATE_READY) {
		;
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}

	/**
	 * @brief	Return the data received
	 *		Sets global variable 'error'
	 * @param	data:	array where data is to be written
	 * @return	None
	 */
void	can_msg_read	(uint8_t data [CAN_DATA_LEN])
{
	/* FIXME */

	int	i;

	if (init_pending) {
		error	|= ERROR_CAN_INIT;
		error_handle();
		return;
	}

	if (!can_msg_pending) {
		error	|= ERROR_CAN_NO_MSG;
		return;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		data[i]	= can_rx_data[i];
	}

	can_msg_pending	= false;
}


/******************************************************************************
 ******* HAL weak functions (redefinitions) ***********************************
 ******************************************************************************/
/**
 * @brief	Rx Fifo 0 message pending callback
 * @param	hcan: pointer to a CAN_HandleTypeDef structure that contains
 *		the configuration information for the specified CAN.
 * @return	None
 */
void	HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *can_handle_ptr)
{
	if (HAL_CAN_GetRxMessage(can_handle_ptr, CAN_RX_FIFO0, &can_rx_header,
								can_rx_data)) {
		error	|= ERROR_CAN_HAL_GET_RX_MSG;
		error_handle();
	} else if (can_msg_pending) {
		error	|= ERROR_CAN_MSG_LOST;
	} else {
		can_msg_pending	= true;
	}
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
