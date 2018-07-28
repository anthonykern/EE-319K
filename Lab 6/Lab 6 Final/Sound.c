// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Dylan Cauwels, Andrew Han
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "tm4c123gh6pm.h"

uint8_t sequence = 0;
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(uint32_t period){//systick init too
	DAC_Init();
	NVIC_ST_CTRL_R = 0;//disable
	NVIC_ST_RELOAD_R = period -1;// 
	NVIC_ST_CURRENT_R = 0; //CURRENT CLEAR
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;//priority 2
	NVIC_ST_CTRL_R = 0x07;// 0111
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play(uint32_t period){
	if(period == 0){
		return;
	}
	NVIC_ST_RELOAD_R = period-1;

}
