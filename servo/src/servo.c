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
static	GPIO_InitTypeDef	gpio_init_values;
static	float			duty_cycle[SERVO_QTY];
static	bool			init_pending[SERVO_QTY]	= {
					true, true, true, true, true};


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
	/**
	 * @brief	Calculate PWM duty cycle for specified servo position
	 * @param	position_decimals:	position (deg) multiplied by 10.
	 *			valid range: [-900, 900]
	 * @param	duty:			Duty cycle
	 *			valid range: [0, 1]
	 * @return	error.
	 */
static	uint32_t	servo_duty_calc	(int16_t position_decimals, float *duty);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init servo s1 in PA15 using TIM2_CH1
	 * @return	error.
	 */
uint32_t	servo_s1_init		(void)
{
	uint32_t	error	= ERR_SERVO_OK;

	/* Init pending */
	if (init_pending[SERVO_S1]) {
		init_pending[SERVO_S1]	= false;
		init_pending[SERVO_SALL]	= init_pending[SERVO_S1]  ||
						init_pending[SERVO_S2]  ||
						init_pending[SERVO_S3]  ||
						init_pending[SERVO_S4];
	} else {
		return	ERR_SERVO_OK;
	}

	/* Initialize TIM2 for PWM */
	error	|= pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	gpio_init_values.Pin		= GPIO_PIN_15;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOA, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	error	|= pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, 1);

	return	error;
}
	/**
	 * @brief	Init servo s2 in PA1 using TIM2_CH2
	 * @return	error.
	 */
uint32_t	servo_s2_init		(void)
{
	uint32_t	error	= ERR_SERVO_OK;

	/* Init pending */
	if (init_pending[SERVO_S2]) {
		init_pending[SERVO_S2]	= false;
		init_pending[SERVO_SALL]	= init_pending[SERVO_S1]  ||
						init_pending[SERVO_S2]  ||
						init_pending[SERVO_S3]  ||
						init_pending[SERVO_S4];
	} else {
		return	ERR_SERVO_OK;
	}

	/* Initialize TIM2 for PWM */
	error	|= pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	gpio_init_values.Pin		= GPIO_PIN_1;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOA, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	error	|= pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, 2);

	return	error;
}
	/**
	 * @brief	Init servo s3 in PB10 using TIM2_CH3
	 * @return	error.
	 */
uint32_t	servo_s3_init		(void)
{
	uint32_t	error	= ERR_SERVO_OK;

	/* Init pending */
	if (init_pending[SERVO_S3]) {
		init_pending[SERVO_S3]	= false;
		init_pending[SERVO_SALL]	= init_pending[SERVO_S1]  ||
						init_pending[SERVO_S2]  ||
						init_pending[SERVO_S3]  ||
						init_pending[SERVO_S4];
	} else {
		return	ERR_SERVO_OK;
	}

	/* Initialize TIM2 for PWM */
	error	|= pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	gpio_init_values.Pin		= GPIO_PIN_10;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOB, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	error	|= pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, 3);

	return	error;
}
	/**
	 * @brief	Init servo s4 in PB11 using TIM2_CH4
	 * @return	error.
	 */
uint32_t	servo_s4_init		(void)
{
	uint32_t	error	= ERR_SERVO_OK;

	/* Init pending */
	if (init_pending[SERVO_S4]) {
		init_pending[SERVO_S4]		= false;
		init_pending[SERVO_SALL]	= init_pending[SERVO_S1]  ||
						init_pending[SERVO_S2]  ||
						init_pending[SERVO_S3]  ||
						init_pending[SERVO_S4];
	} else {
		return	ERR_SERVO_OK;
	}

	/* Initialize TIM2 for PWM */
	error	|= pwm_tim2_init(SERVO_PWM_RESOLUTION_s, SERVO_PWM_PERIOD_us);

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Configure the GPIO LED pin */
	gpio_init_values.Pin		= GPIO_PIN_11;
	gpio_init_values.Mode		= GPIO_MODE_AF_PP;	// alternate function
	gpio_init_values.Pull		= GPIO_NOPULL;		// no pulls
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Alternate	= GPIO_AF1_TIM2;	// Pin connected to TIM2 output
	HAL_GPIO_Init(GPIOB, &gpio_init_values);

	/* Initialize PWM with default duty cycle (0 DEG) */
	error	|= pwm_tim2_chX_set(SERVO_PWM_DUTY_DEF, 4);

	return	error;
}

	/**
	 * @brief	Set servo sX position
	 * @param	position_decimals:	position (deg) multiplied by 10.
	 *			valid range: [-900, 900]
	 * @return	error.
	 */
uint32_t	servo_sX_position_set	(int16_t position_decimals, int8_t servo)
{
	uint32_t	error	= ERR_SERVO_OK;

	/* Check if servo has been initialized */
	if (init_pending[servo]) {
		return	ERR_SERVO_INIT;
	}

	/* Calc duty */
	error	|= servo_duty_calc(position_decimals, &duty_cycle[servo]);
	/* set PWM */
	error	|= pwm_tim2_chX_set(duty_cycle[servo], servo);

	return	error;
}

	/**
	 * @brief	Get servo sX position
	 * @param	*position_decimals:	position (deg) multiplied by 10.
	 * @return	error.
	 */
uint32_t	servo_sX_position_get	(int16_t *position_decimals, int8_t servo)
{
	/* Check if servo has been initialized */
	if (init_pending[servo]) {
		return	ERR_SERVO_INIT;
	}

	*position_decimals	= alx_scale_linear_f(duty_cycle[SERVO_S1],
				SERVO_PWM_DUTY_MIN, SERVO_PWM_DUTY_MAX,
				SERVO_ANGLE_MIN * 10, SERVO_ANGLE_MAX * 10);

	return	ERR_SERVO_OK;
}

	/**
	 * @brief	Stop servos
	 * @return	error.
	 */
uint32_t	servo_sALL_stop		(void)
{
	uint32_t	error	= ERR_SERVO_OK;

	error	|= pwm_tim2_stop();

	init_pending[SERVO_S1]		= true;
	init_pending[SERVO_S2]		= true;
	init_pending[SERVO_S3]		= true;
	init_pending[SERVO_S4]		= true;

	return	error;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
	/**
	 * @brief	Calculate PWM duty cycle for specified servo position
	 * @param	position_decimals:	position (deg) multiplied by 10.
	 *			valid range: [-900, 900]
	 * @param	duty:			Duty cycle
	 *			valid range: [0, 1]
	 * @return	error.
	 */
static	uint32_t	servo_duty_calc	(int16_t position_decimals, float *duty)
{
	uint32_t	error	= ERR_SERVO_OK;

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
		*duty	= SERVO_PWM_DUTY_MIN;
		break;
	case ERR_SERVO_SATURATION_POS:
		*duty	= SERVO_PWM_DUTY_MAX;
		break;
	case ERR_SERVO_OK:
		*duty	= alx_scale_linear_f(position_decimals,
				SERVO_ANGLE_MIN * 10, SERVO_ANGLE_MAX * 10,
				SERVO_PWM_DUTY_MIN, SERVO_PWM_DUTY_MAX);
		break;
	default:
		break;
	}

	return	error;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
