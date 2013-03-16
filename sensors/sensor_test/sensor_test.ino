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
   Serial.begin(9600); 
}
//
void loop()
{ 
//mySensor.rotate90Left();
    while(1)
    {
      delay(200);
//      if(mySensor.getFrontGrid()>1)
//        mySensor.goStraightTile(1);
//      else
//        mySensor.rotate90Left();
//      mySensor.getLeftGrid()
//      mySensor.getRightGrid()
      Serial.println(mySensor.getFrontGrid());
    }
    
    
    
//manual hard code test starts here    
//    mySensor.goStraightTile(3);
//    mySensor.rotate90Right();
//    mySensor.goStraightTile(3);
//    mySensor.rotate90Right();
//    mySensor.goStraightTile(3);
//    mySensor.rotate90Right();
//    mySensor.goStraightTile(3);
//    mySensor.goStraightTile(1);
    
//    while(1)
//    { 
//       int  h= 0;
//    }
//manual hard code test ends here 
}

