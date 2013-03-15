#include <sheep_interrupts.h>
#include <motor.h>
#include <sensor.h>

/*Beacon and distance readings; 
 First array notates the current direction; 
 Second array stores the sensor readings in the following order:
 North, East, South, West
 */
int distance[4][4];
int beacon[4][4];

int posx;
int posy;

int homex;
int homey;

int sheep;

/*
Variables for random walk
 1 = section 1
 2 = section 2
 3 = section 3
 */
int section;

/*
Forward facing direction of the robot
 1 = North
 2 = East
 3 = South
 4 = West
 */
int currentDirection;

sensor drive;

void setup() {
  //We set the starting direction of the robot to 'North'; every change after this point will be the relative direction
  currentDirection = 1;
  section = 1;
  //Variable declaration(s)
  drive = sensor();
  sheep = 0;
  findInitialPosition();
}

void loop() {
  // put your main code here, to run repeatedly: 
  randomWalk();
  goHome();
}

void randomWalk() {
  switch(section) {
  case 1:
    while (posy < 120) {
      if (isRescueComplete) {
        return;
      }
    }
    moveForward();
    thicketSearch();
    section = 2;
    break;
  case 2:
    if (posx < 80) {
      changeOrientation(2);
      while (posx <120) {
        if (isRescueComplete) {
          return;
        }
        moveForward();
        thicketSearch();
      }
      section = 3;
    }
    else {
      changeOrientation(4);
      while (posx > 40) {
        if (isRescueComplete) {
          return;
        }
        moveForward();
        thicketSearch();
      }
      section = 3;
    }
    break;
  case 3:
    while (posy > 40) {
      if (isRescueComplete) {
        return;
      }
      moveForward();
      thicketSearch();
    }
    break;
  default:
    break;
  }
}

//Updates the sensors; Since the sensors on the robot is relative to static North, proper assignment of sensor readings is done to capture data in the 4 directions
void updateReadings() {
  switch (currentDirection) {
  case 1:
    distance[currentDirection][0] = getDistanceFront();
    distance[currentDirection][1] = getDistanceRight();
    distance[currentDirection][2] = getDistanceBack();
    distance[currentDirection][3] = getDistanceLeft();
    beacon[currentDirection][0] = getBeaconFront();
    beacon[currentDirection][1] = getBeaconRight();
    beacon[currentDirection][2] = getBeaconBack();
    beacon[currentDirection][3] = getBeaconLeft();
    break;
  case 2:
    distance[currentDirection][0] = getDistanceLeft();
    distance[currentDirection][1] = getDistanceFront();
    distance[currentDirection][2] = getDistanceRight();
    distance[currentDirection][3] = getDistanceBack();
    beacon[currentDirection][0] = getBeaconLeft();
    beacon[currentDirection][1] = getBeaconFront();
    beacon[currentDirection][2] = getBeaconRight();
    beacon[currentDirection][3] = getBeaconBack();
    break;
  case 3:
    distance[currentDirection][0] = getDistanceBack();
    distance[currentDirection][1] = getDistanceLeft();
    distance[currentDirection][2] = getDistanceFront();
    distance[currentDirection][3] = getDistanceRight();
    beacon[currentDirection][0] = getBeaconBack();
    beacon[currentDirection][1] = getBeaconLeft();
    beacon[currentDirection][2] = getBeaconFront();
    beacon[currentDirection][3] = getBeaconRight();
    break;
  case 4:
    distance[currentDirection][0] = getDistanceRight();
    distance[currentDirection][1] = getDistanceBack();
    distance[currentDirection][2] = getDistanceLeft();
    distance[currentDirection][3] = getDistanceFront();
    beacon[currentDirection][0] = getBeaconRight();
    beacon[currentDirection][1] = getBeaconBack();
    beacon[currentDirection][2] = getBeaconLeft();
    beacon[currentDirection][3] = getBeaconFront();
    break;
  default:
    break;
  }
}


int getDistanceLeft() {

}

int getDistanceFront() {

}

int getDistanceRight() {

}

int getDistanceBack () {

}

int getBeaconLeft() {

}

int getBeaconFront() {

}

int getBeaconRight() {

}

int getBeaconBack () {

}

//Detects the presence of a beacon; returns the direction in which the beacon is being received
int detectBeacon() {
  for(int i = 0; i < 4; i++) {
    if (beacon[currentDirection][i] > 0) {
      return i;
    }
  }
  return 0;
}

//If a beacon is present, search for the thicket
void thicketSearch() {
  while (1) {
    isRescueComplete();
    int targetDirection = detectBeacon();
    if (targetDirection > 0) {
      advance(targetDirection);
    }
  }
}

//Move functions with coordinates
void turnLeft ()
{
  if (currentDirection == 1) {
    currentDirection = 4;
  }

  else {
    currentDirection --;
  }
  drive.rotate90Left();
}

void turnRight ()
{
  if (currentDirection == 4) {
    currentDirection = 1;
  }

  else {
    currentDirection ++;
  }
  drive.rotate90Right();
}

void moveForward () {
  switch (currentDirection) {
  case 1:
    posy++;
    break;
  case 2:
    posx++;
    break;
  case 3:
    posy--;
    break;
  case 4:
    posx--;
    break;
  default:
    break;
  }
  drive.goStraightTile(1);
}

void uTurn () {
  switch (currentDirection)
  {
  case 1:
    currentDirection = 3;
    break;
  case 2:
    currentDirection = 4;
    break;
  case 3:
    currentDirection = 1;
    break;
  case 4:
    currentDirection = 2;
    break;
  default:
    break;
  }
  drive.rotate180Right();
}

//Single move function
void advance (int targetDirection) {
  if (targetDirection != currentDirection) {
    changeOrientation(targetDirection);
  }
  moveForward();
}

//Rotates the robot to specified orientation
void changeOrientation (int targetDirection) {
  //Can I use modulus here?
  int plus = currentDirection + 1;
  int minus = currentDirection - 1;
  if (plus == 5) {
    plus = 1;
  }
  if (plus == 0) {
    plus = 4;
  }

  if (plus == targetDirection) {
    turnRight;
  }
  else if (minus == targetDirection) {
    turnLeft;
  }
  else {
    uTurn;
  }
}

//Checks if golf balls rescue is complete
bool isRescueComplete() {
  if (sheep == 2) {
    return true;
  }
  else {
    return false;
  }
}

//Used during setup; determines the initial coordinates of the robot
void findInitialPosition() {
  //Find initial coordinates
  if (getDistanceLeft() != -1) { 
    posx = 1 + getDistanceLeft(); 
  }
  else if (getDistanceRight() != -1) { 
    posx = 8 - getDistanceRight(); 
  }
  else { 
    findLeftWall(); 
  }
}

void findLeftWall() {
  int offset = 0;
  turnLeft();
  while (getDistanceFront() == -1) {
    moveForward();
    offset++;
  }
  //Reads initial coordinates
  posx = getDistanceFront();
  posy = 1;
  //Recalculates home base
  homex = getDistanceFront() + offset;
  homey = 1;
  turnRight();
}

void goHome() {
  while (posx != homex) {
    int dx = homex - posx;
    if (dx > 0) {
      advance (2);
    }
    else if (dx < 0) {
      advance (4);
    }
    else if (dx = 0) {
      while (posy > 1) {
        advance (3);
      }
      return;
    }
  }
}

