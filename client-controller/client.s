	.file	"client.cpp"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	_Z9toggleLedv
	.type	_Z9toggleLedv, @function
_Z9toggleLedv:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x5
	ldi r25,lo8(32)
	eor r24,r25
	out 0x5,r24
/* epilogue start */
	ret
	.size	_Z9toggleLedv, .-_Z9toggleLedv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"hello"
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r22,lo8(.LC0)
	ldi r23,hi8(.LC0)
	ldi r24,lo8(Serial)
	ldi r25,hi8(Serial)
	rcall _ZN5Print5printEPKc
	sbi 0x4,5
	ldi r24,lo8(-1)
	out 0xa,r24
.L3:
	rcall _Z9toggleLedv
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	rjmp .L3
	.size	main, .-main
	.ident	"GCC: (Fedora 14.2.0-1.fc41) 14.2.0"
.global __do_copy_data
