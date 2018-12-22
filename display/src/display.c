/******************************************************************************
 *	display.c							      *
 *	2018/dec/16							      *
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
	#include "spi.h"

	#include "display.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	DISPLAY_CODE_DISABLE_MAX7219	(0x0C00u)
	# define	DISPLAY_CODE_DISABLE_TEST_MODE	(0x0F00u)
	# define	DISPLAY_CODE_ENABLE_8_DIGITS	(0x0BFFu)
	# define	DISPLAY_CODE_SET_MAX_BRIGHTNESS	(0x0A0Fu)
	# define	DISPLAY_CODE_DISABLE_BCD_MODE	(0x0900u)
	# define	DISPLAY_CODE_ENABLE_MAX7219	(0x0C01u)


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
static	bool	init_pending	= true;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	int	display_start		(void);
static	int	display_data_set	(char ch, uint16_t data [DISPLAY_LINES]);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init LED matrix display using SPI
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	display_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	if (spi_init()) {
		error	|= ERROR_DISPLAY_SPI_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	if (display_start()) {
		error	|= ERROR_DISPLAY_START;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Show ch on the display
	 *		Sets global variable 'error'
	 * @param	ch:	The character to be displayed
	 * @return	Error
	 */
int	display_set	(char ch)
{
	uint16_t	data [DISPLAY_LINES];
	int		i;

	if (init_pending) {
		if (display_init()) {
			error	|= ERROR_DISPLAY_INIT;
			error_handle();
			return	ERROR_NOK;
		}
	}

	if (display_data_set(ch, data)) {
		error	|= ERROR_DISPLAY_CHAR;
		error_handle();
		return	ERROR_NOK;
	}

	for (i = 0; i < DISPLAY_LINES; i++) {
		if (spi_msg_write(data[i])) {
			error	|= ERROR_DISPLAY_SPI_MSG_WRITE;
			error_handle();
			return	ERROR_NOK;
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/

static	int	display_start		(void)
{
	if (spi_msg_write(DISPLAY_CODE_DISABLE_MAX7219)) {
		return	ERROR_NOK;
	}

	if (spi_msg_write(DISPLAY_CODE_DISABLE_TEST_MODE)) {
		return	ERROR_NOK;
	}

	if (spi_msg_write(DISPLAY_CODE_ENABLE_8_DIGITS)) {
		return	ERROR_NOK;
	}

	if (spi_msg_write(DISPLAY_CODE_SET_MAX_BRIGHTNESS)) {
		return	ERROR_NOK;
	}

	if (spi_msg_write(DISPLAY_CODE_DISABLE_BCD_MODE)) {
		return	ERROR_NOK;
	}

	if (spi_msg_write(DISPLAY_CODE_ENABLE_MAX7219)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	display_data_set	(char ch, uint16_t data [DISPLAY_LINES])
{
	switch (ch) {
	case '0':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0344u;
		data[3]	= 0x0444u;
		data[4]	= 0x0544u;
		data[5]	= 0x0644u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '1':
		data[0]	= 0x0110u;
		data[1]	= 0x0230u;
		data[2]	= 0x0310u;
		data[3]	= 0x0410u;
		data[4]	= 0x0510u;
		data[5]	= 0x0610u;
		data[6]	= 0x0710u;
		data[7]	= 0x0838u;
		break;
	case '2':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0304u;
		data[3]	= 0x0404u;
		data[4]	= 0x0508u;
		data[5]	= 0x0610u;
		data[6]	= 0x0720u;
		data[7]	= 0x087Cu;
		break;
	case '3':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0304u;
		data[3]	= 0x0418u;
		data[4]	= 0x0504u;
		data[5]	= 0x0604u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '4':
		data[0]	= 0x0104u;
		data[1]	= 0x020Cu;
		data[2]	= 0x0314u;
		data[3]	= 0x0424u;
		data[4]	= 0x0544u;
		data[5]	= 0x067Cu;
		data[6]	= 0x0704u;
		data[7]	= 0x0804u;
		break;
	case '5':
		data[0]	= 0x017Cu;
		data[1]	= 0x0240u;
		data[2]	= 0x0340u;
		data[3]	= 0x0478u;
		data[4]	= 0x0504u;
		data[5]	= 0x0604u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '6':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0340u;
		data[3]	= 0x0478u;
		data[4]	= 0x0544u;
		data[5]	= 0x0644u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '7':
		data[0]	= 0x017Cu;
		data[1]	= 0x0204u;
		data[2]	= 0x0304u;
		data[3]	= 0x0408u;
		data[4]	= 0x0510u;
		data[5]	= 0x0620u;
		data[6]	= 0x0720u;
		data[7]	= 0x0820u;
		break;
	case '8':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0344u;
		data[3]	= 0x0438u;
		data[4]	= 0x0544u;
		data[5]	= 0x0644u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case '9':
		data[0]	= 0x0138u;
		data[1]	= 0x0244u;
		data[2]	= 0x0344u;
		data[3]	= 0x0444u;
		data[4]	= 0x053Cu;
		data[5]	= 0x0604u;
		data[6]	= 0x0744u;
		data[7]	= 0x0838u;
		break;
	case ' ':
		data[0]	= 0x0100u;
		data[1]	= 0x0200u;
		data[2]	= 0x0300u;
		data[3]	= 0x0400u;
		data[4]	= 0x0500u;
		data[5]	= 0x0600u;
		data[6]	= 0x0700u;
		data[7]	= 0x0800u;
		break;
	default:
		data[0]	= 0x0100u;
		data[1]	= 0x0200u;
		data[2]	= 0x0300u;
		data[3]	= 0x0400u;
		data[4]	= 0x0500u;
		data[5]	= 0x0600u;
		data[6]	= 0x0700u;
		data[7]	= 0x0800u;
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
