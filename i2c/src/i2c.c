/**
 ******************************************************************************
 *	@file	i2c.c							      *
 *	@author	Colomar Andrés, Alejandro				      *
 *	@brief	I2C							      *
 ******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	LGPL-v2.0 only							      *
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

	#include "can.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	I2C_HandleTypeDef	i2c_handle;
/* Volatile ------------------------------------------------------------------*/
static	volatile	bool	i2c_msg_pending;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	void	i2c_gpio_init		(void);
static	void	i2c_nvic_conf		(void);
static	int	i2c_peripherial_init	(void);
static	int	i2c_filter_analog_conf	(void);
static	int	i2c_filter_digital_conf	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize I2C
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	i2c_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_I2C_INIT;
		return	ERROR_OK;
	}

	__HAL_RCC_I2C1_CLK_ENABLE();
	i2c_gpio_init();
	i2c_nvic_conf();
	if (i2c_peripherial_init()) {
		error	|= ERROR_I2C_HAL_I2C_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	if (i2c_filter_analog_conf()) {
		error	|= ERROR_I2C_HAL_I2C_FILTER_ANALOG;
		error_handle();
		return	ERROR_NOK;
	}
	if (i2c_filter_digital_conf()) {
		error	|= ERROR_I2C_HAL_I2C_FILTER_DIGITAL;
		error_handle();
		return	ERROR_NOK;
	}


	return	ERROR_OK;
}

	/**
	 * @brief	Transmit the message in data through I2C
	 *		Sets global variable 'error'
	 * @param	data:	data to transmit
	 * @return	Error
	 */
int	can_msg_write	(uint8_t data [CAN_DATA_LEN])
{
	uint8_t	can_tx_data [CAN_DATA_LEN];
	int	i;

	if (init_pending) {
		error	|= ERROR_CAN_INIT;
		return	ERROR_NOK;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		can_tx_data[i]	= data[i];
	}

	if (HAL_CAN_AddTxMessage(&can_handle, &can_tx_header, can_tx_data,
							&can_tx_mailbox)) {
		error	|= ERROR_CAN_HAL_ADD_TX_MSG;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Return the data received
	 *		Sets global variable 'error'
	 * @param	data:	array where data is to be written
	 * @return	Error
	 */
int	can_msg_read	(uint8_t data [CAN_DATA_LEN])
{
	int	i;

	if (init_pending) {
		error	|= ERROR_CAN_INIT;
		return	ERROR_NOK;
	}

	if (!can_msg_pending) {
		error	|= ERROR_CAN_NO_MSG;
		return	ERROR_NOK;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		data[i]	= can_rx_data[i];
	}

	can_msg_pending	= false;

	return	ERROR_OK;
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
static	void	i2c_gpio_init		(void)
{
	GPIO_InitTypeDef	gpio_init_values;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* PB6 -> I2C1_SCL // PB7 -> I2C1_SDA */
	gpio_init_values.Pin		= GPIO_PIN_6 | GPIO_PIN_7;
	gpio_init_values.Mode		= GPIO_MODE_AF_OD;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_init_values.Pull		= GPIO_NOPULL;
	gpio_init_values.Alternate	= GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &gpio_init_values);
}

static	void	i2c_nvic_conf		(void)
{// FIXME
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
}

static	int	i2c_peripherial_init	(void)
{
	i2c_handle.Instance		= I2C1;
	i2c_handle.Init.Timing			= 0x00000004u;
	i2c_handle.Init.OwnAddress1		= 0;
	i2c_handle.Init.AddressingMode		= I2C_ADDRESSINGMODE_7BIT;
	i2c_handle.Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
	i2c_handle.Init.OwnAddress2		= 0;
	i2c_handle.Init.OwnAddress2Masks	= I2C_OA2_NOMASK;
	i2c_handle.Init.GeneralCallMode		= I2C_GENERALCALL_DISABLE;
	i2c_handle.Init.NoStretchMode		= I2C_NOSTRETCH_DISABLE;

	return	HAL_I2C_Init(&i2c_handle);
}

static	int	i2c_filter_analog_conf	(void)
{
	return	HAL_I2CEx_ConfigAnalogFilter(&can_handle,
						I2C_ANALOGFILTER_ENABLE);
}

static	int	i2c_filter_digital_conf	(void)
{
	return	HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0x00000000u);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
