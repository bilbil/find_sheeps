/*
  v0.0 - initial implementation
  v0.1 - proportional control for speeding up

 PWM Motor Control Logic

  Using Timer 2 for PWM output to motor driver IC
  
  Timer 2:
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 100;   //pin 11 -> BIN2  (right motor)
  OCR2B = 100;  //pin 3   -> BIN1  (right motor)
  
  16 Mhz/ 8 (prescaler)/256 = 3.906 kHz
  
  //pin 10  -> AIN1 (left motor)
  //pin 11  -> AIN2 (left motor)   
  //pin 12  -> BIN1 (right motor)
  //pin 13  -> BIN2 (right motor)
  
 */



 // calibration constants
 #define CALIBRATE_MOTOR_ROTATE 15  //90 degree turn: 5V supply voltage: 60 at 70% duty, 7.5V supply: 20 at 50% duty, 5V supply and 85%: 32, 45 degree at 5V,85%: 
 #define CALIBRATE_MOTOR_STRAIGHT 1

 #include <util/atomic.h>
 
 //motor PWM output pins
 #define AIN1PIN 10
 #define AIN2PIN 11
 #define BIN1PIN 12
 #define BIN2PIN 13
 
 volatile boolean AIN1 = false;
 volatile boolean AIN2 = false;
 volatile boolean BIN1 = false;
 volatile boolean BIN2 = false;
 
 volatile boolean RUNFOREVER = false;
 
 volatile int MOTORDUTYLEFT = 75;      //motor PWM duty
 volatile int MOTORDUTYRIGHT = 75;     //motor PWM duty
 
 volatile int MOTORDUTYLEFTSET = 75;      //motor PWM duty
 volatile int MOTORDUTYRIGHTSET = 75;      //motor PWM duty
 
 volatile int MOTORONDURATIONSET = 0;  //motor PWM on duration
 volatile int MOTORONDURATION = 0;  //motor PWM on duration
 
