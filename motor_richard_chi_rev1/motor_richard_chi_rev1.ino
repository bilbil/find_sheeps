#include <beacon.h>
#define GRIDSENS_BACK_RIGHT  7
#define GRIDSENS_BACK_LEFT   6
#define GRIDSENS_FRONT_RIGHT 5
#define GRIDSENS_FRONT_LEFT  4
#define MotorLForward 10
#define MotorLBackward 12
#define MotorRForward 11
#define MotorRBackward 13
#define frontIR 3
#define leftIR 8
#define backIR 9
#define rightIR 2
#define DISTANCESENS_FRONT	A0
#define DISTANCESENS_LEFT	A1
#define DISTANCESENS_RIGHT	A2
#define DISTANCESENS_BACK	A3

#define forwardSpeed 35
#define forwardSlowSpeed 5
#define backwardSpeed 30
#define backwardSlowSpeed 5
#define gridSpeed 30
#define turnSpeed 45
#define pivotTurn 0
#define fowardDelay 200


boolean frontLeft = false;
boolean frontRight = false;
boolean backLeft = false;
boolean backRight = false;
float offset = 1.25;
float turnOffset = 1.00;
int numWhite = 0;
int counter = 0;
int grid = 0;
int distance_front = 0;
int distance_left = 0;
int distance_right = 0;
int distance_back = 0;
int state = 0;

int totalDistance;
Beacon beacon;
void setup()
{
  pinMode(10,OUTPUT); //Motor 1 forward
  pinMode(11,OUTPUT); //Motor 2 forward
  pinMode(12,OUTPUT); //Motor 1 backward
  pinMode(13,OUTPUT); //Motor 2 backward
  pinMode(4,INPUT); //Front Left
  pinMode(5,INPUT); //Front Right
  pinMode(6,INPUT); //Back Left
  pinMode(7,INPUT); //Back Right
  
  Serial.begin(4800);
}

