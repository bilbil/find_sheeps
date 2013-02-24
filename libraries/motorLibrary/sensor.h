#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

#include <motor.h>

#define GRIDSENS_BACK_RIGHT  6
#define GRIDSENS_BACK_LEFT   7
#define GRIDSENS_FRONT_RIGHT 4
#define GRIDSENS_FRONT_LEFT  5	
	
class sensor
{
  public:
    sensor();
	
	static volatile boolean KEEPSTRAIGHT;
	
	static void setStraight(boolean val);
	
	static void keepStraight();
	
	
	
	static boolean getFrontLeftWhite();
	static boolean getFrontRightWhite();
	static boolean getBackLeftWhite();
	static boolean getBackRightWhite();
	
  private:
 
};

#endif