#!/usr/bin/make -f

################################################################################
# *AUTHORS*

# EMAIL		"1903716@gmail.com"
# FULL NAME	"Colomar Andrés, Alejandro"

# EMAIL		""
# FULL NAME	"García Pedroche, Francisco Javier"

# EMAIL		""
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

LED_DIR			= $(STM32L4_MODULES_DIR)/led/
PWM_DIR			= $(STM32L4_MODULES_DIR)/pwm/
SERVO_DIR		= $(STM32L4_MODULES_DIR)/servo/

LIB_DIR			= $(STM32L4_MODULES_DIR)/lib/

export	STM32L4_MODULES_DIR
export	PARENT_DIR

export	STM32_DRIVERS_DIR
export	STM32_CMSIS_DIR
export	STM32L4_CMSIS_DIR
export	STM32L4_HAL_DIR

export	LIBALX_DIR

export	LED_DIR
export	PWM_DIR
export	SERVO_DIR

################################################################################
# target: dependencies
#	action

all:
	$(Q)make -C $(LED_DIR)
	$(Q)make -C $(PWM_DIR)
	$(Q)make -C $(SERVO_DIR)
	$(Q)make -C $(LIB_DIR)


clean:
	@echo  "	RM	*.o *.s *.a"
	$(Q)find . -type f -name '*.o' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.s' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.a' -exec rm '{}' '+'

################################################################################
######## End of file ###########################################################
################################################################################
