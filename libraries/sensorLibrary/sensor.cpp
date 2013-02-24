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

int sensor::ACTION = false;
volatile int sensor::GOGRIDCOUNT = 0;
int sensor::DEBOUNCE = 0;

void sensor::goStraightTile(int val)
{
	ACTION = STRAIGHT;
	DEBOUNCE = 0;
	GOGRIDCOUNT = val;
	boolean currentAction;
	do
    {   
		currentAction = ACTION;
    }while(currentAction != STAY);
}

void sensor::rotate90Left()
{
	ACTION = TURNLEFT;
	DEBOUNCE = 0;
	boolean currentAction;
	do
    {   
		currentAction = ACTION;
    }while(currentAction != STAY);
}

void sensor::rotate90Right()
{
	ACTION = TURNRIGHT;
	DEBOUNCE = 0;
	boolean currentAction;
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
					if(DEBOUNCE > 100)	//debounce grid sensor to get accurate crossing
					{
						DEBOUNCE = 0;
						GOGRIDCOUNT--;	//count on entering each tile cross
						
						if(GOGRIDCOUNT <= 0)	// when robot passes a defined number of tile crosses
						{
							myMotor.motorStop();
							ACTION = STAY;
						}
					}
				}
			  myMotor.motorStart(true,70,70);
		  }
		  else if (getFrontLeftWhite() == true && getFrontRightWhite() == false)
		  {
			  myMotor.motorStart(true,75,40);
		  }
		  else if (getFrontLeftWhite() == false && getFrontRightWhite() == true)
		  {
			  myMotor.motorStart(true,40,75);
		  }
		  else
		  {
			  DEBOUNCE++;
		  }
	}
	else if(ACTION == TURNLEFT)
	{
		if(getFrontLeftWhite() == true)		
		{
			if(DEBOUNCE > 100)
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStartRotate(true, 40);
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
			if(DEBOUNCE > 100)
			{
				motor::motorStop();
				ACTION = STAY;
			}
			else
			{
				motor::motorStartRotate(true, 40);
			}
		}
		else
		{
			motor::motorStartRotate(true, 60);
			DEBOUNCE++;
		}
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