#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

DELAY_INC_DIR		= $(DELAY_DIR)/inc/
DISPLAY_INC_DIR		= $(DISPLAY_DIR)/inc/
ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/
I2C_INC_DIR		= $(I2C_DIR)/inc/

INC_DIR = $(NUNCHUK_DIR)/inc/
SRC_DIR = $(NUNCHUK_DIR)/src/

# dependencies

ALL	= nunchuk.o nunchuk_test.o

NUNCHUK_INC_LIBALX	=
NUNCHUK_INC_CMSIS	= core_cm4.h
NUNCHUK_INC_CMSIS_L4	= stm32l4xx.h
NUNCHUK_INC_HAL		= stm32l4xx_hal.h
NUNCHUK_INC_ERRORS	= errors.h
NUNCHUK_INC_I2C		= i2c.h
NUNCHUK_INC		= nunchuk.h
NUNCHUK_DEPS		= $(SRC_DIR)/nunchuk.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(NUNCHUK_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(NUNCHUK_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(NUNCHUK_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(NUNCHUK_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(NUNCHUK_INC_ERRORS)) \
			$(patsubst %,$(I2C_INC_DIR)/%,$(NUNCHUK_INC_I2C)) \
			$(patsubst %,$(INC_DIR)/%,$(NUNCHUK_INC))
NUNCHUK_INC_DIRS	= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(I2C_INC_DIR) \
			-I $(ERRORS_INC_DIR)

NUNCHUK_TST_INC_CMSIS	= core_cm4.h
NUNCHUK_TST_INC_CMSIS_L4	= stm32l4xx.h
NUNCHUK_TST_INC_HAL	= stm32l4xx_hal.h
NUNCHUK_TST_INC_DELAY	= delay.h
NUNCHUK_TST_INC_DISPLAY	= display.h
NUNCHUK_TST_INC_ERRORS	= errors.h
NUNCHUK_TST_INC		= nunchuk_test.h	nunchuk.h
NUNCHUK_TST_DEPS	= $(SRC_DIR)/nunchuk_test.c \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(NUNCHUK_TST_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(NUNCHUK_TST_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(NUNCHUK_TST_INC_HAL)) \
			$(patsubst %,$(DELAY_INC_DIR)/%,$(NUNCHUK_TST_INC_DELAY)) \
			$(patsubst %,$(DISPLAY_INC_DIR)/%,$(NUNCHUK_TST_INC_DISPLAY)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(NUNCHUK_TST_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(NUNCHUK_INC))
NUNCHUK_TST_INC_DIRS	= -I $(INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(DELAY_INC_DIR) \
			-I $(DISPLAY_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


nunchuk.s: $(NUNCHUK_DEPS)
	$(Q)$(CC) $(CFLAGS) $(NUNCHUK_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
nunchuk.o: nunchuk.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

nunchuk_test.s: $(NUNCHUK_TST_DEPS)
	$(Q)$(CC) $(CFLAGS) $(NUNCHUK_TST_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
nunchuk_test.o: nunchuk_test.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
