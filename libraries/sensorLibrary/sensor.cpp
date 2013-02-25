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
	pinMode(GRIDSENS_BACK_RIGHT, INPUT);	//grid sensor pin out
	pinMode(GRIDSENS_BACK_LEFT, INPUT);
	pinMode(GRIDSENS_FRONT_RIGHT, INPUT);
	pinMode(GRIDSENS_FRONT_LEFT, INPUT);
	
	myMotor = motor();	//setup motor settings
}

volatile int sensor::ACTION = STAY;
volatile int sensor::GOGRIDCOUNT = 0;
int sensor::DEBOUNCE = 0;

void sensor::goStraightTile(int val)
{
	ACTION = STRAIGHT;
	DEBOUNCE = 0;
	GOGRIDCOUNT = val;
	int currentAction;
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
	ACTION = TURNLEFT;
	DEBOUNCE = 0;
	int currentAction;
	do
    {   
		currentAction = ACTION;
    }while(ACTION != STAY);
}

void sensor::rotate90Right()
{
	ACTION = TURNRIGHT;
	DEBOUNCE = 0;
	int currentAction;
	do
    {   
		currentAction = ACTION;
    }while(currentAction != STAY);
}

void sensor::process()
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
					myMotor.motorStart(true, 40,40);
				}
				
		  }
		  else if (getFrontLeftWhite() == true && getFrontRightWhite() == false)
		  {
			  // myMotor.motorStart(true,65,0);
			  myMotor.motorStartRotate(false,50);
		  }
		  else if (getFrontLeftWhite() == false && getFrontRightWhite() == true)
		  {
			  // myMotor.motorStart(true,0,65);
			  myMotor.motorStartRotate(true,50);
		  }
		  else
		  {
			  myMotor.motorStart(true,40,40);
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
			motor::motorStart(false, 35,35);
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