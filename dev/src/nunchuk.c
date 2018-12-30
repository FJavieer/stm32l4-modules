/******************************************************************************
 *	servo.c								      *
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
	#include "i2c.h"

	#include "nunchuk.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	NUNCHUK_ADDRESS			(0x52u)

	# define	NUNCHUK_COMMAND_START_LEN	(2)
	# define	NUNCHUK_COMMAND_START_DATA	(uint8_t [2]){0x40, 0x00}

	# define	NUNCHUK_COMMAND_GETDATA_LEN	(1)
	# define	NUNCHUK_COMMAND_GETDATA_DATA	(uint8_t [1]){0x00}

	# define	NUNCHUK_DATA_LEN		(6)
	# define	NUNCHUK_DATA_KEY		(UINT8_C(0x17))


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
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	nunchuk_start		(void);
static	void	nunchuk_decrypt		(uint8_t buff [NUNCHUK_DATA_LEN]);
static	void	nunchuk_extract_data	(uint8_t buff [NUNCHUK_DATA_LEN],
						Nunchuk_Data_s  *data);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Init nunchuk
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	nunchuk_init	(void)
{
	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	if (i2c_init()) {
		prj_error	|= ERROR_NUNCHUK_I2C_INIT;
		prj_error_handle();
		return	ERROR_NOK;
	}
	if (nunchuk_start()) {
		prj_error	|= ERROR_NUNCHUK_START;
		prj_error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Read nunchuk data
	 *		Sets global variable 'error'
	 * @param	data:	struct that holds all nunchuk data
	 * @return	Error
	 */
int	nunchuk_read	(Nunchuk_Data_s *data)
{
	uint8_t		buff [NUNCHUK_DATA_LEN];

	if (init_pending) {
		if (nunchuk_init()) {
			prj_error	|= ERROR_NUNCHUK_INIT;
			prj_error_handle();
			return	ERROR_NOK;
		}
	}

	if (i2c_msg_write(NUNCHUK_ADDRESS, NUNCHUK_COMMAND_GETDATA_LEN,
						NUNCHUK_COMMAND_GETDATA_DATA)) {
		prj_error	|= ERROR_NUNCHUK_I2C_TRANSMIT;
		prj_error_handle();
		return	ERROR_NOK;
	}

	if (i2c_msg_ask(NUNCHUK_ADDRESS, NUNCHUK_DATA_LEN)) {
		prj_error	|= ERROR_NUNCHUK_I2C_ASK;
		prj_error_handle();
		return	ERROR_NOK;
	}

	while (!i2c_msg_ready()) {
	}

	if (i2c_msg_read(NUNCHUK_DATA_LEN, buff)) {
		prj_error	|= ERROR_NUNCHUK_I2C_READ;
		prj_error_handle();
		return	ERROR_NOK;
	}

	nunchuk_decrypt(buff);
	nunchuk_extract_data(buff, data);

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	nunchuk_start		(void)
{
	if (i2c_msg_write(NUNCHUK_ADDRESS, NUNCHUK_COMMAND_START_LEN,
						NUNCHUK_COMMAND_START_DATA)) {
		prj_error	|= ERROR_NUNCHUK_I2C_TRANSMIT;
		prj_error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	void	nunchuk_decrypt		(uint8_t buff [NUNCHUK_DATA_LEN])
{
	int	i;

	for (i = 0; i < NUNCHUK_DATA_LEN; i++) {
		buff[i]	^= NUNCHUK_DATA_KEY;
	}
}

static	void	nunchuk_extract_data	(uint8_t buff [NUNCHUK_DATA_LEN],
						Nunchuk_Data_s  *data)
{
	data->jst.x	= buff[0];
	data->jst.y	= buff[1];
	data->acc.x8	= buff[2];
	data->acc.y8	= buff[3];
	data->acc.z8	= buff[4];
	data->acc.x10	= (buff[2] << 2) | (buff[6] & 0x0Cu);
	data->acc.y10	= (buff[3] << 2) | (buff[6] & 0x30u);
	data->acc.z10	= (buff[4] << 2) | (buff[6] & 0xC0u);
	data->btn_c	= !(buff[6] & 0x02);
	data->btn_z	= !(buff[6] & 0x01);
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
