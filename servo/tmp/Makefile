#!/usr/bin/make -f

# directories

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

DELAY_INC_DIR		= $(DELAY_DIR)/inc/
ERRORS_INC_DIR		= $(ERRORS_DIR)/inc/
LED_INC_DIR		= $(LED_DIR)/inc/
PWM_INC_DIR		= $(PWM_DIR)/inc/

INC_DIR = $(SERVO_DIR)/inc/
SRC_DIR = $(SERVO_DIR)/src/

# dependencies

ALL	= servo.o servo_test.o

SERVO_INC_LIBALX	= alx_math.h
SERVO_INC_CMSIS		= core_cm4.h
SERVO_INC_CMSIS_L4	= stm32l4xx.h
SERVO_INC_HAL		= stm32l4xx_hal.h
SERVO_INC_ERRORS	= errors.h
SERVO_INC_PWM		= pwm.h
SERVO_INC		= servo.h
SERVO_DEPS		= $(SRC_DIR)/servo.c \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(SERVO_INC_LIBALX)) \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(SERVO_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(SERVO_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(SERVO_INC_HAL)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(SERVO_INC_ERRORS)) \
			$(patsubst %,$(PWM_INC_DIR)/%,$(SERVO_INC_PWM)) \
			$(patsubst %,$(INC_DIR)/%,$(SERVO_INC))
SERVO_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(PWM_INC_DIR) \
			-I $(ERRORS_INC_DIR)

SERVO_TST_INC_CMSIS	= core_cm4.h
SERVO_TST_INC_CMSIS_L4	= stm32l4xx.h
SERVO_TST_INC_HAL	= stm32l4xx_hal.h
SERVO_TST_INC_DELAY	= delay.h
SERVO_TST_INC_ERRORS	= errors.h
SERVO_TST_INC		= servo_test.h	servo.h
SERVO_TST_DEPS		= $(SRC_DIR)/servo_test.c \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(SERVO_TST_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(SERVO_TST_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(SERVO_TST_INC_HAL)) \
			$(patsubst %,$(DELAY_INC_DIR)/%,$(SERVO_TST_INC_DELAY)) \
			$(patsubst %,$(ERRORS_INC_DIR)/%,$(SERVO_TST_INC_ERRORS)) \
			$(patsubst %,$(INC_DIR)/%,$(SERVO_INC))
SERVO_TST_INC_DIRS	= -I $(INC_DIR) \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR) \
			-I $(DELAY_INC_DIR) \
			-I $(ERRORS_INC_DIR)

# target: dependencies
#	action

all: $(ALL)


servo.s: $(SERVO_DEPS)
	$(Q)$(CC) $(CFLAGS) $(SERVO_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
servo.o: servo.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"

servo_test.s: $(SERVO_TST_DEPS)
	$(Q)$(CC) $(CFLAGS) $(SERVO_TST_INC_DIRS) -S $< -o $@
	@echo  "	CC	$@"
servo_test.o: servo_test.s
	$(Q)$(AS) $< -o $@
	@echo  "	AS	$@"


clean:
	$(Q)rm -f *.o *.s

################################################################################
######## End of file ###########################################################
################################################################################
