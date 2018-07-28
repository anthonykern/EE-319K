// TableTrafficLight.c solution to edX lab 10, EE319KLab 5
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// November 7, 2013

/* solution, do not post

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "TExaS.h"

// Declare your FSM linked structure here
#define northGo 0
#define northYelWest 1
#define westGo 2
#define westYelNorth 3
#define checkWalkW 4
#define checkWalkN 5
#define allRed 6
#define Walk 7 
#define northYelWalk 8
#define westYelWalk 9
#define togRed0 10
#define togOFF0 11
#define togRed1 12
#define togOFF1 13
#define togRed2 14
#define togOFF2 15



typedef struct State{
	uint32_t carLights; //portE
	uint32_t walkLights; //portF
	uint32_t delay;// in 10ms
	uint32_t nextState[8]; // {000, 001, 010, 011, 100, 101, 110, 111}
}STyp;
STyp FSM[16] = {
	{0x21, 0x02, 100, {northGo, checkWalkN, northGo, checkWalkN, northYelWest, checkWalkN, northYelWest, checkWalkN}},
	{0x22, 0x02, 75, {westGo, westGo, westGo, westGo, westGo, westGo, westGo, westGo}},
	{0x0C, 0x02, 100, {westYelNorth, checkWalkW, westYelNorth, checkWalkW, westGo, checkWalkW, westYelNorth, checkWalkW}},
	{0x14, 0x02, 75, {northGo, northGo, northGo, northGo, northGo, northGo, northGo, northGo}},
	{0x0C, 0x02, 100, {westGo, westYelWalk, westYelNorth, westYelWalk, westGo, westYelWalk, westYelNorth, westYelWalk}},
	{0x21, 0x02, 100, {northGo, northYelWalk, northGo, northYelWalk, northYelWest, northYelWalk, northYelWest, northYelWalk}},
	{0x24, 0x02, 75, {Walk, Walk, Walk, Walk, Walk, Walk, Walk, Walk}},
	{0x24, 0x08, 200, {togRed0, togRed0, togRed0, togRed0, togRed0, togRed0, togRed0, togRed0}},
	{0x22, 0x02, 75, {allRed, allRed, allRed, allRed, allRed, allRed, allRed, allRed}},
	{0x14, 0x02, 75, {allRed, allRed, allRed, allRed, allRed, allRed, allRed, allRed}},
	{0x24, 0x02, 20, {togOFF0, togOFF0, togOFF0, togOFF0, togOFF0, togOFF0, togOFF0, togOFF0, }},
	{0x24, 0x00, 20, {togRed1, togRed1, togRed1, togRed1, togRed1, togRed1, togRed1, togRed1}},
	{0x24, 0x02, 20, {togOFF1, togOFF1, togOFF1, togOFF1, togOFF1, togOFF1, togOFF1, togOFF1}},
	{0x24, 0x00, 20, {togRed2, togRed2, togRed2, togRed2, togRed2, togRed2, togRed2, togRed2}},
	{0x24, 0x02, 20, {togOFF2, togOFF2, togOFF2, togOFF2, togOFF2, togOFF2, togOFF2, togOFF2}},
	{0x24, 0x00, 20, {northGo, northGo, northGo, northGo, westGo, westGo, westGo, westGo}}
};


void EnableInterrupts(void);
void initializePorts(){
	volatile uint32_t delaywaist;
	SYSCTL_RCGC2_R |= 0x32;
	delaywaist = SYSCTL_RCGC2_R ;
	GPIO_PORTB_DIR_R |= 0x00;
	GPIO_PORTB_DEN_R |= 0x07;
	GPIO_PORTB_AFSEL_R &= 0x00;
	GPIO_PORTE_DIR_R |=0x3F;
	GPIO_PORTE_DEN_R |= 0x3F;
	GPIO_PORTE_AFSEL_R &= 0x00;
	GPIO_PORTF_DIR_R |=0x0A;
	GPIO_PORTF_DEN_R |=0x0A;
	GPIO_PORTF_AFSEL_R &= 0x00;
}

uint32_t current = 0;
uint32_t next;

int main(void){ volatile unsigned long delay;
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate traffic simulation and set system clock to 80 MHz
  SysTick_Init();     
  EnableInterrupts();
	initializePorts();
  while(1){
		GPIO_PORTE_DATA_R = FSM[current].carLights;//writes to car LEDs
		GPIO_PORTF_DATA_R = FSM[current].walkLights;//writes to board LEDs
		SysTick_Wait10ms(FSM[current].delay);
		next = GPIO_PORTB_DATA_R; 
		current = (FSM[current].nextState[next]);
  }
}

