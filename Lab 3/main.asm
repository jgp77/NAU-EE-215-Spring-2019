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
scores:		.byte 	18,20,6,10,16,16,18,19,13,0,14,16,14,17,16
histogram:	.space  21
maxScore:	.space 	1
modeValue:	.space  1

			.text

main:
			;max
			mov		#scores,R13		;Get starting address
			mov		R13,R14			;Creating an end address
			add		#15,R14			;15=number of elements
			clr		R15				;Clear R15 for use as max value
			call 	#max			;Call max subroutine
			mov.b	R15,maxScore	;Set R15 as computed max value
			;histo
			push	R7 				;Preserve R7
			clr		R7				;Clear out R7
			mov		#scores,R13		;Get starting address
			mov		R13,R14			;Creating an end address
			add		#15,R14			;15=number of elements
;Loop to iterate from 0 to 20 and call the histogram subroutine with each value.
histoLoop:
			clr 	R15				;Clear R15
			mov		R7,R12			;Move R7/Counter into R12
			call 	#histo			;Call histo subroutine
			mov.b	R15,histogram(R7);Move the number of values/R7 into histogram
			inc		R7				;increment counter/R7
			cmp		#21,R7			;See if the loop has run 21 times (0-20)
			jnz		histoLoop		;If it has not run 21 times, loop again
			pop		R7				;Restore R7

			;mode (max histo)
			mov		#histogram,R13	;move the histogram memory address into R13
			mov		R13,R14			;Copy R13 into R14
			add		#19,R14			;Add 19 to get the max memory address
			call 	#max			;Call max subroutine
			sub		R13,R12			;Subtract the highest occuring number's memory address from the begining adress
			mov.b	R12,modeValue	;Set R15 as computed max value
			jmp 	end				;Jump to end

;Max loop
;	Preserves the registers. Move the staring address into a register.
;	Move the item located at current address into a different register.
;	Compare to current max and set equal to whichever value is bigger. (current vs. new)
;	Check if at the end of the list. If we are return to caller, else repeat loop after iterating.
;
;R13 is the starting address, R15 is the current max, R14 is the ending address
max:
			push	R5				;Preserving register R5
			push	R6				;Preserving R6
			mov		R13,R5			;Move starting address into R5
for:		mov.b	@R5,R6			;Move item at R5 into R6
			cmp.b	R6,R15			;Compare fetched item to current max
			jge		skipSet			;If the new item is less, skip setting max
			mov		R6,R15			;Else set new max value
			mov		R5,R12			;and save new max memory address (for mode)
skipSet:	inc		R5				;Increment current memory address
			cmp     R5,R14			;Check if at end adress
			jnz 	for				;If not at end, repeat for
			pop		R6				;Else pop preserved data into R6
			pop		R5				;and into R5
			ret						;return to caller
;Histogram loop
;
;	Preserves register before running. Moves the starting/current address into a register.
;	Compares the value at that adress to the search value.  If it is equal to search value,
;	increment the counter. Else skips the increment. Check if at thte end and if you are then
;	restore registers and return to caller.
;	R13 is the starting address, R12 is the search value, R14 is the end address
histo:
			push 	R5				;Preserving R5
			push	R6				;and R6
			mov		R13,R5			;Move the starting address into R5
loop:		mov.b 	@R5,R6			;Move the data at address of R5 into R6
			cmp.b	R12,R6			;Compare to the current number/score iteration
			jnz		skipInc			;If not equal, skip incrementing
			inc		R15				;Else increment R15/counter
skipInc:	inc		R5				;Increment to next address
			cmp		R5,R14			;Check if at the end of loop
			jnz		loop			;If not at the end, jump back to loop
			pop		R5				;Else restore R5
			pop		R6				;and R6
			ret						;Return to caller

end:
			jmp		$
			NOP
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
            
