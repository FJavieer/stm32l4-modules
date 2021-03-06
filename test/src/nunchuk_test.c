/******************************************************************************
 *	nunchuk_test.c							      *
 *	2018/dec/22							      *
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
	#include "display.h"
	#include "delay.h"
	#include "errors.h"
	#include "servo.h"

	#include "nunchuk.h"

	#include "nunchuk_test.h"


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


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	display_data_clear	(uint16_t display_data[DISPLAY_ROWS]);
static	void	display_data_set	(Nunchuk_Data_s	nunchuk_data,
					uint16_t display_data[DISPLAY_ROWS]);
static	void	nunchuk_servo_set	(Nunchuk_Data_s	nunchuk_data);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Test nunchuk
	 * @return	Error
	 */
int	nunchuk_test_1	(void)
{
	Nunchuk_Data_s	nunchuk_data;
	uint16_t	display_data [DISPLAY_ROWS];

	delay_us_init();
	display_init();
	nunchuk_init();

	do {
		if (nunchuk_read(&nunchuk_data)) {
			return	ERROR_NOK;
		}

		display_data_set(nunchuk_data, display_data);

		if (delay_us(1000000u)) {
			return	ERROR_NOK;
		}
	} while (!nunchuk_data.btn_z);

	return	ERROR_OK;
}

	/**
	 * @brief	Test nunchuk
	 * @return	Error
	 */
int	nunchuk_test_2	(void)
{
	Nunchuk_Data_s	nunchuk_data;

	delay_us_init();
	servo_init();
	nunchuk_init();

	do {
		if (nunchuk_read(&nunchuk_data)) {
			return	ERROR_NOK;
		}

		nunchuk_servo_set(nunchuk_data);

		if (delay_us(100000u)) {
			return	ERROR_NOK;
		}
	} while (true);
//	} while (!nunchuk_data.btn_z);

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	display_data_clear	(uint16_t display_data[DISPLAY_ROWS])
{
	int	i;

	for (i = 0; i < DISPLAY_ROWS; i++) {
		display_data[i]	= DISPLAY_ROW(i);
	}
}

static	void	display_data_set	(Nunchuk_Data_s	nunchuk_data,
					uint16_t display_data[DISPLAY_ROWS])
{
	display_data_clear(display_data);

	display_data[0]	|= nunchuk_data.jst.x;
	display_data[1]	|= nunchuk_data.jst.y;

	if (!nunchuk_data.btn_c) {
		display_data[2]	|= nunchuk_data.acc.x8;
		display_data[3]	|= nunchuk_data.acc.y8;
		display_data[4]	|= nunchuk_data.acc.z8;
	} else {
		display_data[5]	|= (nunchuk_data.acc.x10 >> 2);
		display_data[6]	|= (nunchuk_data.acc.y10 >> 2);
		display_data[7]	|= (nunchuk_data.acc.z10 >> 2);
	}
}

static	void	nunchuk_servo_set	(Nunchuk_Data_s	nunchuk_data)
{
	if (servo_position_set(SERVO_S1, ((float)nunchuk_data.acc.x8 / UINT8_MAX * 90))) {
		return;
	}
	if (servo_position_set(SERVO_S2, ((float)nunchuk_data.acc.y8 / UINT8_MAX * 90))) {
		return;
	}
	if (servo_position_set(SERVO_S3, ((float)nunchuk_data.acc.z8 / UINT8_MAX * 90))) {
		return;
	}
	if (servo_position_set(SERVO_S4, ((float)nunchuk_data.jst.x / UINT8_MAX * 90))) {
		return;
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
