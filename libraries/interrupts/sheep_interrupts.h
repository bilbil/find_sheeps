#ifndef sheep_interrupts_h
#define sheep_interrupts_h

#include "Arduino.h"

#include <motor.h>
#include <sensor.h>

#define INTERRUPT_SCALER 9

int interrupt_div = INTERRUPT_SCALER;
	
ISR(TIMER2_COMPA_vect)          // timer compare interrupt service routine
{
	motor::compAInterrupt();	//call motor functions
}

ISR(TIMER2_COMPB_vect)          // timer compare interrupt service routine
{
    motor::compBInterrupt();	//call motor functions
}

ISR(TIMER2_OVF_vect)        	// interrupt service routine Timer 2 for motor control
{	

	// sensor::updateDistances();
	
	if(interrupt_div < 1)
	{	
		sensor::process();			//call grid sensor functions	
		interrupt_div = INTERRUPT_SCALER;
	}
	else
	{
		interrupt_div--;
	}
	motor::overFlowInterrupt();	//call motor functions
}


#endif