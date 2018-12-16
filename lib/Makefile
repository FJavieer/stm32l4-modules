#!/usr/bin/make -f

# MACRO = substitute with this

# directories

CAN_TMP_DIR	= $(CAN_DIR)/tmp/
CLK_TMP_DIR	= $(CLK_DIR)/tmp/
DELAY_TMP_DIR	= $(DELAY_DIR)/tmp/
ERRORS_TMP_DIR	= $(ERRORS_DIR)/tmp/
LED_TMP_DIR	= $(LED_DIR)/tmp/
PWM_TMP_DIR	= $(PWM_DIR)/tmp/
SERVO_TMP_DIR	= $(SERVO_DIR)/tmp/
SPI_TMP_DIR	= $(SPI_DIR)/tmp/

# dependencies

ALL	= libstm32l4-modules.a

MODS_OBJ_CAN	= can.o
MODS_OBJ_CLK	= clk.o
MODS_OBJ_DELAY	= delay.o
MODS_OBJ_ERRORS	= errors.o
MODS_OBJ_LED	= led.o led_test.o
MODS_OBJ_PWM	= pwm.o
MODS_OBJ_SERVO	= servo.o servo_test.o
MODS_OBJ_SPI	= spi.o
MODS_OBJS	= \
			$(patsubst %,$(CAN_TMP_DIR)/%,$(MODS_OBJ_CAN)) \
			$(patsubst %,$(CLK_TMP_DIR)/%,$(MODS_OBJ_CLK)) \
			$(patsubst %,$(DELAY_TMP_DIR)/%,$(MODS_OBJ_DELAY)) \
			$(patsubst %,$(ERRORS_TMP_DIR)/%,$(MODS_OBJ_ERRORS)) \
			$(patsubst %,$(LED_TMP_DIR)/%,$(MODS_OBJ_LED)) \
			$(patsubst %,$(PWM_TMP_DIR)/%,$(MODS_OBJ_PWM)) \
			$(patsubst %,$(SERVO_TMP_DIR)/%,$(MODS_OBJ_SERVO)) \
			$(patsubst %,$(SPI_TMP_DIR)/%,$(MODS_OBJ_SPI))

# target: dependencies
#	action

all: $(ALL)


libstm32l4-modules.a: $(MODS_OBJS)
	$(Q)$(AR) -r -c -s -o $@ $^
	@echo  "	AR	$@"
	@echo  ""
