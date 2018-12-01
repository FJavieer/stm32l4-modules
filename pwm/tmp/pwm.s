	.cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 4
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"pwm.c"
	.text
	.global	__aeabi_uidiv
	.align	2
	.global	pwm_init
	.syntax unified
	.arm
	.fpu softvfp
	.type	pwm_init, %function
pwm_init:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L18
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #0
	bxeq	lr
	push	{r0, r1, r4, r5, r6, lr}
	mov	r5, #0
	strb	r5, [r3]
	ldr	r3, .L18+4
	ldr	r2, [r3, #88]
	orr	r2, r2, #1
	str	r2, [r3, #88]
	ldr	r3, [r3, #88]
	and	r3, r3, #1
	str	r3, [sp, #4]
	ldr	r3, [sp, #4]
	mov	r3, #1073741824
	ldr	r4, .L18+8
	str	r3, [r4]
	ldr	r3, .L18+12
	mov	r6, r1
	mov	r1, r0
	ldr	r0, [r3]
	bl	__aeabi_uidiv
	sub	r0, r0, #1
	stmib	r4, {r0, r5}
	sub	r6, r6, #1
	mov	r0, r4
	str	r6, [r4, #12]
	str	r5, [r4, #16]
	str	r5, [r4, #20]
	str	r5, [r4, #24]
	bl	HAL_TIM_Base_Init
	cmp	r0, #0
	beq	.L3
.L4:
	b	.L4
.L3:
	mov	r3, #4096
	str	r3, [r4, #64]
	mov	r3, #32768
	str	r0, [r4, #72]
	str	r0, [r4, #76]
	add	r1, r4, #64
	mov	r0, r4
	str	r3, [r4, #68]
	bl	HAL_TIM_ConfigClockSource
	cmp	r0, #0
	beq	.L5
.L6:
	b	.L6
.L5:
	str	r0, [r4, #80]
	str	r0, [r4, #88]
	add	r1, r4, #80
	mov	r0, r4
	bl	HAL_TIMEx_MasterConfigSynchronization
	cmp	r0, #0
	beq	.L7
.L8:
	b	.L8
.L7:
	mov	r0, r4
	bl	HAL_TIM_PWM_Init
	cmp	r0, #0
	beq	.L9
.L10:
	b	.L10
.L9:
	mov	r3, #96
	str	r0, [r4, #100]
	str	r3, [r4, #92]
	str	r0, [r4, #108]
	str	r0, [r4, #104]
	str	r0, [r4, #116]
	str	r0, [r4, #112]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	bx	lr
.L19:
	.align	2
.L18:
	.word	.LANCHOR0
	.word	1073876992
	.word	.LANCHOR1
	.word	SystemCoreClock
	.size	pwm_init, .-pwm_init
	.global	__aeabi_fcmpgt
	.global	__aeabi_fcmplt
	.global	__aeabi_ui2f
	.global	__aeabi_fmul
	.global	__aeabi_f2uiz
	.align	2
	.global	pwm_set
	.syntax unified
	.arm
	.fpu softvfp
	.type	pwm_set, %function
pwm_set:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r1, #1065353216
	mov	r6, r0
	bl	__aeabi_fcmpgt
	cmp	r0, #0
	bne	.L20
	mov	r1, #0
	mov	r0, r6
	bl	__aeabi_fcmplt
	cmp	r0, #0
	bne	.L20
	ldr	r3, .L31
	ldrb	r5, [r3]	@ zero_extendqisi2
	cmp	r5, #0
	beq	.L22
.L23:
	b	.L23
.L22:
	ldr	r4, .L31+4
	ldr	r0, [r4, #12]
	bl	__aeabi_ui2f
	mov	r1, r6
	bl	__aeabi_fmul
	bl	__aeabi_f2uiz
	add	r1, r4, #92
	str	r0, [r4, #96]
	mov	r2, r5
	mov	r0, r4
	bl	HAL_TIM_PWM_ConfigChannel
	subs	r1, r0, #0
	beq	.L24
.L25:
	b	.L25
.L24:
	mov	r0, r4
	bl	HAL_TIM_PWM_Start
	cmp	r0, #0
	beq	.L20
.L26:
	b	.L26
.L20:
	pop	{r4, r5, r6, lr}
	bx	lr
.L32:
	.align	2
.L31:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.size	pwm_set, .-pwm_set
	.align	2
	.global	pwm_stop
	.syntax unified
	.arm
	.fpu softvfp
	.type	pwm_stop, %function
pwm_stop:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r0, .L37
	bl	HAL_TIM_Base_Stop
	cmp	r0, #0
	beq	.L33
.L35:
	b	.L35
.L33:
	pop	{r4, lr}
	bx	lr
.L38:
	.align	2
.L37:
	.word	.LANCHOR1
	.size	pwm_stop, .-pwm_stop
	.data
	.set	.LANCHOR0,. + 0
	.type	init_pending, %object
	.size	init_pending, 1
init_pending:
	.byte	1
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	tim_handle, %object
	.size	tim_handle, 64
tim_handle:
	.space	64
	.type	clk_config, %object
	.size	clk_config, 16
clk_config:
	.space	16
	.type	master_config, %object
	.size	master_config, 12
master_config:
	.space	12
	.type	oc_init, %object
	.size	oc_init, 28
oc_init:
	.space	28
	.ident	"GCC: (15:7-2018-q2-4) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
