#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

#include <motor.h>

// #define GRIDSENS_BACK_RIGHT  7	//original
// #define GRIDSENS_BACK_LEFT   6

// #define GRIDSENS_FRONT_RIGHT 5
// #define GRIDSENS_FRONT_LEFT  4

//pcb version start
#define GRIDSENS_BACK_RIGHT  1	//PB1
#define GRIDSENS_BACK_LEFT   2	//PB2

#define GRIDSENS_FRONT_RIGHT 4	//PB4
#define GRIDSENS_FRONT_LEFT  5	//PB5
//pcb version end

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

#define SPEED_HIGH_STRAIGHT 38
#define SPEED_HIGH_TURN	 42
#define SPEED_LOW_STRAIGHT 45
#define SPEED_LOW_TURN 45	
#define SPEED_SLOW_STOP	 28

#define SPEED_LINE_FOLLOW 30

#define SPEED_ADJUST_MIN 25
#define SPEED_ADJUST_MAX 35

#define SPEED_ADJUST_LAST_L 2
#define SPEED_ADJUST_LAST_R 1
#define SPEED_ADJUST_LAST_M 0

#define TURN_BACK_ON_WHITE 2
#define TURN_FRONT_ON_WHITE 1
#define TURN_NOT_INIT 0

#define DEBOUNCE_STAY_COUNT 50

class sensor
{ 
  public:
    sensor();

	static void sensorEnable(bool val);

	static void goStraightTile(int val); //user specified numver of tiles to move
	static void rotate90Left();
	static void rotate90Right();
	static void rotate180Right();
	static void rotate180Left();
	static void adjust();
	
	static int DEBOUNCE;
	static int DEBOUNCE2;
	static int DEBOUNCE_STAY;
	
	volatile static int ACTION;				//state of the robot
	volatile static int GOGRIDCOUNT; 		//user specified tile count
	volatile static int ERRORCOUNT;
	volatile static int ERRORCOUNT2;
	
	volatile static int ADJUST_LAST_STATE;
	
	static int SPEED_STRAIGHT;
	static int SPEED_TURN;
	
	static bool startOnWhite;	//flag for not miscounting the first tile when going forward
	static bool turnStartOnWhite;	//flag for not miscounting when turning
	static int turnSensorStartCondition;
	
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