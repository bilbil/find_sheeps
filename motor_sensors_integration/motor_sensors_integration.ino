#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

motor myMotor;
sensor mySensor; 
  
void setup()
{
  myMotor = motor();    // setup motor settings
  mySensor = sensor();  // setup grid sensor settings
}

void loop()
{ 
  
  delay(1000);
  mySensor.setStraight(true);
  delay(5000);
  mySensor.setStraight(false);
  
  while(1)
  {
     int g = 0; 
  }
}

