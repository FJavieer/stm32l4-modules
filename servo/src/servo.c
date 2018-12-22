/******************************************************************************
 *	servo.c								      *
 *	2018/nov/27							      *
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
		/* alx_scale_linear_i16() */
	#include "alx_math.h"

/* STM32L4 modules -----------------------------------------------------------*/
	#include "errors.h"
		/* pwm_init(), pwm_set(), pwm_stop() */
	#include "pwm.h"

	#include "servo.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
		/* Resolution = 1/1000000 s = 1 us */
	# define	SERVO_PWM_RESOLUTION_s		(1000000u)

		/* Period = 20 ms = 20000 us */
	# define	SERVO_PWM_PERIOD_us		(20000u)

		/* Angle = 0 deg -> Duty time = 1.5 ms -> Duty cycle = 0.075 */
	# define	SERVO_PWM_DUTY_DEF		(0.075f)
		/* Angle = -90 deg -> Duty time = 1 ms -> Duty cycle = 0.05 */
	# define	SERVO_PWM_DUTY_MIN		(0.020f)//(0.05f) (from datashit)
		/* Angle = 90 deg -> Duty time = 2 ms -> Duty cycle = 0.1 */
	# define	SERVO_PWM_DUTY_MAX		(0.120f)//(0.1f) (from datashit)


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
static	float	duty_cycle [SERVO_QTY];
static	bool	init_pending	= true;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
	/**
	 * @brief	Calculate PWM duty cycle for specified servo position
	 * @param	position:	position (deg)
	 *				valid range: [-90, 900]
	 * @param	duty:			Duty cycle
	 *				valid range: [0, 1]
	 * @return	None
	 */
static	int	servo_duty_calc	(float position, float *duty);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init servos
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	servo_init		(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	if (pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us)) {
		error	|= ERROR_SERVO_PWM_INIT;
		error_handle();
		return	ERROR_NOK;
	}

	if (pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, TIM_CHANNEL_1)) {
		error	|= ERROR_SERVO_PWM_SET;
		error_handle();
		return	ERROR_NOK;
	}
	if (pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, TIM_CHANNEL_2)) {
		error	|= ERROR_SERVO_PWM_SET;
		error_handle();
		return	ERROR_NOK;
	}
	if (pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, TIM_CHANNEL_3)) {
		error	|= ERROR_SERVO_PWM_SET;
		error_handle();
		return	ERROR_NOK;
	}
	if (pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, TIM_CHANNEL_4)) {
		error	|= ERROR_SERVO_PWM_SET;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Set servo sX position
	 *		Sets global variable 'error'
	 * @param	servo:		servo to set
	 * @param	position:	position (deg)
	 *				valid range: [-90, 90]
	 * @return	Error
	 */
int	servo_position_set	(int8_t servo, float position)
{
	uint32_t	tim_chan;

	if (init_pending) {
		if (servo_init()) {
			error	|= ERROR_SERVO_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	switch (servo) {
	case SERVO_S1:
		tim_chan	= TIM_CHANNEL_1;
		break;
	case SERVO_S2:
		tim_chan	= TIM_CHANNEL_2;
		break;
	case SERVO_S3:
		tim_chan	= TIM_CHANNEL_3;
		break;
	case SERVO_S4:
		tim_chan	= TIM_CHANNEL_4;
		break;
	default:
		error	|= ERROR_SERVO_ID;
		error_handle();
		return	ERROR_NOK;
	}

	servo_duty_calc(position, &duty_cycle[servo]);
	if (pwm_tim2_chX_set(duty_cycle[servo], tim_chan)) {
		error	|= ERROR_SERVO_PWM_SET;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Stop servos
	 * @return	Error
	 */
int	servo_stop		(void)
{
	if (init_pending) {
		return	ERROR_OK;
	}

	if (pwm_tim2_stop()) {
		error	|= ERROR_SERVO_PWM_STOP;
		error_handle();
		return	ERROR_NOK;
	}

	init_pending	= true;

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
	/**
	 * @brief	Calculate PWM duty cycle for specified servo position
	 * @param	position:	position (deg)
	 *				valid range: [-90, 90]
	 * @param	duty:		Duty cycle
	 *				valid range: [0, 1]
	 * @return	saturation:	0 =	OK,
	 *				>0 =	POSITIVE SATURATION,
	 *				<0 =	NEGATIVE SATURATION
	 */
static	int	servo_duty_calc	(float position, float *duty)
{
	int	saturation	= SERVO_SATURATION_OK;

	/* Check if there is saturation */
	if (position < (SERVO_ANGLE_MIN)) {
		saturation	= SERVO_SATURATION_NEG;
	}
	if (position > (SERVO_ANGLE_MAX)) {
		saturation	= SERVO_SATURATION_POS;
	}

	/* set duty cycle */
	switch (saturation) {
	case SERVO_SATURATION_NEG:
		*duty	= SERVO_PWM_DUTY_MIN;
		break;
	case SERVO_SATURATION_POS:
		*duty	= SERVO_PWM_DUTY_MAX;
		break;
	default:
		*duty	= alx_scale_linear_f(position,
				SERVO_ANGLE_MIN, SERVO_ANGLE_MAX,
				SERVO_PWM_DUTY_MIN, SERVO_PWM_DUTY_MAX);
		break;
	}

	return	saturation;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
