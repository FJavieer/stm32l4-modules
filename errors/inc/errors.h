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
# define	ERROR_MODULE_SHIFT		(16)

# define	ERROR_MODULE_CLK		(0x0001u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_LED		(0x0002u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_DELAY		(0x0004u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_PWM		(0x0008u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_SPI		(0x0010u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_I2C		(0x0020u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_CAN		(0x0040u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_SERVO		(0x0080u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_DISPLAY		(0x0100u << ERROR_MODULE_SHIFT)
# define	ERROR_MODULE_NUNCHUK		(0x0200u << ERROR_MODULE_SHIFT)

# define	ERROR_CAN_HAL_CAN_INIT		(0x0001u | ERROR_MODULE_CAN)
# define	ERROR_CAN_HAL_CAN_FILTER	(0x0002u | ERROR_MODULE_CAN)
# define	ERROR_CAN_HAL_CAN_START		(0x0004u | ERROR_MODULE_CAN)
# define	ERROR_CAN_HAL_CAN_ACTI_NOTIF	(0x0008u | ERROR_MODULE_CAN)
# define	ERROR_CAN_HAL_ADD_TX_MSG	(0x0010u | ERROR_MODULE_CAN)
# define	ERROR_CAN_HAL_GET_RX_MSG	(0x0020u | ERROR_MODULE_CAN)
# define	ERROR_CAN_INIT			(0x0040u | ERROR_MODULE_CAN)
# define	ERROR_CAN_MSG_LOST		(0x0080u | ERROR_MODULE_CAN)
# define	ERROR_CAN_NO_MSG		(0x0100u | ERROR_MODULE_CAN)

# define	ERROR_CLK_HAL_RCC_OSC_CONF	(0x0001u | ERROR_MODULE_CLK)
# define	ERROR_CLK_HAL_RCC_CLK_CONF	(0x0002u | ERROR_MODULE_CLK)

# define	ERROR_DELAY_HAL_TIM_INIT	(0x0001u | ERROR_MODULE_DELAY)
# define	ERROR_DELAY_HAL_TIM_START	(0x0002u | ERROR_MODULE_DELAY)
# define	ERROR_DELAY_HAL_TIM_STOP	(0x0004u | ERROR_MODULE_DELAY)
# define	ERROR_DELAY_INIT		(0x0008u | ERROR_MODULE_DELAY)
# define	ERROR_DELAY_NULL		(0x0010u | ERROR_MODULE_DELAY)

# define	ERROR_DISPLAY_INIT		(0x0001u | ERROR_MODULE_DISPLAY)
# define	ERROR_DISPLAY_SPI_INIT		(0x0002u | ERROR_MODULE_DISPLAY)
# define	ERROR_DISPLAY_START		(0x0004u | ERROR_MODULE_DISPLAY)
# define	ERROR_DISPLAY_CHAR		(0x0008u | ERROR_MODULE_DISPLAY)
# define	ERROR_DISPLAY_SPI_MSG_WRITE	(0x0010u | ERROR_MODULE_DISPLAY)

# define	ERROR_I2C_HAL_I2C_INIT		(0x0001u | ERROR_MODULE_I2C)
# define	ERROR_I2C_HAL_I2C_FILTER_A	(0x0002u | ERROR_MODULE_I2C)
# define	ERROR_I2C_HAL_I2C_FILTER_D	(0x0004u | ERROR_MODULE_I2C)

# define	ERROR_NUNCHUK_INIT		(0x0001u | ERROR_MODULE_NUNCHUK)

# define	ERROR_LED_INIT			(0x0001u | ERROR_MODULE_LED)

# define	ERROR_PWM_HAL_TIM_INIT		(0x0001u | ERROR_MODULE_PWM)
# define	ERROR_PWM_HAL_TIM_CLK_CONF	(0x0002u | ERROR_MODULE_PWM)
# define	ERROR_PWM_HAL_TIM_MASTER_CONF	(0x0004u | ERROR_MODULE_PWM)
# define	ERROR_PWM_HAL_TIM_PWM_INIT	(0x0008u | ERROR_MODULE_PWM)
# define	ERROR_PWM_HAL_TIM_PWM_CONF	(0x0010u | ERROR_MODULE_PWM)
# define	ERROR_PWM_HAL_TIM_PWM_START	(0x0020u | ERROR_MODULE_PWM)
# define	ERROR_PWM_HAL_TIM_STOP		(0x0040u | ERROR_MODULE_PWM)
# define	ERROR_PWM_INIT			(0x0080u | ERROR_MODULE_PWM)
# define	ERROR_PWM_DUTY			(0x0100u | ERROR_MODULE_PWM)
# define	ERROR_PWM_CHAN			(0x0200u | ERROR_MODULE_PWM)

# define	ERROR_SPI_HAL_SPI_INIT		(0x0001u | ERROR_MODULE_SPI)
# define	ERROR_SPI_HAL_SPI_TRANSMIT	(0x0002u | ERROR_MODULE_SPI)
# define	ERROR_SPI_INIT			(0x0004u | ERROR_MODULE_SPI)

# define	ERROR_SERVO_INIT		(0x0001u | ERROR_MODULE_SERVO)
# define	ERROR_SERVO_ID			(0x0002u | ERROR_MODULE_SERVO)
# define	ERROR_SERVO_PWM_INIT		(0x0004u | ERROR_MODULE_SERVO)
# define	ERROR_SERVO_PWM_SET		(0x0008u | ERROR_MODULE_SERVO)
# define	ERROR_SERVO_PWM_STOP		(0x0010u | ERROR_MODULE_SERVO)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/
	enum	Error_Stm32l4_Modules {
		ERROR_OK = 0,
		ERROR_NOK
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
void	error_handle	(void);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* errors.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
