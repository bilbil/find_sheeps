#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

//motor myMotor;
sensor mySensor; 
  
void setup()
{
//  myMotor = motor();    // setup motor settings
  mySensor = sensor();  // setup grid sensor settings
}

void loop()
{ 
  
  delay(2000);

//  mySensor.goStraightTile(2);
//  
//  mySensor.rotate90Left();
//  
//  mySensor.goStraightTile(2);
//  
//  mySensor.rotate90Right();
  
  mySensor.goStraightTile(4);
  
  mySensor.rotate180Right();
  
  mySensor.goStraightTile(4);
  
  while(1)
  {
     int g = 0; 
  }
}

