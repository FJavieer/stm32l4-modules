#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(TIM_DIR)/inc/
SRC_DIR = $(TIM_DIR)/src/

# dependencies

ALL	= tim.o

TIM_INC_LIBALX		=
TIM_INC_CMSIS		= core_cm4.h
TIM_INC_CMSIS_L4	= stm32l4xx.h
TIM_INC_HAL		= stm32l4xx_hal.h
TIM_INC_ERRORS		= errors.h
TIM_INC			= tim.h
TIM_DEPS		= $(SRC_DIR)/tim.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(TIM_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(TIM_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(TIM_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(TIM_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(TIM_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(TIM_INC))
TIM_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


tim.s: $(TIM_DEPS)
	$(Q)$(CC) $(CFLAGS) $(TIM_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
tim.o: tim.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
