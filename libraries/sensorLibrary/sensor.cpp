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
	// pinMode(GRIDSENS_FRONT_RIGHT_WIDE, INPUT);	//grid sensor pin out
	// pinMode(GRIDSENS_FRONT_LEFT_WIDE, INPUT);
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

volatile int sensor::grid = 0;

volatile int sensor::ERRORCOUNT = 0;

void sensor::test(int val)
{
	motor::motorStart(true,45,45);
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
	rotate90Left();
}

void sensor::rotate180Right()
{
	rotate90Right();
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

	// motor::motorStart(true,10,10);
 		  
    bool frontLeft = getFrontLeftWhite();
	bool frontRight = getFrontRightWhite();
	bool backLeft = getBackLeftWhite();
	bool backRight = getBackRightWhite();
	
	if(ACTION == STRAIGHT)	// guide robot straight
	{		
		  if (frontLeft == true && frontRight == true)	// on white
		  {
				ERRORCOUNT = 0;
				
				if(backLeft == false && backRight == false)
				{
					if(DEBOUNCE > 1)	//debounce grid sensor to get accurate crossing
					{
						DEBOUNCE = 0;
						GOGRIDCOUNT--;	//count on entering each tile cross
						
						if(GOGRIDCOUNT <= 0)	// when robot passes a defined number of tile crosses
						{
							motor::motorStop();
							// ACTION = STAY;
							ACTION = BACKUP;
						}
						else if(GOGRIDCOUNT < 3)	// when robot passes a defined number of tile crosses
						{
							myMotor.motorStart(true,40,40);
						}
						else
						{
							myMotor.motorStart(true,40,40);
						}
					}
					else
					{
						if(GOGRIDCOUNT < 3)
						{
							myMotor.motorStart(true, 40,40);
						}
						else
						{
							myMotor.motorStart(true,40,40);
						}
					}
				}
				else
				{	
					if(GOGRIDCOUNT < 3)
					{
						myMotor.motorStart(true, 40,40);
					}
					else	
					{
						myMotor.motorStart(true, 40,40);
					}
				}
		  }
		  else if (frontLeft == true && frontRight == false)	//need to turn right
		  {
			int temp = 25 + ERRORCOUNT;
			if( temp > 40 )
			{	
				temp = 40;
			}
			  // myMotor.motorStart(true,65,0);
			  myMotor.motorStartRotate(false,temp);
			
			ERRORCOUNT++;
		  }
		  else if (frontLeft == false && frontRight == true)	// need to turn left
		  {
			int temp = 25 + ERRORCOUNT;
			if( temp > 40 )
			{	
				temp = 40;
			}
			  // myMotor.motorStart(true,0,65);
			  myMotor.motorStartRotate(true,temp);
			
			ERRORCOUNT++;
		  }
		  else	// both on black
		  {
				// if(getFrontLeftWideWhite() == false && getFrontRightWideWhite() == false)	//on black of cross
				// {
					DEBOUNCE++;
				// }
				myMotor.motorStart(true, 40,40);	
				ERRORCOUNT = 0;
		  }
	} 
	else if(ACTION == TURNLEFT)
	{
		if(frontLeft == true)		
		{
			if(DEBOUNCE > 10)
			{
				motor::motorStop();
				ACTION = STAY;
				// ACTION = ADJUST;
			}
			else
			{
				motor::motorStartRotate(true, 60);
				// motor::motorStart(true,0,65);
			}
		}
		else
		{
			motor::motorStartRotate(true, 60);
			// motor::motorStart(true,0,65);
			DEBOUNCE++;
		}
	} 
    else if(ACTION == TURNRIGHT)
	{
		if(frontRight == true)		
		{
			if(DEBOUNCE > 10)
			{
				motor::motorStop();
				ACTION = STAY;
				// ACTION = ADJUST;
			}
			else
			{
				motor::motorStartRotate(false, 60);
				// motor::motorStart(true,65,0);
			}
		}
		else
		{
			motor::motorStartRotate(false, 60);
			// motor::motorStart(true,65,0);
			DEBOUNCE++;
		}
	} 
	else if(ACTION == BACKUP)
	{
		if(backLeft == true || backRight == true)
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
		ACTION = STAY;
	}   
}

boolean sensor::getFrontLeftWhite()
{
	if(digitalRead(GRIDSENS_FRONT_LEFT) == 1)
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
	if(digitalRead(GRIDSENS_FRONT_RIGHT) == 1)
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
	if(digitalRead(GRIDSENS_BACK_LEFT) == 1)
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
	if(digitalRead(GRIDSENS_BACK_RIGHT) == 1)
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
	if(digitalRead(GRIDSENS_FRONT_LEFT_WIDE) == 1)
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
	if(digitalRead(GRIDSENS_FRONT_RIGHT_WIDE) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int sensor::getFrontGrid()
{
	double front_volt=0;
	for(int i=0;i<10;i++)
	{
		distance_front = analogRead(DISTANCESENS_FRONT); 
		front_volt = (float)5/(float)1024*(float)distance_front+front_volt;
	}
	front_volt = front_volt/10;
	if(front_volt>1.78)
		grid = 0;
	  else if(front_volt>1.37)
		grid = 1;
	  else if(front_volt>0.98)
		grid = 2;
	  else if(front_volt>0.68)
		grid = 3;
	  else if(front_volt>0.55)
		grid = 4;
	  else
		grid = 10;
	return grid;
}

int sensor::getLeftGrid()
{
	double left_volt=0;
	for(int i=0;i<3;i++)
	{
		distance_left = analogRead(DISTANCESENS_LEFT); 
		left_volt = (float)5/(float)1024*(float)distance_left+left_volt;
	}
	left_volt = left_volt/3;
	if(left_volt>1.98)
		grid = 0;
	  else if(left_volt>0.71)
		grid = 1;
	  else if(left_volt>0.32)
		grid = 2;
	  else if(left_volt>1)
		grid = 3;
	  else if(left_volt>1)
		grid = 4;
	  else
		grid = 10;
	return grid;
}

int sensor::getRightGrid()
{
	double right_volt=0;
	for(int i=0;i<3;i++)
	{
		distance_right = analogRead(DISTANCESENS_RIGHT); 
		right_volt = (float)5/(float)1024*(float)distance_right+right_volt;
	}
	right_volt = right_volt/3;
	if(right_volt>1.90)
		grid = 0;
	  else if(right_volt>1.04)
		grid = 1;
	  else if(right_volt>0.65)
		grid = 2;
	  else if(right_volt>1)
		grid = 3;
	  else if(right_volt>1)
		grid = 4;
	  else
		grid = 10;
	return grid;
}

int sensor::getBackGrid()
{
	double back_volt=0;
	for(int i=0;i<3;i++)
	{
		distance_back = analogRead(DISTANCESENS_BACK); 
		back_volt = (float)5/(float)1024*(float)distance_back+back_volt;
	}
	back_volt = back_volt/3;
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