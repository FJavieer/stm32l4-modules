/**
 ******************************************************************************
 *	@file	i2c.c							      *
 *	@author	Colomar Andrés, Alejandro				      *
 *	@author	Javi							      *
 *	@brief	I2C							      *
 ******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	Copyright (C) 2018	Javi					      *
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

	#include "i2c.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	I2C_ADDRESS_OWN		(UINT16_C(0x00F))
	# define	I2C_ADDRESS_JOYSTICK	(UINT16_C(/*FIXME*/))
	# define	I2C_TIMING		(0x00D00E28u)
	# define	TX_BUFF_SIZE		(8u)
	# define	RX_BUFF_SIZE		(8u)
	# define	I2C_CR2_SADD_7BIT_SHIFT	(1)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	I2C_HandleTypeDef	i2c_handle;

/* Volatile ------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	void	i2c_msp_init		(void);
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

	i2c_msp_init();

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
int	can_msg_write	(uint8_t data [TX_BUFF_SIZE])
{
	uint8_t	i2c_tx_buff [TX_BUFF_SIZE];
	int	i;

	if (init_pending) {
		error	|= ERROR_I2C_INIT;
		return	ERROR_NOK;
	}

	for (i = 0; i < TX_BUFF_SIZE; i++) {
		i2c_tx_buff[i]	= data[i];
	}

	do {
		if (HAL_I2C_Master_Transmit_IT(&i2c_handle,
						I2C_ADDRESS_JOYSTICK <<
							I2C_CR2_SADD_7BIT_SHIFT,
						i2c_tx_buff, TX_BUFF_SIZE)) {
			error	|= ;
			error_handle();
			return	ERROR_NOK;
		}

		while (HAL_I2C_GetState(&i2c_handle) != HAL_I2C_STATE_READY) {
		}
	} while (HAL_I2C_GetError(&i2c_handle) == HAL_I2C_ERROR_AF);

	return	ERROR_OK;
}

	/**
	 * @brief	Read the data received
	 *		Sets global variable 'error'
	 * @param	data:	array where data is to be written
	 * @return	Error
	 */
int	can_msg_read	(uint8_t data [RX_BUFF_SIZE])
{
	int	i;

	if (init_pending) {
		error	|= ERROR_I2C_INIT;
		return	ERROR_NOK;
	}
/*
	if (!can_msg_pending) {
		error	|= ERROR_CAN_NO_MSG;
		return	ERROR_NOK;
	}
*/
	do {
		if (HAL_I2C_Master_Receive_IT(&i2c_handle,
						I2C_ADDRESS_JOYSTICK <<
							I2C_CR2_SADD_7BIT_SHIFT,
						i2c_rx_buff, RX_BUFF_SIZE)) {
			error	|= ;
			error_handle();
			return	ERROR_NOK;
		}

		while (HAL_I2C_GetState(&i2c_handle) != HAL_I2C_STATE_READY) {
		}
	} while (HAL_I2C_GetError(&i2c_handle) == HAL_I2C_ERROR_AF);

	for (i = 0; i < RX_BUFF_SIZE; i++) {
		data[i]	= can_rx_data[i];
	}

	can_msg_pending	= false;

	return	ERROR_OK;
}


/******************************************************************************
 ******* HAL weak functions (redefinitions) ***********************************
 ******************************************************************************/
	/**
	 * @brief	This function handles I2C event interrupt request
	 * @return	None
	 */
void	I2Cx_EV_IRQHandler		(void)
{
	HAL_I2C_EV_IRQHandler(&i2c_handle);
}

	/**
	 * @brief	This function handles I2C error interrupt request
	 * @return	None
	 */
void	I2Cx_ER_IRQHandler		(void)
{
	HAL_I2C_ER_IRQHandler(&i2c_handle);
}

	/**
	 * @brief	I2C error callbacks.
	 * @param	I2cHandle: I2C handle
	 * @note	This example shows a simple way to report transfer error, and you can
	 *		add your own implementation.
	 * @return	None
	 */
void	HAL_I2C_ErrorCallback		(I2C_HandleTypeDef *i2c_handle_ptr)
{
	/** Error_Handler() function is called when error occurs.
	 * 1- When Slave don't acknowledge it's address, Master restarts communication.
	 * 2- When Master don't acknowledge the last data transferred, Slave don't care in this example.
	 */
	if (HAL_I2C_GetError(i2c_handle_ptr) != HAL_I2C_ERROR_AF) {
		Error_Handler();
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	i2c_msp_init		(void)
{
	__HAL_RCC_I2C1_CLK_ENABLE();
	i2c_gpio_init();
	i2c_nvic_conf();
}

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
{
	HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}

static	int	i2c_peripherial_init	(void)
{
	// Wii works @ 100 kHz FIXME
	i2c_handle.Instance		= I2C1;
	i2c_handle.Init.Timing			= I2C_TIMING;
	i2c_handle.Init.OwnAddress1		= I2C_ADDRESS;
	i2c_handle.Init.AddressingMode		= I2C_ADDRESSINGMODE_10BIT;
	i2c_handle.Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
	i2c_handle.Init.OwnAddress2Masks	= I2C_OA2_NOMASK;
	i2c_handle.Init.GeneralCallMode		= I2C_GENERALCALL_DISABLE;
	i2c_handle.Init.NoStretchMode		= I2C_NOSTRETCH_DISABLE;

	return	HAL_I2C_Init(&i2c_handle);
}

static	int	i2c_filter_analog_conf	(void)
{
	return	HAL_I2CEx_ConfigAnalogFilter(&i2c_handle,
						I2C_ANALOGFILTER_ENABLE);
}

static	int	i2c_filter_digital_conf	(void)
{
	return	HAL_I2CEx_ConfigDigitalFilter(&i2c_handle, 0x00000000u);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
