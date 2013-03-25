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
	
	sensor::ACTION = STAY;
}

volatile int sensor::ACTION = STAY;
volatile int sensor::GOGRIDCOUNT = 0;
int sensor::DEBOUNCE = 0;
int sensor::DEBOUNCE2 = 0;

int sensor::SPEED_STRAIGHT = SPEED_HIGH_STRAIGHT;
int sensor::SPEED_TURN = SPEED_HIGH_TURN;

volatile int sensor::distance_front = 0;
volatile int sensor::distance_left = 0;
volatile int sensor::distance_right = 0;
volatile int sensor::distance_back = 0;

volatile int sensor::grid = 0;

volatile int sensor::ERRORCOUNT = 0;

bool sensor::startOnWhite = false;

volatile bool sensor::frontLeft = true;
volatile bool sensor::frontRight = true;
volatile bool sensor::backLeft = true;
volatile bool sensor::backRight = true;

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
	
	startOnWhite = false;
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

void sensor::process()
{		
 		  
    frontLeft = getFrontLeftWhite();
	frontRight = getFrontRightWhite();
	backLeft = getBackLeftWhite();
	backRight = getBackRightWhite();
	
	if(ACTION == STRAIGHT)	// guide robot straight
	{		
		  if (frontLeft == true && frontRight == true)	// on white
		  {
				startOnWhite = true;
				ERRORCOUNT = 0;
				
				//test start
				// if(DEBOUNCE > 1)	//debounce grid sensor to get accurate crossing
				// {
					// DEBOUNCE = 0;
					// GOGRIDCOUNT--;	//count on entering each tile cross
					
					// if(GOGRIDCOUNT <= 0)	// when robot passes a defined number of tile crosses
					// {
						// motor::motorStop();
						// ACTION = ADJUST;
						// ACTION = BACKUP_CHECK;
						
					// }
					// else if(GOGRIDCOUNT < 2)	// when robot passes a defined number of tile crosses
					// {
						// myMotor.motorStart(true,SPEED_STRAIGHT,SPEED_STRAIGHT);
					// }
					// else
					// {
						// myMotor.motorStart(true,SPEED_STRAIGHT,SPEED_STRAIGHT);
					// }
				// }
				// else
				// {
					// if(GOGRIDCOUNT < 2)
					// {
						// myMotor.motorStart(true, SPEED_STRAIGHT,SPEED_STRAIGHT);
					// }
					// else
					// {
						// myMotor.motorStart(true,SPEED_STRAIGHT,SPEED_STRAIGHT);
					// }
				// }
		
				//end test
				
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
							// ACTION = BACKUP;

							ACTION = ADJUST;
							
						}
						else if(GOGRIDCOUNT < 2)	// when robot passes a defined number of tile crosses
						{
							myMotor.motorStart(true,SPEED_STRAIGHT,SPEED_STRAIGHT);
						}
						else
						{
							myMotor.motorStart(true,SPEED_STRAIGHT,SPEED_STRAIGHT);
						}
					}
					else
					{
						if(GOGRIDCOUNT < 2)
						{
							myMotor.motorStart(true, SPEED_STRAIGHT,SPEED_STRAIGHT);
						}
						else
						{
							myMotor.motorStart(true,SPEED_STRAIGHT,SPEED_STRAIGHT);
						}
					}
				}
				else
				{	
					if(GOGRIDCOUNT < 2)
					{
						myMotor.motorStart(true, SPEED_STRAIGHT,SPEED_STRAIGHT);
					}
					else	
					{
						myMotor.motorStart(true, SPEED_STRAIGHT,SPEED_STRAIGHT);
					}
				}
		  }
		  else if (frontLeft == true && frontRight == false)	//need to turn right
		  {
			int temp = 38 + ERRORCOUNT;
			if( temp > SPEED_STRAIGHT )
			{	
				temp = SPEED_STRAIGHT;
			}
			  myMotor.motorStartRotate(false,temp);
			
			ERRORCOUNT++;
		  }
		  else if (frontLeft == false && frontRight == true)	// need to turn left
		  {
			int temp = 38 + ERRORCOUNT;
			if( temp > SPEED_STRAIGHT )
			{	
				temp = SPEED_STRAIGHT;
			}
			  myMotor.motorStartRotate(true,temp);
			
			ERRORCOUNT++;
		  }
		  else	// both on black
		  {	
				if(startOnWhite == true)
				{
					DEBOUNCE++;
				}
				myMotor.motorStart(true, SPEED_SLOW_STOP,SPEED_SLOW_STOP);	
				ERRORCOUNT = 0;
				
		  }
	} 
	else if(ACTION == TURNLEFT)
	{
		if(frontLeft == true || frontRight == true || backLeft == true || backRight == true)		
		{
			if(DEBOUNCE > 2)
			{
				// motor::motorStop();
				// ACTION = STAY;
				DEBOUNCE2 = 0;
				ACTION = ADJUST;
				return;
			}
			else
			{
				motor::motorStartRotate(true, SPEED_TURN);
			}
		}
		else if(frontRight == false)	
		{
			motor::motorStartRotate(true, SPEED_TURN);
			DEBOUNCE++;
		}
	} 
    else if(ACTION == TURNRIGHT)
	{
		if(frontRight == true || frontLeft == true || backLeft == true || backRight == true)		
		{
			if(DEBOUNCE > 2)
			{
				// motor::motorStop();
				// ACTION = STAY;
				DEBOUNCE2 = 0;
				ACTION = ADJUST;
				return;
			}
			else
			{
				motor::motorStartRotate(false, SPEED_TURN);
			}
		}
		else if(frontLeft == false)	
		{
			motor::motorStartRotate(false, SPEED_TURN);
			DEBOUNCE++;
		}
	} 
	else if(ACTION == BACKUP)
	{
		if(backLeft == true || backRight == true)
		{		
			if(frontLeft == true || frontRight == true)
			{
				ACTION = STAY;	
			}
			else
			{
				ERRORCOUNT = 0;
				DEBOUNCE2 = 0;
				ACTION = ADJUST;
			}
		}
		else
		{
		  if (frontLeft == true && frontRight == false)	//need to turn right
		  {
			int temp = 42 + ERRORCOUNT;
			if( temp > SPEED_HIGH_TURN )
			{	
				temp = SPEED_HIGH_TURN;
			}
			  myMotor.motorStartRotate(false,temp);
			
			ERRORCOUNT++;
		  }
		  else if (frontLeft == false && frontRight == true)	// need to turn left
		  {
			int temp = 42 + ERRORCOUNT;
			if( temp > SPEED_HIGH_TURN )
			{	
				temp = SPEED_HIGH_TURN;
			}
			  myMotor.motorStartRotate(true,temp);
			
			ERRORCOUNT++;
		  }
		  else
		  {
			 motor::motorStart(false, SPEED_SLOW_STOP,SPEED_SLOW_STOP);
		  }
		}
	}
	
	else if(ACTION == BACKUP_CHECK)
	{
		if(backLeft == true && backRight == true)
		{		
			motor::motorStop();
			ACTION = STAY;	
		}
		else
		{
		  if (frontLeft == true && frontRight == false)	//need to turn right
		  {
			int temp = 42 + ERRORCOUNT;
			if( temp > SPEED_HIGH_TURN )
			{	
				temp = SPEED_HIGH_TURN;
			}
			  myMotor.motorStartRotate(false,temp);
			
			ERRORCOUNT++;
		  }
		  else if (frontLeft == false && frontRight == true)	// need to turn left
		  {
			int temp = 42 + ERRORCOUNT;
			if( temp > SPEED_HIGH_TURN )
			{	
				temp = SPEED_HIGH_TURN;
			}
			  myMotor.motorStartRotate(true,temp);
			
			ERRORCOUNT++;
		  }
		  else
		  {
			 motor::motorStart(false, SPEED_SLOW_STOP,SPEED_SLOW_STOP);
		  }
		}
	}
	
	else if(ACTION == ADJUST)
	{
		if (frontLeft == true && frontRight == true)	// on white
		{	
			ERRORCOUNT = 0;
			DEBOUNCE2++;
			if(DEBOUNCE2 > 10)
			{
				motor::motorStop();
				ACTION = BACKUP;
			}
		}
		else if (frontLeft == true && frontRight == false)	//need to turn right
		  {
			int temp = 42 + ERRORCOUNT;
			if( temp > SPEED_HIGH_TURN )
			{	
				temp = SPEED_HIGH_TURN;
			}
			  myMotor.motorStartRotate(false,temp);
			
			ERRORCOUNT++;
		  }
		  else if (frontLeft == false && frontRight == true)	// need to turn left
		  {
			int temp = 42 + ERRORCOUNT;
			if( temp > SPEED_HIGH_TURN )
			{	
				temp = SPEED_HIGH_TURN;
			}
			  myMotor.motorStartRotate(true,temp);
			
			ERRORCOUNT++;
		  }
		  else
		  {	 
			  DEBOUNCE2 = 0;
			  ERRORCOUNT = 0;
			  myMotor.motorStart(true, SPEED_SLOW_STOP,SPEED_SLOW_STOP);
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

//change start
/* boolean sensor::getFrontLeftWhite()
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
} */
////change end

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

int sensor::getFrontTest()
{
	distance_front = analogRead(DISTANCESENS_FRONT); 

	return distance_front;
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
	if(front_volt>1.80)
		grid = 0;
	  else if(front_volt>0.89)
		grid = 1;
	  else if(front_volt>0.65)
		grid = 2;
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
	if(left_volt>1.64)
		grid = 0;
	  else if(left_volt>1.34)
		grid = 1;
	  else if(left_volt>1.10)
		grid = 2;
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
	if(right_volt>1.73)
		grid = 0;
	  else if(right_volt>0.85)
		grid = 1;
	  else if(right_volt>0.6)
		grid = 2;
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

void sensor::setSpeedHigh(bool val)
{
	if(val == true)
	{
		SPEED_STRAIGHT = SPEED_HIGH_STRAIGHT;
		SPEED_TURN = SPEED_HIGH_TURN;
	}
	else
	{
		SPEED_STRAIGHT = SPEED_LOW_STRAIGHT;
		SPEED_TURN = SPEED_LOW_TURN;
	}
}

bool sensor::moveCenter(bool right)
{
 	//Variables used
	int numWhite=0;
	
	frontLeft = getFrontLeftWhite();
	frontRight = getFrontRightWhite();
	backLeft = getBackLeftWhite();
	backRight = getBackRightWhite();
	
	if(frontLeft)
		numWhite++;
	if(frontRight)
		numWhite++;
	if(backLeft)
		numWhite++;
	if(backRight)
		numWhite++;
		
	if(numWhite>2)
	{
		return true;
		// Do nothing it is probably in the center}
	}
	else
	{
		if(numWhite == 1)
		{
			if(frontLeft)
			{
				myMotor.motorStart(false,45,45);
				while(getFrontLeftWhite() || getFrontRightWhite())
					delay(50);
				myMotor.motorStart(true,45,0);
				delay(150);
				while(getFrontLeftWhite())
					delay(50);
				myMotor.motorStart(true,0,45);
				while(getFrontRightWhite())
					delay(50);
				myMotor.motorStop();
			}
			else if(frontRight)
			{
				myMotor.motorStart(false,45,45);
				while(getFrontLeftWhite() || getFrontRightWhite())
					delay(50);
				myMotor.motorStart(true,0,45);
				delay(150);
				while(getFrontRightWhite())
					delay(50);
				myMotor.motorStart(true,45,0);
				while(getFrontLeftWhite())
					delay(50);
				myMotor.motorStop();
			}
			else if(backLeft)
			{
				myMotor.motorStart(true,45,45);
				while(getBackLeftWhite() || getFrontRightWhite())
					delay(50);
				myMotor.motorStart(false,45,0);
				delay(150);
				while(getBackLeftWhite())
					delay(50);
				myMotor.motorStart(false,0,45);
				while(getBackRightWhite())
					delay(50);
				myMotor.motorStop();
			}
			else
			{
				myMotor.motorStart(true,45,45);
				while(getBackLeftWhite() || getFrontRightWhite())
					delay(50);
				myMotor.motorStart(false,0,45);
				delay(150);
				while(getBackRightWhite())
					delay(50);
				myMotor.motorStart(false,45,0);
				while(getBackLeftWhite())
					delay(50);
				myMotor.motorStop();
			}
		}
		else if(numWhite == 2)
		{
			if(frontLeft && frontRight)
			{
				myMotor.motorStart(false,45,45);
				while(!getBackLeftWhite() || !getBackRightWhite())
					delay(50);
				myMotor.motorStop();
			}
			else if(frontLeft && backLeft)
			{
				myMotor.motorStart(true,45,0);
				while(getFrontLeftWhite())
					delay(50);
				myMotor.motorStart(true,45,45);
				while(!getFrontRightWhite())
					delay(50);
				myMotor.motorStart(true,0,45);
				while(!getFrontLeftWhite())
					delay(50);
				myMotor.motorStop();
			}
			else if(frontLeft && backRight)
			{
				myMotor.motorStart(true,0,45);
				while(getBackRightWhite())
					delay(50);
				myMotor.motorStop();
			}
			else if(frontRight && backLeft)
			{
				myMotor.motorStart(true,45,0);
				while(getBackLeftWhite())
					delay(50);
				myMotor.motorStop();
			}
			else if(frontRight && backRight)
			{
				myMotor.motorStart(true,0,45);
				while(getFrontRightWhite())
					delay(50);
				myMotor.motorStart(true,45,45);
				while(!getFrontLeftWhite())
					delay(50);
				myMotor.motorStart(true,0,45);
				while(!getFrontRightWhite())
					delay(50);
				myMotor.motorStop();
			}
			else
			{
				myMotor.motorStart(true,45,45);
				while(!getFrontLeftWhite() || !getFrontRightWhite())
					delay(50);
				myMotor.motorStop();
			}
		}
		else
		{
			// All black turn more to the direction of the movement
		}
		return false;
	}
			
}