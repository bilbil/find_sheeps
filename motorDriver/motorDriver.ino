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
 
void setup()  { 
} 

void loop()  { 

  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 200;   //pin 11
  OCR2B = 200;  //pin 3
  
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS00) | _BV(CS01);
//  TCCR0B = _BV(CS02);
  OCR0A = 0;  //pin 6
  OCR0B = 0;  //pin 5

//  testing driver logic
//  motorDirDist(int dir, int dist, int modulation);

    //turn 90 degrees left and drive forward distance of 4000 (arbitrary length) with full modulation
//    motorDirDist(-90, 4000, 100);
    
    //turn 90 degrees right and drive backward distance of 2000 (arbitrary length) with full modulation
//    motorDirDist(90, -2000, 90);
    
    motorDirDist(90, 4000, 50);
    
    //don't turn and start driving forward indefinitely with full modulation
//    motorDirStart(0, true, 100);

    //let it drive for 5 seconds
//    delay(5000);

    //stop driving
//    motorStop();
    
    //end
    while(1)
    {
       int f = 6;
    }
}

void motorDir(int dir, int modulation)  // internal helper function for turning the robot
{
      // initialize turning duration according to direction and modulation; yet to be determined
      int durationTurn = dir * 100 / modulation * 20; 
      
      if(durationTurn < 0)
      {
          //left turn
          OCR0A = modulation * 255 / 100;  //pin 6  -> AIN2 on (left motor)
          OCR0B = 0;                       //pin 5  -> AIN1 off (left motor)
          
          OCR2A = 0;                       //pin 6  -> BIN2 off (right motor)
          OCR2B = modulation * 255 / 100;  //pin 5  -> BIN1 on (right motor)
          
          while(durationTurn < 0)
          {
              durationTurn++;
              delay(1);  //1ms delay for now
          }
      }      
      else if(durationTurn > 0)
      {
          //right turn
          OCR0A = 0;                       //pin 6  -> AIN2  off (left motor)
          OCR0B = modulation * 255 / 100;  //pin 5  -> AIN1  on (left motor)
  
          OCR2A = modulation * 255 / 100;  //pin 11 -> BIN2  on (right motor)
          OCR2B = 0;                       //pin 3  -> BIN1  off (right motor)
          
          while(durationTurn > 0)
          {
             durationTurn--;
             delay(1);  //1ms delay for now
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
      
      //initialize duration for going straight; exact formula yet to be determined
      int durationForward = dist / modulation * 100;

      motorDir(dir, modulation);  //turn the robot
     
      //move robot straight
      if(durationForward > 0)
      {
          //move forward 
          OCR0A = 0;                       //pin 6  -> AIN2  off (left motor)
          OCR0B = modulation * 255 / 100;  //pin 5  -> AIN1  on (left motor)
  
          OCR2A = 0;                       //pin 11 -> BIN2  off (right motor)
          OCR2B = modulation * 255 / 100;  //pin 3  -> BIN1  on (right motor)
          
          while(durationForward > 0)  //loop for finite distance
          {
            durationForward--;
            delay(1);  //1ms delay for now
          }
          
      }
      else if(durationForward < 0)
      {    
          //move backward
          OCR0A = modulation * 255 / 100;   //pin 6  -> AIN2  on (left motor)
          OCR0B = 0;                        //pin 5  -> AIN1  off (left motor)
  
          OCR2A = modulation * 255 / 100;   //pin 11 -> BIN2  on (right motor)
          OCR2B = 0;                        //pin 3  -> BIN1  off (right motor)
          
          while(durationForward < 0)  //loop for finite distance
          {
            durationForward++;
            delay(1);  //1ms delay for now
          }
      }
      
      motorStop();
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
      OCR0A = 0;  //pin 6  -> AIN2  off (left motor)
      OCR0B = 0;  //pin 5  -> AIN1  off (left motor)
  
      OCR2A = 0;  //pin 11 -> BIN2  off (right motor)
      OCR2B = 0;  //pin 3  -> BIN1  off (right motor)
      
      return 0;
}
