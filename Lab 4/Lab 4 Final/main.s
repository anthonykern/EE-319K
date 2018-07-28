;****************** main.s ***************
; Program written by: ***Your Names**update this***
; Date Created: 2/26/2017
; Last Modified: 2/14/2017
; Brief description of the program
;   The LED toggles at 8 Hz and a varying duty-cycle
;   Repeat the functionality from Lab2-3 but now we want you to 
;   insert debugging instruments which gather data (state and timing)
;   to verify that the system is functioning as expected.
; Hardware connections (External: One button and one LED)
;  PE1 is Button input  (1 means pressed, 0 means not pressed)
;  PE0 is LED output (1 activates external LED on protoboard)
;  PF2 is Blue LED on Launchpad used as a heartbeat
; Instrumentation data to be gathered is as follows:
; After Button(PE1) press collect one state and time entry. 
; After Buttin(PE1) release, collect 7 state and
; time entries on each change in state of the LED(PE0): 
; An entry is one 8-bit entry in the Data Buffer and one 
; 32-bit entry in the Time Buffer
;  The Data Buffer entry (byte) content has:
;    Lower nibble is state of LED (PE0)
;    Higher nibble is state of Button (PE1)
;  The Time Buffer entry (32-bit) has:
;    24-bit value of the SysTick's Current register (NVIC_ST_CURRENT_R)
; Note: The size of both buffers is 50 entries. Once you fill these
;       entries you should stop collecting data
; The heartbeat is an indicator of the running of the program. 
; On each iteration of the main loop of your program toggle the 
; LED to indicate that your code(system) is live (not stuck or dead).
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_AFSEL_R EQU 0x40024420
GPIO_PORTE_DEN_R   EQU 0x4002451C
; PortF device registers
GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
GPIO_PORTF_LOCK_R  EQU 0x40025520
GPIO_PORTF_CR_R    EQU 0x40025524
GPIO_PORTF_AMSEL_R EQU 0x40025528
GPIO_LOCK_KEY      EQU 0x4C4F434B  ; Unlocks the GPIO_CR register
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
;
NVIC_ST_CURRENT_R  EQU 0xE000E018
	;RAM AREA
	   AREA DATA, ALIGN =1
Databuffer SPACE 54					;DATA ARRAY
Timebuffer SPACE 200				;TIME ARRAY
Tpt SPACE 4							;TIME ARRAY POINTER
Dpt SPACE 4							;DATA ARRAY POINTER

       IMPORT  TExaS_Init
	   IMPORT  SysTick_Init
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
	   ;EXPORT  SysTick_Init
       EXPORT  Start
Start
 ; TExaS_Init sets bus clock at 80 MHz
    BL  TExaS_Init ; voltmeter, scope on PD3
    CPSIE  I    ; TExaS voltmeter, scope runs on interrupts
	LDR R1, =SYSCTL_RCGCGPIO_R      ;Activate Port F/E Clock
    LDR R0, [R1]                 
    ORR R0, R0, #0x30               
    STR R0, [R1]                  	;Clock Initialize Time
    NOP
    NOP                             
    LDR R1, =GPIO_PORTF_LOCK_R      ;Unlock PortF Register
    LDR R0, =0x4C4F434B             ;MAYBE YES OR NO NOT SURE WHY
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_CR_R        
    MOV R0, #0xFF                   
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_AMSEL_R     ;Disable Analog
    MOV R0, #0                      
    STR R0, [R1]                                    
    LDR R1, =GPIO_PORTF_DIR_R       ;Set Direction Register
    MOV R0, #0x04                   ;PF2 Output 
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_AFSEL_R     ;Initialize PortF
    MOV R0, #0                      
    STR R0, [R1]                    
	LDR R1, =GPIO_PORTE_AFSEL_R     ;Initialize PortE
    MOV R0, #0                      
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_PUR_R       ;Pull Up Resistors
    MOV R0, #0x10                   
    STR R0, [R1]              
    LDR R1, =GPIO_PORTF_DEN_R       ;Enable PortF
    MOV R0, #0xFF                   
    STR R0, [R1] 
	LDR R1, =GPIO_PORTE_DEN_R       ;Enable PortE
    MOV R0, #0xFF                  
    STR R0, [R1] 
	LDR R1, =GPIO_PORTE_DIR_R       ;Set Direction Register
    MOV R0,#0x01                    ;0 Output, 1 Input
    STR R0, [R1]  
	;
	MOV R3, #1						; R3 CONTAINS COUNT FOR DUTY MULTIPLIER
	MOV R4, #4						; R4 CONTAINS COUNT FOR LEDOFF DUTY MULTIPLIER
	MOV R5, #25						; PUTS MULITPLE OF 25 IN R5
	MOV R12, #0
	;R6 WILL CONTAIN 25*X IN LEDON
	;R7 WILL CONTAIN 25*Y IN LEDOFF
	;R0 AND R1 RESERVED FOR LOADING OF ADDRESS AND DATA
	;
	;
	;
	BL Debug_Init
