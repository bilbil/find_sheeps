#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

#include <motor.h>

#define GRIDSENS_BACK_RIGHT  7
#define GRIDSENS_BACK_LEFT   6
#define GRIDSENS_FRONT_RIGHT 5
#define GRIDSENS_FRONT_LEFT  4	

#define TURNLEFT 0
#define TURNRIGHT 1
#define STAY 2
#define STRAIGHT 3
#define BACKUP 4
	
class sensor
{
  public:
    sensor();
	
	static void goStraightTile(int val); //user specified numver of tiles to move
	static void rotate90Left();
	static void rotate90Right();
	static void rotate180Right();
	static void rotate180Left();
	
	static int DEBOUNCE;
	volatile static int ACTION;				//state of the robot
	volatile static int GOGRIDCOUNT; 		//user specified tile count
	static void process();
	static boolean getFrontLeftWhite();
	static boolean getFrontRightWhite();
	static boolean getBackLeftWhite();
	static boolean getBackRightWhite();

  private:
 
};

#endif