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
	/**
	 * @brief	Initialize base time for PWM using TIM2
	 *		Sets global variable 'error'
	 * @param	resolution_s:	divisions in 1 s
	 * @param	period:		period of the pwm (in resolution_s units).
	 * @return	None
	 */
void	pwm_tim2_init		(uint32_t resolution_s, uint32_t period);

	/**
	 * @brief	Set PWM using TIM2
	 *		Sets global variable 'error'
	 * @param	duty_cycle:	duty cycle value (fraction)
	 * @param	chan:		channel to be used (1 through 4; 0=ALL)
	 * @return	None
	 */
void	pwm_tim2_chX_set	(float duty_cycle, uint32_t tim_chan);

	/**
	 * @brief	Stop PWM using TIM2
	 *		Sets global variable 'error'
	 * @return	None
	 */
void	pwm_tim2_stop		(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* pwm.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
