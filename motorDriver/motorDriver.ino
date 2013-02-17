/*
 PWM Motor Control Logic

  Using Timer 0 and 2 for PWM output to motor driver IC
  
  Typical settings used:
  
  Timer 0:
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS00) | _BV(CS01);
  OCR0A = 100;  //pin 6  -> AIN2  (left motor)
  OCR0B = 100;  //pin 5  -> AIN1  (left motor)
  
  Timer 2:
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 100;   //pin 11 -> BIN2  (right motor)
  OCR2B = 100;  //pin 3   -> BIN1  (right motor)
  
  16 Mhz/ 64 (prescaler)/256 = 976.56 Hz PWM currently 
 */
 
 //motor PWM output pins
 #define AIN1PIN 10
 #define AIN2PIN 11
 #define BIN1PIN 12
 #define BIN2PIN 13
 
 volatile boolean AIN1 = false;
 volatile boolean AIN2 = false;
 volatile boolean BIN1 = false;
 volatile boolean BIN2 = false;
 
 volatile boolean PWMHIGH = 0;  //initial state of PWM output
 
 volatile int PWMDUTY = 75;     //motor PWM duty
 
 volatile int MOTORONDURATION = 0;  //motor PWM on duration
 
 int dummy = 0;
 
void setup()  {
  
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

  TCNT2 = 0;            // preload timer 16MHz/64
  TCCR2B = 0x02;        // 8 prescaler 3.906 kHz
  TIMSK2 |= (1 << TOIE2);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  
} 

ISR(TIMER2_OVF_vect)        // interrupt service routine for motor control
{
  if(MOTORONDURATION <= 0)
  {
    
  }
  else
  {
      if(PWMHIGH == 0)  //turn PWM to high
      {
         
         TCNT2 = (100 - PWMDUTY)*255/100;       //adjust to PWM duty cycle using counters
           
         if(PWMDUTY != 0)
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
         
         PWMHIGH = 1;
      }
      else  //turn PWN to low
      {
         TCNT2 = PWMDUTY*255/100;       // preload timer range 0 - 255 counting up
         
         if(PWMDUTY != 100)
         {     
             digitalWrite(AIN1PIN, 0);
             digitalWrite(AIN2PIN, 0);
             digitalWrite(BIN1PIN, 0);
             digitalWrite(BIN2PIN, 0);
         }
         PWMHIGH = 0;
      }   
      MOTORONDURATION--;  //decrease timer for motor on duration 
  }
};

void loop()  { 

  int error = 0;
  
//  pinMode(3, OUTPUT);
//  pinMode(11, OUTPUT);
//  
//  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
//  TCCR2B = _BV(CS22);
//  OCR2A = 0;   //pin 11
//  OCR2B = 0;  //pin 3
  
//  pinMode(5, OUTPUT);
//  pinMode(6, OUTPUT);
//  
//  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
//  TCCR0B = _BV(CS00) | _BV(CS01);
////  TCCR0B = _BV(CS02);
//  OCR0A = 0;  //pin 6
//  OCR0B = 0;  //pin 5

//  testing driver logic
//  motorDirDist(int dir, int dist, int modulation);

    //turn 90 degrees left and drive forward distance of 4000 (arbitrary length) with full modulation
//    motorDirDist(-90, 100, 100);
    
          AIN1 = true;
          AIN2 = false;   
          
          BIN1 = false;
          BIN2 = true; 
          
          MOTORONDURATION = 6000;
          
//          interrupts(); 
          while(MOTORONDURATION > 0)
          {
            dummy++;
          }
          
          delay(1000);
         
          AIN1 = true;
          AIN2 = false;   
          
          BIN1 = true;
          BIN2 = false; 
          
          MOTORONDURATION = 6000;
          
          while(MOTORONDURATION > 0)
          {
            dummy++;
          }
          
    //turn 90 degrees right and drive backward distance of 2000 (arbitrary length) with full modulation
//    motorDirDist(90, -2000, 90);
    
    //don't turn and start driving forward indefinitely with full modulation
//    motorDirStart(0, true, 100);

    //let it drive for 5 seconds
//    delay(5000);

    //stop driving
//    motorStop();
       

//    AIN1 = true;   
//    AIN2 = false; 
//    BIN1 = true;   
//    BIN2 = true; 
//      MOTORONDURATION = 48000;
      
    //end
    while(1)
    {
       int f = 6;
    }
}

