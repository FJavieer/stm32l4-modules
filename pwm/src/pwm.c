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
	/*
	 * @brief	Initialize base time for PWM.
	 * @param	resolution_s:	divisions in 1 s.
	 * @param	period:		period of the pwm (in resolution_s units).
	 */
int32_t	pwm_init	(uint32_t resolution_s, uint32_t period)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	0;
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
		return	-2;
	}

	clk_config.ClockSource		= TIM_CLOCKSOURCE_INTERNAL;
	clk_config.ClockPolarity	= TIM_CLOCKPOLARITY_INVERTED;
	clk_config.ClockPrescaler	= TIM_CLOCKPRESCALER_DIV1;
	clk_config.ClockFilter		= 0;
	if (HAL_TIM_ConfigClockSource(&tim_handle, &clk_config) != HAL_OK) {
		return	-3;
	}

	master_config.MasterOutputTrigger	= TIM_TRGO_RESET;
	master_config.MasterSlaveMode		= TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&tim_handle,
						&master_config) != HAL_OK) {
		return	-3;
	}

	if (HAL_TIM_PWM_Init(&tim_handle) != HAL_OK) {
		return	-4;
	}

	/* Configure PWM values */
	oc_init.OCMode		= TIM_OCMODE_PWM1;
	oc_init.OCPolarity	= TIM_OCPOLARITY_HIGH;
	oc_init.OCFastMode	= TIM_OCFAST_DISABLE;
	oc_init.OCNPolarity	= TIM_OCNPOLARITY_HIGH;
	oc_init.OCNIdleState	= TIM_OCNIDLESTATE_RESET;
	oc_init.OCIdleState	= TIM_OCIDLESTATE_RESET;
	oc_init.OCIdleState	= TIM_OCIDLESTATE_RESET;

	return	0;
}

	/**
	 * @brief	Set PWM
	 * @param	duty_cycle:	duty cycle value (fraction).
	 */
int32_t	pwm_set	(float duty_cycle)
{
	/* Invalid duty cycle */
	if (duty_cycle > 1.0 || duty_cycle < 0.0) {
		return	-5;
	}

	/* Initialize base time */
	if (init_pending) {
		return	-1;
	}

	/* Initialize PWN with duty cycle */
	oc_init.Pulse	= tim_handle.Init.Period * duty_cycle;
	if (HAL_TIM_PWM_ConfigChannel(&tim_handle, &oc_init,
						TIM_CHANNEL_1) != HAL_OK) {
		return	-6;
	}

	/* Start PWM */
	if (HAL_TIM_PWM_Start(&tim_handle, TIM_CHANNEL_1) != HAL_OK) {
		return	-7;
	}

	return	0;
}

	/**
	 * @brief	Stop PWM.
	 */
int32_t	pwm_stop	(void)
{
	/* Initialize base time */
	if (init_pending) {
		return	-1;
	}

	/* Stop timer */
	if (HAL_TIM_Base_Stop(&tim_handle) != HAL_OK) {
		return	-8;
	}

	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
