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
list: 	.byte 20,77,18,17,16,18,14,2,120,1,10,90,8,7,6,127,4,3,2,1

		.text
		clr.b 	R5				;Clear R5 for use as the index
		clr.b   R6				;Clear R6 for use as the boolean Swapped
		mov.b 	#1,R4			;Set R4 to 1 for use as index+1 or rightElement
		jmp 	firstRun		;Setup complete. Jump to firstRun

BubbleSort:
		cmp.w 	#0,R6 			;Checks to see if a swap occured
		jz 		exit			;If a swap did not occur (R6=0), exit
		clr.b 	R6 				;Else clear out swapped and repeat
		clr.b 	R5				;Reset the leftElement index
		mov.b 	#2,R4			;Reset the rightElement index

firstRun:
		mov.b 	list(R5),R9		;Move leftElement into R9
		mov.b 	list(R4),R10	;Move rightElement into R10
		cmp.w 	R10,R9			;Compare the left and right elements
		jeq		skipSwap		;If Left=Right, no need to swap
		jn 		skipSwap 		;Else if Left <= Right, Skip the swap
		call 	#Swap	  		;Else Right < left, Swap

Swap:
		mov.b 	R9,list(R4)		;Put the leftElement into the right element location
		mov.b 	R10,list(R5)	;Put the rightElement into the right element location
		mov.w 	#1,R6			;Set R6 to 1, Swapped = true
		ret						;Return to caller


skipSwap:
		inc 	R5			   	;Increment the leftElement pointer by 2
		inc 	R4				;Increment the rightElement pointer by 2
		mov.b 	R5,R15			;Moves the current leftElement index into R15 for manipulation
		add.w 	#list,R15		;Adds the lists memory address to R15 to get current memory address
		mov.w 	#list,R14		;Moves the lists memory address into R14
		add.w 	#19,R14			;Adds 19 to R14, 19 is the number of elements in the list decremented by 1
		cmp.w 	R14,R15			;If it has hit the end of the list
		jz 		BubbleSort		;jump to BubbleSort
		jmp 	firstRun		;Else repeat sort

exit:
		jmp 	$				;Infinite loop to end program


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
