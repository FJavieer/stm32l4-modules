#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

DELAY_INC_DIR		= $(DELAY_DIR)/inc/
ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(LED_DIR)/inc/
SRC_DIR = $(LED_DIR)/src/

# dependencies

ALL	= led.o led_test.o

LED_INC_CMSIS		= core_cm4.h
LED_INC_CMSIS_L4	= stm32l4xx.h
LED_INC_HAL		= stm32l4xx_hal.h
LED_INC_ERRORS		= errors.h
LED_INC			= led.h
LED_DEPS		= $(SRC_DIR)/led.c \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(LED_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(LED_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(LED_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(LED_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(LED_INC))
LED_INC_DIRS		= -I $(INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

LED_TST_INC_CMSIS	= core_cm4.h
LED_TST_INC_CMSIS_L4	= stm32l4xx.h
LED_TST_INC_HAL		= stm32l4xx_hal.h
LED_TST_INC_DELAY	= delay.h
LED_TST_INC_ERRORS	= errors.h
LED_TST_INC		= led_test.h	led.h
LED_TST_DEPS		= $(SRC_DIR)/led_test.c \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(LED_TST_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(LED_TST_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(LED_TST_INC_HAL)) \
			$(patsubst %,$(DELAY_INC_DIR)/%,$(LED_TST_INC_DELAY)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(LED_TST_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(LED_INC))
LED_TST_INC_DIRS	= -I $(INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(DELAY_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


led.s: $(LED_DEPS)
	$(Q)$(CC) $(CFLAGS) $(LED_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
led.o: led.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

led_test.s: $(LED_TST_DEPS)
	$(Q)$(CC) $(CFLAGS) $(LED_TST_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
led_test.o: led_test.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
