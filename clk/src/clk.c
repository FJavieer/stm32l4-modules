/******************************************************************************
 *	clk.c								      *
 *	2018/dec/15							      *
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

	#include "clk.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
	uint32_t	sysclk_freq;

/* Static --------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	System Clock Configuration
	 *		The system Clock is configured as follows
	 *			System Clock source            = PLL (MSI)
	 *			SYSCLK(Hz)                     = 80000000
	 *			HCLK(Hz)                       = 80000000
	 *			AHB Prescaler                  = 1
	 *			APB1 Prescaler                 = 1
	 *			APB2 Prescaler                 = 1
	 *			MSI Frequency(Hz)              = 4000000
	 *			PLL_M                          = 1
	 *			PLL_N                          = 40
	 *			PLL_R                          = 2
	 *			PLL_P                          = 7
	 *			PLL_Q                          = 4
	 *			Flash Latency(WS)              = 4
	 * @return	None
	 */
void	sysclk_config	(void)
{
	RCC_OscInitTypeDef	rcc_osc_init_values;
	RCC_ClkInitTypeDef	rcc_clk_init_values;

	/* MSI is enabled after System reset, activate PLL with MSI as source */
	rcc_osc_init_values.OscillatorType	= RCC_OSCILLATORTYPE_MSI;
	rcc_osc_init_values.MSIState		= RCC_MSI_ON;
	rcc_osc_init_values.MSIClockRange	= RCC_MSIRANGE_6;
	rcc_osc_init_values.MSICalibrationValue	= RCC_MSICALIBRATION_DEFAULT;
	rcc_osc_init_values.PLL.PLLState	= RCC_PLL_ON;
	rcc_osc_init_values.PLL.PLLSource	= RCC_PLLSOURCE_MSI;
	rcc_osc_init_values.PLL.PLLM		= 1;
	rcc_osc_init_values.PLL.PLLN		= 40;
	rcc_osc_init_values.PLL.PLLR		= 2;
	rcc_osc_init_values.PLL.PLLP		= 7;
	rcc_osc_init_values.PLL.PLLQ		= 4;
	if (HAL_RCC_OscConfig(&rcc_osc_init_values) != HAL_OK) {
		/* Initialization Error */
		error	|= ERROR_CLK_HAL_RCC_OSC_CONF;
		while(true) {
			;
		}
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1
	 * and PCLK2 clocks dividers */
	rcc_clk_init_values.ClockType		= (RCC_CLOCKTYPE_SYSCLK |
							RCC_CLOCKTYPE_HCLK |
							RCC_CLOCKTYPE_PCLK1 |
							RCC_CLOCKTYPE_PCLK2);
	rcc_clk_init_values.SYSCLKSource	= RCC_SYSCLKSOURCE_PLLCLK;
	rcc_clk_init_values.AHBCLKDivider	= RCC_SYSCLK_DIV1;
	rcc_clk_init_values.APB1CLKDivider	= RCC_HCLK_DIV1;  
	rcc_clk_init_values.APB2CLKDivider	= RCC_HCLK_DIV1;  
	if (HAL_RCC_ClockConfig(&rcc_clk_init_values, FLASH_LATENCY_4) != HAL_OK) {
		/* Initialization Error */
		error	|= ERROR_CLK_HAL_RCC_CLK_CONF;
		while(true) {
			;
		}
	}

	sysclk_freq	= 80000000u;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