void setup()  {
  
  noInterrupts();  
  
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

ISR(TIMER2_COMPA_vect)          // timer compare interrupt service routine
{
     if(MOTORDUTYLEFT != 100)         //set motor output AIN1, AIN2 to low
     { 
         digitalWrite(AIN1PIN, 0);
         digitalWrite(AIN2PIN, 0);
         
         OCR2A = MOTORDUTYLEFT*255/100;            // set compare registers to duty
     }
}

ISR(TIMER2_COMPB_vect)          // timer compare interrupt service routine
{
     if(MOTORDUTYRIGHT != 100)         //set motor output BIN1, BIN2 to low
     { 
         digitalWrite(BIN1PIN, 0);
         digitalWrite(BIN2PIN, 0);
         
         OCR2B = MOTORDUTYRIGHT*255/100;            // set compare registers to duty
     }     
}

ISR(TIMER2_OVF_vect)        // interrupt service routine Timer 2 for motor control
{
    int dur;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      dur = MOTORONDURATION;  
    }
    if(dur <= 0)
    {
//       AIN1 = false;
//       AIN2 = false;
//       BIN1 = false;
//       BIN2 = false;
       
       AIN1 = true;
       AIN2 = true;
       BIN1 = true;
       BIN2 = true;
    }
    
    boolean getRunForever;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        getRunForever = RUNFOREVER;
    }
    
    int adjustDuration = MOTORONDURATIONSET/8;
    float adjustThresholdFactor = 0.8;
    int timeToMidDuration;
    float adjustmentFactor = 0;
    
    if(getRunForever == false)    // if  motor is not running indefinitely
    {        
          if(MOTORONDURATIONSET -  MOTORONDURATION < adjustDuration && MOTORONDURATION > MOTORONDURATIONSET/2)    // in 1st half of duration
          {
              adjustmentFactor = ((float)(MOTORONDURATIONSET - MOTORONDURATION))/(float)adjustDuration;  // factor to ramp up the speed
              
              if(adjustmentFactor < adjustThresholdFactor)      //minimum threshold factor for ramp
              {
                  motorDutyEase(MOTORDUTYLEFTSET, MOTORDUTYRIGHTSET, adjustThresholdFactor);
              }
              else
              {
                  motorDutyEase(MOTORDUTYLEFTSET, MOTORDUTYRIGHTSET, adjustmentFactor);
              }
          }
          else if(MOTORONDURATION < adjustDuration && MOTORONDURATION < MOTORONDURATIONSET/2)                   // in 2nd half of duraiton
          {
              adjustmentFactor = (float)1 - ((float)(MOTORONDURATION) /(float)adjustDuration);  // factor to ramp down the speed
              
              if(adjustmentFactor < adjustThresholdFactor)    //minimum threshold factor for ramp
              {
                  motorDutyEase(0, 0, adjustThresholdFactor);
              }
              else
              {
                  motorDutyEase(0, 0, adjustmentFactor); 
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
        
        if(MOTORONDURATION > 0)
        {
          MOTORONDURATION = MOTORONDURATION - 1;  //decrease timer for motor on duration 
        }
    }
    else                          // if motor is running forever
    {
        motorDutyEase(MOTORDUTYLEFTSET, MOTORDUTYRIGHTSET, 1);    //speed up
    }

     TCNT2 = 0;       // preload timer range 0 - 255 counting up
   
     if(MOTORDUTYLEFT != 0)
     {   
       if(AIN1 == true)      //left motor AIN1
       {
          digitalWrite(AIN1PIN, 1);
       }
       else
       {
          digitalWrite(AIN1PIN, 0);
       }
       
       if(AIN2 == true)     //left motor AIN2
       {
          digitalWrite(AIN2PIN, 1);
       }
       else
       {
          digitalWrite(AIN2PIN, 0);
       }
     }
     
    if(MOTORDUTYRIGHT != 0) 
    { 
       if(BIN1 == true)     //right motor BIN1
       {
          digitalWrite(BIN1PIN, 1);
       }
       else
       {
          digitalWrite(BIN1PIN, 0);
       }
       
       if(BIN2 == true)     //right motor BIN2
       {
          digitalWrite(BIN2PIN, 1);
       }
       else
       {
          digitalWrite(BIN2PIN, 0);
       }  
    }
}

void loop()  { 

    //turn 90 degrees left and drive forward distance of 4000 (arbitrary length) with full modulation
//    motorDirDist(-10, 0, 70);
    
    delay(1000);
    
//    motorDirDist(180, 0, 75);
//    motorDirDist(-90, 0, 75);
//    motorDirDist(90, 0, 75);
//    motorDirDist(-90, 0, 75);


//      motorDirDist(0, 3000, 80);
      
//    
    motorStartRotate(true, 75);
//
    delay(1000);
//    
    motorStop();
//    
//    motorStartRotate(true, 50);
//    
//    delay(450);
//    
//    motorStop();
    
    //don't turn and start driving forward indefinitely with full modulation
    
//    motorStart(true, 90, 90);
    
//    delay(3000);
//    
//    motorDutySet(50, 50);
//    
//    delay(5000);
//    
//    motorStop();
    
// 
//    motorStop();
//    
//    motorStart(false, 75, 75);
//    
//    delay(250);
// 
//    motorStop();
//    
//    motorStart(false, 75, 75);
//    
//    delay(350);
//    
//    motorStop();
    
//    motorStart(true, 50, 100);
//    
//    delay(2000);
//    
//    motorStop();
   
    //let it drive for some time
//    delay(500);

    //stop driving
//    motorStop();

//    delay(2000);
    
//    motorDirDist(0, -2000, 70);
//    motorDirDist(0, -3000, 80);
    
    //end
    while(1)
    {
       int f = 6;
    }
}

void motorDir(int dir, int modulation)  // internal helper function for turning the robot
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

void motorStartRotate(boolean left, int modulation)  // public function for rotating robot until motorStop is called
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

/* motorDirDist
  dir = [-180,180]
  dist = [-5000, 5000] //TDB
  modulation = [1, 100]
*/
int motorDirDist(int dir, int dist, int modulation)  //public function for turning robot and move a certain distance
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

/* motorDirStart
  dir = [-180,180]
  forward = [true/false]
  modulation = [1, 100]
*/
int motorStart(boolean forward,int modulationLeft, int modulationRight)  //public function for moving indefinitely
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

void motorDutyEase(int left, int right, float adjustFactor)    // internal helper function for PWM duty easing using duty setpoints
{
  
     int currentLeft = MOTORDUTYLEFT;
     int currentRight = MOTORDUTYRIGHT;
     
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

void motorDutySet(int left, int right)    // function for PWM duty setpoint
{
    MOTORDUTYLEFTSET = left;
    MOTORDUTYRIGHTSET = right;
}

void motorStop()  //public function for stopping the robot
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
