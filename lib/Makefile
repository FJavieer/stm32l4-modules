#!/usr/bin/make -f

# MACRO = substitute with this

# directories

CAN_TMP_DIR	= $(CAN_DIR)/tmp/
CLK_TMP_DIR	= $(CLK_DIR)/tmp/
DELAY_TMP_DIR	= $(DELAY_DIR)/tmp/
DISPLAY_TMP_DIR	= $(DISPLAY_DIR)/tmp/
ERRORS_TMP_DIR	= $(ERRORS_DIR)/tmp/
I2C_TMP_DIR	= $(I2C_DIR)/tmp/
LED_TMP_DIR	= $(LED_DIR)/tmp/
NUNCHUK_TMP_DIR	= $(NUNCHUK_DIR)/tmp/
PWM_TMP_DIR	= $(PWM_DIR)/tmp/
SERVO_TMP_DIR	= $(SERVO_DIR)/tmp/
SPI_TMP_DIR	= $(SPI_DIR)/tmp/

# dependencies

ALL	= libstm32l4-modules.a

MODS_OBJ_CAN		= can.o
MODS_OBJ_CLK		= clk.o
MODS_OBJ_DELAY		= delay.o
MODS_OBJ_DISPLAY	= display.o display_test.o
MODS_OBJ_ERRORS		= errors.o
MODS_OBJ_I2C		= i2c.o
MODS_OBJ_LED		= led.o led_test.o
MODS_OBJ_NUNCHUK	= nunchuk.o nunchuk_test.o
MODS_OBJ_PWM		= pwm.o
MODS_OBJ_SERVO		= servo.o servo_test.o
MODS_OBJ_SPI		= spi.o
MODS_OBJS		= \
			$(patsubst %,$(CAN_TMP_DIR)/%,$(MODS_OBJ_CAN)) \
			$(patsubst %,$(CLK_TMP_DIR)/%,$(MODS_OBJ_CLK)) \
			$(patsubst %,$(DELAY_TMP_DIR)/%,$(MODS_OBJ_DELAY)) \
			$(patsubst %,$(DISPLAY_TMP_DIR)/%,$(MODS_OBJ_DISPLAY)) \
			$(patsubst %,$(ERRORS_TMP_DIR)/%,$(MODS_OBJ_ERRORS)) \
			$(patsubst %,$(I2C_TMP_DIR)/%,$(MODS_OBJ_I2C)) \
			$(patsubst %,$(LED_TMP_DIR)/%,$(MODS_OBJ_LED)) \
			$(patsubst %,$(NUNCHUK_TMP_DIR)/%,$(MODS_OBJ_NUNCHUK)) \
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
