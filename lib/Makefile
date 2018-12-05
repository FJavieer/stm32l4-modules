#!/usr/bin/make -f

# MACRO = substitute with this

# directories

PWM_TMP_DIR	= $(PWM_DIR)/tmp/
SERVO_TMP_DIR	= $(SERVO_DIR)/tmp/

# dependencies

ALL	= libstm32l4-modules.a

MODS_OBJ_PWM	= pwm.o
MODS_OBJ_SERVO	= servo.o servo_test.o
MODS_OBJS	= $(patsubst %,$(PWM_TMP_DIR)/%,$(MODS_OBJ_PWM)) \
			$(patsubst %,$(SERVO_TMP_DIR)/%,$(MODS_OBJ_SERVO))

# target: dependencies
#	action

all: $(ALL)


libstm32l4-modules.a: $(MODS_OBJS)
	$(Q)$(AR) -r -c -s -o $@ $^
	@echo  "	AR	$@"
	@echo  ""
