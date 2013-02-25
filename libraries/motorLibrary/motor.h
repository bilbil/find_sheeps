/*
v0.0 - initial implementation
v0.1 - proportional control for speeding up

PWM Motor Control Logic

Using Timer 2 for PWM output to motor driver IC

Timer 2:
TCCR2B = 0x02; 16 Mhz/ 8 (prescaler)/256 = 3.906 kHz
OCR2A compare interrupt for PWM duty of AIN1, AIN2
OCR2B compare interrupt for PWM duty of BIN1, BIN2

//pin 10  -> AIN1 (left motor)
//pin 11  -> AIN2 (left motor)   
//pin 12  -> BIN1 (right motor)
//pin 13  -> BIN2 (right motor)

*/

#ifndef motor_h
#define motor_h

#include "Arduino.h"

// calibration constants
#define CALIBRATE_MOTOR_ROTATE 30  //90 degree turn: 5V supply voltage: 60 at 70% duty, 7.5V supply: 20 at 50% duty, 5V supply and 85%: 32, 45 degree at 5V,85%: 
#define CALIBRATE_MOTOR_STRAIGHT 1

#include <util/atomic.h>

//motor PWM output pins
#define AIN1PIN 10
#define AIN2PIN 11
#define BIN1PIN 12
#define BIN2PIN 13
	
class motor
{
  public:
    motor();
	
	static volatile boolean AIN1;	//left motor AIN1
	static volatile boolean AIN2;	//left motor AIN2
	static volatile boolean BIN1;	//right motor BIN1
	static volatile boolean BIN2;	//right motor BIN2
	 
	static volatile boolean RUNFOREVER;
	 
	static volatile int MOTORDUTYLEFT;      //motor PWM duty
	static volatile int MOTORDUTYRIGHT;     //motor PWM duty
	 
	static volatile int MOTORDUTYLEFTSET;      //motor PWM duty
	static volatile int MOTORDUTYRIGHTSET;      //motor PWM duty
	 
	static volatile int MOTORONDURATIONSET;  //motor PWM on duration
	static volatile int MOTORONDURATION;  //motor PWM on duration
	
    static void motorStartRotate(boolean left, int modulation);	// public function for rotating robot until motorStop is called
    static int motorDirDist(int dir, int dist, int modulation);	//public function for turning robot and move a certain distance
    static int motorDistNoBlock(int dist, int modulation);
	static int motorStart(boolean forward,int modulationLeft, int modulationRight);	//public function for moving indefinitely
    static void motorDutySet(int left, int right);	// function for PWM duty setpoint
    static void motorStop();	//public function for stopping the robot
	static void motorDutyEase(int left, int right, float adjustFactor);	//function for PWM duty easing using duty setpoints
	static void motorDir(int dir, int modulation);	// internal helper function for turning the robot
	
	static void compAInterrupt();
	static void compBInterrupt();
	static void overFlowInterrupt();
	
  private:
 
};

#endif