void motorDir(int dir, int modulation)  // internal helper function for turning the robot
{
      // initialize turning duration according to direction and modulation; yet to be determined
      int durationTurn = dir * 6000; 
      unsigned long timeStart;
      unsigned long timeEnd;
      int dummy = 0;
      
      if(durationTurn < 0)
      {
          //left turn

          AIN1 = false;
          AIN2 = true;   
          
          BIN1 = true;
          BIN2 = false; 
          
          MOTORONDURATION = -1 * durationTurn;  
    
//          OCR0A = modulation * 255 / 100;  //pin 6  -> AIN2 on (left motor)
//          OCR0B = 0;                       //pin 5  -> AIN1 off (left motor)
//          
//          OCR2A = 0;                       //pin 6  -> BIN2 off (right motor)
//          OCR2B = modulation * 255 / 100;  //pin 5  -> BIN1 on (right motor)
//          
//          timeStart = millis();
//          timeEnd = millis();
//          while(timeEnd - timeStart < durationTurn)
//          {
//            dummy++;
//            timeEnd = millis();
//          }
//          while(durationTurn < 0)
//          {
//              durationTurn++;
//              delay(1);  //1ms delay for now
//          }
      }      
      else if(durationTurn > 0)
      {
          //right turn
          AIN1 = true;
          AIN2 = false;   
          
          BIN1 = false;
          BIN2 = true; 
          
          MOTORONDURATION = durationTurn;

//          OCR0A = 0;                       //pin 6  -> AIN2  off (left motor)
//          OCR0B = modulation * 255 / 100;  //pin 5  -> AIN1  on (left motor)
//  
//          OCR2A = modulation * 255 / 100;  //pin 11 -> BIN2  on (right motor)
//          OCR2B = 0;                       //pin 3  -> BIN1  off (right motor)
//          
//          timeStart = millis();
//          timeEnd = millis();
//          while(timeEnd - timeStart < durationTurn)
//          {
//            dummy++;
//            timeEnd = millis();
//          }
          
//          while(durationTurn > 0)
//          {
//             durationTurn--;
////             delay(1);  //1ms delay for now
//          }
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
      int durationForward = dist * 6000;
      unsigned long timeStart;
      unsigned long timeEnd;
      int dummy = 0;

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
      
      do
      {
         dummy++;
      }while(MOTORONDURATION > 0);    
          
      //move robot straight
      if(durationForward > 0)
      {
          AIN1 = true;
          AIN2 = false;   
          
          BIN1 = true;
          BIN2 = false; 
          
          MOTORONDURATION = durationForward;  
          
//          //move forward 
//          OCR0A = 0;                       //pin 6  -> AIN2  off (left motor)
//          OCR0B = modulation * 255 / 100;  //pin 5  -> AIN1  on (left motor)
//  
//          OCR2A = 0;                       //pin 11 -> BIN2  off (right motor)
//          OCR2B = modulation * 255 / 100;  //pin 3  -> BIN1  on (right motor)
//             
//          timeStart = millis();
//          timeEnd = millis();
//          while(timeEnd - timeStart < durationForward)
//          {
//            dummy++;
//            timeEnd = millis();
//          }   
                  
//          while(durationForward > 0)  //loop for finite distance
//          {
//            durationForward--;
//            delay(1);  //1ms delay for now
//          }
          
      }
      else if(durationForward < 0)
      {    
          //move backward

          AIN1 = false;
          AIN2 = true;   
          
          BIN1 = false;
          BIN2 = true; 
          
          MOTORONDURATION = -1* durationForward; 

//          OCR0A = modulation * 255 / 100;   //pin 6  -> AIN2  on (left motor)
//          OCR0B = 0;                        //pin 5  -> AIN1  off (left motor)
//  
//          OCR2A = modulation * 255 / 100;   //pin 11 -> BIN2  on (right motor)
//          OCR2B = 0;                        //pin 3  -> BIN1  off (right motor)
//          
//          timeStart = millis();
//          timeEnd = millis();
//          while(timeEnd - timeStart < durationForward)
//          {
//            dummy++;
//            timeEnd = millis();
//          }  
          
//          while(durationForward < 0)  //loop for finite distance
//          {
//            durationForward++;
//            delay(1);  //1ms delay for now
//          }
      }
      do{
         dummy++; 
      }while(MOTORONDURATION > 0);
     
//      motorStop();
      return 0;
}

/* motorDirStart
  dir = [-180,180]
  forward = [true/false]
  modulation = [1, 100]
*/
int motorDirStart(int dir, boolean forward,int modulation)  //public function for turning the robot and start moving indefinitely
{
      //set modulation 0-100
      if (modulation < 0 || modulation > 100)
      {
          return -1;
      }
      
      //dir 0: no turn, 180: 180 degrees to left, -180: 180 degrees to right
      if(dir < -180 || dir > 180)
      {
          return -1;
      }
      
      motorDir(dir, modulation); //turn the robot
      
      //move robot straight indefinitely
      if(forward)
      {
          //move forward 
          OCR0A = 0;                       //pin 6  -> AIN2  off (left motor)
          OCR0B = modulation * 255 / 100;  //pin 5  -> AIN1  on (left motor)
  
          OCR2A = 0;                       //pin 11 -> BIN2  off (right motor)
          OCR2B = modulation * 255 / 100;  //pin 3  -> BIN1  on (right motor)
      }
      else
      {    
          //move backward
          OCR0A = modulation * 255 / 100;   //pin 6  -> AIN2  on (left motor)
          OCR0B = 0;                        //pin 5  -> AIN1  off (left motor)
  
          OCR2A = modulation * 255 / 100;   //pin 11 -> BIN2  on (right motor)
          OCR2B = 0;                        //pin 3  -> BIN1  off (right motor)
      }
      
      return 0;
}

int motorStop()  //public function for stopping the robot
{
//      OCR0A = 0;  //pin 6  -> AIN2  off (left motor)
//      OCR0B = 0;  //pin 5  -> AIN1  off (left motor)
//  
//      OCR2A = 0;  //pin 11 -> BIN2  off (right motor)
//      OCR2B = 0;  //pin 3  -> BIN1  off (right motor)
//      
      MOTORONDURATION = 0;
      
      AIN1 = false;
      AIN2 = false;   
      
      BIN1 = false;
      BIN2 = false; 
      
      return 0;
}
