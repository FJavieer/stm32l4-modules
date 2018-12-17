#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

DELAY_INC_DIR		= $(DELAY_DIR)/inc/
LED_INC_DIR		= $(LED_DIR)/inc/

INC_DIR = $(ERRORS_DIR)/inc/
SRC_DIR = $(ERRORS_DIR)/src/

# dependencies

ALL	= errors.o

ERRORS_INC_LIBALX	= alx_mask.h
ERRORS_INC_CMSIS	= core_cm4.h
ERRORS_INC_CMSIS_L4	= stm32l4xx.h
ERRORS_INC_HAL		= stm32l4xx_hal.h
ERRORS_INC_DELAY	= delay.h
ERRORS_INC_LED		= led.h
ERRORS_INC		= errors.h
ERRORS_DEPS		= $(SRC_DIR)/errors.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(ERRORS_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(ERRORS_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(ERRORS_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(ERRORS_INC_HAL)) \
			$(patsubst %,$(DELAY_INC_DIR)/%,$(ERRORS_INC_DELAY)) \
			$(patsubst %,$(LED_INC_DIR)/%,$(ERRORS_INC_LED)) \
			$(patsubst %,$(INC_DIR)/%,$(ERRORS_INC))
ERRORS_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(DELAY_INC_DIR) \
			-I $(LED_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


errors.s: $(ERRORS_DEPS)
	@echo  "	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(ERRORS_INC_DIRS) -S $< -o $@
errors.o: errors.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
