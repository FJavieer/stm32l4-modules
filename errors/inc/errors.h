/******************************************************************************
 *	errors.h							      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		ERRORS_H
	# define	ERRORS_H


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdint.h>

/* Drivers -------------------------------------------------------------------*/
/* libalx --------------------------------------------------------------------*/
/* STM32L4 modules -----------------------------------------------------------*/


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Error_Stm32l4_Modules {
		ERROR_OK = 0,
		ERROR_NOK
	};

	enum	Error_Stm32l4_Modules_Can {
		ERROR_CAN_HAL_CAN_INIT			= 0x00010001u,
		ERROR_CAN_HAL_CAN_FILTER		= 0x00010002u,
		ERROR_CAN_HAL_CAN_START			= 0x00010004u,
		ERROR_CAN_HAL_CAN_ACTIVATE_NOTIFICATION	= 0x00010008u,
		ERROR_CAN_HAL_ADD_TX_MSG		= 0x00010010u,
		ERROR_CAN_HAL_GET_RX_MSG		= 0x00010020u,
		ERROR_CAN_INIT				= 0x00010040u,
		ERROR_CAN_MSG_LOST			= 0x00010080u,
		ERROR_CAN_NO_MSG			= 0x00010100u
	};

	enum	Error_Stm32l4_Modules_Clk {
		ERROR_CLK_HAL_RCC_OSC_CONF		= 0x00020001u,
		ERROR_CLK_HAL_RCC_CLK_CONF		= 0x00020002u
	};

	enum	Error_Stm32l4_Modules_Delay {
		ERROR_DELAY_HAL_TIM_INIT		= 0x00040001u,
		ERROR_DELAY_HAL_TIM_START		= 0x00040002u,
		ERROR_DELAY_HAL_TIM_STOP		= 0x00040004u,
		ERROR_DELAY_INIT			= 0x00040008u,
		ERROR_DELAY_NULL			= 0x00040010u
	};

	enum	Error_Stm32l4_Modules_Display {
		ERROR_DISPLAY_INIT			= 0x00080001u,
		ERROR_DISPLAY_SPI_INIT			= 0x00080002u,
		ERROR_DISPLAY_CHAR			= 0x00080004u,
		ERROR_DISPLAY_SPI_MSG_WRITE		= 0x00080008u
	};

	enum	Error_Stm32l4_Modules_I2c {
		ERROR_I2C_INIT				= 0x00100001u
	};

	enum	Error_Stm32l4_Modules_Joystick {
		ERROR_JOYSTICK_INIT			= 0x00200001u
	};

	enum	Error_Stm32l4_Modules_Led {
		ERROR_LED_INIT				= 0x00400001u
	};

	enum	Error_Stm32l4_Modules_Pwm {
		ERROR_PWM_HAL_TIM_INIT			= 0x00800001u,
		ERROR_PWM_HAL_TIM_CLK_CONF		= 0x00800002u,
		ERROR_PWM_HAL_TIM_MASTER_CONF		= 0x00800004u,
		ERROR_PWM_HAL_TIM_PWM_INIT		= 0x00800008u,
		ERROR_PWM_HAL_TIM_PWM_CONF		= 0x00800010u,
		ERROR_PWM_HAL_TIM_PWM_START		= 0x00800020u,
		ERROR_PWM_HAL_TIM_STOP			= 0x00800040u,
		ERROR_PWM_INIT				= 0x00800080u,
		ERROR_PWM_DUTY				= 0x00800100u,
		ERROR_PWM_CHAN				= 0x00800200u
	};

	enum	Error_Stm32l4_Modules_Servo {
		ERROR_SERVO_INIT			= 0x01000001u,
		ERROR_SERVO_ID				= 0x01000002u,
	};

	enum	Error_Stm32l4_Modules_Spi {
		ERROR_SPI_HAL_SPI_INIT			= 0x02000001u,
		ERROR_SPI_HAL_SPI_TRANSMIT		= 0x02000002u,
		ERROR_SPI_INIT				= 0x02000004u
	};


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
extern	uint32_t	error;


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
	/**
	 * @brief	Handle error
	 *		Displays the error value by flashing a led from MSB
	 *		to LSB.  A long flash is a 1 and a short flash is a 0.
	 *		After displaying the value, it resets 'error'.
	 * @return	None
	 */
void	error_handle	(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* errors.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
