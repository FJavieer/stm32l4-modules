#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(SPI_DIR)/inc/
SRC_DIR = $(SPI_DIR)/src/

# dependencies

ALL	= spi.o

SPI_INC_LIBALX		=
SPI_INC_CMSIS		= core_cm4.h
SPI_INC_CMSIS_L4	= stm32l4xx.h
SPI_INC_HAL		= stm32l4xx_hal.h
SPI_INC_ERRORS		= errors.h
SPI_INC			= spi.h
SPI_DEPS		= $(SRC_DIR)/spi.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(SPI_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(SPI_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(SPI_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(SPI_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(SPI_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(SPI_INC))
SPI_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


spi.s: $(SPI_DEPS)
	$(Q)$(CC) $(CFLAGS) $(SPI_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
spi.o: spi.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
