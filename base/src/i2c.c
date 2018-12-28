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
	# define	I2C_TIMING		(0x00D00E28u)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	uint8_t			i2c_buff [I2C_BUFF_SIZE];
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	I2C_HandleTypeDef	i2c;

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
		return	ERROR_OK;
	}

	i2c_msp_init();

	if (i2c_peripherial_init()) {
		error	|= ERROR_I2C_HAL_I2C_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	if (i2c_filter_analog_conf()) {
		error	|= ERROR_I2C_HAL_I2C_FILTER_A;
		error_handle();
		return	ERROR_NOK;
	}
	if (i2c_filter_digital_conf()) {
		error	|= ERROR_I2C_HAL_I2C_FILTER_D;
		error_handle();
		return	ERROR_NOK;
	}


	return	ERROR_OK;
}

	/**
	 * @brief	Transmit the message in data through I2C
	 *		Sets global variable 'error'
	 * @param	addr:		address of slave
	 * @param	data_len:	length of data
	 * @param	data:		data to transmit
	 * @return	Error
	 */
int	i2c_msg_write	(uint8_t addr, uint8_t data_len, uint8_t data [data_len])
{
	int	i;

	if (init_pending) {
		if (i2c_init()) {
			error	|= ERROR_I2C_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	for (i = 0; i < data_len; i++) {
		i2c_buff[i]	= data[i];
	}

	/*  << 1 is because of HAL bug */
	if (HAL_I2C_Master_Transmit_IT(&i2c, addr << 1, i2c_buff, data_len)) {
		error	|= ERROR_I2C_TRANSMIT;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Read the data received
	 *		Sets global variable 'error'
	 * @param	addr:		address of slave
	 * @param	data_len:	length of data to be received
	 * @return	Error
	 */
int	i2c_msg_ask	(uint8_t addr, uint8_t data_len)
{
	if (init_pending) {
		if (i2c_init()) {
			error	|= ERROR_I2C_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	/*  << 1 is because of HAL bug */
	if (HAL_I2C_Master_Receive_IT(&i2c, addr << 1, i2c_buff, data_len)) {
		error	|= ERROR_I2C_RECEIVE;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	returns true if transmit has finished
	 * @return	ready:	true if ready
	 */
bool	i2c_msg_ready	(void)
{
	return	(HAL_I2C_GetState(&i2c) == HAL_I2C_STATE_READY);
}

	/**
	 * @brief	Read the data received
	 *		Sets global variable 'error'
	 * @param	data_len:	length of data
	 * @param	data:		array where data is to be written
	 * @return	Error
	 */
int	i2c_msg_read	(uint8_t data_len, uint8_t data [data_len])
{
	int	i;

	if (init_pending) {
		if (i2c_init()) {
			error	|= ERROR_I2C_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	if (!i2c_msg_ready()) {
		error	|= ERROR_I2C_NOT_READY;
		return	ERROR_NOK;
	}

	for (i = 0; i < data_len; i++) {
		data[i]	= i2c_buff[i];
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* HAL weak functions (redefinitions) ***********************************
 ******************************************************************************/
	/**
	 * @brief	This function handles I2C event interrupt request
	 */
void	I2Cx_EV_IRQHandler		(void)
{
	HAL_I2C_EV_IRQHandler(&i2c);
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
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio.Pin	= GPIO_PIN_6 | GPIO_PIN_7;
	gpio.Mode	= GPIO_MODE_AF_OD;
	gpio.Speed	= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Alternate	= GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &gpio);
}

static	void	i2c_nvic_conf		(void)
{
	HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C_PRIORITY, I2C_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}

static	int	i2c_peripherial_init	(void)
{
	// Wii works @ 100 kHz FIXME
	i2c.Instance		= I2C1;
	i2c.Init.Timing			= I2C_TIMING;
	i2c.Init.DualAddressMode	= I2C_DUALADDRESS_DISABLE;
	i2c.Init.GeneralCallMode	= I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode		= I2C_NOSTRETCH_DISABLE;

	return	HAL_I2C_Init(&i2c);
}

static	int	i2c_filter_analog_conf	(void)
{
	return	HAL_I2CEx_ConfigAnalogFilter(&i2c,
						I2C_ANALOGFILTER_ENABLE);
}

static	int	i2c_filter_digital_conf	(void)
{
	return	HAL_I2CEx_ConfigDigitalFilter(&i2c, 0);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
