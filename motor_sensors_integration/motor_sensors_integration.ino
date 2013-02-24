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

  mySensor.goStraightTile(2);
  
  delay(1000);
  
  mySensor.rotate90Left();
  
  delay(1000);
  
  mySensor.goStraightTile(2);
  
  delay(2000);
//  
//  delay(500);
//  
//  mySensor.rotate90Left();
//  
//  delay(500);
  
  while(1)
  {
     int g = 0; 
  }
}

