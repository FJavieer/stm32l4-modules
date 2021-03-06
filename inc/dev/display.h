/******************************************************************************
 *	display.h							      *
 *	2018/dec/17							      *
 ******************************************************************************/


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# ifndef		DISPLAY_H
	# define	DISPLAY_H


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
	# define	DISPLAY_ROWS	(8)
	# define	DISPLAY_COLS	(8)
	# define	DISPLAY_ROW(r)	(((uint16_t)(r) + 1) << DISPLAY_COLS)


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
int	display_init	(void);
int	display_set	(uint16_t data [DISPLAY_ROWS]);
int	display_set_ch	(char ch);


/******************************************************************************
 ******* include guard ********************************************************
 ******************************************************************************/
# endif			/* display.h */


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
