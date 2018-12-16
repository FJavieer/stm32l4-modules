/******************************************************************************
 *	can.c								      *
 *	2018/dec/11							      *
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

	#include "can.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	CAN_HandleTypeDef	can_handle;
static	CAN_TxHeaderTypeDef	can_tx_header;
static	CAN_RxHeaderTypeDef	can_rx_header;
static	uint8_t			can_rx_data [CAN_DATA_LEN];
static	uint32_t		can_tx_mailbox;
/* Volatile ------------------------------------------------------------------*/
static	volatile	bool	can_msg_pending;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/
static	void	can_clk_enable		(void);
static	void	can_gpio_init		(void);
static	int	can_peripherial_init	(void);
static	int	can_filter_conf		(void);
static	void	can_tx_header_conf	(void);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize CAN
	 *		Sets global variable 'error'
	 * @return	Error
	 */
int	can_init	(void)
{

	if (init_pending) {
		init_pending	= false;
	} else {
		error	|= ERROR_CAN_INIT;
		return	ERROR_OK;
	}

	can_msg_pending	= false;
	can_clk_enable();
	can_gpio_init();
	if (can_peripherial_init()) {
		error	|= ERROR_CAN_HAL_CAN_INIT;
		error_handle();
		return	ERROR_NOK;
	}
	if (can_filter_conf()) {
		error	|= ERROR_CAN_HAL_CAN_FILTER;
		error_handle();
		return	ERROR_NOK;
	}
	if (HAL_CAN_Start(&can_handle)) {
		error	|= ERROR_CAN_HAL_CAN_START;
		error_handle();
		return	ERROR_NOK;
	}
	if (HAL_CAN_ActivateNotification(&can_handle, CAN_IT_RX_FIFO0_MSG_PENDING)) {
		error	|= ERROR_CAN_HAL_CAN_ACTIVATE_NOTIFICATION;
		error_handle();
		return	ERROR_NOK;
	}

	can_tx_header_conf();

	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

	return	ERROR_OK;
}

	/**
	 * @brief	Transmit the message in data through CAN
	 *		Sets global variable 'error'
	 * @param	data:	data to transmit
	 * @return	Error
	 */
int	can_msg_write	(uint8_t data [CAN_DATA_LEN])
{
	uint8_t	can_tx_data [CAN_DATA_LEN];
	int	i;

	if (init_pending) {
		error	|= ERROR_CAN_INIT;
		return	ERROR_NOK;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		can_tx_data[i]	= data[i];
	}

	if (HAL_CAN_AddTxMessage(&can_handle, &can_tx_header, can_tx_data,
							&can_tx_mailbox)) {
		error	|= ERROR_CAN_HAL_ADD_TX_MSG;
		error_handle();
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

	/**
	 * @brief	Return the data received
	 *		Sets global variable 'error'
	 * @param	data:	array where data is to be written
	 * @return	Error
	 */
int	can_msg_read	(uint8_t data [CAN_DATA_LEN])
{
	int	i;

	if (init_pending) {
		error	|= ERROR_CAN_INIT;
		return	ERROR_NOK;
	}

	if (!can_msg_pending) {
		error	|= ERROR_CAN_NO_MSG;
		return	ERROR_NOK;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		data[i]	= can_rx_data[i];
	}

	can_msg_pending	= false;

	return	ERROR_OK;
}


/******************************************************************************
 ******* HAL weak functions (redefinitions) ***********************************
 ******************************************************************************/
/**
 * @brief	Rx Fifo 0 message pending callback
 * @param	hcan: pointer to a CAN_HandleTypeDef structure that contains
 *		the configuration information for the specified CAN.
 * @return	None
 */
void	HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *can_handle_ptr)
{
	if (HAL_CAN_GetRxMessage(can_handle_ptr, CAN_RX_FIFO0, &can_rx_header,
								can_rx_data)) {
		error	|= ERROR_CAN_HAL_GET_RX_MSG;
		error_handle();
	} else if (can_msg_pending) {
		error	|= ERROR_CAN_MSG_LOST;
	} else {
		can_msg_pending	= true;
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	can_clk_enable		(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__CAN1_CLK_ENABLE();
}

static	void	can_gpio_init		(void)
{
	GPIO_InitTypeDef	gpio_init_values;

	/* PA12 -> TX // PA11 -> RX */
	gpio_init_values.Pin		= GPIO_PIN_12 | GPIO_PIN_11;
	gpio_init_values.Mode		= GPIO_MODE_AF_OD;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Pull		= GPIO_NOPULL;
	gpio_init_values.Alternate	= GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA, &gpio_init_values);
}

static	int	can_peripherial_init	(void)
{
	can_handle.Instance		= CAN1;
	can_handle.Init.TimeTriggeredMode	= DISABLE;
	can_handle.Init.AutoBusOff		= DISABLE;
	can_handle.Init.AutoWakeUp		= DISABLE;
	can_handle.Init.AutoRetransmission	= ENABLE;
	can_handle.Init.ReceiveFifoLocked	= DISABLE;
	can_handle.Init.TransmitFifoPriority	= DISABLE;
	can_handle.Init.Mode			= CAN_MODE_NORMAL;
	can_handle.Init.SyncJumpWidth		= CAN_SJW_1TQ;
	can_handle.Init.TimeSeg1		= CAN_BS1_4TQ;
	can_handle.Init.TimeSeg2		= CAN_BS2_5TQ;
	/* CAN clock = 1 MHz = 80 MHz / 80;  Period = 1 us */
	can_handle.Init.Prescaler		= SystemCoreClock / 1000000u;

	return	HAL_CAN_Init(&can_handle);
}

static	int	can_filter_conf		(void)
{
	CAN_FilterTypeDef	can_filter;

	can_filter.FilterIdHigh		= 0x0000u;
	can_filter.FilterIdLow		= 0x0000u;
	can_filter.FilterMaskIdHigh	= 0x0000u;
	can_filter.FilterMaskIdLow	= 0x0000u;
	can_filter.FilterFIFOAssignment	= CAN_FILTER_FIFO0;
	can_filter.FilterBank		= 5;
	can_filter.FilterMode		= CAN_FILTERMODE_IDMASK;
	can_filter.FilterScale		= CAN_FILTERSCALE_16BIT;
	can_filter.FilterActivation	= ENABLE;

	return	HAL_CAN_ConfigFilter(&can_handle, &can_filter);
}

static	void	can_tx_header_conf	(void)
{
	can_tx_header.StdId			= 0x3AAu;
	can_tx_header.ExtId			= 0x00u;
	can_tx_header.IDE			= CAN_ID_STD;
	can_tx_header.RTR			= CAN_RTR_DATA;
	can_tx_header.DLC			= CAN_DATA_LEN;
	can_tx_header.TransmitGlobalTime	= DISABLE;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
