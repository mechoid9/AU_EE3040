;*******************************************************
;* ELEC 3050 - Lab 1, Program 1                        *
;*  Exercise the HCS12 with assembly language          *
;*  Project board LED1 is connected to PORTA, bit 0    *
;*                SW1 is connected to PORTE, bit 0     *
;*******************************************************
;export symbols
	XDEF Entry	;export 'Entry' symbol
	ABSENTRY Entry 	;for absolute assembly: mark this as application entry point

;include derivative specific macros
	INCLUDE 'MC9S12C32.inc'

ROMStart EQU $4000 ; absolute address to place my code/constant data

;variable/data section
	ORG RAMStart
;Insert here your data definition.
PAvalue DS.B 1		;holds pattern currently in PORTA

;code section 
	ORG 	ROMStart
Entry: 	LDS 	#RAMEnd+1	;initialize the stack pointer
	bset	DDRA,%00000001	;configure PA0 as output (LED1)
	bclr	DDRE,%00000001	;configure PE0 as input (SW1)
	clr	PAvalue		;initialize variable to 0
E1:	movb 	PAvalue,PORTA	;write pattern to PORTA (LED1)
E2:	brclr	PORTE,%00000001,E2 ;wait for button press
	bsr	Delay		;time delay for debounce
	com	PAvalue		;toggle LED state
	bra E1			;infinite loop

;* Delay subroutine - use nested loops to create a delay *
Delay:	ldaa #50	;do outer loop 50 times
Outer: 	lx #0		;do inner loop 64k times
Inner:	dex		;X counts inner loops
	bne Iner	;repeat inner until X=0
	deca		;A counts outer loops
	bne Outer	;repeat outer until A=0
	rts		;return to main 

;****************************************
;*	Interupt Vectors		*
;****************************************
	ORG $FFFE
	DC.W Entry	;Reset Vector

