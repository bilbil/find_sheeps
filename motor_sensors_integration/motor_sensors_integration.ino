#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

//motor myMotor;
sensor mySensor; 

boolean led = true;
int led_pin = 8;
int grid = 0;
float test = 0;
  
void setup()
{
//  myMotor = motor();    // setup motor settings
  mySensor = sensor();  // setup grid sensor settings
   
   // initialize serial communications at 9600 bps:
//   Serial.begin(9600); 
}

void startdelay()
{
  delay(2000);
}

void loop()
{ 
//  digitalWrite(led_pin, led);
  
//    mySensor.goStraightTile(5);
//    mySensor.rotate180Right();
//    mySensor.goStraightTile(5);
    
//    while(1)
//    { 
//      int g = 0;
//    }
  while(1)
  {    
//    delay(3000);

      mySensor.updateDistances();
//      delay(500);
//      Serial.print("front = " );          
    
      test = (float)5/(float)1024*(float)mySensor.distance_front;
      
      if(test>1.9)
        grid = 0;
      else if(test>1.3)
        grid = 1;
      else if(test>0.83)
        grid = 2;
      else if(test>0.63)
        grid = 3;
      else if(test>0.54)
        grid = 4;
      else
        grid = 10;
//      Serial.println(test); 
      
      if(grid > 1)
      {
//        mySensor.test(1);
          mySensor.goStraightTile(1);
          delay(200);
      }
      else
      {
          mySensor.rotate90Left();
          delay(200);
      }

//      Serial.println(grid); 
  }
}

