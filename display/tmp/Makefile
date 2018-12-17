#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

DELAY_INC_DIR		= $(DELAY_DIR)/inc/
ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/
SPI_INC_DIR		= $(SPI_DIR)/inc/

INC_DIR = $(DISPLAY_DIR)/inc/
SRC_DIR = $(DISPLAY_DIR)/src/

# dependencies

ALL	= display.o display_test.o

DISPLAY_INC_CMSIS		= core_cm4.h
DISPLAY_INC_CMSIS_L4		= stm32l4xx.h
DISPLAY_INC_HAL			= stm32l4xx_hal.h
DISPLAY_INC_ERRORS		= errors.h
DISPLAY_INC_SPI			= spi.h
DISPLAY_INC			= display.h
DISPLAY_DEPS			= $(SRC_DIR)/display.c \
				$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(DISPLAY_INC_CMSIS)) \
				$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(DISPLAY_INC_CMSIS_L4)) \
				$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(DISPLAY_INC_HAL)) \
				$(patsubst %,$(ERRORS_INC_DIR)/%,$(DISPLAY_INC_ERRORS)) \
				$(patsubst %,$(SPI_INC_DIR)/%,$(DISPLAY_INC_SPI)) \
				$(patsubst %,$(INC_DIR)/%,$(DISPLAY_INC))
DISPLAY_INC_DIRS		= -I $(INC_DIR) \
				-I $(STM32_CMSIS_INC_DIR) \
				-I $(STM32L4_CMSIS_INC_DIR) \
				-I $(STM32L4_HAL_INC_DIR) \
				-I $(ERRORS_INC_DIR) \
				-I $(SPI_INC_DIR)

DISPLAY_TST_INC_CMSIS		= core_cm4.h
DISPLAY_TST_INC_CMSIS_L4	= stm32l4xx.h
DISPLAY_TST_INC_HAL		= stm32l4xx_hal.h
DISPLAY_TST_INC_DELAY		= delay.h
DISPLAY_TST_INC_ERRORS		= errors.h
DISPLAY_TST_INC			= display_test.h	display.h
DISPLAY_TST_DEPS		= $(SRC_DIR)/display_test.c \
				$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(DISPLAY_TST_INC_CMSIS)) \
				$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(DISPLAY_TST_INC_CMSIS_L4)) \
				$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(DISPLAY_TST_INC_HAL)) \
				$(patsubst %,$(DELAY_INC_DIR)/%,$(DISPLAY_TST_INC_DELAY)) \
				$(patsubst %,$(ERRORS_INC_DIR)/%,$(DISPLAY_TST_INC_ERRORS)) \
				$(patsubst %,$(INC_DIR)/%,$(DISPLAY_INC))
DISPLAY_TST_INC_DIRS		= -I $(INC_DIR) \
				-I $(STM32_CMSIS_INC_DIR) \
				-I $(STM32L4_CMSIS_INC_DIR) \
				-I $(STM32L4_HAL_INC_DIR) \
				-I $(DELAY_INC_DIR) \
				-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


display.s: $(DISPLAY_DEPS)
	$(Q)$(CC) $(CFLAGS) $(DISPLAY_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
display.o: display.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

display_test.s: $(DISPLAY_TST_DEPS)
	$(Q)$(CC) $(CFLAGS) $(DISPLAY_TST_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
display_test.o: display_test.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
