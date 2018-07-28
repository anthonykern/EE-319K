//****************** main.s ***************
// Program written by: Dylan Cauwels 
// Date Created: 2/26/2017
// Last Modified: 2/14/2017
// Brief description of the program
//   The LED toggles at 8 Hz and a varying duty-cycle
//   Repeat the functionality from Lab2-3 but now we want you to 
//   insert debugging instruments which gather data (state and timing)
//   to verify that the system is functioning as expected.
// Hardware connections (External: One button and one LED)
//  PE1 is Button input  (1 means pressed, 0 means not pressed)
//  PE0 is LED output (1 activates external LED on protoboard)
//  PF2 is Blue LED on Launchpad used as a heartbeat
// Instrumentation data to be gathered is as follows:
// After Button(PE1) press collect one state and time entry. 
// After Buttin(PE1) release, collect 7 state and
// time entries on each change in state of the LED(PE0): 
// An entry is one 8-bit entry in the Data Buffer and one 
// 32-bit entry in the Time Buffer
//  The Data Buffer entry (byte) content has:
//    Lower nibble is state of LED (PE0)
//    Higher nibble is state of Button (PE1)
//  The Time Buffer entry (32-bit) has:
//    24-bit value of the SysTick's Current register (NVIC_ST_CURRENT_R)
// Note: The size of both buffers is 50 entries. Once you fill these
//       entries you should stop collecting data
// The heartbeat is an indicator of the running of the program. 
// On each iteration of the main loop of your program toggle the 
// LED to indicate that your code(system) is live (not stuck or dead).
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "TExaS.h"

int16_t a = 0;
int16_t dCount = 0;
int16_t tCount = 0;
int8_t runCount = 0;
int8_t dataBuffer[50];
int32_t timeBugger[50];

int main(void){
	fillArrays();
	while(1){
		buttonCheck();
		LEDOn();
		dataEntry();
		LEDOff();	
		dataEntry();
}

void fillArrays(){
	for(int i = 0; i < 50; i++)
	{
		dataBuffer[i] = 0xFF;
		timeBuffer[i] = 0xFFFFFFFF;
	}
}

void LEDOn(){
	GPIO_PORTE_DATA_R |= 0x01;
	GPIO_PORTF_DATA_R |= 0x04;
	SysTick_Wait(19990);
	runCount = 1;
}

void LEDOff(){
	GPIO_PORTE_DATA_R &= 0x00;
	GPIO_PORTF_DATA_R &= 0x00;
	SysTick_Wait(20000);
	runCount = 7;
}

void buttonCheck(){
	a = GPIO_PORTE_DATA_R;
	a >> #1;
	a ^= 0x01;
	if(a == 0)
	{
		LEDOn();
		LEDOff();
	}
}

void dataEntry()
{
	if(runCount != 0)
		a = GPIO_PORTE_DATA_R;
		a &= 0x02;
		a << #3;
		a |= (GPIO_PORTE_DATA_R &= 0x01);
		dataArray[dCount] = a;
		dCount ++;
		a = NVIC_ST_CURRENT_R;
		timeArray[tCount] = a;
		tCount ++;
		runCount --;
}