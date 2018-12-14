#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(DELAY_DIR)/inc/
SRC_DIR = $(DELAY_DIR)/src/

# dependencies

ALL	= delay.o

DELAY_INC_LIBALX	=
DELAY_INC_CMSIS		= core_cm4.h
DELAY_INC_CMSIS_L4	= stm32l4xx.h
DELAY_INC_HAL		= stm32l4xx_hal.h
DELAY_INC_ERRORS	= errors.h
DELAY_INC		= delay.h
DELAY_DEPS		= $(SRC_DIR)/delay.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(DELAY_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(DELAY_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(DELAY_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(DELAY_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(DELAY_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(DELAY_INC))
DELAY_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


delay.s: $(DELAY_DEPS)
	@echo  "	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(DELAY_INC_DIRS) -S $< -o $@
delay.o: delay.s
	@echo  "	AS	$@"
	$(Q)$(AS) $< -o $@


clean:
	@echo  "	RM	*.o *.s"
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
