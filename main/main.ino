int receiver[n];
int infrared[n];
int currentDirection;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly: 
  readingUpdate();
  int direction = determinDirection();
  int steps = determineSteps(direction, ??)
}

void updateReadings() {
 for (all beaconReceivers) {
 receiver[n] <- receiver.driver.distance
 } 
 
 for (all infraredSensors) {
   infrared[n] <- infrared.driver.distance
 }
}

int determineDirection() {
  int maximum1 = 0;
  int maximum2 = 0;
 for (all beaconReceivers) {
   if (receiver[n] > maximum1) { maximum1 = receiver[n]; }
   else if (receiver[n] > maximum2) {maximum2 = receiver[n]; }
 }
}

int determineSteps(int direction, int distance) {
  if (infrared[direction] < distance) {
    return infrared[direction];
  }
  
  else return distance;
}

void move (int direction, int distance) {
  int difference = currentDirection - direction;
  switch difference {
    case 0:
    break;
    case 1:
    turnLeft;
    break;
    case 2:
    turnLeft; turnLeft;
    break;
    case 3;
    turnLeft; turnLeft; turnLeft;
    break;
    case 4;
    turnRight;
    break;
  }
  for (distance) {
    moveForward;
  }
}
