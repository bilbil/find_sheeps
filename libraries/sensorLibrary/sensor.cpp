/*
Integrated module with motor code for navigating the robot on the grid
goStraightTile: robot goes forward a given number of tiles
rotate90Left: robot performs a 90 degree turn when aligned at the center of a cross
rotate90Right: robot performs a 90 degree turn when aligned at the center of a cross
*/

#include <motor.h>
#include "Arduino.h"
#include <sensor.h>

motor myMotor;

sensor::sensor()	//constructor
{
	// pinMode(GRIDSENS_BACK_RIGHT, INPUT);	//grid sensor pin out
	// pinMode(GRIDSENS_BACK_LEFT, INPUT);
	pinMode(GRIDSENS_FRONT_RIGHT_WIDE, INPUT);	//grid sensor pin out
	pinMode(GRIDSENS_FRONT_LEFT_WIDE, INPUT);
	pinMode(GRIDSENS_FRONT_RIGHT, INPUT);
	pinMode(GRIDSENS_FRONT_LEFT, INPUT);
	
	myMotor = motor();	//setup motor settings
}

volatile int sensor::ACTION = STAY;
volatile int sensor::GOGRIDCOUNT = 0;
int sensor::DEBOUNCE = 0;

volatile int sensor::distance_front = 0;
volatile int sensor::distance_left = 0;
volatile int sensor::distance_right = 0;
volatile int sensor::distance_back = 0;

void sensor::test(int val)
{
	if(val == 1)
	{
		motor::motorStart(true,90,90);
	}
	else
	{
		motor::motorStop();
	}
}

void sensor::goStraightTile(int val)
{
	int currentAction;
	do
    {   
		currentAction = ACTION;
    }while(currentAction != STAY);
	
	ACTION = STRAIGHT;
	DEBOUNCE = 0;
	GOGRIDCOUNT = val;
	
	do
    {   
		currentAction = ACTION;
    }while(currentAction != STAY);
}

void sensor::rotate180Left()
{
	rotate90Left();
	delay(100);
	rotate90Left();
}

void sensor::rotate180Right()
{
	rotate90Right();
	delay(100);
	rotate90Right();
}

void sensor::rotate90Left()
{
	int currentAction;
	do
    {   
		currentAction = ACTION;
    }while(ACTION != STAY);
	
	ACTION = TURNLEFT;
	DEBOUNCE = 0;
	
	do
    {   
		currentAction = ACTION;
    }while(ACTION != STAY);
}

void sensor::rotate90Right()
{
	int currentAction;
	do
    {   
		currentAction = ACTION;
    }while(currentAction != STAY);
	
	ACTION = TURNRIGHT;
	DEBOUNCE = 0;
	
	do
    {   
		currentAction = ACTION;
    }while(ACTION != STAY);
}

/* void sensor::process()
{			  
	if(ACTION == STRAIGHT)	// guide robot straight
	{
		  if (getFrontLeftWhite() == true && getFrontRightWhite() == true)
		  {
				if(getBackLeftWhite() == false && getBackRightWhite() == false)
				{
					if(DEBOUNCE > 1)	//debounce grid sensor to get accurate crossing
					{
						DEBOUNCE = 0;
						GOGRIDCOUNT--;	//count on entering each tile cross
						
						if(GOGRIDCOUNT <= 0)	// when robot passes a defined number of tile crosses
						{
							myMotor.motorStop();
							// ACTION = STAY;
							ACTION = BACKUP;
						}
						else
						{
							myMotor.motorStart(true,75,75);
						}
					}
					else
					{
						myMotor.motorStart(true,75,75);
					}
				}
				else
				{
					myMotor.motorStart(true, 60,60);
				}
				
		  }
		  else if (getFrontLeftWhite() == true && getFrontRightWhite() == false)
		  {
			  // myMotor.motorStart(true,65,0);
			  myMotor.motorStartRotate(false,60);
		  }
		  else if (getFrontLeftWhite() == false && getFrontRightWhite() == true)
		  {
			  // myMotor.motorStart(true,0,65);
			  myMotor.motorStartRotate(true,60);
		  }
		  else
		  {
			  myMotor.motorStart(true,60,60);
			  DEBOUNCE++;
		  }
	}
	else if(ACTION == TURNLEFT)
	{
		if(getFrontLeftWhite() == true)		
		{
			if(DEBOUNCE > 300)
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStartRotate(true, 60);
			}
		}
		else
		{
			motor::motorStartRotate(true, 60);
			DEBOUNCE++;
		}
	}
	else if(ACTION == TURNRIGHT)
	{
		if(getFrontRightWhite() == true)		
		{
			if(DEBOUNCE > 300)
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStartRotate(false, 60);
			}
		}
		else
		{
			motor::motorStartRotate(false, 60);
			DEBOUNCE++;
		}
	}
	else if(ACTION == BACKUP)
	{
		if(getBackLeftWhite() == true && getBackRightWhite() == true)
		{
			motor::motorStop();
			ACTION = STAY;
		}
		else
		{
			motor::motorStart(false, 40,40);
		}
	}
	else
	{
		motor::motorStop();
	}
} */

