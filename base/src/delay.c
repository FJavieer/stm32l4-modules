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

	#include "delay.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	RESOLUTION_1_US		(1000000u)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	TIM_HandleTypeDef	tim;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	delay_us_tim_init	(void);
static	void	delay_us_delay_init	(uint32_t time_us, uint32_t *overflows);
static	void	delay_us_delay_loop	(uint32_t overflows);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize base time for delay_us()
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	delay_us_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_DELAY_INIT;
		return	ERROR_OK;
	}

	__HAL_RCC_TIM6_CLK_ENABLE();
	if (delay_us_tim_init()) {
		error	|= ERROR_DELAY_HAL_TIM_INIT;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Delay <time_us> microseconds
	 *		Sets global variable 'error'
	 * @param	time_us:	Delay value (us)
	 * @return	Error
	 */
int	delay_us	(uint32_t time_us)
{
	uint32_t	overflows;

	if (init_pending) {
		if (delay_us_init()) {
			error	|= ERROR_DELAY_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	if (!time_us) {
		return	ERROR_OK;
	}

	delay_us_delay_init(time_us, &overflows);

	if (HAL_TIM_Base_Start(&tim)) {
		error	|= ERROR_DELAY_HAL_TIM_START;
		error_handle();
		return	ERROR_NOK;
	}

	delay_us_delay_loop(overflows);

	if (HAL_TIM_Base_Stop(&tim)) {
		error	|= ERROR_DELAY_HAL_TIM_STOP;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	delay_us_tim_init	(void)
{
	/* Resolution: 1 us */
	tim.Instance		= TIM6; 
	tim.Init.Prescaler		= (SystemCoreClock /
							RESOLUTION_1_US) - 1u;
	tim.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim.Init.Period			= UINT16_MAX;
	tim.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	tim.Init.RepetitionCounter	= 0;
	tim.Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_DISABLE;

	return	HAL_TIM_Base_Init(&tim);
}

static	void	delay_us_delay_init	(uint32_t time_us, uint32_t *overflows)
{
	uint32_t	counter_initial;
	uint32_t	partial;

	*overflows	= (time_us / ((uint32_t)UINT16_MAX + 1u)) + 1u;
	partial		= time_us % ((uint32_t)UINT16_MAX + 1u);
	counter_initial	= (uint32_t)UINT16_MAX + 1u - partial;

	__HAL_TIM_SET_COUNTER(&tim, counter_initial);
}

static	void	delay_us_delay_loop	(uint32_t overflows)
{
	uint32_t	counter_flags;
	bool		flag;

	counter_flags	= 0;
	while (counter_flags < overflows) {
		flag	= __HAL_TIM_GET_FLAG(&tim, TIM_FLAG_UPDATE);
		if (flag) {
			__HAL_TIM_CLEAR_FLAG(&tim, TIM_FLAG_UPDATE);
			counter_flags++;
		}
	}
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
