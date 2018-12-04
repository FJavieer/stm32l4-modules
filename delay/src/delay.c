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
	/*
	 * @brief	Initialize base time for delay_us().
	 * @param	None.
	 * @return	None.
	 */
int32_t	delay_us_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	0;
	}

	__HAL_RCC_TIM6_CLK_ENABLE();
	
	/* Resolution: 1 us */
	tim_handle.Instance		= TIM6; 
	tim_handle.Init.Prescaler		= SystemCoreClock / 1000000u - 1;
	tim_handle.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim_handle.Init.Period			= UINT16_MAX;
	tim_handle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.RepetitionCounter	= 0x00u;
	tim_handle.Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK) {
		return	-1;
	}

	return	0;
}

	/**
	 * @brief	Delay <time_us> microseconds.
	 * @param	time_us:	Delay value (us).
	 * @return	None.
	 */
int32_t	delay_us	(uint32_t time_us)
{
	if (init_pending) {
		return	-1;
	}

	/* Delay == 0;  exit now */
	if (time_us < 1) {
		return	0;
	}

	/* Initialize delay value */
	uint32_t	overflows;
	uint32_t	partial;
	uint32_t	counter_initial;
	overflows	= (time_us / ((uint32_t)UINT16_MAX + 1)) + 1;
	partial		= time_us % ((uint32_t)UINT16_MAX + 1);
	counter_initial	= (uint32_t)UINT16_MAX + 1 - partial;
	__HAL_TIM_SET_COUNTER(&tim_handle, counter_initial);

	/* Start delay */
	if (HAL_TIM_Base_Start(&tim_handle) != HAL_OK) {
		return	-2;
	}

	/* Count flags until delay reached */
	uint32_t	counter_flags;
	bool		flag;
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
		return	-3;
	}

	return	0;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