mainloop;-----------------------------------------------------------------

	BL buttoncheck					;CHECKS PORTE1(BUTTON)
checkpoint
	BL LEDON						;TOGGLE LED ON
	BL dataEntry
	BL LEDOFF						;TOGGLE LED OFF
	BL dataEntry
	B mainloop						;REPEAT
	;
	;
	;
LEDON;---------------------------------------------------------------------
	CMP R3, #0
	BEQ next
	LDR R1, =GPIO_PORTE_DATA_R  	;Load PortE Data Address
	LDR R0, [R1]					;Loading PortE Data 	
	MOV R0, #0x01					;Toggling LED PE0
	STR R0, [R1]					;Storing Result back in PortE
	LDR R1, =GPIO_PORTF_DATA_R		; LOAD PORTE DATA ADDRESS 
	LDR R0, [R1]
	MOV R0, #0x04					; ON
	STR R0, [R1]
	MUL R6, R3, R5					; NUMBER OF MILISECS, R5 HAS 25, R3 HAS NUMBER OF TIMES TO MULTIPLY BY, PUT IN R6
LOOP
	MOV R2, #19990					; COUNT
delay
	SUBS R2, R2, #1					; R2 CONTAINS A NUMBER TO GET UP TO 1MS (16000)
	BNE delay 
	SUBS R6, R6, #1					; R6 CONTAINS NUMBER OF MS COUNT
	BNE LOOP
next
	BX LR
	;
	;
	;
LEDOFF;---------------------------------------------------------------
	CMP R4, #0
	BEQ next1
	LDR R1, =GPIO_PORTE_DATA_R		; LOAD PORTE DATA ADDRESS 
	LDR R0, [R1]
	MOV R0, #0x00					; OFF
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DATA_R		; LOAD PORTF DATA ADDRESS 
	LDR R0, [R1]
	MOV R0, #0x00					; OFF
	STR R0, [R1]
	MUL R7, R4, R5					;# OF MILISECS, R5 HAS 25 AND, R4 HAS NUMBER OF TIMES TO MULTIPLY BY, PUT IN R7
LOOP1
	MOV R2, #20000 				   	; COUNT
delay1
	SUBS R2, R2, #1					; R2 CONTAINS A NUMBER TO GET UP TO 1MS (16000)
	BNE delay1 
	SUBS R7, R7, #1					; R4 CONTAINS NUMBER OF OFF MS COUNT
	BNE LOOP1
next1
	BX LR
	;
	;
	;
buttoncheck;-------------------------------------------------------------
	LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	LSR R0, #1
	EOR R0, #0x01
	CMP R0, #0						;CHECKS IF PE1 IS PRESSED
	BEQ check				
	BX LR							;BACK TO MAINLOOP
