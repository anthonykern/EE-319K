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

void EnableInterrupts(void);
#define northGo		0
#define	northSlow	1
#define	westGo		2
#define	westSlow 	3
#define walkGo		4
#define walkSlow	5
#define walkNo		6
#define true 			1
#define false 		0
uint8_t 	stateCounter;
uint8_t 	walk;
uint32_t 	current = northGo;
uint32_t	westButton;
uint32_t	northButton;

struct State{																																																					//data structure containing information of states
	uint32_t 	carLights;																																																//has to be at the front so the compiler doesnt reject the other methods
	uint32_t	walkLights;
	uint32_t	flash;
	uint32_t	delay;
};

typedef struct State STyp;
STyp FSM[7] = {																																																				//pertain to Port A and Port B- to be changed
	{0x30,0x2,0,5},
	{0x50,0x2,0,2},
	{0x84,0x2,0,5},
	{0x88,0x2,0,2},
	{0x90,0x2,0},
	{0x90,0x8,1},
	{0x90,0x8,1}
};

void initializePorts()																																																//waiting on specific port information to finish constants
{
	//Clock
	//DIR 
	//DEN
	//AFSEL (clear all)
}

void lights(uint32_t carLights, uint32_t walkLights, uint32_t delay)																									//basic method that sets the car and walk lights based on the current state's data
{
	GPIO_PORTE_DATA_R &= carLights;
	GPIO_PORTE_DATA_R ^= carLights;
	GPIO_PORTF_DATA_R &= walkLights;
	GPIO_PORTF_DATA_R ^= walkLights;
	SysTick_Wait(delay);
}

void walkCycle()																																																			//used for the unique special snowaflake walk cycle 
{
	uint8_t i;
	current = walkGo;
	lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);
	current = walkSlow;
	for(i = 0; i < 6; i++)
	{
			lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);
			current = walkNo;
			lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);
			current = walkSlow;
	}
	walk = false;
}

void nextState()																																																			//uses variables set by ISR's and basic logic to figure out the next state
{																																																											//****REQUIRES interrupts to put values into variables to test
	if(walk == true && current != walkGo && current != walkSlow && current != walkNo)															//walk has been pressed and the program isnt already in the walk state
	{
		if(current == northGo || current == westGo)																																	//program isnt in transition states
		{
				current += 1;																																														//transition
				lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);
		}
		walkCycle();																																																//go to walk cycle 
	}
	else if(walk == true)																																													//change the walk to false if the program is already in that state
		walk = false;
	else if(westButton == true && current == northGo)																															//checking if the west button has been pressed
		{
				current += 1;																																														//transition if it has
				lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);
				current = westGo;
		}
	else if(northButton == true && current == northGo)																														//checking if the north button has been pressed
		{
				current += 1;																																														//transition if it is
				lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);
				current = northGo;
		}
}


int main(void){ volatile unsigned long delay;
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate traffic simulation and set system clock to 80 MHz
  SysTick_Init();     
  EnableInterrupts();
	initializePorts();
  while(1){																																																							//infinite loop
	lights(FSM[current].carLights, FSM[current].walkLights, FSM[current].delay);																					//execute state
	nextState();																																																					//find next state (honestly the name should've told you that)
  }
}