void sensor::process()
{			  
	// updateDistances();
	
	if(ACTION == STRAIGHT)	// guide robot straight
	{		
		  if (getFrontLeftWhite() == false && getFrontRightWhite() == false)
		  {
				if(getFrontLeftWideWhite() == false && getFrontRightWideWhite() == false)	//on black of cross
				{
					DEBOUNCE++;
				}
				myMotor.motorStart(true, 90,90);	
		  }
		  else if (getFrontLeftWhite() == true && getFrontRightWhite() == false)	//need to turn right
		  {
			  // myMotor.motorStart(true,65,0);
			  myMotor.motorStartRotate(false,75);
		  }
		  else if (getFrontLeftWhite() == false && getFrontRightWhite() == true)	// need to turn left
		  {
			  // myMotor.motorStart(true,0,65);
			  myMotor.motorStartRotate(true,75);
		  }
		  else	// both on white
		  {
				if(getFrontLeftWideWhite() == true && getFrontRightWideWhite() == true)	//going form black cross to past it
				{
					if(DEBOUNCE > 20)	//debounce grid sensor to get accurate crossing
					{
						DEBOUNCE = 0;
						GOGRIDCOUNT--;	//count on entering each tile cross
						
						if(GOGRIDCOUNT <= 0)	// when robot passes a defined number of tile crosses
						{
							myMotor.motorStop();
							ACTION = STAY;
							// ACTION = BACKUP;
						}
						else if(GOGRIDCOUNT > 1)	// normal speed if there is more grid tile to go
						{
							myMotor.motorStart(true,90,90);
						}
						else
						{
							myMotor.motorStart(true,70,70);	//slow down robot on last grid cross encounter
						}
					}
					else
					{
						myMotor.motorStart(true,90,90);
					}
				}
				else
				{
					myMotor.motorStart(true,90,90);
				}
		  }
	}
	else if(ACTION == TURNLEFT)
	{
		if(getFrontLeftWhite() == true)		
		{
			if(DEBOUNCE > 20)
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStartRotate(true, 80);
				// motor::motorStart(true,0,65);
			}
		}
		else
		{
			motor::motorStartRotate(true, 80);
			// motor::motorStart(true,0,65);
			DEBOUNCE++;
		}
	}
	else if(ACTION == TURNRIGHT)
	{
		if(getFrontRightWhite() == true)		
		{
			if(DEBOUNCE > 20)
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStartRotate(false, 80);
				// motor::motorStart(true,65,0);
			}
		}
		else
		{
			motor::motorStartRotate(false, 80);
			// motor::motorStart(true,65,0);
			DEBOUNCE++;
		}
	}
	else if(ACTION == BACKUP)
	{
		if(getFrontLeftWhite() == false && getFrontRightWhite() == false)
		{
			if(getFrontLeftWideWhite() == false && getFrontRightWideWhite() == false)	//going form black cross to past it
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStart(false, 40,40);
			}
		}
		else
		{
			motor::motorStart(false, 40,40);
		}
	}
	else
	{
		motor::motorStop();
	}
}

boolean sensor::getFrontLeftWhite()
{
	if(digitalRead(GRIDSENS_FRONT_LEFT) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

boolean sensor::getFrontRightWhite()
{
	if(digitalRead(GRIDSENS_FRONT_RIGHT) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

boolean sensor::getBackLeftWhite()
{
	if(digitalRead(GRIDSENS_BACK_LEFT) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

boolean sensor::getBackRightWhite()
{
	if(digitalRead(GRIDSENS_BACK_RIGHT) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

boolean sensor::getFrontLeftWideWhite()
{
	if(digitalRead(GRIDSENS_FRONT_LEFT_WIDE) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

boolean sensor::getFrontRightWideWhite()
{
	if(digitalRead(GRIDSENS_FRONT_RIGHT_WIDE) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void sensor::updateDistances()
{
	distance_front = analogRead(DISTANCESENS_FRONT); 
	distance_left = analogRead(DISTANCESENS_LEFT); 
	distance_right = analogRead(DISTANCESENS_RIGHT); 
	distance_back = analogRead(DISTANCESENS_BACK);
}

int getFrontGrid()
{
	double front_volt = (float)5/(float)1024*(float)distance_front;
	if(front_volt>1.9)
		grid = 0;
	  else if(front_volt>1.3)
		grid = 1;
	  else if(front_volt>1.1)
		grid = 2;
	  else if(front_volt>0.86)
		grid = 3;
	  else if(front_volt>0.73)
		grid = 4;
	  else
		grid = 10;
	return grid;
}

int getLeftGrid()
{
	double left_volt = (float)5/(float)1024*(float)distance_left;
	if(left_volt>1.9)
		grid = 0;
	  else if(left_volt>1.3)
		grid = 1;
	  else if(left_volt>1.1)
		grid = 2;
	  else if(left_volt>0.86)
		grid = 3;
	  else if(left_volt>0.73)
		grid = 4;
	  else
		grid = 10;
	return grid;
}

int getRightGrid()
{
	double right_volt = (float)5/(float)1024*(float)distance_right;
	if(right_volt>1.9)
		grid = 0;
	  else if(right_volt>1.3)
		grid = 1;
	  else if(right_volt>1.1)
		grid = 2;
	  else if(right_volt>0.86)
		grid = 3;
	  else if(right_volt>0.73)
		grid = 4;
	  else
		grid = 10;
	return grid;
}

int getBackGrid()
{
	double back_volt = (float)5/(float)1024*(float)distance_back;
	if(back_volt>1.9)
		grid = 0;
	  else if(back_volt>1.3)
		grid = 1;
	  else if(back_volt>1.1)
		grid = 2;
	  else if(back_volt>0.86)
		grid = 3;
	  else if(back_volt>0.73)
		grid = 4;
	  else
		grid = 10;
	return grid;
}