#include <motor.h>
#include <sensor.h>
#include <sheep_interrupts.h>

int distance[4];
int beacon[4];

int posx;
int posy;

int homex;
int homey;

int sheep;

/*
Forward facing direction of the robot
1 = North
2 = East
3 = South
4 = West
*/
int currentDirection;

void setup() {
  //We set the starting direction of the robot to 'North'; every change after this point will be the relative direction
  currentDirection = 1;
  
  //Variable declaration(s)
  sensor drive;
  sheep = 0;
  
  findInitialPosition();
}

void loop() {
  // put your main code here, to run repeatedly: 
  randomWalk();
  goHome();
}

void randomWalk() {
  while (y < 120) {
    if (isRescueComplete) {
      return;
    }
    moveForward();
    thicketSearch();
  }
  if (x < 80) {
    changeOrientation(2);
    while (x <120)
    {
      if (isRescueComplete) {
        return;
      }
      moveForward();
      thicketSearch();
    }
  else {
    changeOrientation(4);
    while (x > 40) {
      if (isRescueComplete) {
        return;
      }
      moveForward();
      thicketSearch();
  }
  while (y > 40) {
    if (isRescueComplete) {
      return;
    }
    moveForward();
    thicketSearch();
  }
}

void updateReadings() {
 for (all receivres) {
 receiver[n] <- receiver.driver.distance
 } 
 
 for (all infraredSensors) {
   infrared[n] <- infrared.driver.distance
 }
}

//Detects the presence of a beacon
int detectBeacon() {
  for (int i = 0; i <4; i++) {
    if (receiver[i] > 0) {
      return true;
    }
  }
  return false;
}

//If a beacon is present, search for the thicket
void thicketSearch() {
  int targetDirection = detectBeacon();
  if (targetDirection != 0) {
    while (sheep == 0) {
      changeOrientation(targetDirection);
      moveForward();
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

//Move functions with coordinates
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

void changeOrientation (int targetDirection) {
  if (currentDirection == targetDirection) {
    return;
  }
  switch (targetDirection) {
    case 1:
      switch (currentDirection) {
        case 2:
          turnLeft();
          break;
        case 3:
          uTurn();
          break;
        case 4();
          turnRight();
          break;
        default:
          break;
      }
    case 2:
      switch (currentDirection) {
        case 3:
          turnLeft();
          break;
        case 4:
          uTurn();
          break;
        case 1();
          turnRight();
          break;
        default:
          break;
      }
    case 3:
      switch (currentDirection) {
        case 4:
          turnLeft();
          break;
        case 1:
          uTurn();
          break;
        case 2();
          turnRight();
          break;
        default:
          break;
      }
    case 4:
      switch (currentDirection) {
        case 1:
          turnLeft();
          break;
        case 2:
          uTurn();
          break;
        case 3();
          turnRight();
          break;
        default:
          break;
      }
  }
}

bool isRescueComplete() {
  if (sheep == 2) {
    return true;
  }
  else {
    return false;
  }
}

void findInitialPosition() {
  //Find initial coordinates
  if (left and right sensors read nothing,) {
    turnLeft();
    offset = 0;
    while (front reads nothing,) {
      moveForward();
      offset++;
    }
    posx = front reading;
    posy = 1;
    
    homex = front reading + offset;
    homey = 1;
    
    turnRight();
  }
}