void loop()
{
  //Wait 1 second
  delay(1000);
  
  if(digitalRead(leftIR)==0)
  {
    //Turn left
    state = 1;
    while(state!=0)
    {
      state = turnLeft(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(digitalRead(rightIR)==0)
  {
    //Turn right
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(digitalRead(backIR)==0)
  {
    //Turn 180
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }

    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(digitalRead(frontIR)==0 && getFrontGrid()==0)
  {
    //Stop
    while(1)
    {
      int i = 0;
    }
  }
  else
  {
    state = 1;
    while(state!=0)
    {
      state = goStraight(state);
    }
  }
//  
//  state = 1;
//  while(state!=0)
//  {
//    state = turnLeft(state);
//  }
//  
//  state = 1;
//  while(state!=0)
//  {
//    state = goStraight(state);
//  }

//  Serial.print(digitalRead(2));
//  Serial.print("  ");
//  Serial.print(digitalRead(3));
//  Serial.print("  ");
//  Serial.print(digitalRead(8));
//  Serial.print("  ");
//  Serial.println(digitalRead(9));
//  delay(200);

//  Serial.print(getFrontGrid());
//  Serial.print("  ");
//  Serial.print(getLeftGrid());
//  Serial.print("  ");
//  Serial.println(getRightGrid());
  
//  state = 1;
//  while(state!=0)
//  {
//    state = turnRight(state);
//  }
//
//  state = 1;
//  while(state!=0)
//  {
//    state = moveCenter(state);
//  }
  
//  //Stop
//  while(1)
//  {
//    int i = 0;
//  }
}

void startMotor(boolean forward, int leftMotor, int rightMotor)
{
  if(forward)
  {
    analogWrite(MotorLForward,leftMotor);
    analogWrite(MotorRForward,(rightMotor*offset));
    digitalWrite(MotorLBackward,LOW);
    digitalWrite(MotorRBackward,LOW);
  }
  else
  {
    analogWrite(MotorLForward,255-leftMotor);
    analogWrite(MotorRForward,255-(rightMotor*offset));
    digitalWrite(MotorLBackward,HIGH);
    digitalWrite(MotorRBackward,HIGH);
  }
}

void startTurn(boolean left, int turningSpeed)
{
  if(left)
  {
    analogWrite(MotorLForward,255-turningSpeed+pivotTurn);
    analogWrite(MotorRForward,(turningSpeed*turnOffset));
    digitalWrite(MotorLBackward,HIGH);
    digitalWrite(MotorRBackward,LOW);
  }
  else
  {
    analogWrite(MotorLForward,turningSpeed);
    analogWrite(MotorRForward,255-(turningSpeed*turnOffset)+pivotTurn);
    digitalWrite(MotorLBackward,LOW);
    digitalWrite(MotorRBackward,HIGH);
  }
}

void stopMotor()
{
  digitalWrite(MotorLForward,LOW);
  digitalWrite(MotorRForward,LOW);
  digitalWrite(MotorLBackward,LOW);
  digitalWrite(MotorRBackward,LOW);
}
  
int goStraight(int state)
{
  updateSensors();
  switch(state)
  {
    //Make sure robot move forward a bit
    case 1:
    startMotor(true,forwardSpeed,forwardSpeed);
    delay(250);
    return 2;
    break;
    
    //Grid following until robot reaches next cross
    case 2:
    if(frontLeft == false)
    {
        if(frontRight == false)
        {
          //grid approaching
          startMotor(true,gridSpeed,gridSpeed);
          return 3;
          break;
        }
        else
        {
          //adjust to left
          startMotor(true,forwardSlowSpeed,forwardSpeed);
          return 2;
          break;
        }
    }
    else if (frontRight == false)
    {
      //adjust to right
      startMotor(true,forwardSpeed,forwardSlowSpeed);
      return 2;
      break;
    }
    else
    {
      //continue
      startMotor(true,forwardSpeed,forwardSpeed);
      return 2;
      break;
    }
    
    //Motor control when robot is in cross
    case 3:
    if(frontLeft == true || frontRight == true)
    {
      stopMotor();
      delay(100);
      return 6;
      break;
    }
    else
    {
      if(backLeft == false)
      {
        //adjust to left
        startMotor(true,forwardSlowSpeed,gridSpeed);
        
      }
      else
      {
        if(backRight == false)
        {
          //adjust to right
          startMotor(true,gridSpeed,forwardSlowSpeed);
        }
        else
        {
          startMotor(true,gridSpeed,gridSpeed);
        }
      }
      return 3;
      break;
    }
    
//    //Aligning the robot by backing up after crossing
//    case 4:
//    if(frontLeft == true && frontRight == true)
//    {
//      return 5;
//      break;
//    }
//    else
//    {
//      if(frontLeft == false)
//      {
//        //adjust to left
//        startMotor(true,forwardSlowSpeed,gridSpeed);
//      }
//      else
//      {
//        //adjust to right
//        startMotor(true,gridSpeed,forwardSlowSpeed);
//      }
//      return 4;
//      break;
//    }
//    
//    //Going back to the center of the cross (make sure they cross black)
//    case 5:
//    if(backLeft == false && backRight == false)
//    {
//      stopMotor();
//      delay(100);
//      return 6;
//      break;
//    }
//    else
//    {
//      startMotor(false,gridSpeed,gridSpeed);
//      return 5;
//      break;
//    }
//    
    //Stop at white
    case 6:
    if(backLeft == true || backRight == true)
    {
      stopMotor();
      return 0;
      break;
    }
    else
    {
      startMotor(false,gridSpeed,gridSpeed);
      return 6;
      break;
    }
  }
}

int turnLeft(int state)
{
  updateSensors();
  switch(state)
  {
    //Make sure the robot starts turning a bit
    case 1:
    startTurn(true,turnSpeed);
    delay(250);
    return 2;
    break;
    
    //Turns to the left till two sensors are both in black
    case 2:
    if(frontLeft == false && backRight == false)
    {
      startTurn(true,turnSpeed);
      return 3;
      break;
    }
    else
    {
      startTurn(true,turnSpeed);
      return 2;
      break;
    }
    
    //Once both sensors are black turn until one of them are white
    case 3:
    if(frontLeft == true || backRight == true)
    {
      stopMotor();
      return 0;
      break;
    }
    else
    {
      startTurn(true,turnSpeed);
      return 3;
      break;
    }
  }
}

int turnRight(int state)
{
  updateSensors();
  switch(state)
  {
    //Make sure the robot starts turning a bit
    case 1:
    startTurn(false,turnSpeed);
    delay(250);
    return 2;
    break;
    
    //Turns to the left till two sensors are both in black
    case 2:
    if(frontRight == false && backLeft == false)
    {
      startTurn(false,turnSpeed);
      return 3;
      break;
    }
    else
    {
      startTurn(false,turnSpeed);
      return 2;
      break;
    }
    
    //Once both sensors are black turn until one of them are white
    case 3:
    if(frontRight == true || backLeft == true)
    {
      stopMotor();
      return 0;
      break;
    }
    else
    {
      startTurn(false,turnSpeed);
      return 3;
      break;
    }
  }
}

int moveCenter(int state)
{
  updateSensors();
  switch(state)
  {
    //Move robot backwards
    case 1:
    if(backRight == true || backLeft == true)
    {
      stopMotor();
      delay(200);
      return 2;
      break;
    }
    else
    {
      startMotor(false,gridSpeed,gridSpeed);
      return 1;
      break;
    }
    
    //Align itself
    case 2:
    if(backRight == true && backLeft == true)
    {
      stopMotor();
      return 3;
      break;
    }
    else
    {
//      if(backLeft == false)
//      {
//        //adjust to left
//        startMotor(false,backwardSlowSpeed,gridSpeed);
//        return 2;
//        break;
//      }
//      else
//      {
//        //adjust to right
//        startMotor(false,gridSpeed,backwardSlowSpeed);
//        return 2;
//        break;
//      }
      counter = 0;
      if(frontLeft == false)
      {
        //adjust to left
        startMotor(false,backwardSlowSpeed,gridSpeed);
        while(getFrontRightWhite() || getBackRightWhite())
        {}
        startMotor(false,backwardSpeed,backwardSpeed);
        while(!getBackLeftWhite())
        {}
        startMotor(true,gridSpeed,gridSpeed);
        while(getBackLeftWhite())
        {}
        stopMotor();
        return 4;
        break;
      }
      else if(frontRight == false)
      {
        //adjust to right
        startMotor(false,gridSpeed,backwardSlowSpeed);
        while(getFrontLeftWhite() || getBackLeftWhite())
        {}
        startMotor(false,backwardSpeed,backwardSpeed);
        while(!getBackRightWhite())
        {}
        startMotor(true,gridSpeed,gridSpeed);
        while(getBackRightWhite())
        {}
        stopMotor();
        return 5;
        break;
      }
      else
      {
        startMotor(false,backwardSpeed,backwardSpeed);
        while(!getBackLeftWhite() && !getBackRightWhite())
        {}
        stopMotor();
        return 3;
        break;
      }
    }
    
    //go forward
    case 3:
    if(frontLeft == true || frontRight == true)
    {
      stopMotor();
      return 0;
      break;
    }
    else
    {
      startMotor(true,gridSpeed,gridSpeed);
      return 3;
      break;
    }
    
    //go forward bit to right
    case 4:
    if(frontRight == true)
    {
      stopMotor();
      return 0;
      break;
    }
    else
    {
      startMotor(true,forwardSlowSpeed,gridSpeed);
      return 4;
      break;
    }
    
    //go forward bit to left
    case 5:
    if(frontLeft == true)
    {
      stopMotor();
      return 0;
      break;
    }
    else
    {
      startMotor(true,gridSpeed,forwardSlowSpeed);
      return 5;
      break;
    }
  }

        
  
// 	//Variables used
//	numWhite=0;
//	updateSensors();
//	
//	if(frontLeft)
//		numWhite++;
//	if(frontRight)
//		numWhite++;
//	if(backLeft)
//		numWhite++;
//	if(backRight)
//		numWhite++;
//		
//	if(numWhite>2)
//	{
//		return true;
//		// Do nothing it is probably in the center}
//	}
//	else
//	{
//		if(numWhite == 1)
//		{
//			if(frontLeft)
//			{
//				startMotor(false,gridSpeed,gridSpeed);
//				while(getFrontLeftWhite() || getFrontRightWhite())
//				{}
//				startMotor(true,turnSpeed,0);
//				while(getFrontLeftWhite())
//				{}
//				startMotor(true,0,turnSpeed);
//				while(getFrontRightWhite())
//				{}
//				stopMotor();
//			}
//			else if(frontRight)
//			{
//				startMotor(false,gridSpeed,gridSpeed);
//				while(getFrontLeftWhite() || getFrontRightWhite())
//				{}
//				startMotor(true,0,turnSpeed);
//				while(getFrontRightWhite())
//				{}
//				startMotor(true,turnSpeed,0);
//				while(getFrontLeftWhite())
//				{}
//				stopMotor();
//			}
//			else if(backLeft)
//			{
//				startMotor(true,gridSpeed,gridSpeed);
//				while(getBackLeftWhite() || getFrontRightWhite())
//				{}
//				startMotor(false,turnSpeed,0);
//				while(getBackLeftWhite())
//				{}
//				startMotor(false,0,turnSpeed);
//				while(getBackRightWhite())
//				{}
//				stopMotor();
//			}
//			else
//			{
//				startMotor(true,gridSpeed,gridSpeed);
//				while(getBackLeftWhite() || getFrontRightWhite())
//				{}
//				startMotor(false,0,turnSpeed);
//				while(getBackRightWhite())
//				{}
//				startMotor(false,turnSpeed,0);
//				while(getBackLeftWhite())
//				{}
//				stopMotor();
//			}
//		}
//		else if(numWhite == 2)
//		{
//			if(frontLeft && frontRight)
//			{
//				startMotor(false,gridSpeed,gridSpeed);
//				while(!getBackLeftWhite() && !getBackRightWhite())
//				{}
//				stopMotor();
//			}
//			else if(frontLeft && backLeft)
//			{
//				startMotor(true,turnSpeed,0);
//				while(getFrontLeftWhite())
//				{}
//				startMotor(true,gridSpeed,gridSpeed);
//				while(!getFrontRightWhite())
//				{}
//				startMotor(true,0,turnSpeed);
//				while(!getFrontLeftWhite())
//				{}
//				stopMotor();
//			}
//			else if(frontLeft && backRight)
//			{
//				startMotor(true,0,turnSpeed);
//				while(getBackRightWhite())
//				{}
//				stopMotor();
//			}
//			else if(frontRight && backLeft)
//			{
//				startMotor(true,turnSpeed,0);
//				while(getBackLeftWhite())
//				{}
//				stopMotor();
//			}
//			else if(frontRight && backRight)
//			{
//				startMotor(true,0,turnSpeed);
//				while(getFrontRightWhite())
//				{}
//				startMotor(true,gridSpeed,gridSpeed);
//				while(!getFrontLeftWhite())
//				{}
//				startMotor(true,0,turnSpeed);
//				while(!getFrontRightWhite())
//			        {}
//				stopMotor();
//			}
//			else
//			{
//				startMotor(true,gridSpeed,gridSpeed);
//				while(!getFrontLeftWhite() && !getFrontRightWhite())
//                                {}
//				stopMotor();
//			}
//		}
//		else
//		{
//			// All black turn more to the direction of the movement
//		}
//		return false;
//	}
			
}

void updateSensors()
{
  frontLeft = digitalRead(GRIDSENS_FRONT_LEFT); 
  frontRight = digitalRead(GRIDSENS_FRONT_RIGHT);
  backLeft = digitalRead(GRIDSENS_BACK_LEFT); 
  backRight = digitalRead(GRIDSENS_BACK_RIGHT); 
}

boolean getFrontLeftWhite()
{
  return digitalRead(GRIDSENS_FRONT_LEFT); 
}

boolean getFrontRightWhite()
{
  return digitalRead(GRIDSENS_FRONT_RIGHT); 
}

boolean getBackLeftWhite()
{
  return digitalRead(GRIDSENS_BACK_LEFT); 
}

boolean getBackRightWhite()
{
  return digitalRead(GRIDSENS_BACK_RIGHT); 
}

int getFrontGrid()
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

int getLeftGrid()
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

int getRightGrid()
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
void beacon2rescue ()
{
if(beacon.getLeftBeacon()==2)
  {
    //Turn left
    state = 1;
    while(state!=0)
    {
      state = turnLeft(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(beacon.getRightBeacon()==2)
  {
    //Turn right
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(beacon.getBackBeacon()==2)
  {
    //Turn 180
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }

    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(beacon.getFrontBeacon()==2 && getFrontGrid()==0)
  {
    //Stop
    while(1)
    {
	//TODO: rescue routine, then return
      int i = 0;
    }
  }
  else
  {
    state = 1;
    while(state!=0)
    {
      state = goStraight(state);
      totalDistance++;
    }
  }
}
void beacon1rescue ()
{
if(beacon.getLeftBeacon()==1)
  {
    //Turn left
    state = 1;
    while(state!=0)
    {
      state = turnLeft(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(beacon.getRightBeacon()==1)
  {
    //Turn right
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(beacon.getBackBeacon()==1)
  {
    //Turn 180
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
    
    state = 1;
    while(state!=0)
    {
      state = turnRight(state);
    }

    state = 1;
    while(state!=0)
    {
      state = moveCenter(state);
    }
  }
  else if(beacon.getFrontBeacon()==1 && getFrontGrid()==0)
  {
    //Stop
    while(1)
    {
	//TODO: rescue routine, then return
      int i = 0;
    }
  }
  else
  {
    state = 1;
    while(state!=0)
    {
      state = goStraight(state);
      totalDistance++;
    }
  }
}