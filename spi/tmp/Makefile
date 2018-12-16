#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(CAN_DIR)/inc/
SRC_DIR = $(CAN_DIR)/src/

# dependencies

ALL	= can.o

CAN_INC_LIBALX		=
CAN_INC_CMSIS		= core_cm4.h
CAN_INC_CMSIS_L4	= stm32l4xx.h
CAN_INC_HAL		= stm32l4xx_hal.h
CAN_INC_ERRORS		= errors.h
CAN_INC			= can.h
CAN_DEPS		= $(SRC_DIR)/can.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(CAN_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(CAN_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(CAN_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(CAN_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(CAN_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(CAN_INC))
CAN_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


can.s: $(CAN_DEPS)
	$(Q)$(CC) $(CFLAGS) $(CAN_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
can.o: can.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
