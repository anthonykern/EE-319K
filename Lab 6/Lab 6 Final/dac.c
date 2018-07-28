// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Dylan Cauwels, Andrew Han
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: pins 0-3
void DAC_Init(void){
	uint32_t delay;
	SYSCTL_RCGC2_R |= 0x02;//PORTB
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_DEN_R |= 0x0F; //0000 1111 4 bit dac
	GPIO_PORTB_DIR_R |= 0x0F;// all outputs
	GPIO_PORTB_AFSEL_R &= 0x00;	
	//Clock already initialized 
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data;	
}
