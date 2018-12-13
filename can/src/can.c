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
	#include "can.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	CAN_DATA_LEN


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool			init_pending	= true;
static	CAN_HandleTypeDef	can_handle;
static	CAN_TxHeaderTypeDef	can_tx_header;
static	CAN_RxHeaderTypeDef	can_rx_header;
static	uint8_t			can_tx_data [CAN_DATA_LEN];
static	uint8_t			can_rx_data [CAN_DATA_LEN];
static	uint32_t		can_tx_mailbox;
/* Volatile ------------------------------------------------------------------*/
static	volatile	bool	flag_can_data_received;


/******************************************************************************
 ******* static functions (declarations) **************************************
 ******************************************************************************/


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize CAN.
	 * @return	error
	 */
uint32_t	can_init		(void)
{
	uint32_t		error	= ERR_CAN_OK;
	GPIO_InitTypeDef	gpio_init_values;
	CAN_FilterTypeDef	can_filter;

	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERR_CAN_OK;
	}

	/* -0- initialize data ############################################## */
	flag_can_data_received	= false;

	/* -1- First we have to activate the clocks!! ####################### */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__CAN1_CLK_ENABLE();

	/* -2- Configure and connect pins to CAN1 peripheral ################ */

	/* PA12->TX // PA11->RX */
	gpio_init_values.Pin		= GPIO_PIN_12 | GPIO_PIN_11;
	gpio_init_values.Mode		= GPIO_MODE_AF_OD;
	gpio_init_values.Speed		= GPIO_SPEED_FREQ_LOW;
	gpio_init_values.Pull		= GPIO_NOPULL;
	gpio_init_values.Alternate	= GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA, &gpio_init_values);

	/* -3- Configure the CAN peripheral ################################# */
	can_handle.Instance		= CAN1;
	can_handle.Init.TimeTriggeredMode	= DISABLE;	/* Enable or disable the time triggered communication mode..........................*/
	can_handle.Init.AutoBusOff		= DISABLE;	/* Enable or disable the automatic bus-off management...............................*/
	can_handle.Init.AutoWakeUp		= DISABLE;	/* Enable or disable the automatic wake-up mode.....................................*/
	can_handle.Init.AutoRetransmission	= ENABLE;	/* Enable or disable the non-automatic retransmission mode..........................*/
	can_handle.Init.ReceiveFifoLocked	= DISABLE;	/* Enable or disable the Receive FIFO Locked mode...................................*/
	can_handle.Init.TransmitFifoPriority	= DISABLE;	/* Enable or disable the transmit FIFO priority.....................................*/
	can_handle.Init.Mode			= CAN_MODE_NORMAL;	/* CAN operating mode...............................................................*/
	can_handle.Init.SyncJumpWidth		= CAN_SJW_1TQ;	/* Maximum number of time quanta ("CAN_synchronisation_jump_width").................*/
	can_handle.Init.TimeSeg1		= CAN_BS1_4TQ;	/* Number of time quanta in Bit Segment 1 ("CAN_time_quantum_in_bit_segment_1").....*/
	can_handle.Init.TimeSeg2		= CAN_BS2_5TQ;	/* Number of time quanta in Bit Segment 2 ("CAN_time_quantum_in_bit_segment_2").....*/
	can_handle.Init.Prescaler		= 180;	/* Length of a time quantum: CAN clock = 80MHz/180 = 444.44kHz. Period = 2.25uS.....*/

	if (HAL_CAN_Init(&can_handle) != HAL_OK) {
		return	ERR_CAN_HAL_CAN_INIT;
	}

	/* -4- Configure the CAN Filter ##################################### */
	can_filter.FilterBank		= 5; // <14 -> CAN1
	can_filter.FilterMode		= CAN_FILTERMODE_IDMASK;
	can_filter.FilterScale		= CAN_FILTERSCALE_16BIT;
	can_filter.FilterIdHigh		= 0x0000u;
	can_filter.FilterIdLow		= 0x0000u;
	can_filter.FilterMaskIdHigh	= 0x0000u;
	can_filter.FilterMaskIdLow	= 0x0000u;
	can_filter.FilterFIFOAssignment	= 0;
	can_filter.FilterActivation	= ENABLE;
	can_filter.SlaveStartFilterBank	= 0;

	if (HAL_CAN_ConfigFilter(&can_handle, &can_filter) != HAL_OK) {
		return	ERR_CAN_HAL_CAN_FILTER;
	}

	/* -5- Start the CAN peripheral ##################################### */
	if (HAL_CAN_Start(&can_handle) != HAL_OK) {
		return	ERR_CAN_HAL_CAN_START;
	}

	/* -6- Activate CAN RX notification ################################# */
	if (HAL_CAN_ActivateNotification(&can_handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		return	ERR_CAN_HAL_CAN_ACTI_NOTIFICATION;
	}

	/* -7- Configure Transmission process ############################### */
	can_tx_header.StdId			= 0x03AAu;													/* Standard identifier: between Min_Data = 0 and Max_Data = 0x7FF...................*/		
	can_tx_header.ExtId			= 0x00u;													/* Extended identifier: between Min_Data = 0 and Max_Data = 0x1FFFFFFF..............*/
	can_tx_header.RTR			= CAN_RTR_DATA;										/* Type of identifier for the message that will be transmitted......................*/
	can_tx_header.IDE			= CAN_ID_STD;											/* Type of frame for the message that will be transmitted...........................*/
	can_tx_header.DLC			= 1;																/* We'll send a 1-byte data packet..................................................*/
	can_tx_header.TransmitGlobalTime	= DISABLE;

	return	error;
}

	/**
	 * @brief	Transmit the message in data through CAN
	 * @param	data:		data to transmit
	 * @return	error
	 */
uint32_t	can_message_send	(uint8_t data [CAN_DATA_LEN])
{
	uint32_t	error	= ERR_CAN_OK;

	if (init_pending) {
		return	ERR_CAN_INIT;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		can_tx_data[i]	= data[i];
	}

	/* Transmit TX message */
	if (HAL_CAN_AddTxMessage(&can_handle, &can_tx_header,
				&can_tx_data, &can_tx_mailbox) != HAL_OK) {
		error	= ERR_CAN_HAL_ADD_TX_MSG;
	}

	return	error;
}

	/**
	 * @brief	Return the data received
	 * @param	None
	 * @retval	uint8_t RxData
	 */
uint32_t	can_message_receive	(uint8_t data [CAN_DATA_LEN])
{
	uint32_t	error	= ERR_CAN_OK;

	if (init_pending) {
		return	ERR_CAN_INIT;
	}

	for (i = 0; i < CAN_DATA_LEN; i++) {
		data[i]	= can_rx_data[i];
	}

	return	error;
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
	/* Get RX message */
	if (HAL_CAN_GetRxMessage(can_handle_ptr, CAN_RX_FIFO0,
				&can_rx_header, &can_rx_data) != HAL_OK) {
		error	|= ERR_CAN_HAL_GET_RX_MSG;
		Error_Handler(ERR_PWM_INIT);
	} else {
		flag_can_data_received	= true;
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
