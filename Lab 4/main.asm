;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
			.data
list:		.byte	1,3,4,7,9,10,12,17,19,21,26
indexFound:	.space	1
valueFound:	.space	1

			.text					;R5 is low, R6 is high, R7 is goal
setup:		mov		#list,R5		;Move list memory address into R5
			mov		R5,R6			;Copy starting address into R6
			add		#10,R6			;Add 10 to find ending address
			mov.b	#9,R7			;GOAL
			push	R9				;Search index
			push	R4				;Current Search val
			push	R10				;search index helper
			push	R11
			mov		R5,R11
			;Special case testing
			mov		R6,R9			;Move high address into R9
			mov		R5,R10			;Move low address into R10
			sub		R10,R9			;Subtract the two to get length
			cmp		#1,R9			;If length==2
			jz		special			;Jump to special
			clr		R9				;Else clear modified registers to prevent errors
			clr		R10				;These are probably not needed, but for safety are included

BSearch:	mov		R6,R9			;Move current high address into R9
			mov		R5,R10			;Move current low into R10
			sub		R10,R9			;Subtract High-Low
			rra		R9				;Acts as divide by 2 and floor, finds middle index
			add		R9,R10			;Add middle index to low
			mov.b	@R10,R4			;Gets the number at middle memory address
			cmp.b	R7,	R4			;Check to see if current number is the goal number
			jz		found			;If values are equal, jump to found
			jn		higher			;Else if negative, jump to higher
			jmp		lower			;Else must be lower, jump to lower

lower:		mov		R10,R6			;Move new address into old high address
			jmp		BSearch			;Jump back to while loop

higher:		mov		R10,R5			;Move new address into old low address
			jmp		BSearch			;Jump back to while loop

found:		sub		R11,R10			;Subtract original high address from current address
			mov.b	R10,indexFound	;Store the index into memory
			mov.b	R7,valueFound	;Store the value into memory
			jmp		exit			;Jump back to while loop

exit:		pop		R11				;Restore all the registers that were used
			pop		R10
			pop		R4
			pop		R9
			jmp		$				;Infinite loop to end program
			NOP
;-------------------------------------------------------------------------------
;-	This is the special handler for if the length of the list is 2.            -
;-	This is needed due to how rra floors the value when dividing by 2.         -
;-	Due to the floor, if the length is 2, it will always come back with a      -
;-	middle index of 1. This will cause a infinite loop, as if the goal is in   -
;-	the high address, the normal operation WILL NOT ever be able to find it.   -
;-------------------------------------------------------------------------------
special:	mov.b	@R6,R10			;Move the value at high address into R10
			cmp.b	R7,R10			;Check if High is the goal
			jz		specialHigh		;If high  value is goal, Jump to specialHigh handler
specialLow:	mov.b	#0,indexFound	;Else it MUST be low address, save 0 as indexFound
			mov.b	@R5,valueFound	;Save value at low address into valueFound
			jmp		exit			;Jump to exit
specialHigh:mov.b	#1,indexFound	;MUST be high address, Save 1 as indexFound
			mov.b	@R6,valueFound	;Save value at high address into valueFound
			jmp		exit			;Jump to exit

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
