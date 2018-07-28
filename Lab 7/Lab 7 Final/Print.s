; Print.s
; Student names: Andrew Han and Dylan Cauwels
; Last modification date: 3/27/17
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
    AREA    |.text|, CODE, READONLY, ALIGN=2
e	DCB "*.***"
	PRESERVE8
X EQU 0
Y EQU 4


    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {R4, LR}				;Pushes values that are to be saved.
	SUB SP, #4					;Decremenets the stack pointer, one memory space. #4 because 32 bits as specified in input.
	MOV R1, #10
	CMP R0, R1					;Checks to see if the input is greater than 10.
	BLO Print					;If greater than 10 goes to print to LCD screen.
	UDIV R3, R0, R1				;Divides the input by 10 and stores into R3.
	BL Remainder				;Calls function that does Remainder and stores into R0. 
	STR R0, [SP]				;Stores modulus value onto the stack pointer.
	MOV R0, R3					;Puts the input back into R0.
	BL LCD_OutDec
	LDR R0, [SP, #X]				;Loads the value of R0, from the stack.
	
Print
	ADD R0, R0, #0x30			;Converts R0, into ASCII character.
	BL ST7735_OutChar			;Calls function to output a character onto LCD screen.
	ADD SP, #4
	POP {R4, PC}		;Pops the values pushed onto the stack.

    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {R4,LR}					
	MOV R1, #9999						;Check for overflow scenario. If the input is greater than 9999.
	CMP R0, R1
	BHI Overflow
	SUB SP, #4							;Decremenets the stack pointer, one memory space. #4 because 32 bits as specified in input.
	STR R0, [SP, #X]					;Store R0 because we are using it constantly.
	MOV R1, #1000						
	UDIV R0, R0, R1						;Divides to get the specific value. Example 3### becomes 3.
	ADD R0, #0x30						;Converts to ASCII character.
	BL ST7735_OutChar					;Outputs the thousandths place.		
	MOV R0, #0x2E						;Outputs the period.
	BL ST7735_OutChar
	LDR R0, [SP, #X]
	MOV R1, #1000						;Prepares dividend. 
	BL Remainder													
	MOV R1, #100						;Divides to get the specific value in the other place. #3## becomes #.3##.
	UDIV R0, R0, R1				
	ADD R0, #0x30
	BL ST7735_OutChar					;Outputs.
	LDR R0, [SP, #X]
	MOV R1, #100
	BL Remainder							;Call to modulus.
	MOV R1, #10							;Same thing but with the second to last place. 	
	UDIV R0, R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	LDR R0, [SP, #X]
	MOV R1, #10
	BL Remainder
	ADD R0, #0x30						;Same thing but with the last place. 
	BL ST7735_OutChar
	ADD SP, #4
	BX LR
Overflow								;Outputs each character of the string that signals overflow, "*.***".
	LDR R0, =e
	BL ST7735_OutString
	POP {R4,PC}
	BX  LR
	
Remainder 
	UDIV R2, R0, R1		;Function to recreate modulous since assembly doesn't have it.
	MUL	R1, R2, R1
	SUB R0, R0, R1
	BX LR
    
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file