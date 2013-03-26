#include <motor.h>
#include <sensor.h>
#include <beacon.h>
#include <sheep_interrupts.h>
#include <Arduino.h>

sensor mySensor;
Beacon beacon;

boolean led = true;
int grid = 0;
float test = 0;

int frontBeacon = 0;
int backBeacon = 0;
int leftBeacon = 0;
int rightBeacon = 0;
  
void setup()
{
  mySensor = sensor();  // setup grid sensor settings
  beacon.init();
   
   // initialize serial communications at 9600 bps:
//   Serial.begin(9600); 
}

void startdelay()
{
  delay(2000);
}

void loop()
{ 
    noInterrupts();
    int counter = 0;
    while (counter < 3) {
    frontBeacon = beacon.getFrontBeacon();
    backBeacon = beacon.getBackBeacon();
    leftBeacon = beacon.getLeftBeacon();
    rightBeacon = beacon.getRightBeacon();
    if (frontBeacon || backBeacon || rightBeacon || leftBeacon == 1)  {
        break;
      }
      counter ++;
    }
    counter = 0;
    interrupts();
    
    if (leftBeacon == 1) {
      mySensor.rotate90Left();
    }
    if (rightBeacon == 1) {
      mySensor.rotate90Right();
    }
    if (frontBeacon == 1) {
      mySensor.goStraightTile(1);
    }
    
    while(mySensor.ACTION != STAY)
    {
      int dummy = 0; 
    }
//manual hard code test ends here

}

