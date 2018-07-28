// SysTick.c
// Implements two busy-wait based delay routines
#include <stdint.h>
// Initialize SysTick with busy wait running at bus clock.
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))//reload is the counter register
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;//Disable
	NVIC_ST_RELOAD_R = 0x00FFFFFF;// set to max value 
	NVIC_ST_CURRENT_R = 0;// Clears current
	NVIC_ST_CTRL_R = 5;// 0101 Clk and enable
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){//in terms of cycles
	NVIC_ST_RELOAD_R = delay-1;
	NVIC_ST_CURRENT_R = 0;
	while((NVIC_ST_CTRL_R &0x00010000)==0){
	}
}

// Time delay using busy wait.
// waits for count*10ms
// 10000us equals 10ms
void SysTick_Wait10ms(delay){//in terms of 10ms
volatile uint8_t index;
	for(index = 0; index < delay; index ++){
		SysTick_Wait(800000);
	}
}

