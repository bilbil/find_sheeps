#include <sheep_interrupts.h>
#include <motor.h>
#include <sensor.h>
#include <beacon.h>

Beacon beaconDriver;
sensor drive;

long BaudRate = 4800;

unsigned long startTime;
unsigned long elapsedTime;

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

int travelled;

void setup() {
  Serial.begin(BaudRate);

  startTime = millis();

  // put your setup code here, to run once:
  //We set the starting direction of the robot to 'North'; every change after this point will be the relative direction
  currentDirection = 1;
  section = 1;
  //Variable declaration(s)
  drive = sensor();
  sheep = 0;
  travelled = 0;
  delay(2000);
  //findInitialPosition();

  //Tests if the correct x coordinate is returned
  findLeftWall();
  printHomeCoordinates();
  printCurrentCoordinates();

  //Tests orientation: Face east, face west, face south
  //test_orientationChange();

  //Tests if the sensors are updated according correctly according to its direction
  //test_directionalUpdates();

  //Tests updating of coordinates, bot should move in a square, and end up at the origin
  //test_advance();

  //Move somewhere in the maze, and goes home
  //test_findHome();
}

void loop() {
  // put your main code here, to run repeatedly: 
  //randomWalk();
  //for (int i = 0; i < 4; i++) {
  //  advance(2);
  //}
  //goHome();
  char incomingByte;

  int serialAvailable = Serial.available();
  if (serialAvailable > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(incomingByte == 'w')
    {
      advance(1);
    }
    else if(incomingByte == 'a')
    {
      advance(4);
    }
    else if(incomingByte == 'd')
    {
      advance(2);
    }
    else if (incomingByte == 's')
    {
      advance(4);
    }
    else if(incomingByte == 'h')
    {
      goHome();
    }
    else if(incomingByte == 'r')
    {
      randomWalk();
    }
  } 
}

