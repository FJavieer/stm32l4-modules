/******************************************************************************
 *	tim_test.c							      *
 *	2018/dec/25							      *
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
	#include "alx_mask.h"

/* STM32L4 modules -----------------------------------------------------------*/
	#include "delay.h"
	#include "errors.h"
	#include "led.h"

	#include "tim.h"

	#include "tim_test.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/
struct	Tim_Test_Data {
	float	period_us;
};
typedef	struct Tim_Test_Data	Tim_Test_Data_s;


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	flash_u8	(void *data);
static	int	flash_sos	(void *data);
static	int	flash_long	(float bit_len);
static	int	flash_short	(float bit_len);
static	int	execution_loop	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Test TIM periodic interrupts
	 * @param	freq_hz:	frequency of the interrupts
	 * @return	Error
	 * @note	Tim should be initialized before calling tim_test
	 */
int	tim_test	(uint32_t freq_hz)
{
	Tim_Test_Data_s	tim_test_data;

	tim_test_data.period_us	= (1.0 / freq_hz) * 1000000;

	if (delay_us_init()) {
		return	ERROR_NOK;
	}
	led_init();

	if (tim_callback_push(&flash_u8, (void *)&tim_test_data)) {
		return	ERROR_NOK;
	}
	if (execution_loop()) {
		return	ERROR_NOK;
	}

	if (tim_callback_push(&flash_sos, (void *)&tim_test_data)) {
		return	ERROR_NOK;
	}
	if (execution_loop()) {
		return	ERROR_NOK;
	}

	if (tim_callback_pop()) {
		return	ERROR_NOK;
	}
	if (tim_callback_pop()) {
		return	ERROR_NOK;
	}
	if (tim_callback_push(&flash_sos, (void *)&tim_test_data)) {
		return	ERROR_NOK;
	}
	if (execution_loop()) {
		return	ERROR_NOK;
	}

	if (tim_callback_pop()) {
		return	ERROR_NOK;
	}
	if (tim_callback_push(&flash_u8, (void *)&tim_test_data)) {
		return	ERROR_NOK;
	}
	if (tim_callback_push(&flash_sos, (void *)&tim_test_data)) {
		return	ERROR_NOK;
	}
	if (execution_loop()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	flash_u8	(void *data)
{
	static	uint8_t	num	= 0;
	Tim_Test_Data_s	*data_cast;
	int		i;
	bool		bit;
	float		bit_len;

	data_cast	= (Tim_Test_Data_s *)data;
	bit_len		= (data_cast->period_us / 3) / 8;

	for (i = 7; i >= 0; i--) {
		bit	= (bool)(num & alx_maskgen_u8(i));

		if (bit) {
			if (flash_long(bit_len)) {
				return	ERROR_NOK;
			}
		} else {
			if (flash_short(bit_len)) {
				return	ERROR_NOK;
			}
		}
	}

	num++;

	return	ERROR_OK;
}

static	int	flash_sos	(void *data)
{
	Tim_Test_Data_s	*data_cast;
	float		bit_len;

	data_cast	= (Tim_Test_Data_s *)data;
	bit_len		= (data_cast->period_us / 3) / 8;

	if (flash_short(bit_len)) {
		return	ERROR_NOK;
	}
	if (flash_short(bit_len)) {
		return	ERROR_NOK;
	}
	if (flash_short(bit_len)) {
		return	ERROR_NOK;
	}

	if (flash_long(bit_len)) {
		return	ERROR_NOK;
	}
	if (flash_long(bit_len)) {
		return	ERROR_NOK;
	}
	if (flash_long(bit_len)) {
		return	ERROR_NOK;
	}

	if (flash_short(bit_len)) {
		return	ERROR_NOK;
	}
	if (flash_short(bit_len)) {
		return	ERROR_NOK;
	}
	if (flash_short(bit_len)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	flash_long	(float bit_len)
{
	led_set();
	if (delay_us(bit_len * 0.8)) {
		return	ERROR_NOK;
	}

	led_reset();
	if (delay_us(bit_len * 0.2)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	flash_short	(float bit_len)
{
	led_set();
	if (delay_us(bit_len * 0.2)) {
		return	ERROR_NOK;
	}

	led_reset();
	if (delay_us(bit_len * 0.8)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	execution_loop	(void)
{
	int	i;

	for (i = 0; i <= 10; i++) {
		__WFE();
		if (tim_tim3_interrupt) {
			if (tim_callback_exe()) {
				return	ERROR_NOK;
			}
			tim_tim3_interrupt	= false;
			i++;
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
