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
	.file	"servo.c"
	.text
	.align	2
	.global	servo_init
	.syntax unified
	.arm
	.fpu softvfp
	.type	servo_init, %function
servo_init:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L10
	ldrb	r2, [r3]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L7
	push	{r4, lr}
	mov	r4, #0
	sub	sp, sp, #24
	ldr	r1, .L10+4
	ldr	r0, .L10+8
	strb	r4, [r3]
	bl	pwm_init
	ldr	r3, .L10+12
	ldr	r2, [r3, #76]
	orr	r2, r2, #1
	str	r2, [r3, #76]
	ldr	r3, [r3, #76]
	and	r3, r3, #1
	str	r3, [sp]
	mov	r2, #32
	ldr	r3, [sp]
	mov	r3, #2
	stmib	sp, {r2, r3}
	mov	r3, #1
	add	r1, sp, #4
	mov	r0, #1207959552
	str	r3, [sp, #20]
	str	r4, [sp, #12]
	str	r4, [sp, #16]
	bl	HAL_GPIO_Init
	ldr	r0, .L10+16
	bl	pwm_set
	mov	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, lr}
	bx	lr
.L7:
	mov	r0, #0
	bx	lr
.L11:
	.align	2
.L10:
	.word	.LANCHOR0
	.word	20000
	.word	1000000
	.word	1073876992
	.word	1033476506
	.size	servo_init, .-servo_init
	.global	__aeabi_i2f
	.align	2
	.global	servo_position_set
	.syntax unified
	.arm
	.fpu softvfp
	.type	servo_position_set, %function
servo_position_set:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L21
	push	{r0, r1, r4, r5, r6, lr}
	ldrb	r4, [r2]	@ zero_extendqisi2
	cmp	r4, #0
	movne	r4, #2
	bne	.L12
	cmn	r0, #900
	ldr	r5, .L21+4
	bge	.L14
	mvn	r4, #0
	ldr	r3, .L21+8
	str	r3, [r5]	@ float
.L15:
	ldr	r0, [r5]	@ float
	bl	pwm_set
.L12:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	bx	lr
.L14:
	ldr	r6, .L21+12
	cmp	r0, #900
	strgt	r6, [r5]	@ float
	movgt	r4, #1
	bgt	.L15
	bl	__aeabi_i2f
	str	r6, [sp]	@ float
	ldr	r3, .L21+8
	ldr	r2, .L21+16
	ldr	r1, .L21+20
	bl	alx_scale_linear_f
	str	r0, [r5]	@ float
	b	.L15
.L22:
	.align	2
.L21:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	1028443341
	.word	1036831949
	.word	1147207680
	.word	-1000275968
	.size	servo_position_set, .-servo_position_set
	.global	__aeabi_f2iz
	.align	2
	.global	servo_position_get
	.syntax unified
	.arm
	.fpu softvfp
	.type	servo_position_get, %function
servo_position_get:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L27
	push	{r0, r1, r2, r4, r5, lr}
	ldrb	r4, [r3]	@ zero_extendqisi2
	cmp	r4, #0
	movne	r0, #2
	bne	.L23
	ldr	r3, .L27+4
	mov	r5, r0
	str	r3, [sp]	@ float
	ldr	r0, .L27+8
	ldr	r3, .L27+12
	ldr	r2, .L27+16
	ldr	r1, .L27+20
	ldr	r0, [r0]	@ float
	bl	alx_scale_linear_f
	bl	__aeabi_f2iz
	strh	r0, [r5]	@ movhi
	mov	r0, r4
.L23:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, lr}
	bx	lr
.L28:
	.align	2
.L27:
	.word	.LANCHOR0
	.word	1147207680
	.word	.LANCHOR1
	.word	-1000275968
	.word	1036831949
	.word	1028443341
	.size	servo_position_get, .-servo_position_get
	.align	2
	.global	servo_stop
	.syntax unified
	.arm
	.fpu softvfp
	.type	servo_stop, %function
servo_stop:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	bl	pwm_stop
	mov	r0, #0
	pop	{r4, lr}
	bx	lr
	.size	servo_stop, .-servo_stop
	.data
	.set	.LANCHOR0,. + 0
	.type	init_pending, %object
	.size	init_pending, 1
init_pending:
	.byte	1
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	duty, %object
	.size	duty, 4
duty:
	.space	4
	.ident	"GCC: (15:7-2018-q2-4) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
