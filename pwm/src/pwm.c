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
static	bool			init_pending	= true;
static	TIM_HandleTypeDef	tim;
static	TIM_OC_InitTypeDef	oc_init;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	pwm_tim2_tim_init	(uint32_t resolution_sec,
								uint32_t period);
static	int	pwm_tim2_clk_conf	(void);
static	int	pwm_tim2_master_conf	(void);
static	void	pwm_tim2_oc_conf	(void);
static	void	pwm_tim2_ch1_gpio_init	(void);
static	void	pwm_tim2_ch2_gpio_init	(void);
static	void	pwm_tim2_ch3_gpio_init	(void);
static	void	pwm_tim2_ch4_gpio_init	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize PWM using TIM2
	 *		Sets global variable 'error'
	 * @param	resolution_s:	divisions in 1 s
	 * @param	period:		period of the pwm (in resolution_s units)
	 * @return	Error
	 */
int	pwm_tim2_init		(uint32_t resolution_sec, uint32_t period)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	__HAL_RCC_TIM2_CLK_ENABLE();
	if (pwm_tim2_tim_init(resolution_sec, period)) {
		error	|= ERROR_PWM_HAL_TIM_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	if (pwm_tim2_clk_conf()) {
		error	|= ERROR_PWM_HAL_TIM_CLK_CONF;
		error_handle();
		return	ERROR_NOK;
	}
	if (pwm_tim2_master_conf()) {
		error	|= ERROR_PWM_HAL_TIM_MASTER_CONF;
		error_handle();
		return	ERROR_NOK;
	}
	if (HAL_TIM_PWM_Init(&tim)) {
		error	|= ERROR_PWM_HAL_TIM_PWM_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	pwm_tim2_oc_conf();

	pwm_tim2_ch1_gpio_init();
	pwm_tim2_ch2_gpio_init();
	pwm_tim2_ch3_gpio_init();
	pwm_tim2_ch4_gpio_init();

	return	ERROR_OK;
}

	/**
	 * @brief	Set PWM using TIM2
	 *		Sets global variable 'error'
	 * @param	duty_cycle:	duty cycle value (fraction)
	 * @param	chan:		channel to be used (1 through 4)
	 * @return	Error
	 */
int	pwm_tim2_chX_set	(float duty_cycle, uint32_t tim_chan)
{
	if (init_pending) {
		error	|= ERROR_PWM_INIT;
		error_handle();
		return	ERROR_NOK;
	}

	if (duty_cycle > 1.0) {
		oc_init.Pulse	= tim.Init.Period;
		error	|= ERROR_PWM_DUTY;
	} else if (duty_cycle < 0.0) {
		oc_init.Pulse	= 0;
		error	|= ERROR_PWM_DUTY;
	} else {
		oc_init.Pulse	= tim.Init.Period * duty_cycle;
	}

	if (HAL_TIM_PWM_ConfigChannel(&tim, &oc_init, tim_chan)) {
		error	|= ERROR_PWM_HAL_TIM_PWM_CONF;
		error_handle();
		return	ERROR_NOK;
	}

	if (HAL_TIM_PWM_Start(&tim, tim_chan)) {
		error	|= ERROR_PWM_HAL_TIM_PWM_START;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Stop PWM using TIM2
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	pwm_tim2_stop		(void)
{
	if (init_pending) {
		error	|= ERROR_PWM_INIT;
		error_handle();
		return	ERROR_NOK;
	}

	if (HAL_TIM_Base_Stop(&tim)) {
		error	|= ERROR_PWM_HAL_TIM_STOP;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	pwm_tim2_tim_init	(uint32_t resolution_sec,
								uint32_t period)
{
	tim.Instance		= TIM2;
	tim.Init.Prescaler		= (SystemCoreClock / resolution_sec)
									- 1u;
	tim.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim.Init.Period			= period - 1u;
	tim.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	tim.Init.RepetitionCounter	= 0;
	tim.Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_DISABLE;

	return	HAL_TIM_Base_Init(&tim);
}

static	int	pwm_tim2_clk_conf	(void)
{
	TIM_ClockConfigTypeDef	clk;

	clk.ClockSource		= TIM_CLOCKSOURCE_INTERNAL;
	clk.ClockPolarity	= TIM_CLOCKPOLARITY_INVERTED;
	clk.ClockPrescaler	= TIM_CLOCKPRESCALER_DIV1;
	clk.ClockFilter		= 0;

	return	HAL_TIM_ConfigClockSource(&tim, &clk);
}

static	int	pwm_tim2_master_conf	(void)
{
	TIM_MasterConfigTypeDef	master;

	master.MasterOutputTrigger	= TIM_TRGO_RESET;
	master.MasterSlaveMode		= TIM_MASTERSLAVEMODE_DISABLE;

	return	HAL_TIMEx_MasterConfigSynchronization(&tim, &master);
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

static	void	pwm_tim2_ch1_gpio_init	(void)
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio.Pin	= GPIO_PIN_15;
	gpio.Mode	= GPIO_MODE_AF_OD;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Speed	= GPIO_SPEED_FREQ_LOW;
	gpio.Alternate	= GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &gpio);
}

static	void	pwm_tim2_ch2_gpio_init	(void)
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio.Pin	= GPIO_PIN_1;
	gpio.Mode	= GPIO_MODE_AF_OD;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Speed	= GPIO_SPEED_FREQ_LOW;
	gpio.Alternate	= GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &gpio);
}

static	void	pwm_tim2_ch3_gpio_init	(void)
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio.Pin	= GPIO_PIN_10;
	gpio.Mode	= GPIO_MODE_AF_OD;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Speed	= GPIO_SPEED_FREQ_LOW;
	gpio.Alternate	= GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &gpio);
}

static	void	pwm_tim2_ch4_gpio_init	(void)
{
	GPIO_InitTypeDef	gpio;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio.Pin	= GPIO_PIN_11;
	gpio.Mode	= GPIO_MODE_AF_OD;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Speed	= GPIO_SPEED_FREQ_LOW;
	gpio.Alternate	= GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &gpio);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
