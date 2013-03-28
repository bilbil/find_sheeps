#include "motor.h"
#include "Arduino.h"
#include <avr/interrupt.h>

//static member initial definition
volatile boolean motor::AIN1 = false;
volatile boolean motor::AIN2 = false;
volatile boolean motor::BIN1 = false;
volatile boolean motor::BIN2 = false;

volatile int motor::MOTORACTION = MOTORSTOP;
	 
volatile boolean motor::RUNFOREVER = false;
	 
volatile int motor::MOTORDUTYLEFT = 75;      //motor PWM duty
volatile int motor::MOTORDUTYRIGHT = 75;     //motor PWM duty
	
	 
volatile int motor::MOTORONDURATIONSET = 0;  //motor PWM on duration
volatile int motor::MOTORONDURATION = 0;  //motor PWM on duration

motor::motor()	//constructor
{
    noInterrupts();  
	
	MOTORACTION = MOTORSTOP;
  
    pinMode(AIN1PIN, OUTPUT);
    pinMode(AIN2PIN, OUTPUT);
    pinMode(BIN1PIN, OUTPUT);
    pinMode(BIN2PIN, OUTPUT);
    
    TCCR2A = 0;
    TCCR2B = 0;
    
    OCR2A = 110;            //compare registers
    OCR2B = 100;
  
    TCNT2 = 0;            // preload timer
	
    TCCR2B = 0x02;        // 8 prescaler 7.813 kHz
	// TCCR2B = 0x01;        // 1 prescaler 62.5 kHz

    // TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
    // TIMSK2 |= (1 << OCIE2B);  // enable timer compare interrupt
	// TIMSK2 |= (1 << TOIE2);   // enable timer overflow interrupt
    
	//enable specific interrupts from Sensor Class
	
    interrupts();             // enable all interrupts
}

void motor::motorStartRotate(boolean left, int modulation)
{
    motorDutySet(modulation, modulation);
      
      if(left == true)
      {			
			MOTORACTION = MOTORPIVOTLEFT;
      }      
      else
      {
			MOTORACTION = MOTORPIVOTRIGHT;
      } 
}

void motor::motorStartRotateSeparation(boolean left, int modulationLeft, int modulationRight)
{
	  motorDutySet(modulationLeft, modulationRight);
      
      if(left == true)
      {			
			MOTORACTION = MOTORPIVOTLEFT;
      }      
      else
      {
			MOTORACTION = MOTORPIVOTRIGHT;
      } 
}

int motor::motorStart(boolean forward,int modulationLeft, int modulationRight)
{
    
      motorDutySet(modulationLeft, modulationRight);
      
      //move robot straight indefinitely
      if(forward)
      {		
			MOTORACTION = MOTORFORWARD;
      }
      else
      {    
			MOTORACTION = MOTORBACKWARD;
      }
      
      return 0;
}

void motor::motorDutySet(int left, int right)
{
	MOTORDUTYLEFT = left;
	MOTORDUTYRIGHT = right;
	
	OCR2B = motor::MOTORDUTYRIGHT*255/100;            // set compare registers to duty
	OCR2A = motor::MOTORDUTYLEFT*255/100;            // set compare registers to duty
}

void motor::motorStop()
{
		MOTORACTION = MOTORSTOP;       
}

void motor::compAInterrupt()
{
	//set motor output AIN1, AIN2 for left motor to stop turning

	 // PORTB |= B00001100;	//original
	 // PORTB |= B00010100;	//switched pin 4 and 5 version
	 
	 //PCB version start
	PORTD &= B11110111;	//left motor input 1 is PD3 (4th bit)
	PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
	 //PCB version end
	
}

void motor::compBInterrupt()
{
	//set motor output BIN1, BIN2 for right motor to stop turning
	
	 // PORTB |= B00110000; //original
	 // PORTB |= B00101000; 	//switched pin 4 and 5 version
	 
	 //PCB version start
	PORTD &= B10011111;	//right motor input 3 is PD6 (7th bit), input 4 is PD5 (6th bit)
	 //PCB version end
}

void motor::overFlowInterrupt()
{
	switch(MOTORACTION)
	{
		case MOTORSTOP:
			// PORTB |= B00111100;	//original
			// PORTB |= B00111100;	//switched pin 4 and 5 version
			
			//PCB version start
			PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
			PORTD &= B10010111;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			 //PCB version end
		break;
		case MOTORFORWARD:
			// PORTB &= B11000011;	//original
			// PORTB |= B00010100;	//original
			// PORTB |= B00001100;	//switched pin 4 and 5 version
			
			//PCB version start
			PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
			PORTD &= B10010111;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			
			PORTD |= B01001000;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			 //PCB version end
		break;
		case MOTORPIVOTLEFT:
			// PORTB &= B11000011;	//original
			// PORTB |= B00011000;	//original	
			// PORTB |= B00011000;	//switched pin 4 and 5 version
			
			//PCB version start
			//stop first
			PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
			PORTD &= B10010111;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			
			//turn left input 2 and right input 3
			PORTB |= B00001000;	//left motor input 2 is PB3 (4th bit)
			PORTD |= B01000000;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			 //PCB version end
		break;
		case MOTORPIVOTRIGHT:
			// PORTB &= B11000011;	//original
			// PORTB |= B00100100;	//original
			// PORTB |= B00100100;	//switched pin 4 and 5 version
			
			//PCB version start
			//stop first
			PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
			PORTD &= B10010111;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			
			//turn left input 1 and right input 4
			PORTD |= B00101000;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			 //PCB version end
		break;
		case MOTORBACKWARD:
			// PORTB &= B11000011;	//original
			// PORTB |= B00101000;	//original
			// PORTB |= B00110000;	//switched pin 4 and 5 version
			
			//PCB version start
			PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
			PORTD &= B10010111;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			
			PORTB |= B00001000;	//left motor input 2 is PB3 (4th bit)
			PORTD |= B00100000;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			 //PCB version end
		break;
		default:
			// PORTB &= B11000011;	//original
			
			//stop motors
			//PCB version start
			PORTB &= B11110111;	//left motor input 2 is PB3 (4th bit)
			PORTD &= B10010111;	//right motor input 3 is PD6 (7th bit), right motor input 4 is PD5 (6th bit), left motor input 1 is PD3 (4th bit)
			 //PCB version end
	}
	
}


