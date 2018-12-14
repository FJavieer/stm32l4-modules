/******************************************************************************
 *	led.c								      *
 *	2018/dec/04							      *
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

	#include "led.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


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
static	bool	init_pending	= true;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init LED in GPIO_PIN_5
	 *		Sets global variable 'error'
	 * @return	None
	 */
void	led_init	(void)
{
	/* Initialize base time */
	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_LED_INIT;
		return;
	}
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	GPIO_InitTypeDef	gpio_init_values;
	gpio_init_values.Pin		= GPIO_PIN_5;		// LED2 pin
	gpio_init_values.Mode		= GPIO_MODE_OUTPUT_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_init_values);

	/* Reset LED */
	led_reset();
}

	/**
	 * @brief	LED on
	 *		Sets global variable 'error'
	 * @return	None
	 */
void	led_set		(void)
{
	if (init_pending) {
		error	|= ERROR_LED_INIT;
		return;
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

	/**
	 * @brief	LED off
	 *		Sets global variable 'error'
	 * @return	None
	 */
void	led_reset	(void)
{
	if (init_pending) {
		error	|= ERROR_LED_INIT;
		return;
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
