/******************************************************************************
 *	servo_test.c							      *
 *	2018/dec/05							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
/* Drivers -------------------------------------------------------------------*/
	#include "stm32l4xx_hal.h"

/* libalx --------------------------------------------------------------------*/

/* STM32L4 modules -----------------------------------------------------------*/
	#include "delay.h"
	#include "errors.h"

	#include "servo.h"

	#include "servo_test.h"


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
 ******* static functions (declarations) **************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Test servos
	 * @return	Error
	 */
int	servo_test	(void)
{
	int	i;

	for (i = 0; i <= 90; i+=10) {
		if (servo_sX_position_set(-(i*10), SERVO_S1)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(-(i*10), SERVO_S2)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(-(i*10), SERVO_S3)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(-(i*10), SERVO_S4)) {
			return	ERROR_NOK;
		}
		if (delay_us(1000000u)) {
			return	ERROR_NOK;
		}

		if (servo_sX_position_set(-0, SERVO_S1)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(-0, SERVO_S2)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(-0, SERVO_S3)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(-0, SERVO_S4)) {
			return	ERROR_NOK;
		}
		if (delay_us(1000000u)) {
			return	ERROR_NOK;
		}

		if (servo_sX_position_set((i*10), SERVO_S1)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set((i*10), SERVO_S2)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set((i*10), SERVO_S3)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set((i*10), SERVO_S4)) {
			return	ERROR_NOK;
		}
		if (delay_us(1000000u)) {
			return	ERROR_NOK;
		}

		if (servo_sX_position_set(0, SERVO_S1)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(0, SERVO_S2)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(0, SERVO_S3)) {
			return	ERROR_NOK;
		}
		if (servo_sX_position_set(0, SERVO_S4)) {
			return	ERROR_NOK;
		}
		if (delay_us(1000000u)) {
			return	ERROR_NOK;
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
