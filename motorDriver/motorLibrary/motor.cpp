#include "motor.h"
#include "Arduino.h"
#include <avr/interrupt.h>

//static member initial definition
volatile boolean motor::AIN1 = false;
volatile boolean motor::AIN2 = false;
volatile boolean motor::BIN1 = false;
volatile boolean motor::BIN2 = false;
	 
volatile boolean motor::RUNFOREVER = false;
	 
volatile int motor::MOTORDUTYLEFT = 75;;      //motor PWM duty
volatile int motor::MOTORDUTYRIGHT = 75;;     //motor PWM duty
	 
volatile int motor::MOTORDUTYLEFTSET = 75;;      //motor PWM duty
volatile int motor::MOTORDUTYRIGHTSET = 75;;      //motor PWM duty
	 
volatile int motor::MOTORONDURATIONSET = 0;  //motor PWM on duration
volatile int motor::MOTORONDURATION = 0;  //motor PWM on duration

motor::motor()	//constructor
{
    noInterrupts();  
	
	AIN1 = false;
	AIN2 = false;
	BIN1 = false;
	BIN2 = false;
  
    pinMode(AIN1PIN, OUTPUT);
    pinMode(AIN2PIN, OUTPUT);
    pinMode(BIN1PIN, OUTPUT);
    pinMode(BIN2PIN, OUTPUT);
    
    digitalWrite(AIN1PIN, 0);
    digitalWrite(AIN2PIN, 0);
    digitalWrite(BIN1PIN, 0);
    digitalWrite(BIN2PIN, 0);
    
    TCCR2A = 0;
    TCCR2B = 0;
    
    OCR2A = 100;            //compare registers
    OCR2B = 100;
  
    TCNT2 = 0;            // preload timer
    TCCR2B = 0x02;        // 8 prescaler 3.906 kHz
    TIMSK2 |= (1 << TOIE2);   // enable timer overflow interrupt
    TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2B);  // enable timer compare interrupt
    
    interrupts();             // enable all interrupts
}

void motor::motorStartRotate(boolean left, int modulation)
{
    motorDutySet(modulation, modulation);
      
      if(left == true)
      {
          //left turn
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = true;
            
            AIN1 = false;    //pin 10  -> AIN1 off (left motor)
            AIN2 = true;     //pin 11  -> AIN2 on (left motor)
            
            BIN1 = true;     //pin 12  -> BIN1 on (right motor)
            BIN2 = false;    //pin 13  -> BIN2 off (right motor)
            
            MOTORONDURATIONSET = 1;
            MOTORONDURATION = 1; 
          }
      }      
      else
      {
          //right turn
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = true;
            
            AIN1 = true;     //pin 10  -> AIN1 on (left motor)
            AIN2 = false;    //pin 11  -> AIN2 off (left motor)
          
            BIN1 = false;    //pin 12  -> BIN1 off (right motor)
            BIN2 = true;     //pin 13  -> BIN2 on (right motor)
        
            MOTORONDURATIONSET = 1;
            MOTORONDURATION = 1;
          }
      } 
}

int motor::motorDirDist(int dir, int dist, int modulation)
{
      //initialize duration for going straight; exact formula yet to be determined
      int durationForward = dist * CALIBRATE_MOTOR_STRAIGHT / modulation * 100 ; 

      //set modulation 0-100
      if (modulation < 1 || modulation > 100)
      {
          return -1;
      }
      
      //dir 0: no turn, -180: 180 degrees to left, +180: 180 degrees to right
      if(dir < -180 || dir > 180)
      {
          return -1;
      }
     
      //distance to go straight forward or backward, exact range yet to be determined
      if(dist < -10000 || dist > 10000)
      {    
          return -1;
      }   

      motorDir(dir, modulation);  //turn the robot 
      
      int dur = 0;
      do
      {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
          int j = 0;
          dur = MOTORONDURATION;
        }
      }while(dur > 0); 
     
      motorDutySet(modulation, modulation);
      
      if(durationForward > 0)          //move robot straight
      {
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = false;
            
            AIN1 = true;      //pin 10  -> AIN1 on (left motor)
            AIN2 = false;     //pin 11  -> AIN2 off (left motor)
            
            BIN1 = true;      //pin 12  -> BIN1 on (right motor)
            BIN2 = false;     //pin 13  -> BIN2 off (right motor)
  
            MOTORONDURATIONSET = durationForward;
            MOTORONDURATION = durationForward;    
          }   
      }
      else if(durationForward < 0)     //move backward
      {   
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = false;
            
            AIN1 = false;     //pin 10  -> AIN1 off (left motor)
            AIN2 = true;      //pin 11  -> AIN2 on (left motor)
            
            BIN1 = false;     //pin 12  -> BIN1 off (right motor)
            BIN2 = true;      //pin 13  -> BIN2 on (right motor)
            
            MOTORONDURATIONSET = -1* durationForward;
            MOTORONDURATION = -1* durationForward; 
          }
      }
      
      do
      {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
          int g = 0;
          dur = MOTORONDURATION;
        }
      }while(dur > 0);
     
      return 0;
}

