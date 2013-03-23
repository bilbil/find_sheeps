#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

#include <motor.h>

#define GRIDSENS_BACK_RIGHT  7
#define GRIDSENS_BACK_LEFT   6

#define GRIDSENS_FRONT_RIGHT_WIDE  4
#define GRIDSENS_FRONT_LEFT_WIDE   7

#define GRIDSENS_FRONT_RIGHT 5
#define GRIDSENS_FRONT_LEFT  4

#define TURNLEFT 0
#define TURNRIGHT 1
#define STAY 2
#define STRAIGHT 3
#define BACKUP 4
#define ADJUST 5
	
#define DISTANCESENS_FRONT	A0
#define DISTANCESENS_LEFT	A1
#define DISTANCESENS_RIGHT	A2
#define DISTANCESENS_BACK	A3

#define SPEED_HIGH_STRAIGHT 50
#define SPEED_HIGH_TURN	 65
#define SPEED_LOW_STRAIGHT 40
#define SPEED_LOW_TURN 40	

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
	static int DEBOUNCE2;
	volatile static int ACTION;				//state of the robot
	volatile static int GOGRIDCOUNT; 		//user specified tile count
	volatile static int ERRORCOUNT;
	
	static int SPEED_STRAIGHT;
	static int SPEED_TURN;
	
	static void process();
	static boolean getFrontLeftWhite();
	static boolean getFrontRightWhite();
	static boolean getBackLeftWhite();
	static boolean getBackRightWhite();
	static boolean getFrontLeftWideWhite();
	static boolean getFrontRightWideWhite();
	static int getFrontGrid();
	static int getLeftGrid();
	static int getRightGrid();
	static int getBackGrid();
	volatile static int distance_front;
	volatile static int distance_left;
	volatile static int distance_right;
	volatile static int distance_back;
	volatile static int grid;
	
	static void test(int val);
	static int getFrontTest();
	
	static bool moveCenter(bool right);
	volatile static bool frontLeft;
	volatile static bool frontRight;
	volatile static bool backLeft;
	volatile static bool backRight;
	
	static void setSpeedHigh(bool val);
	
  private:
 
};

#endif