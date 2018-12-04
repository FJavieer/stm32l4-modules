/******************************************************************************
 *	pwm.c								      *
 *	2018/nov/27							      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		PWM_H
	# define	PWM_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdint.h>

/* Drivers -------------------------------------------------------------------*/
/* libalx --------------------------------------------------------------------*/
/* STM32L4 modules -----------------------------------------------------------*/


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


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	/*
	 * @brief	Initialize base time for PWM.
	 * @param	resolution_s:	divisions in 1 s.
	 * @param	period:		period of the pwm (in resolution_s units).
	 */
int32_t	pwm_init	(uint32_t resolution_s, uint32_t period);

	/**
	 * @brief	Set PWM
	 * @param	duty_cycle:	duty cycle value (fraction).
	 */
int32_t	pwm_set		(float duty_cycle);

	/**
	 * @brief	Stop PWM.
	 */
int32_t	pwm_stop	(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* pwm.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