int motor::motorStart(boolean forward,int modulationLeft, int modulationRight)
{
      //set modulation 0-100
      if (modulationLeft < 0 || modulationLeft > 100)
      {
          return -1;
      }
      
      if (modulationRight < 0 || modulationRight > 100)
      {
          return -1;
      }
      
      motorDutySet(modulationLeft, modulationRight);
      
      //move robot straight indefinitely
      if(forward)
      {
          //move forward 
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = true;
            
            AIN1 = true;      //pin 10  -> AIN1 on (left motor)
            AIN2 = false;     //pin 11  -> AIN2 off (left motor)
            
            BIN1 = true;      //pin 12  -> BIN1 on (right motor)
            BIN2 = false;     //pin 13  -> BIN2 off (right motor)
            
            MOTORONDURATIONSET = 1;
            MOTORONDURATION = 1;
          }
      }
      else
      {    
          //move backward
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = true;
            
            AIN1 = false;     //pin 10  -> AIN1 off (left motor)
            AIN2 = true;      //pin 11  -> AIN2 on (left motor)
            
            BIN1 = false;     //pin 12  -> BIN1 off (right motor)
            BIN2 = true;      //pin 13  -> BIN2 on (right motor)
            
            MOTORONDURATIONSET = 1;
            MOTORONDURATION = 1;
          }
      }
      
      return 0;
}

void motor::motorDutySet(int left, int right)
{
    MOTORDUTYLEFTSET = left;
    MOTORDUTYRIGHTSET = right;
}

void motor::motorStop()
{
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
      { 
        RUNFOREVER = false;
        
        motorDutySet(0,0);
        
        MOTORONDURATIONSET = 50;
        MOTORONDURATION = 50;    //0 duration on for motors
        
//        AIN1 = false;    //pin 10  -> AIN1  off (left motor)
//        AIN2 = false;    //pin 11  -> AIN2  off (left motor)
//        
//        BIN1 = false;    //pin 12  -> BIN1  off (right motor)
//        BIN2 = false;    //pin 13  -> BIN2  off (right motor)
        
        AIN1 = true;    //pin 10  -> AIN1  off (left motor)
        AIN2 = true;    //pin 11  -> AIN2  off (left motor)
        
        BIN1 = true;    //pin 12  -> BIN1  off (right motor)
        BIN2 = true;    //pin 13  -> BIN2  off (right motor)
      }
}

void motor::motorDir(int dir, int modulation)
{
      // initialize turning duration according to direction and modulation; yet to be determined
      int durationTurn = dir * CALIBRATE_MOTOR_ROTATE / modulation *100; 
      
      motorDutySet(modulation, modulation);
      
      if(durationTurn < 0)
      {
          //left turn
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = false;
            
            AIN1 = false;    //pin 10  -> AIN1 off (left motor)
            AIN2 = true;     //pin 11  -> AIN2 on (left motor)
            
            BIN1 = true;     //pin 12  -> BIN1 on (right motor)
            BIN2 = false;    //pin 13  -> BIN2 off (right motor)
            
            MOTORONDURATIONSET = -1 * durationTurn;
            MOTORONDURATION = -1 * durationTurn; 
          }         
      }      
      else if(durationTurn > 0)
      {
          //right turn
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = false;
            
            AIN1 = true;     //pin 10  -> AIN1 on (left motor)
            AIN2 = false;    //pin 11  -> AIN2 off (left motor)
          
            BIN1 = false;    //pin 12  -> BIN1 off (right motor)
            BIN2 = true;     //pin 13  -> BIN2 on (right motor)
        
            MOTORONDURATIONSET = durationTurn; 
            MOTORONDURATION = durationTurn;
          }
     
      } 
}

void motor::motorDutyEase(int left, int right, float adjustFactor)
{
     int currentLeft = motor::MOTORDUTYLEFT;
     int currentRight = motor::MOTORDUTYRIGHT;
     
     boolean leftGoingUp;
     boolean rightGoingUp;
     
     int leftError, rightError;
     
     if(left > currentLeft)
     {
         leftGoingUp = true;
     }
     else
     {
         leftGoingUp = false;
     }
     
     if(right > currentRight)
     {
         rightGoingUp = true;
     }
     else
     {
         rightGoingUp = false;
     }

      if( (left - currentLeft < 5 && leftGoingUp == true) || (currentLeft - left < 5 && leftGoingUp == false) )
      {
          currentLeft = left;
      }
      else
      {
          leftError = left - currentLeft;
//          currentLeft = currentLeft + (int)((float)leftError/(float)50);
          currentLeft = currentLeft + (int)((float)leftError * adjustFactor);
      }
      
      if( (right - currentRight < 5 && rightGoingUp == true) || (currentRight - right < 5 && rightGoingUp == false) )
      {
          currentRight = right;
      }
      else
      {
          rightError = right - currentRight;
//          currentRight = currentRight + (int)((float)rightError/(float)50);
          currentRight = currentRight + (int)((float)rightError * adjustFactor);
      }
      
      MOTORDUTYLEFT = currentLeft;
      MOTORDUTYRIGHT = currentRight;
}