check	
	PUSH {R12, LR}					;WHEN BUTTON IS PRESSED INCREASES R3 COUNT AND DECREASES R4
	MOV R12, #1						;Setting dataEntry to run once after button release
	BL dataEntry
here
	LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	LSR R0, #1
	EOR R0, #0x01
	CMP R0, #0
	BNE done 
	BL LEDON 
	BL LEDOFF
	B  here
done
	POP {R12, LR}
	ADD R3, #1
	CMP R3, #6						;SEES IF BUTTON HAS BEEN PRESSED 5 TIMES (6TH TIME SHOULD SET TO
	BNE change2
	AND R3, #0
change2
	MOV R12, #7						;Setting dataEntry to run 7 times after button release
	SUBS R4, R4 , #1
	CMP  R4, #0
	BPL back						;IF POSITIVE OR ZERO JUMP, IF NEGATIVE WANT TO ADD 5 TO GET TO 4(RESET)
	ADD R4, #6
back
	BX LR							;TO MAIN LOOP
	;
	;
	;
Debug_Init;-----------------------------------------------------------------
	PUSH {R12,LR}
	LDR R1, =Databuffer
	MOV R0, #0xFF					; NO STATE SAVED YET
	STR R0, [R1]
	LDR R1, =Timebuffer
	MOV R0, #0xFFFFFFFF				; NO TIMING SAVED YET
	STR R0, [R1]
	LDR R1, =Databuffer				;INITIALIZATION Data Array Pointer	
	LDR R0, =Dpt
	STR R1, [R0]	
	LDR R1, =Timebuffer				;INITIALIZATION Time Array Pointer	
	LDR R0, =Tpt
	STR R1, [R0]
	BL SysTick_Init					;TO SYSTIC SR
	MOV R6, #50
	MOV R2, #0xFFFFFFFF
	LDR R1,=Tpt
	LDR R0, [R1]
L1
	CMP R6, #0
	BEQ fin1
	STR R2, [R0]
	ADD R0, #4
	SUBS R6, #1
	B L1
fin1
	MOV R6, #50
	MOV R2, #0xFF
	LDR R1, =Dpt
	LDR R0, [R1]
L2
	CMP R6, #0
	BEQ fin2
	STRB R2, [R0]
	ADD R0, #1
	SUBS R6, #1
	B L2
fin2
	POP {R12, LR}
	BX LR							;TO MAIN LOOP
	;
	;
	;
dataEntry ;DEBUG capture-----------------------------------
	CMP R12, #0						;R12 = amount of times dataEntry should run w/out reset (7 or 1)
	BEQ back 
	SUBS R12, #1
	CMP R11, #50					;R11 = index counter (if 50 then done)
	BEQ back
	ADD R11, #1
	LDR R1, =GPIO_PORTE_DATA_R  	;Loading PortE Data
	LDR R0, [R1]				 	
	MOV R2, R0						;Data Manipulation
	AND R2, #0x02
	LSL R2, #3
	AND R0, #0x01
	ORR R2, R2, R0					;Final Value 
	LDR R1, =Dpt					;Storing in the Array R1=DPOINTER
	LDR R0, [R1]					;ADDRESS IN R0
	STR R2, [R0]					;VALUE IN R2
	ADD R0, #1						;1 BECAUSE 8BIT (should it be 4 because register address is 32bit)
	STR R0, [R1]					;
	LDR R1, =NVIC_ST_CURRENT_R		;systick timer register	
	LDR R2, [R1]					;value into R2
	LDR R1, =Tpt					;Storing in the Array R1=TPOINTER
	LDR R0, [R1]					;ADDRESS IN R0
	STR R2, [R0]					;VALUE IN R2
	ADD R0, #4						;4 BECAUSE 32BIT
	STR R0, [R1]					;Fin
	BX LR							;TO CHECK LOOP IN DATAENTRY

      ALIGN      ; make sure the end of this section is aligned
      END        ; end of file

