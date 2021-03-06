/******************************************************************************
 *	delay.c								      *
 *	2018/dec/04							      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		DELAY_H
	# define	DELAY_H


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


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* functions ************************************************************
 ******************************************************************************/
int	delay_us_init	(void);
int	delay_us	(uint32_t time_us);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* delay.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