void randomWalk() {
  switch(section) {
  case 1:
    while (posy < 8) {
      moveForward();
      thicketSearch();
      if (isRescueComplete) {
        return;
      }
    }
    section = 2;
    break;
    
  case 2:
    if (posx < 4) {
      changeOrientation(2);
      while (posx <6) {
        moveForward();
        thicketSearch();
        if (isRescueComplete) {
          return;
        }
      }
      section = 3;
    }
    else {
      changeOrientation(4);
      while (posx > 2) {
        moveForward();
        thicketSearch();
        if (isRescueComplete) {
          return;
        }
      }
      section = 3;
    }
    break;
  
  case 3:
    while (posy > 2) {
      moveForward();
      thicketSearch();
      if (isRescueComplete) {
        return;
      }
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
    distance[currentDirection][2] = -1;
    distance[currentDirection][3] = getDistanceLeft();
    break;
  case 2:
    distance[currentDirection][0] = getDistanceLeft();
    distance[currentDirection][1] = getDistanceFront();
    distance[currentDirection][2] = getDistanceRight();
    distance[currentDirection][3] = -1;
    break;
  case 3:
    distance[currentDirection][0] = -1;
    distance[currentDirection][1] = getDistanceLeft();
    distance[currentDirection][2] = getDistanceFront();
    distance[currentDirection][3] = getDistanceRight();
    break;
  case 4:
    distance[currentDirection][0] = getDistanceRight();
    distance[currentDirection][1] = -1;
    distance[currentDirection][2] = getDistanceLeft();
    distance[currentDirection][3] = getDistanceFront();
    break;
  default:
    break;
  }
}

//Updates the sensors; Since the sensors on the robot is relative to static North, proper assignment of sensor readings is done to capture data in the 4 directions
void updateBeacon() {
  switch (currentDirection) {
  case 1:
    beacon[currentDirection][0] = getBeaconFront();
    beacon[currentDirection][1] = getBeaconRight();
    beacon[currentDirection][2] = getBeaconBack();
    beacon[currentDirection][3] = getBeaconLeft();
    break;
  case 2:
    beacon[currentDirection][0] = getBeaconLeft();
    beacon[currentDirection][1] = getBeaconFront();
    beacon[currentDirection][2] = getBeaconRight();
    beacon[currentDirection][3] = getBeaconBack();
    break;
  case 3:
    beacon[currentDirection][0] = getBeaconBack();
    beacon[currentDirection][1] = getBeaconLeft();
    beacon[currentDirection][2] = getBeaconFront();
    beacon[currentDirection][3] = getBeaconRight();
    break;
  case 4:
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
  return drive.getLeftGrid();
}

int getDistanceFront() {
  return drive.getFrontGrid();
}

int getDistanceRight() {
  return drive.getRightGrid();
}

int getBeaconLeft() {
  return beaconDriver.getLeftBeacon();
}

int getBeaconFront() {
  return beaconDriver.getFrontBeacon();
}

int getBeaconRight() {
  return beaconDriver.getRightBeacon();
}

int getBeaconBack() {
  return beaconDriver.getBackBeacon();
}

//Detects the presence of a beacon; returns the direction in which the beacon is being received
int detectBeacon() {
  for(int i = 0; i < 4; i++) {
    if (beacon[currentDirection][i] > 0) {
      Serial.print("Debug: Beacon detected in direction ");
      Serial.println(i);
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
  drive.rotate90Right();
  drive.rotate90Right();
}

//Single move function
void advance (int targetDirection) {
  if (targetDirection != currentDirection) {
    changeOrientation(targetDirection);
  }
  moveForward();
  travelled++;
  Serial.println("Current");
  Serial.println(posx);
  Serial.println(posy);
  Serial.println("\n");
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
    turnRight();
  }
  else if (minus == targetDirection) {
    turnLeft();
  }
  else {
    uTurn();
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
  Serial.print("Home:");
  Serial.print(homex);
  Serial.println(homey);
}

void findLeftWall() {
  int offset = 0;
  turnLeft();
  Serial.println(currentDirection);
  while (getDistanceFront() > 1) {
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
//    else if (dx = 0) {
//      while (posy > 1) {
//        advance (3);
//      }
//      return;
//    }
  }
  while(posy > homey)
  {
    advance(3);
  }
 return; 
}

void avoid() {
  if (getDistanceLeft() > getDistanceRight())
  {
    turnLeft();
    while (getDistanceRight == 0) {
      moveForward();
    }
    turnRight();
  }
  else
  {
    turnRight();
    while (getDistanceLeft == 0) {
      moveForward();
    }
    turnLeft();
  }
}

void calculateElapsedTime () {
  elapsedTime = millis() - startTime;
  Serial.println(elapsedTime);
}

void test_orientationChange () {
  changeOrientation(2);
  delay(1000);
  changeOrientation(4);
  delay(1000);
  changeOrientation(3);
}

void test_findHome () {
  homex = 1;
  homey = 1;
  advance(1);
  delay(1000);
  advance(1);
  delay(1000);
  advance(1);
  delay(2000);
  goHome();
}

void test_advance() {
  Serial.println("Starting test_advance");
  posx = 20;
  posy = 20;
  printCurrentCoordinates();
  delay(1000);
  advance(1); //posx = 20, posy = 21
  printCurrentCoordinates();
  delay(1000);
  advance(2); //posx = 21, posy = 21
  printCurrentCoordinates();
  delay(1000);
  advance(3); //posx = 21, posy = 20
  printCurrentCoordinates();
  delay(1000);
  advance(4); //posx = 20, posy = 20
  printCurrentCoordinates();
}

void test_directionalUpdates() {
  Serial.println("Starting test_directionalUpdates");
  Serial.println("Facing east");
  changeOrientation(2);
  //updateBeacon();
  updateReadings();
  printSensorReadings();
  delay(3000);

  Serial.println("Facing south");
  changeOrientation(3);
  //updateBeacon();
  updateReadings();
  printSensorReadings();
  delay(3000);

  Serial.println("Facing west");
  changeOrientation(4);
  //updateBeacon();
  updateReadings();
  printSensorReadings();
  delay(3000);

  Serial.println("Facing north");
  changeOrientation(1);
  //updateBeacon();
  updateReadings();
  printSensorReadings();
  delay(3000);
}

void printHomeCoordinates () {
  Serial.print("Home:");
  Serial.print("\t");
  Serial.print("x:");
  Serial.print(homex);
  Serial.print("\t");
  Serial.print("y:");
  Serial.println(homey);
}

void printCurrentCoordinates () {
  Serial.print("Current:");
  Serial.print("\t");
  Serial.print("x:");
  Serial.print(posx);
  Serial.print("\t");
  Serial.print("y:");
  Serial.print(posy);
  Serial.print("Direction");
  Serial.println(currentDirection);
}

void printSensorReadings () {
  Serial.print("North reading");
  Serial.println(distance[currentDirection][0]);

  Serial.print("East reading");
  Serial.println(distance[currentDirection][1]);

  Serial.print("South reading");
  Serial.println(distance[currentDirection][2]);

  Serial.print("West reading");
  Serial.println(distance[currentDirection][3]);
}



