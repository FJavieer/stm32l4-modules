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


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	TIM_HandleTypeDef	tim_handle;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize base time for delay_us()
	 *		Sets global variable 'error'
	 * @param	None
	 * @return	None
	 */
void	delay_us_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_DELAY_INIT;
		return;
	}

	__HAL_RCC_TIM6_CLK_ENABLE();
	
	/* Resolution: 1 us */
	tim_handle.Instance		= TIM6; 
	tim_handle.Init.Prescaler		= SystemCoreClock / 1000000u - 1u;
	tim_handle.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim_handle.Init.Period			= UINT16_MAX;
	tim_handle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.RepetitionCounter	= 0x00u;
	tim_handle.Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK) {
		error	|= ERROR_DELAY_HAL_TIM_INIT;
		error_handle();
		return;
	}
}

	/**
	 * @brief	Delay <time_us> microseconds
	 *		Sets global variable 'error'
	 * @param	time_us:	Delay value (us)
	 * @return	None
	 */
void	delay_us	(uint32_t time_us)
{
	uint32_t	overflows;
	uint32_t	partial;
	uint32_t	counter_initial;
	uint32_t	counter_flags;
	bool		flag;

	if (init_pending) {
		error	|= ERROR_DELAY_INIT;
		error_handle();
		return;
	}

	/* Delay == 0;  exit now */
	if (time_us < 1) {
		error	|= ERROR_DELAY_NULL;
		return;
	}

	/* Initialize delay value */
	overflows	= (time_us / ((uint32_t)UINT16_MAX + 1)) + 1;
	partial		= time_us % ((uint32_t)UINT16_MAX + 1);
	counter_initial	= (uint32_t)UINT16_MAX + 1 - partial;
	__HAL_TIM_SET_COUNTER(&tim_handle, counter_initial);

	/* Start delay */
	if (HAL_TIM_Base_Start(&tim_handle) != HAL_OK) {
		error	|= ERROR_DELAY_HAL_TIM_START;
		error_handle();
		return;
	}

	/* Count flags until delay reached */
	counter_flags	= 0;
	while (counter_flags < overflows) {
		flag	= __HAL_TIM_GET_FLAG(&tim_handle, TIM_FLAG_UPDATE);
		if (flag) {
			__HAL_TIM_CLEAR_FLAG(&tim_handle, TIM_FLAG_UPDATE);
			counter_flags++;
		}
	}

	/* Stop delay */
	if (HAL_TIM_Base_Stop(&tim_handle) != HAL_OK) {
		error	|= ERROR_DELAY_HAL_TIM_STOP;
		error_handle();
		return;
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
