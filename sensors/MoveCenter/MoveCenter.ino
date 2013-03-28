#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

sensor mySensor; 
  
void setup()
{
  // setup grid sensor settings
  mySensor = sensor();
  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop()
{
  mySensor.center(true);
  while(1)
  { 
     int  h= 0;
  }
}

