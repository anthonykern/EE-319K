// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 
// Last Modified:  
// Lab number: 6


#include "tm4c123gh6pm.h"
#include <stdint.h>


//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
	volatile uint32_t delay;
// --UUU-- Code to initialize PF4 and PF2
	SYSCTL_RCGC2_R = 0x20;//port a and f
	delay = SYSCTL_RCGC2_R;	
	GPIO_PORTF_DIR_R |= 0x04;// PF2-LED PF4-switch 0000 0100
	GPIO_PORTF_DEN_R |= 0x14;// 0001 0100
	GPIO_PORTF_PUR_R |= 0x10;// either pull down or up
	GPIO_PORTF_AFSEL_R = 0x00;
	//--------------------------------------- do I need this??
	//GPIO_PORTA_DIR_R |= 0x00;
	//GPIO_PORTA_DEN_R |= 0xEC;// 1110 1100
	//GPIO_PORTA_AFSEL_R = 0x00;
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
	GPIO_PORTF_DATA_R ^= 0x04;// --UUU-- PF2 is heartbeat
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
	volatile uint32_t count = 20000;
	volatile uint32_t times = 20;
	volatile uint32_t holder = 1;// just a variable, set to 1 to get into loop
	volatile uint32_t waiter = 0;// another variable 
	while(holder > 0){
		holder = GPIO_PORTF_DATA_R >> 4;
		holder = holder &= 0x01;
	}
	while(times > 0){
		while(count > 0){
			count = count - 1;
		}
		times = times - 1;
	}
	while(waiter == 0){
		waiter = GPIO_PORTF_DATA_R >> 4;
		waiter = waiter &= 0x01;		
	}
}  //delay from Lab 2-3

