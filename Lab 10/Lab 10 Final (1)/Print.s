; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
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
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {R1, LR}
	CMP R0, #0x0A
	BLO Single
	MOV R2, #0x0A
	UDIV R3, R0, R2
	MUL R1, R3, R2
	SUB R1, R0, R1
	ADD R0, R3, #0x00
	BL LCD_OutDec
	ADD R0, R1, #0x00
Single
	ADD R0, R0, #0x30
	BL ST7735_OutChar
	POP {R1, LR}
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
	PUSH {R4-R8, LR}
	MOV R1, #0x2710
	CMP R0, R1
	BHS Err
	MOV R2, #0x0A
	UDIV R3, R0, R2
	MUL R1, R3, R2
	SUB R1, R0, R1
	ADD R4, R1, #0x30
	
	ADD R0, R3, #0x00
	UDIV R3, R0, R2
	MUL R1, R3, R2
	SUB R1, R0, R1
	ADD R5, R1, #0x30
	
	ADD R0, R3, #0x00
	UDIV R3, R0, R2
	MUL R1, R3, R2
	SUB R1, R0, R1
	ADD R6, R1, #0x30
	
	ADD R0, R3, #0x30
	BL ST7735_OutChar
	MOV R0, #0x2E
	BL ST7735_OutChar
	ADD R0, R6, #0x00
	BL ST7735_OutChar
	ADD R0, R5, #0x00
	BL ST7735_OutChar
	ADD R0, R4, #0x00
	BL ST7735_OutChar
	
	B FixEnd
	
Err
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2E
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	
FixEnd
	POP {R4-R8, LR}
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
