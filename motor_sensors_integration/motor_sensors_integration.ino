#include <motor.h>
#include <Arduino.h>

motor myMotor;

void setup()
{
  myMotor = motor();
}

void loop()
{ 
    delay(2000);
  
    myMotor.motorDirDist(90, 5000, 90);
    
//    myMotor.motorStartRotate(70, 70);
//    
//    delay(3000);
//    
//    myMotor.motorStop();
//    
//    myMotor.motorStart(true,40,40);
//
//    delay(2000);
////    
//    myMotor.motorStop();
    
    while(1)
    {
       int h = 0; 
    }
}
