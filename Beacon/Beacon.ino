
#include <beacon.h>

float rawAnalog;
int frontBeacon = 0;
int backBeacon = 0;
int leftBeacon = 0;
int rightBeacon = 0;

Beacon beacon;

void setup()
{ 
  beacon.init();
  Serial.begin(9600);
  Serial.setTimeout(50);
}

void loop()
{
  frontBeacon = beacon.getFrontBeacon();
  backBeacon = beacon.getBackBeacon();
  leftBeacon = beacon.getLeftBeacon();
  rightBeacon = beacon.getRightBeacon();
  
  delay(100);
  
  Serial.print("Front Beacon:");
  Serial.println(frontBeacon);
  Serial.print("Back Beacon:");
  Serial.println(backBeacon);
  Serial.print("Right Beacon:");
  Serial.println(rightBeacon);
  Serial.print("Left Beacon:");
  Serial.println(leftBeacon);
}
