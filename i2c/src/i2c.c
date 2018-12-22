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
	uint8_t			i2c_buff [I2C_BUFF_LEN];
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
	 * @param	addr:		address of slave
	 * @param	data_len:	length of data
	 * @param	data:		data to transmit
	 * @return	Error
	 */
int	i2c_msg_write	(uint8_t addr, uint8_t data_len, uint8_t data [data_len])
{
	if (init_pending) {
		if (i2c_init()) {
			error	|= ERROR_I2C_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	if (data_len > I2C_BUFF_SIZE) {
		error	|= ERROR_I2C_BUFF;
		return	ERROR_NOK;
	}

	for (i = 0; i < data_len; i++) {
		buff[i]	= data[i];
	}

	/*  <<1 is because of HAL bug */
	if (HAL_I2C_Master_Transmit_IT(&i2c_handle, addr <<1, buff, data_len)) {
		error	|= ;
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

	if (data_len > I2C_BUFF_SIZE) {
		error	|= ERROR_I2C_BUFF;
		return	ERROR_NOK;
	}

	/*  <<1 is because of HAL bug */
	if (HAL_I2C_Master_Receive_IT(&i2c_handle, addr <<1, buff, data_len)) {
		error	|= ;
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
	return	(HAL_I2C_GetState(&i2c_handle) == HAL_I2C_STATE_READY);
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

	if (data_len > I2C_BUFF_SIZE) {
		error	|= ERROR_I2C_BUFF;
		return	ERROR_NOK;
	}

	for (i = 0; i < data_len; i++) {
		data[i]	= buff[i];
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
	HAL_I2C_EV_IRQHandler(&i2c_handle);
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

	gpio_init_values.Pin		= GPIO_PIN_6 | GPIO_PIN_7;
	gpio_init_values.Mode		= GPIO_MODE_AF_OD;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_init_values.Pull		= GPIO_NOPULL;
	gpio_init_values.Alternate	= GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &gpio_init_values);
}

static	void	i2c_nvic_conf		(void)
{
	HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C_PRIORITY, I2C_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}

static	int	i2c_peripherial_init	(void)
{
	// Wii works @ 100 kHz FIXME
	i2c_handle.Instance		= I2C1;
	i2c_handle.Init.Timing			= I2C_TIMING;
	i2c_handle.Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
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
	return	HAL_I2CEx_ConfigDigitalFilter(&i2c_handle, 0);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
