#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

INC_DIR = $(LED_DIR)/inc/
SRC_DIR = $(LED_DIR)/src/

# dependencies

ALL	= led.o

LED_INC_CMSIS		= core_cm4.h
LED_INC_CMSIS_L4	= stm32l4xx.h
LED_INC_HAL		= stm32l4xx_hal.h
LED_INC			= led.h
LED_DEPS		= $(SRC_DIR)/led.c \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(LED_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(LED_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(LED_INC_HAL)) \
			$(patsubst %,$(INC_DIR)/%,$(LED_INC))
LED_INC_DIRS		= -I $(INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


led.s: $(LED_DEPS)
	$(Q)$(CC) $(CFLAGS) $(LED_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
led.o: led.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
