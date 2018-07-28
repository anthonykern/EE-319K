// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "tm4c123gh6pm.h"
//
void DAC_Init(void){
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;//PORTB
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTB_DEN_R |= 0x0F; //0000 1111 4 bit dac
	GPIO_PORTB_DIR_R |= 0x0F;// all outputs
	GPIO_PORTB_AFSEL_R &= 0x00;	
}
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data;	
}