void motor::compAInterrupt()
{
	if(motor::MOTORDUTYLEFT != 100)         //set motor output AIN1, AIN2 to low
     { 
         digitalWrite(AIN1PIN,0);
         digitalWrite(AIN2PIN,0);
         
         OCR2A = motor::MOTORDUTYLEFT*255/100;            // set compare registers to duty
     }
}

void motor::compBInterrupt()
{
	if(motor::MOTORDUTYRIGHT != 100)         //set motor output BIN1, BIN2 to low
     { 
         digitalWrite(BIN1PIN, 0);
         digitalWrite(BIN2PIN, 0);
         
         OCR2B = motor::MOTORDUTYRIGHT*255/100;            // set compare registers to duty
     }  
}

void motor::overFlowInterrupt()
{
	    int dur;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      dur = motor::MOTORONDURATION;  
    }
    if(dur <= 0)
    {
//       AIN1 = false;
//       AIN2 = false;
//       BIN1 = false;
//       BIN2 = false;
       
       motor::AIN1 = true;
       motor::AIN2 = true;
       motor::BIN1 = true;
       motor::BIN2 = true;
    }
    
    boolean getRunForever;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        getRunForever = motor::RUNFOREVER;
    }
    
    int adjustDuration = motor::MOTORONDURATIONSET/8;
    float adjustThresholdFactor = 0.8;
    int timeToMidDuration;
    float adjustmentFactor = 0;
    
    if(getRunForever == false)    // if  motor is not running indefinitely
    {        
          if(motor::MOTORONDURATIONSET -  motor::MOTORONDURATION < adjustDuration && motor::MOTORONDURATION > motor::MOTORONDURATIONSET/2)    // in 1st half of duration
          {
              adjustmentFactor = ((float)(motor::MOTORONDURATIONSET - motor::MOTORONDURATION))/(float)adjustDuration;  // factor to ramp up the speed
              
              if(adjustmentFactor < adjustThresholdFactor)      //minimum threshold factor for ramp
              {
                 motor::motorDutyEase(motor::MOTORDUTYLEFTSET, motor::MOTORDUTYRIGHTSET, adjustThresholdFactor);
              }
              else
              {
                  motor::motorDutyEase(motor::MOTORDUTYLEFTSET, motor::MOTORDUTYRIGHTSET, adjustmentFactor);
              }
          }
          else if(motor::MOTORONDURATION < adjustDuration && motor::MOTORONDURATION < motor::MOTORONDURATIONSET/2)                   // in 2nd half of duraiton
          {
              adjustmentFactor = (float)1 - ((float)(motor::MOTORONDURATION) /(float)adjustDuration);  // factor to ramp down the speed
              
              if(adjustmentFactor < adjustThresholdFactor)    //minimum threshold factor for ramp
              {
                  motor::motorDutyEase(0, 0, adjustThresholdFactor);
              }
              else
              {
                  motor::motorDutyEase(0, 0, adjustmentFactor); 
              }
          }
        
//        if(dur < 200)                                        //motor PWM ease
//        {
//            motorDutyEase(0,0);                             //slow down
//        }
//        else if(MOTORONDURATIONSET - MOTORONDURATION < 200)  
//        {
//            motorDutyEase(MOTORDUTYLEFTSET, MOTORDUTYRIGHTSET);    //speed up
//        }
        
        if(motor::MOTORONDURATION > 0)
        {
          motor::MOTORONDURATION = motor::MOTORONDURATION - 1;  //decrease timer for motor on duration 
        }
    }
    else                          // if motor is running forever
    {
        motor::motorDutyEase(motor::MOTORDUTYLEFTSET, motor::MOTORDUTYRIGHTSET, 1);    //speed up
    }

     TCNT2 = 0;       // preload timer range 0 - 255 counting up
   
     if(motor::MOTORDUTYLEFT != 0)
     {   
       if(motor::AIN1 == true)      //left motor AIN1
       {
          digitalWrite(AIN1PIN,1);
       }
       else
       {
          digitalWrite(AIN1PIN, 0);
       }
       
       if(motor::AIN2 == true)     //left motor AIN2
       {
          digitalWrite(AIN2PIN, 1);
       }
       else
       {
          digitalWrite(AIN2PIN, 0);
       }
     }
     
    if(motor::MOTORDUTYRIGHT != 0) 
    { 
       if(motor::BIN1 == true)     //right motor BIN1
       {
          digitalWrite(BIN1PIN, 1);
       }
       else
       {
          digitalWrite(BIN1PIN, 0);
       }
       
       if(motor::BIN2 == true)     //right motor BIN2
       {
          digitalWrite(BIN2PIN, 1);
       }
       else
       {
          digitalWrite(BIN2PIN, 0);
       }  
    }
}
ISR(TIMER2_COMPA_vect)          // timer compare interrupt service routine
{
	motor::compAInterrupt();
}

ISR(TIMER2_COMPB_vect)          // timer compare interrupt service routine
{
    motor::compBInterrupt();
}

ISR(TIMER2_OVF_vect)        // interrupt service routine Timer 2 for motor control
{
	motor::overFlowInterrupt();
}

