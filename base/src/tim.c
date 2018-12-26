/******************************************************************************
 *	tim.c								      *
 *	2018/dec/25							      *
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

	#include "tim.h"


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
	volatile	bool	tim_tim3_interrupt;

/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	TIM_HandleTypeDef	tim;
static	int			stack_len;
static	Callback_s		callback_stack [CALLBACK_STACK_MAX];

/* Volatile ------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	tim_tim3_tim_init	(uint32_t freq_hz);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize periodic interrupts using TIM3
	 *		Sets global variable 'error'
	 * @param	freq_hz:	frequency of interrupts in Hz
	 * @return	Error
	 */
int	tim_tim3_init		(uint32_t freq_hz)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	tim_tim3_interrupt	= false;

	__HAL_RCC_TIM3_CLK_ENABLE();
	if (tim_tim3_tim_init(freq_hz)) {
		error	|= ERROR_TIM_HAL_TIM_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	if (HAL_TIM_Base_Start_IT(&tim)) {
		error	|= ERROR_TIM_HAL_TIM_START_IT;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Push a callback into the stack
	 * @param	func:	pointer to a function void (*func)(void *);
	 * @param	data:	data to be used by func
	 * @return	Error
	 */
int	tim_callback_push	(int (*func)(void *), void *data)
{
	if (stack_len >= CALLBACK_STACK_MAX) {
		error	|= ERROR_TIM_STACK;
		error_handle();
		return	ERROR_NOK;
	}

	callback_stack[stack_len].func	= func;
	callback_stack[stack_len].data	= data;
	stack_len++;

	return	ERROR_OK;
}

	/**
	 * @brief	Pop a callback from the stack
	 * @return	stack_len
	 */
int	tim_callback_pop	(void)
{
	if (!stack_len) {
		return	0;
	}

	stack_len--;

	return	stack_len;
}

	/**
	 * @brief	Execute callbacks in the stack (first in, first exe)
	 * @return	Error
	 */
int	tim_callback_exe	(void)
{
	int	i;

	if (init_pending) {
		error	|= ERROR_TIM_INIT;
		error_handle();
		return	ERROR_NOK;
	}

	for (i = 0; i < stack_len; i++) {
		if (callback_stack[i].func(callback_stack[i].data)) {
			error	|= ERROR_TIM_CALLBACK_EXE;
			error_handle();
			return	ERROR_NOK;
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* HAL weak functions (redefinitions) ***********************************
 ******************************************************************************/
/**
 * @brief	Handle TIM3 interrupt request
 */
void	TIM3_IRQHandler			(void)
{
	HAL_TIM_IRQHandler(&tim);
}

/**
 * @brief	TIM callback
 * @param	tim_ptr:	pointer to a TIM_HandleTypeDef structure; unused
 */
void	HAL_TIM_PeriodElapsedCallback	(TIM_HandleTypeDef *tim_ptr)
{
	/* Silence unused variable */
	(void)tim_ptr;

	tim_tim3_interrupt	= true;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	tim_tim3_tim_init	(uint32_t freq_hz)
{
	tim.Instance		= TIM3;
	tim.Init.Period			= freq_hz - 1;
	tim.Init.Prescaler		= (SystemCoreClock / freq_hz) - 1;
	tim.Init.ClockDivision		= 0;
	tim.Init.CounterMode		= TIM_COUNTERMODE_UP;
	tim.Init.RepetitionCounter	= 0;

	return	HAL_TIM_Base_Init(&tim);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
