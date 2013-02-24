#include "motor.h"
#include "Arduino.h"
#include "sensor.h"

sensor::sensor()	//constructor
{
	pinMode(GRIDSENS_BACK_RIGHT, INPUT);	//grid sensor pin out
	pinMode(GRIDSENS_BACK_LEFT, INPUT);
	pinMode(GRIDSENS_FRONT_RIGHT, INPUT);
	pinMode(GRIDSENS_FRONT_LEFT, INPUT);
}

volatile boolean sensor::KEEPSTRAIGHT = false;

void sensor::keepStraight()
{
	  int left=digitalRead(GRIDSENS_FRONT_LEFT);
      int right=digitalRead(GRIDSENS_FRONT_RIGHT);
      
      int leftback=digitalRead(GRIDSENS_BACK_LEFT);
      int rightback=digitalRead(GRIDSENS_BACK_RIGHT);
      
      if (left==0 && right==0)
      {
          motor::motorStart(true,70,70);
      }
      else if (left==1 && right==0)
      {
          motor::motorStart(true,0,70);
      }
      else if (left==0 && right==1)
      {
          motor::motorStart(true,70,0);
      }
      else
      {
          motor::motorStart(true,70,70);
      }
}

void sensor::setStraight(boolean val)
{
	KEEPSTRAIGHT = val;
	if(val == false)
	{
		motor::motorStop();
	}
}

boolean sensor::getFrontLeftWhite()
{
	digitalRead(GRIDSENS_FRONT_LEFT);
}

boolean sensor::getFrontRightWhite()
{
	digitalRead(GRIDSENS_FRONT_RIGHT);
}

boolean sensor::getBackLeftWhite()
{
	digitalRead(GRIDSENS_BACK_LEFT);
}

boolean sensor::getBackRightWhite()
{
	digitalRead(GRIDSENS_BACK_RIGHT);
}