/*------------------------------------------------------------------------
  external libraries
  -------------------------------------------------------------------------*/
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define NUM_SENSORS        6  // the number of sensors attached to robot
#define FORWARD_SPEED     130 // how fast the robot can go forward
#define REVERSE_SPEED     150 // how fast it takes to revsere
#define TURN_SPEED        150 // how fast it takes to turn
#define DURATION          150 // how long it takes to turn in ms
#define TURN_DURATION_1   150 // how long it takes to turn in ms
#define DETECT_LINE       150 // threshold for detecting the borders


//declration of objects
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
ZumoBuzzer buzzer;
//variable declrations
int incomingByte;                             // a variable to read incoming serial data into
unsigned int sensor_values[NUM_SENSORS];      // declare number of sensors on the zumo
bool isAutoModeOn = false;                    // a bool for the automated movement
int calibratedValue[6];                       // the calibrated QTR_THRESHOLD of the black line
int corridors[6] = {0, 0, 0, 0, 0, 0};                             // array to hold the lengths of corridors
char turns[6] = {'U', 'U', 'U', 'U', 'U', 'U'};                                // array to hold the turns
int corridorLength = 0;
int corridorCounter = 0;
int turnCounter = -1;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  buzzer.play(">g32>>c32");           // Play a little welcome song
}

void loop() {
  checkInput();
}

void checkInput() {
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'F') {
      goForward(DURATION);
    }
    if (incomingByte == 't') {
      //      sensors.init();                     // Initialize the reflectance sensors module
      calibrateZumo();
      for (int i = 0; i < NUM_SENSORS; i++)
      {
        calibratedValue[i] = sensors.calibratedMaximumOn[i];
      }
    }
    if (incomingByte == 'B') {
      goBackwards(DURATION);
    }
    if (incomingByte == 'L') {
      turnLeft(TURN_DURATION_1);
    }
    if (incomingByte == 'R') {
      turnRight(TURN_DURATION_1);
    }
    if (incomingByte == 'S') {
      zumoStop();
    }
    if (incomingByte == 'A') {
      automatedMovement();
    }
    if (incomingByte == 'r') {
      ++turnCounter;
      turns[turnCounter] = 'r';
    }
    if (incomingByte == 'l') {
      ++turnCounter;
      turns[turnCounter] = 'l';
    }
  }
}

void goForward(int duration) {
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  delay(duration);
  motors.setSpeeds(0, 0);
  if (isAutoModeOn == true)
    corridorLength = corridorLength + duration;
}

void goBackwards(int duration) {
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(duration);
  motors.setSpeeds(0, 0);
}

void turnLeft(int turnDuration) {
  motors.setSpeeds(-REVERSE_SPEED, REVERSE_SPEED);
  delay(turnDuration);
  motors.setSpeeds(0, 0);
}

void turnRight(int turnDuration) {
  motors.setSpeeds(REVERSE_SPEED, -REVERSE_SPEED);
  delay(turnDuration);
  motors.setSpeeds(0, 0);
}
void zumoStop() {
  isAutoModeOn = false;
  motors.setSpeeds(0, 0);
}

bool detectWall() {
  sensors.read(sensor_values);
  if (sensor_values[1] >=  DETECT_LINE || sensor_values[2] >=  DETECT_LINE
      || sensor_values[4] >= DETECT_LINE)
  {
    // if any other sensors detect a wall, it stops the zumo
    goBackwards(DURATION);
    zumoStop();
    isAutoModeOn = false;
    corridors[corridorCounter] = corridorLength;
    corridorLength = 0;
    ++corridorCounter;
    return true;
  }
  else
    return false;
}

void detectCorridor() {
  sensors.read(sensor_values);
  if (sensor_values[0] >= DETECT_LINE)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    goBackwards(DURATION);
    turnRight(TURN_DURATION_1);
  }

  if (sensor_values[5] >=  DETECT_LINE)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    goBackwards(DURATION);
    turnLeft(TURN_DURATION_1);
  }
}

void automatedMovement() {
  isAutoModeOn = true;
  while (isAutoModeOn == true) {
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      if (incomingByte == 'S') {
        zumoStop();
      }
    }
    if (!isAutoModeOn)
      break;
    sensors.read(sensor_values);
    goForward(DURATION);
    if (detectWall())
      break;
    detectCorridor();
  }
}

void calibrateZumo() {
  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  delay(1000);

  //loop used for rotating the zumo in place to sweep the sensors over the line
  for (int i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    sensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);   //stop it from moving

  // Turn off LED and play buzzer to indicate we afre through with calibration
  digitalWrite(13, LOW);
  buzzer.play(">g32>>c32");
}
