#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(I2C_DIR)/inc/
SRC_DIR = $(I2C_DIR)/src/

# dependencies

ALL	= i2c.o

I2C_INC_LIBALX		=
I2C_INC_CMSIS		= core_cm4.h
I2C_INC_CMSIS_L4	= stm32l4xx.h
I2C_INC_HAL		= stm32l4xx_hal.h
I2C_INC_ERRORS		= errors.h
I2C_INC			= i2c.h
I2C_DEPS		= $(SRC_DIR)/i2c.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(I2C_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(I2C_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(I2C_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(I2C_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(I2C_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(I2C_INC))
I2C_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


i2c.s: $(I2C_DEPS)
	$(Q)$(CC) $(CFLAGS) $(I2C_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
i2c.o: i2c.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
