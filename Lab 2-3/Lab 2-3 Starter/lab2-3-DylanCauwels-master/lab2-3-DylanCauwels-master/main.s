;****************** main.s ***************
; Program written by: ***Your Names**update this***
; Date Created: 2/4/2017
; Last Modified: 2/4/2017
; Brief description of the program
;   The LED toggles at 8 Hz and a varying duty-cycle
; Hardware connections (External: One button and one LED)
;  PE1 is Button input  (1 means pressed, 0 means not pressed)
;  PE0 is LED output (1 activates external9 LED on protoboard)
;  PF4 is builtin button SW1 on Launchpad (Internal) 
;        Negative Logic (0 means pressed, 1 means not pressed)
; Overall functionality of this system is to operate like this
;   1) Make PE0 an output and make PE1 and PF4 inputs.
;   2) The system starts with the the LED toggling at 8Hz,
;      which is 8 times per second with a duty-cycle of 20%.
;      Therefore, the LED is ON for (0.2*1/8)th of a second
;      and OFF for (0.8*1/8)th of a second.
;   3) When the button on (PE1) is pressed-and-released increase
;      the duty cycle by 20% (modulo 100%). Therefore for each
;      press-and-release the duty cycle changes from 20% to 40% to 60%
;      to 80% to 100%(ON) to 0%(Off) to 20% to 40% so on
;   4) Implement a "breathing LED" when SW1 (PF4) on the Launchpad is pressed:
;      a) Be creative and play around with what "breathing" means.
;         An example of "breathing" is most computers power LED in sleep mode
;         (e.g., https://www.youtube.com/watch?v=ZT6siXyIjvQ).
;      b) When (PF4) is released while in breathing mode, resume blinking at 8Hz.
;         The duty cycle can either match the most recent duty-
;         cycle or reset to 20%.
;      TIP: debugging the breathing LED algorithm and feel on the simulator is impossible.
; PortE device registers
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
GPIO_PORTF_PCTL_R  EQU 0x4002552C
GPIO_LOCK_KEY      EQU 0x4C4F434B  ; Unlocks the GPIO_CR register
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
       IMPORT  TExaS_Init
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT  Start
Start
 ; TExaS_Init sets bus clock at 80 MHz
      BL  TExaS_Init ; voltmeter, scope on PD3
      CPSIE  I    ; TExaS voltmeter, scope runs on interrupts
	LDR R1, =SYSCTL_RCGCGPIO_R      ;Activate Port F Clock
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
    LDR R1, =GPIO_PORTF_PCTL_R      ;Start GPIO
    MOV R0, #0x00000000             
    STR R0, [R1]                  
    LDR R1, =GPIO_PORTF_DIR_R       ;Set Direction Register
    MOV R0,#0x00                    ;All Input 
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_AFSEL_R     ;Initialize PortF
    MOV R0, #0                      
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTF_PUR_R       ;Pull Up Resistors
    MOV R0, #0x11                   
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
	
	MOV R4, #10				;R4 = Time On 
	MOV R5, #10					;R5 = TimeOff
loop  
	BL	buttonCheck
	AND R0, #0x00						;Clearing R0
	ADD R1, R4, #0 	 				;Loading Delay values 
	BL	LEDDelay
	AND R0, #0x00						;Clearing R0
	ADD R2, R5, #0 	 				;Loading Delay values 
	BL	LEDDelay 
	B   loop
	
buttonCheck 
	BX LR
	
LEDDelay 
	LDR R3, =GPIO_PORTE_DATA_R  	;Load PortF Data Address
	LDR R6, [R3]					;Loading PortF Data 	
	EOR R6, #0x01					;Toggling LED bit 
	STR R6, [R3]		;Storing Result back in PortF 					
	MOV R0, #2
	B	delay 						;delaying
	BX LR
	
delay 
	;LDR R1, [R3]
	;AND R1, #0x2
	;CMP R1, #2
	;BEQ buttonPushed
	SUBS R0, #1
	BNE delay
	MOV R0, #2
	SUBS R1, #1
	BNE delay
	BX LR 

buttonPushed 
	ADD R6, #1
	BX LR

      ALIGN      ; make sure the end of this section is aligned
      END        ; end of file

;PF4 0-pressed, 