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
		/* pwm_init(), pwm_set(), pwm_stop() */
	#include "pwm.h"

	#include "servo.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
		/* Resolution = 1/1000000 s = 1 us */
	# define	SERVO_PWM_RESOLUTION_s		1000000u

		/* Period = 20 ms = 20000 us */
	# define	SERVO_PWM_PERIOD_us		20000u

		/* Angle = 0 deg -> Duty time = 1.5 ms -> Duty cycle = 0.075 */
	# define	SERVO_PWM_DUTY_DEF		0.075f
		/* Angle = -90 deg -> Duty time = 1 ms -> Duty cycle = 0.05 */
	# define	SERVO_PWM_DUTY_MIN		0.05f
		/* Angle = 90 deg -> Duty time = 2 ms -> Duty cycle = 0.1 */
	# define	SERVO_PWM_DUTY_MAX		0.1f


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
static	float	duty;
static	bool	init_pending[5]	= true;
static	bool	init_pending	= true;
static	bool	init_pending	= true;
static	bool	init_pending	= true;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init servo s1 in PA15 (GPIOA, GPIO_PIN_15)
	 * @return	error.
	 */
int32_t	servo_s0_init		(void)
{
	/* Initialize base time */
	if (init_pending_s1) {
		init_pending_s1	= false;
	} else {
		return	0;
	}

	pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	GPIO_InitTypeDef	gpio_init_values;
	gpio_init_values.Pin		= GPIO_PIN_15;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOA, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	pwm_set(SERVO_PWM_DUTY_DEF);

	return	0;
}
	/**
	 * @brief	Init servo s2 in PA1 (GPIOA, GPIO_PIN_1)
	 * @return	error.
	 */
int32_t	servo_s2_init		(void)
{
	/* Initialize base time */
	if (init_pending_s2) {
		init_pending_s2	= false;
	} else {
		return	0;
	}

	pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	GPIO_InitTypeDef	gpio_init_values;
	gpio_init_values.Pin		= GPIO_PIN_1;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOA, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	pwm_set(SERVO_PWM_DUTY_DEF);

	return	0;
}
	/**
	 * @brief	Init servo s3 in PB10 (GPIOB, GPIO_PIN_10)
	 * @return	error.
	 */
int32_t	servo_s3_init		(void)
{
	/* Initialize base time */
	if (init_pending_s3) {
		init_pending_s3	= false;
	} else {
		return	0;
	}

	pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	GPIO_InitTypeDef	gpio_init_values;
	gpio_init_values.Pin		= GPIO_PIN_10;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOB, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	pwm_set(SERVO_PWM_DUTY_DEF);

	return	0;
}
	/**
	 * @brief	Init servo s4 in PB11 (GPIOB, GPIO_PIN_11)
	 * @return	error.
	 */
int32_t	servo_s4_init		(void)
{
	/* Initialize base time */
	if (init_pending_s4) {
		init_pending_s4	= false;
	} else {
		return	0;
	}

	pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	GPIO_InitTypeDef	gpio_init_values;
	gpio_init_values.Pin		= GPIO_PIN_11;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOB, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	pwm_set(SERVO_PWM_DUTY_DEF);

	return	0;
}

	/**
	 * @brief	Set servo position
	 * @param	position_decimals:	position (deg) multiplied by 10.
	 *			valid range: [-900, 900]
	 * @return	error.
	 */
int32_t	servo_s1_position_set	(int16_t position_decimals)
{
	int32_t	error	= ERR_SERVO_OK;

	/* Check if servo has been initialized */
	if (init_pending_s1) {
		return	ERR_SERVO_INIT;
	}

	/* Check if there is saturation */
	if (position_decimals < (SERVO_ANGLE_MIN * 10)) {
		error	= ERR_SERVO_SATURATION_NEG;
	}
	if (position_decimals > (SERVO_ANGLE_MAX * 10)) {
		error	= ERR_SERVO_SATURATION_POS;
	}

	/* set duty cycle */
	switch (error) {
	case ERR_SERVO_SATURATION_NEG:
		duty	= SERVO_PWM_DUTY_MIN;
		break;
	case ERR_SERVO_SATURATION_POS:
		duty	= SERVO_PWM_DUTY_MAX;
		break;
	case ERR_SERVO_OK:
		duty	= alx_scale_linear_f(position_decimals,
				SERVO_ANGLE_MIN * 10, SERVO_ANGLE_MAX * 10,
				SERVO_PWM_DUTY_MIN, SERVO_PWM_DUTY_MAX);
		break;
	default:
		return	error;;
	}

	/* set PWM */
	pwm_tim2_ch1_set(duty);

	return	error;
}

	/**
	 * @brief	Get servo position
	 * @param	*position_decimals:	position (deg) multiplied by 10.
	 * @return	error.
	 */
int32_t	servo_position_get	(int16_t *position_decimals)
{
	int32_t	error	= ERR_SERVO_OK;

	/* Check if servo has been initialized */
	if (init_pending) {
		error	= ERR_SERVO_INIT;
		return	error;
	}

	*position_decimals	= alx_scale_linear_f(duty,
				SERVO_PWM_DUTY_MIN, SERVO_PWM_DUTY_MAX,
				SERVO_ANGLE_MIN * 10, SERVO_ANGLE_MAX * 10);

	return	error;
}

	/**
	 * @brief	Stop servo
	 * @return	error.
	 */
int32_t	servo_stop		(void)
{
	pwm_stop();

	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
	/**
	 * @brief	Set servo position
	 * @param	position_decimals:	position (deg) multiplied by 10.
	 *			valid range: [-900, 900]
	 * @return	error.
	 */
int32_t	servo_s1_position_set	(int16_t position_decimals)
{
	int32_t	error	= ERR_SERVO_OK;

	/* Check if servo has been initialized */
	if (init_pending_s1) {
		error	= ERR_SERVO_INIT;
		return	error;
	}

	/* Check if there is saturation */
	if (position_decimals < (SERVO_ANGLE_MIN * 10)) {
		error	= ERR_SERVO_SATURATION_NEG;
	}
	if (position_decimals > (SERVO_ANGLE_MAX * 10)) {
		error	= ERR_SERVO_SATURATION_POS;
	}

	/* set duty cycle */
	switch (error) {
	case ERR_SERVO_SATURATION_NEG:
		duty	= SERVO_PWM_DUTY_MIN;
		break;
	case ERR_SERVO_SATURATION_POS:
		duty	= SERVO_PWM_DUTY_MAX;
		break;
	case ERR_SERVO_OK:
		duty	= alx_scale_linear_f(position_decimals,
				SERVO_ANGLE_MIN * 10, SERVO_ANGLE_MAX * 10,
				SERVO_PWM_DUTY_MIN, SERVO_PWM_DUTY_MAX);
		break;
	default:
		break;
	}

	/* set PWM */
	pwm_set(duty);

	return	error;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
