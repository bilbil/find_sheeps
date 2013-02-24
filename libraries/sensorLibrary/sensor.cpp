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
	
	myMotor = motor();
}

boolean sensor::KEEPSTRAIGHT = false;
volatile boolean sensor::STAY = false;
int sensor::GRIDCOUNT = 0;
volatile int sensor::GOGRIDCOUNT = 0;
volatile boolean sensor::ONTILE = false;
int sensor::DEBOUNCE = 0;
boolean sensor::EVALCROSSTILE = false;
boolean sensor::BACKALIGN = false;

void sensor::setStraight(boolean val)
{
	KEEPSTRAIGHT = val;
}

void sensor::goStraightTile(int val)
{
	STAY = false;
	DEBOUNCE = 0;
	GOGRIDCOUNT = val;
	setStraight(true);
	EVALCROSSTILE = true;
	boolean currentStay;
	do
    {   
		currentStay = STAY;
    }while(currentStay == false);
}

void sensor::rotate90Left()
{
	EVALCROSSTILE = false;
	myMotor.motorDirDist(90, 0, 90);
}

void sensor::rotate90Right()
{
	EVALCROSSTILE = false;
	myMotor.motorDirDist(-90, 0, 90);
}

void sensor::process()
{	
	if(getFrontLeftWhite() == true && getFrontRightWhite() == true	// when robot crosses a tile cross
	&& getBackLeftWhite() == false && getBackRightWhite() == false 
	&& EVALCROSSTILE == true) 
	{	
		if(ONTILE == false && DEBOUNCE > 100)
		{
			DEBOUNCE = 0;
			
			ONTILE = true;	// set ONTILE flag off
			
			GOGRIDCOUNT--;	//count on entering each tile cross
			
			if(GOGRIDCOUNT <= 0)	// when robot passes a defined number of tile crosses
			{
				myMotor.motorStop();
				BACKALIGN = true;
				STAY = true;
				EVALCROSSTILE = false;
			}
		}
	}
		  
	if(KEEPSTRAIGHT == true && STAY == false)	// guide robot straight
	{
		  if (getFrontLeftWhite() == true && getFrontRightWhite() == true)
		  {
			  myMotor.motorStart(true,70,70);
			  if(ONTILE == true)
			  {
				 ONTILE = false;
			  }
		  }
		  else if (getFrontLeftWhite() == true && getFrontRightWhite() == false)
		  {
			  myMotor.motorStart(true,75,40);
			  // motor::motorDutySet(75,40);
		  }
		  else if (getFrontLeftWhite() == false && getFrontRightWhite() == true)
		  {
			  myMotor.motorStart(true,40,75);
			  // motor::motorDutySet(40,75);
		  }
		  else
		  {
			  if(STAY == false)
			  {
					myMotor.motorStart(true,60,60);
					// motor::motorDutySet(40,40);
			  }
			  
			  
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