#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

//motor myMotor;
sensor mySensor; 

boolean led = true;
int led_pin = 8
;
  
void setup()
{
//  myMotor = motor();    // setup motor settings
  mySensor = sensor();  // setup grid sensor settings
   
   // initialize serial communications at 9600 bps:
   Serial.begin(9600); 
}

void startdelay()
{
  delay(2000);
}

void loop()
{ 
//  digitalWrite(led_pin, led);
  
//  mySensor.goStraightTile(3);
//  
//  mySensor.rotate90Left();
    
  mySensor.updateDistances();
  delay(50);
  Serial.print("front = " );          

  float test = (float)5/(float)1024*(float)mySensor.distance_front;  
  Serial.println(test);   

  delay(500);
  
//  
//  mySensor.goStraightTile(5);
//  
//  mySensor.rotate90Left();
//  
//  mySensor.goStraightTile(5);
//  
//  mySensor.rotate90Left();
//  
//  mySensor.goStraightTile(5); 
//  
//  
//  mySensor.rotate90Right();
  
//  mySensor.goStraightTile(4);
//  
//  mySensor.rotate180Right();
//  
//  mySensor.goStraightTile(4);
//  
//  led = !led;
}

