// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: Dylan Cauwels, Andrew Han
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "dac.h"

// basic functions defined at end of startup.s

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
uint32_t value;
uint32_t delaywaist;
uint32_t input;
uint32_t previous = 1;
const uint8_t sinewave[32] = {8,9,10,12,13,14,14,15,15,15,14,14,13,12,10,9,8,6,4,3,2,2,1,1,1,2,2,3,4,6,8,8};
uint32_t i=0;
void SysTick_Handler(void){// is called when systick timer hits 0
	GPIO_PORTF_DATA_R ^= 0x04; //toggle blue led
	i = (i+1)&0x1F;// accounts for 32 elements
	if(input ==0){
		DAC_Out(0);
		return;
	}
	else{
		DAC_Out(sinewave[i]);
	}
}
void HeartBeat_Init(void){
	SYSCTL_RCGC2_R |= 0x20;
	delaywaist = SYSCTL_RCGC2_R;
	GPIO_PORTF_DIR_R |= 0x04;
	GPIO_PORTF_DEN_R |= 0x04;
}
int main(void){      
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  Piano_Init();// porte init
  Sound_Init(0);//systick timer init
  EnableInterrupts();
	HeartBeat_Init();//portf init
  while(1){
		input = Piano_In();
		if(previous != input){
		switch(input){
			case 0x00: Sound_Play(0);
			break;
			case 0x01: Sound_Play(25510);//C4-9557, F4-7159
			break;
			case 0x02: Sound_Play(17030);//E4-7585, A4-5682
			break;
			case 0x03: Sound_Play(9557);//C4, G4-6378
			break;
			case 0x04: Sound_Play(11364);//F#4-6757, B4-5062, A3-11364
			break;
			case 0x05: Sound_Play(13514);//F#3-13514, D5-4257
			break;
			case 0x06: Sound_Play(10126);//B3-10126,E5- 3792
			break;
			case 0x07: Sound_Play(15170);//E3, E4
			break;
			case 0x08: Sound_Play(12756 	);//G4
			break;
		}
		previous = input;
		}
	}
}
/*int main(void){
	uint32_t Data;
	TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);
	DAC_Init();
		for(;;){
			DAC_Out(Data);
			Data = 0x0F&(Data + 1);
		}
	}*/