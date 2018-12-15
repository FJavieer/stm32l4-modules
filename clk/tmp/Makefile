#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/

INC_DIR = $(CLK_DIR)/inc/
SRC_DIR = $(CLK_DIR)/src/

# dependencies

ALL	= clk.o

CLK_INC_LIBALX		=
CLK_INC_CMSIS		= core_cm4.h
CLK_INC_CMSIS_L4	= stm32l4xx.h
CLK_INC_HAL		= stm32l4xx_hal.h
CLK_INC_ERRORS		= errors.h
CLK_INC			= clk.h
CLK_DEPS		= $(SRC_DIR)/clk.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(CLK_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(CLK_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(CLK_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(CLK_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(CLK_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(CLK_INC))
CLK_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


clk.s: $(CLK_DEPS)
	$(Q)$(CC) $(CFLAGS) $(CLK_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
clk.o: clk.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
