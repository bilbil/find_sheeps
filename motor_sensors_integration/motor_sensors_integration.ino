#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

//motor myMotor;
sensor mySensor; 

boolean led = true;
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

// //obstacle detection
//    while(1)
//    {    
//      double b = 0;
//      
//      for(int h = 0; h < 10; h++)
//      {
//        int a;
//        a = mySensor.getFrontGrid();
//         b = b + (double) a;
//      }
//      
//      b = round(b/10);
//      
//  //      Serial.println(b); 
//  
//      if(b>1)
//      {
//          mySensor.goStraightTile(1);    
//      }
//        delay(1000);
//    }
//    
//    mySensor.test(0);
//manual hard code test starts here    
    mySensor.goStraightTile(2);
    mySensor.rotate90Left();
    mySensor.goStraightTile(2);
    mySensor.rotate90Right();
//    mySensor.rotate90Right();
//    mySensor.rotate90Left();
//    mySensor.goStraightTile(4);
//    mySensor.rotate90Left();
//    mySensor.goStraightTile(4);
//    mySensor.rotate90Left();
//    mySensor.goStraightTile(4);
//    mySensor.goStraightTile(3);
//    mySensor.rotate180Left();
//    mySensor.goStraightTile(3);
//    mySensor.goStraightTile(1);
    
    while(1)
    { 
       int  h= 0;
    }
//manual hard code test ends here 
}

