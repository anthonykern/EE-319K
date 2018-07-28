; SysTick.s
; Module written by: ***Your Names**update this***
; Date Created: 2/14/2017
; Last Modified: 2/14/2017 
; Brief Description: Initializes SysTick

NVIC_ST_CTRL_R        EQU 0xE000E010
NVIC_ST_RELOAD_R      EQU 0xE000E014
NVIC_ST_CURRENT_R     EQU 0xE000E018

        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
; ;-UUU-Export routine(s) from SysTick.s to callers
		EXPORT SysTick_Init

;------------SysTick_Init------------
; ;-UUU-Complete this subroutine
; Initialize SysTick with busy wait running at bus clock.
; Input: none
; Output: none
; Modifies: ??
SysTick_Init
	; DISABLE SYSTICK DURING SETUP
	LDR R1, =NVIC_ST_CTRL_R;
	MOV R0, #0
	STR R0, [R1]
	; SET RELOAD TO MAX VALUE
	LDR R1, =NVIC_ST_RELOAD_R
	LDR R0, =0x00FFFFFF; RELOAD VALUE (SHOULD THIS BE A MOV)
	STR R0, [R1]
	; CLEARS CURRENT
	LDR R1, =NVIC_ST_CURRENT_R
	MOV R0, #0
	STR R0, [R1]
	;ENABLE
	LDR R1, =NVIC_ST_CTRL_R
	MOV R0, #0x0005; 0101 clk and enabe
	STR R0, [R1]
    BX  LR                          ; return


    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
