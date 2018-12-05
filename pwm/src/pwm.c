/******************************************************************************
 *	pwm.c								      *
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
/* STM32L4 modules -----------------------------------------------------------*/
	#include "pwm.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	TIM_HandleTypeDef	tim_handle;
static	TIM_ClockConfigTypeDef	clk_config;
static	TIM_MasterConfigTypeDef	master_config;
static	TIM_OC_InitTypeDef	oc_init;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize base time for PWM using TIM2.
	 * @param	resolution_s:	divisions in 1 s.
	 * @param	period:		period of the pwm (in resolution_s units).
	 * @return	error
	 */
uint32_t	pwm_tim2_init		(uint32_t resolution_s, uint32_t period)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERR_PWM_OK;
	}

	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Resolution: 1 us;  Periodo: 1 ms */
	tim_handle.Instance		= TIM2;
	tim_handle.Init.Prescaler		= SystemCoreClock / resolution_s - 1;
	tim_handle.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim_handle.Init.Period			= period - 1;
	tim_handle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.RepetitionCounter	= 0x00u;
	tim_handle.Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK) {
		return	ERR_PWM_TIM_INIT;
	}

	clk_config.ClockSource		= TIM_CLOCKSOURCE_INTERNAL;
	clk_config.ClockPolarity	= TIM_CLOCKPOLARITY_INVERTED;
	clk_config.ClockPrescaler	= TIM_CLOCKPRESCALER_DIV1;
	clk_config.ClockFilter		= 0;
	if (HAL_TIM_ConfigClockSource(&tim_handle, &clk_config) != HAL_OK) {
		return	ERR_PWM_TIM_CLK_CONF;
	}

	master_config.MasterOutputTrigger	= TIM_TRGO_RESET;
	master_config.MasterSlaveMode		= TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&tim_handle,
						&master_config) != HAL_OK) {
		return	ERR_PWM_TIM_MASTER_CONF;
	}

	if (HAL_TIM_PWM_Init(&tim_handle) != HAL_OK) {
		return	ERR_PWM_TIM_PWM_INIT;
	}

	/* Configure PWM values */
	oc_init.OCMode		= TIM_OCMODE_PWM1;
	oc_init.OCPolarity	= TIM_OCPOLARITY_HIGH;
	oc_init.OCFastMode	= TIM_OCFAST_DISABLE;
	oc_init.OCNPolarity	= TIM_OCNPOLARITY_HIGH;
	oc_init.OCNIdleState	= TIM_OCNIDLESTATE_RESET;
	oc_init.OCIdleState	= TIM_OCIDLESTATE_RESET;
	oc_init.OCIdleState	= TIM_OCIDLESTATE_RESET;

	return	ERR_PWM_OK;
}

	/**
	 * @brief	Set PWM using TIM2
	 * @param	duty_cycle:	duty cycle value (fraction)
	 * @param	chan:		channel to be used (1 through 4; 0=ALL)
	 * @return	error
	 */
uint32_t	pwm_tim2_chX_set	(float duty_cycle, int8_t chan)
{
	uint32_t	tim_chan;

	/* Invalid duty cycle */
	if (duty_cycle > 1.0 || duty_cycle < 0.0) {
		return	ERR_PWM_DUTY;
	}

	/* Init pending */
	if (init_pending) {
		return	ERR_PWM_INIT;
	}

	/* Select channel */
	switch (chan) {
	case 0:
		tim_chan	= TIM_CHANNEL_ALL;
		break;
	case 1:
		tim_chan	= TIM_CHANNEL_1;
		break;
	case 2:
		tim_chan	= TIM_CHANNEL_2;
		break;
	case 3:
		tim_chan	= TIM_CHANNEL_3;
		break;
	case 4:
		tim_chan	= TIM_CHANNEL_4;
		break;
	default:
		return	ERR_PWM_CHAN;
	}

	/* Initialize PWN with duty cycle */
	oc_init.Pulse	= tim_handle.Init.Period * duty_cycle;
	if (HAL_TIM_PWM_ConfigChannel(&tim_handle, &oc_init, tim_chan) != HAL_OK) {
		return	ERR_PWM_TIM_PWM_CONF;
	}

	/* Start PWM */
	if (HAL_TIM_PWM_Start(&tim_handle, tim_chan) != HAL_OK) {
		return	ERR_PWM_TIM_PWM_START;
	}

	return	ERR_PWM_OK;
}

	/**
	 * @brief	Stop PWM using TIM2
	 * @return	error
	 */
uint32_t	pwm_tim2_stop		(void)
{
	/* Initialize base time */
	if (init_pending) {
		return	ERR_PWM_INIT;
	}

	/* Stop timer */
	if (HAL_TIM_Base_Stop(&tim_handle) != HAL_OK) {
		return	ERR_PWM_TIM_BASE_STOP;
	}

	return	ERR_PWM_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
