/******************************************************************************
 *	servo_test.c							      *
 *	2018/dec/05							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdint.h>

/* Drivers -------------------------------------------------------------------*/
	#include "stm32l4xx_hal.h"

/* libalx --------------------------------------------------------------------*/

/* STM32L4 modules -----------------------------------------------------------*/
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
	 * @return	error.
	 */
uint32_t	servo_test		(void)
{
	uint32_t	error	= ERR_SERVO_OK;


	error	|= servo_s1_init();
	error	|= servo_sX_position_set(-850, SERVO_S1);
	HAL_Delay(2000);

	error	|= servo_s3_init();
	error	|= servo_sX_position_set(-850, SERVO_S3);
	HAL_Delay(2000);

	error	|= servo_s4_init();
	error	|= servo_sX_position_set(500, SERVO_SALL);
	HAL_Delay(2000);

	error	|= servo_sALL_stop();
	HAL_Delay(2000);

	error	|= servo_s2_init();
	error	|= servo_sX_position_set(850, SERVO_S2);
	HAL_Delay(2000);

	error	|= servo_s4_init();
	error	|= servo_sX_position_set(850, SERVO_S4);
	HAL_Delay(2000);

	error	|= servo_s1_init();
	error	|= servo_sX_position_set(-900, SERVO_SALL);
	HAL_Delay(2000);

	error	|= servo_sALL_stop();
	HAL_Delay(2000);

	return	error;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
