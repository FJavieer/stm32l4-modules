#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(PWM_DIR)/inc/
SRC_DIR = $(PWM_DIR)/src/

# dependencies

ALL	= pwm.o

PWM_INC_LIBALX		=
PWM_INC_CMSIS		= core_cm4.h
PWM_INC_CMSIS_L4	= stm32l4xx.h
PWM_INC_HAL		= stm32l4xx_hal.h
PWM_INC_ERRORS		= errors.h
PWM_INC			= pwm.h
PWM_DEPS		= $(SRC_DIR)/pwm.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(PWM_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(PWM_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(PWM_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(PWM_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(PWM_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(PWM_INC))
PWM_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


pwm.s: $(PWM_DEPS)
	$(Q)$(CC) $(CFLAGS) $(PWM_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
pwm.o: pwm.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
