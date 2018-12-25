#!/usr/bin/make -f

################################################################################
# *AUTHORS*

# EMAIL		<1903716@gmail.com>
# FULL NAME	"Colomar Andrés, Alejandro"

# EMAIL		<@.>
# FULL NAME	"García Pedroche, Francisco Javier"

# EMAIL		<@.>
# FULL NAME	"Junquera Carrero, Santiago"

################################################################################
# Beautify output
# ---------------------------------------------------------------------------
# Prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)some command here
#
# If BUILD_VERBOSE equals 0 then the above command will be hidden.
# If BUILD_VERBOSE equals 1 then the above command is displayed.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifeq ("$(origin V)","command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif

ifeq ($(BUILD_VERBOSE), 1)
  Q =
else
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  Q = @
endif

export	Q
export	BUILD_VERBOSE

################################################################################
# Do not print "Entering directory ...",
# but we want to display it when entering to the output directory
# so that IDEs/editors are able to understand relative filenames.
MAKEFLAGS += --no-print-directory

################################################################################
# Make variables (CC, etc...)
CC	= arm-none-eabi-gcc
AS	= arm-none-eabi-as
AR	= arm-none-eabi-ar

export	CC
export	AS
export	AR

################################################################################
# directories

STM32L4_MODULES_DIR	= $(CURDIR)
PARENT_DIR		= $(STM32L4_MODULES_DIR)/../

STM32_DRIVERS_DIR	= $(PARENT_DIR)/stm32l4-drivers/
STM32_CMSIS_DIR		= $(STM32_DRIVERS_DIR)/CMSIS/
STM32L4_CMSIS_DIR	= $(STM32_CMSIS_DIR)/ST/STM32L4xx/
STM32L4_HAL_DIR		= $(STM32_DRIVERS_DIR)/STM32L4xx_HAL_Driver/

LIBALX_DIR		= $(PARENT_DIR)/libalx/

CAN_DIR			= $(STM32L4_MODULES_DIR)/can/
CLK_DIR			= $(STM32L4_MODULES_DIR)/clk/
DELAY_DIR		= $(STM32L4_MODULES_DIR)/delay/
DISPLAY_DIR		= $(STM32L4_MODULES_DIR)/display/
ERRORS_DIR		= $(STM32L4_MODULES_DIR)/errors/
I2C_DIR			= $(STM32L4_MODULES_DIR)/i2c/
LED_DIR			= $(STM32L4_MODULES_DIR)/led/
NUNCHUK_DIR		= $(STM32L4_MODULES_DIR)/nunchuk/
PWM_DIR			= $(STM32L4_MODULES_DIR)/pwm/
SERVO_DIR		= $(STM32L4_MODULES_DIR)/servo/
SPI_DIR			= $(STM32L4_MODULES_DIR)/spi/
TIM_DIR			= $(STM32L4_MODULES_DIR)/tim/

LIB_DIR			= $(STM32L4_MODULES_DIR)/lib/

export	STM32L4_MODULES_DIR
export	PARENT_DIR

export	STM32_DRIVERS_DIR
export	STM32_CMSIS_DIR
export	STM32L4_CMSIS_DIR
export	STM32L4_HAL_DIR

export	LIBALX_DIR

export	CAN_DIR
export	CLK_DIR
export	DELAY_DIR
export	DISPLAY_DIR
export	ERRORS_DIR
export	I2C_DIR
export	LED_DIR
export	NUNCHUK_DIR
export	PWM_DIR
export	SERVO_DIR
export	SPI_DIR
export	TIM_DIR

################################################################################
# target: dependencies
#	action

all:
	@echo	'	MAKE	modules:	can'
	$(Q)make -C $(CAN_DIR)
	@echo	'	MAKE	modules:	clk'
	$(Q)make -C $(CLK_DIR)
	@echo	'	MAKE	modules:	delay'
	$(Q)make -C $(DELAY_DIR)
	@echo	'	MAKE	modules:	display'
	$(Q)make -C $(DISPLAY_DIR)
	@echo	'	MAKE	modules:	errors'
	$(Q)make -C $(ERRORS_DIR)
	@echo	'	MAKE	modules:	i2c'
	$(Q)make -C $(I2C_DIR)
	@echo	'	MAKE	modules:	led'
	$(Q)make -C $(LED_DIR)
	@echo	'	MAKE	modules:	nunchuk'
	$(Q)make -C $(NUNCHUK_DIR)
	@echo	'	MAKE	modules:	pwm'
	$(Q)make -C $(PWM_DIR)
	@echo	'	MAKE	modules:	servo'
	$(Q)make -C $(SERVO_DIR)
	@echo	'	MAKE	modules:	spi'
	$(Q)make -C $(SPI_DIR)
	@echo	'	MAKE	modules:	tim'
	$(Q)make -C $(TIM_DIR)
	@echo	'	MAKE	modules:	lib'
	$(Q)make -C $(LIB_DIR)


clean:
	@echo  "	RM	*.o *.s *.a"
	$(Q)find . -type f -name '*.o' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.s' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.a' -exec rm '{}' '+'

################################################################################
######## End of file ###########################################################
################################################################################
