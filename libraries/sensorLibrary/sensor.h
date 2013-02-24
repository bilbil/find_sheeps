#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

#include <motor.h>

#define GRIDSENS_BACK_RIGHT  7
#define GRIDSENS_BACK_LEFT   6
#define GRIDSENS_FRONT_RIGHT 4
#define GRIDSENS_FRONT_LEFT  5	
	
class sensor
{
  public:
    sensor();
	
	static void goStraightTile(int val); //user specified numver of tiles to move
	static void rotate90Left();
	static void rotate90Right();
	
	static boolean BACKALIGN;
	static boolean EVALCROSSTILE;
	static int DEBOUNCE;
	volatile static boolean ONTILE;			// flag for on entering each tile cross
	static boolean KEEPSTRAIGHT;	//flag for keeping robot on straight
	volatile static boolean STAY;			//flag for telling robot to stay on current tile cross
	static int GRIDCOUNT;			//counter to count to user specified count
	volatile static int GOGRIDCOUNT; 		//user specified tile count
	static void setStraight(boolean val);	
	static void process();
	static boolean getFrontLeftWhite();
	static boolean getFrontRightWhite();
	static boolean getBackLeftWhite();
	static boolean getBackRightWhite();

  private:
 
};

#endif