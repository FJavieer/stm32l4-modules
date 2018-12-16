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
	#include "errors.h"

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
static	TIM_OC_InitTypeDef	oc_init;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	int	pwm_tim2_tim_init	(void);
static	int	pwm_tim2_clk_conf	(void);
static	int	pwm_tim2_master_conf	(void);
static	void	pwm_tim2_oc_conf	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize base time for PWM using TIM2
	 *		Sets global variable 'error'
	 * @param	resolution_s:	divisions in 1 s
	 * @param	period:		period of the pwm (in resolution_s units)
	 * @return	None
	 */
void	pwm_tim2_init		(uint32_t resolution_s, uint32_t period)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_PWM_INIT;
		return;
	}

	__HAL_RCC_TIM2_CLK_ENABLE();
	if (pwm_tim2_tim_init()) {
		error	|= ERROR_PWM_HAL_TIM_INIT;
		error_handle();
		return;
	}
	if (pwm_tim2_clk_conf()) {
		error	|= ERROR_PWM_HAL_TIM_CLK_CONF;
		error_handle();
		return;
	}
	if (pwm_tim2_master_conf()) {
		error	|= ERROR_PWM_HAL_TIM_MASTER_CONF;
		error_handle();
		return;
	}
	if (HAL_TIM_PWM_Init(&tim_handle) != HAL_OK) {
		error	|= ERROR_PWM_HAL_TIM_PWM_INIT;
		error_handle();
		return;
	}
	pwm_tim2_oc_conf();
}

	/**
	 * @brief	Set PWM using TIM2
	 *		Sets global variable 'error'
	 * @param	duty_cycle:	duty cycle value (fraction)
	 * @param	chan:		channel to be used (1 through 4; 0=ALL)
	 * @return	None
	 */
void	pwm_tim2_chX_set	(float duty_cycle, uint32_t tim_chan)
{
	/* Invalid duty cycle */
	if (duty_cycle > 1.0 || duty_cycle < 0.0) {
		error	|= ERROR_PWM_DUTY;
		return;
	}

	/* Init pending */
	if (init_pending) {
		error	|= ERROR_PWM_INIT;
		return;
	}

	/* Initialize PWN with duty cycle */
	oc_init.Pulse	= tim_handle.Init.Period * duty_cycle;
	if (HAL_TIM_PWM_ConfigChannel(&tim_handle, &oc_init, tim_chan) != HAL_OK) {
		error	|= ERROR_PWM_HAL_TIM_PWM_CONF;
		error_handle();
		return;
	}

	/* Start PWM */
	if (HAL_TIM_PWM_Start(&tim_handle, tim_chan) != HAL_OK) {
		error	|= ERROR_PWM_HAL_TIM_PWM_START;
		error_handle();
		return;
	}
}

	/**
	 * @brief	Stop PWM using TIM2
	 *		Sets global variable 'error'
	 * @return	None
	 */
void	pwm_tim2_stop		(void)
{
	/* Initialize base time */
	if (init_pending) {
		error	|= ERROR_PWM_INIT;
		return;
	}

	/* Stop timer */
	if (HAL_TIM_Base_Stop(&tim_handle) != HAL_OK) {
		error	|= ERROR_PWM_HAL_TIM_STOP;
		error_handle();
		return;
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	pwm_tim2_tim_init	(void)
{
	/* Resolution: 1 us;  Periode: 1 ms */
	tim_handle.Instance		= TIM2;
	tim_handle.Init.Prescaler		= (SystemCoreClock /
							resolution_s) - 1u;
	tim_handle.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim_handle.Init.Period			= period - 1u;
	tim_handle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.RepetitionCounter	= 0x00u;
	tim_handle.Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_DISABLE;

	return	HAL_TIM_Base_Init(&tim_handle);
}

static	int	pwm_tim2_clk_conf	(void)
{
	TIM_ClockConfigTypeDef	clk_config;

	clk_config.ClockSource		= TIM_CLOCKSOURCE_INTERNAL;
	clk_config.ClockPolarity	= TIM_CLOCKPOLARITY_INVERTED;
	clk_config.ClockPrescaler	= TIM_CLOCKPRESCALER_DIV1;
	clk_config.ClockFilter		= 0;

	return	HAL_TIM_ConfigClockSource(&tim_handle, &clk_config);
}

static	int	pwm_tim2_master_conf	(void)
{
	TIM_MasterConfigTypeDef	master_config;

	master_config.MasterOutputTrigger	= TIM_TRGO_RESET;
	master_config.MasterSlaveMode		= TIM_MASTERSLAVEMODE_DISABLE;

	return	HAL_TIMEx_MasterConfigSynchronization(&tim_handle,
								&master_config);
}

static	void	pwm_tim2_oc_conf	(void)
{
	oc_init.OCMode		= TIM_OCMODE_PWM1;
	oc_init.OCPolarity	= TIM_OCPOLARITY_HIGH;
	oc_init.OCFastMode	= TIM_OCFAST_DISABLE;
	oc_init.OCNPolarity	= TIM_OCNPOLARITY_HIGH;
	oc_init.OCNIdleState	= TIM_OCNIDLESTATE_RESET;
	oc_init.OCIdleState	= TIM_OCIDLESTATE_RESET;
	oc_init.OCIdleState	= TIM_OCIDLESTATE_RESET;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
