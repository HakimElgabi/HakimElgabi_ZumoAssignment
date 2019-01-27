/*------------------------------------------------------------------------
  external libraries - Final
  -------------------------------------------------------------------------*/
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define NUM_SENSORS        6            // the number of sensors attached to robot
#define TURN_SPEED        150           // how fast it takes to turn
#define DURATION          100           // how long it takes to turn in ms
#define TURN_DURATION_1   100           // how long it takes to turn in ms
#define TURN_DURATION_2   90            // how long it takes to turn in ms
#define MAX_DISTANCE      50            // Maximum distance we want to ping for (in centimeters), depth of the rooms
#define TRIGGER_PIN        6            // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN           2            // Arduino pin tied to echo pin on the ultrasonic sensor.
#define STRING_TERMINATOR "."           // used as the last char to be sent over serial connection
const int ELEMENT_COUNT_MAX = 5;
const int ledPin = 13; // the pin that the LED is attached to


//declration of objects
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
ZumoBuzzer buzzer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// variable declaration
// Due to the seperate zumo wheels not being the same speed,
// I need to compensate with this by having different durations for ninety degree angles
int NINETY_DEGREE_TURN_RIGHT = 900;       // how long it takes to do a ninety degrees angle to the right
int NINETY_DEGREE_TURN_LEFT = 800;        // how long it takes to do a ninety degrees angle to the left
int FORWARD_SPEED = 130;           // how fast the robot can go forward
int REVERSE_SPEED = 150;          // how fast it takes to revsere
int ERROR_COUNTER = 0;          // Error counter to ensure the Zumo doesn't get stuck in corners
int DETECT_LINE = 200;          // threshold for detecting the black lines


//int ENTER_ROOM_DURATION = 400;
unsigned int sensor_values[NUM_SENSORS];  // declare number of sensors on the zumo
int calibratedValue[6];                   // the calibrated QTR_THRESHOLD of the black line
int incomingByte;                         // a variable to read incoming serial data into
int roomCounter = -1;                     // counter for the rooms on the current corridor
int corridorCounter = 0;                  // counter for the current corridor
int corridorLength = 0;                   // counter to count the length of the current corridor
bool isAutoModeOn = false;                // ensures that the robot does not start instantly aftar calibration
bool isReturnTrip = false;                // bool for whether or not the zumo is on automode
bool isPersonFound = false;               // bool for whether the zumo has found a person or not in the room
bool isAtTJunction = false;               // bool to tell whether or not the zumo is currently on the T-Junction
bool returnToJunctionBool = false;        // bool to tell zumo to the return to the T-Junction
char currentSide = 'U';                   // char to tell the zumo which side it is on in the T-Junction


//---------------------------------------------------------------------------------------------------
// Since the size of arrays need to be hard coded, each class has a variable called roomStatus or corridorStatus
// so that the 'true' size of the array can be determined depending on the numbers of rooms or corridors actually
// being used in the array
//---------------------------------------------------------------------------------------------------
class Room
{
  public:
    // Constructor
    Room();
    // Setters
    void setPersonFound(bool);
    void setRoomStatus(bool);
    void setRoomSide(char);
    void setRoomPos(int);
    void setRoomNum(int);
    void setCorridorNum(int);
    // Getters
    bool getRoomStatus();
    bool getPersonFound();
    char getRoomSide();
    int getRoomPos();
    int getRoomNum();
    int getCorridorNum();
  private:
    // Variables
    bool _roomStatus;
    bool _personInRoom;
    char _side ;
    int _pos;
    int _roomNum;
    int _corridorNum;
};

Room::Room() {
  _roomStatus = false;
  _personInRoom = false;
  _roomNum = 0;
  _corridorNum = 0;
  _side = 'U';
  _pos = 0;
}

void Room::setPersonFound(bool personInRoom) {
  _personInRoom = personInRoom;
}

void Room::setRoomStatus(bool roomStatus) {
  _roomStatus = roomStatus;
}

bool Room::getRoomStatus() {
  return _roomStatus;
}

void Room::setRoomSide(char side) {
  _side = side;
}
void Room::setRoomPos(int pos) {
  _pos = pos;
}
void Room::setRoomNum(int roomNum) {
  _roomNum = roomNum;
}

void Room::setCorridorNum(int corridorNum) {
  _corridorNum = corridorNum;
}

bool Room::getPersonFound() {
  return _personInRoom;
}

char Room::getRoomSide() {
  return _side;
}

int Room::getRoomPos() {
  return _pos;
}

int Room::getRoomNum() {
  return _roomNum;
}

int Room::getCorridorNum() {
  return _corridorNum;
}

class Corridor
{
  public:
    // Constructor
    Corridor();
    // Setter
    void setCorridorStatus (bool);
    void setCorridorLength(int);
    void setCorridorType(String);
    void setTurnPosition(int);
    void setNumOfRooms();
    void setPersonInCorridor();
    // Getters
    bool getCorridorStatus();
    int getCorridorLength();
    int getCorridorNumber();
    int getTurnPosition();
    String getCorridorType();
    int getNumOfRooms();
    bool getPersonInCorridor();
    // Rooms Array
    Room rooms[3] = {Room::Room(), Room::Room(), Room::Room()};   // Initialise the room array
  private:
    // Variables
    String _type;
    bool _corridorStatus;
    bool _personInCorridor;
    int _duration;
    int _corNum;
    int _turnPosition;
    int _numOfRooms;
};

Corridor::Corridor() {
  _corridorStatus = false;
  _corNum = corridorCounter;
  _personInCorridor = false;
  _type = "U";
  _duration = 0;
  _numOfRooms = 0;
}

void Corridor::setCorridorLength(int duration) {
  _duration = duration;
}

void Corridor::setCorridorStatus(bool corridorStatus) {
  _corridorStatus = corridorStatus;
}

bool Corridor::getCorridorStatus() {
  return _corridorStatus;
}

void Corridor::setCorridorType(String type) {
  _type = type;
}

void Corridor::setNumOfRooms() {
  ++_numOfRooms;
}

int Corridor::getCorridorLength() {
  return _duration;
}

int Corridor::getCorridorNumber() {
  return _corNum;
}

String Corridor::getCorridorType() {
  return _type;
}

int Corridor::getNumOfRooms() {
  return _numOfRooms;
}

void Corridor::setPersonInCorridor() {
  _personInCorridor = true;
}

bool Corridor::getPersonInCorridor() {
  return _personInCorridor;
}

Corridor corridors[6];

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  corridors[0].setCorridorStatus(true);
  sensors.init(QTR_NO_EMITTER_PIN);
  Serial.print("Start");                      // Singal the UI that it's ready to start
  Serial.print(STRING_TERMINATOR);
  Serial.print("Ri: ");                       // Send the Right turn duration to the UI
  Serial.print(NINETY_DEGREE_TURN_RIGHT);
  Serial.print(STRING_TERMINATOR);
  Serial.print("Le: ");                       // Send the Left turn duration to the UI
  Serial.print(NINETY_DEGREE_TURN_LEFT);
  Serial.print(STRING_TERMINATOR);
  pinMode(ledPin, OUTPUT);
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
    if (incomingByte == 'z') {
      //sensors.init();                     // Initialize the reflectance sensors module
      calibrateZumo();
      for (int i = 0; i < NUM_SENSORS; i++)
      {
        calibratedValue[i] = sensors.calibratedMaximumOn[i];
      }
    }
    if (incomingByte == 'B') {
      goBackwards(DURATION);
    }
    if (incomingByte == 'a') {
      turnLeft(TURN_DURATION_2);
    }
    if (incomingByte == 'd') {
      turnRight(TURN_DURATION_1);
    }
    if (incomingByte == 'S') {
      ERROR_COUNTER = 0;
      zumoStop();
    }
    if (incomingByte == 'A') {
      automatedMovement();
    }
    if (incomingByte == 'r') {
      ++roomCounter;          // Increment Room Counter
      Serial.print("Room to the right mapped");
      Serial.print(STRING_TERMINATOR);            //Send a message to the UI
      corridors[corridorCounter].rooms[roomCounter].setRoomStatus(true);
      corridors[corridorCounter].rooms[roomCounter].setRoomNum(roomCounter);
      corridors[corridorCounter].rooms[roomCounter].setRoomPos(corridorLength + 300);
      corridors[corridorCounter].rooms[roomCounter].setRoomSide('r');
      goForward(450);
    }
    if (incomingByte == 'l') {
      ++roomCounter;
      Serial.print("Room to the left mapped");
      Serial.print(STRING_TERMINATOR);            //Send a message to the UI
      corridors[corridorCounter].rooms[roomCounter].setRoomStatus(true);
      corridors[corridorCounter].rooms[roomCounter].setRoomNum(roomCounter);
      corridors[corridorCounter].rooms[roomCounter].setRoomPos(corridorLength + 300);
      corridors[corridorCounter].rooms[roomCounter].setRoomSide('l');
      goForward(450);
    }
    if (incomingByte == 'R') {
      Serial.print("Corridor to Right Mapped");
      Serial.print(STRING_TERMINATOR);
      mapCorridor("R");
    }
    if (incomingByte == 'L') {
      Serial.print("Corridor to left Mapped");
      Serial.print(STRING_TERMINATOR);
      mapCorridor("L");
    }
    if (incomingByte == 's') {
      scanRoom();
    }
    if (incomingByte == 'e') {
      returnRoute();
    }
    if (incomingByte == 'E') {
      returnToTJunction();
    }
    if (incomingByte == 'U') {
      // Increment current delay for a ninety degree turn
      NINETY_DEGREE_TURN_RIGHT = NINETY_DEGREE_TURN_RIGHT + 50;
      // Send the new delay to the UI
      Serial.print("Ri: ");
      Serial.print(NINETY_DEGREE_TURN_RIGHT);
      Serial.print(STRING_TERMINATOR);
    }
    if (incomingByte == 'M') {
      // Decrement delay for a ninety degree turn
      NINETY_DEGREE_TURN_RIGHT = NINETY_DEGREE_TURN_RIGHT - 50;
      // Send the new delay to the UI
      Serial.print("Ri: ");
      Serial.print(NINETY_DEGREE_TURN_RIGHT);
      Serial.print(STRING_TERMINATOR);
    }
    if (incomingByte == 'u') {
      // Increment current delay for a ninety degree turn
      NINETY_DEGREE_TURN_LEFT = NINETY_DEGREE_TURN_LEFT + 50;
      // Send the new delay to the UI
      Serial.print("Le: ");
      Serial.print(NINETY_DEGREE_TURN_LEFT);
      Serial.print(STRING_TERMINATOR);
    }
    if (incomingByte == 'm') {
      // Decrement delay for a ninety degree turn
      NINETY_DEGREE_TURN_LEFT = NINETY_DEGREE_TURN_LEFT - 50;
      // Send the new delay to the UI
      Serial.print("Le: ");
      Serial.print(NINETY_DEGREE_TURN_LEFT);
      Serial.print(STRING_TERMINATOR);
    }
    if (incomingByte == 't') {
      isAtTJunction = true;
      Serial.print("Zumo has arrived at T-Junction");
      Serial.print(STRING_TERMINATOR);
    }
    if (incomingByte == 'T') {
      turnRight(NINETY_DEGREE_TURN_RIGHT);
    }
    if (incomingByte == 'K') {
      turnLeft(NINETY_DEGREE_TURN_LEFT);
    }
    if (incomingByte == '1') {
      FORWARD_SPEED = 90;
      REVERSE_SPEED = 100;
    }
    if (incomingByte == '2') {
      FORWARD_SPEED = 130;
      REVERSE_SPEED = 150;
    }
    if (incomingByte == '3') {
      FORWARD_SPEED = 170;
      REVERSE_SPEED = 180;
    }
  }
}

void goForward(int duration) {
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  delay(duration);
  if (isAutoModeOn && !isReturnTrip)
    corridorLength = corridorLength + duration;     // Increment the corridorLenght counter by the duration
  motors.setSpeeds(0, 0);
}

void goBackwards(int duration) {
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(duration);
  if (isAutoModeOn && !isReturnTrip)
    corridorLength = corridorLength - (DURATION + 50); // Decrement the corridorLenght counter by the duration + 50
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
void spin (int turnDuration) {
  motors.setSpeeds(REVERSE_SPEED, -REVERSE_SPEED);
  delay(turnDuration);
  motors.setSpeeds(0, 0);
}
void zumoStop() {
  motors.setSpeeds(0, 0);
  isAutoModeOn = false;
}

bool detectWall() {
  sensors.read(sensor_values);
  if ((sensor_values[0] >=  DETECT_LINE && sensor_values[5] >=  DETECT_LINE) || // Check both the first and last sensor
      (sensor_values[0] >=  DETECT_LINE && sensor_values[1] >=  DETECT_LINE) || // Check both the 1st and 2nd sensor on the left hand side
      (sensor_values[4] >=  DETECT_LINE && sensor_values[4] >=  DETECT_LINE) || // Check both the 1st and 2nd sensor on the right hand side
      sensor_values[1] >=  DETECT_LINE || sensor_values[2] >=  DETECT_LINE      // Check sensors all the between the 1st and last separately
      || sensor_values[4] >= DETECT_LINE || ERROR_COUNTER > 10)                 // Error counter to ensure the Zumo doesn't get caught in an infinite loop in corners
  {
    // if any other sensors detect a wall, it stops the zumo
    goBackwards(DURATION);
    zumoStop();
    corridors[corridorCounter].setCorridorLength(corridorLength + 300); // The 300 is added to compensate for the Zumo's length
    corridorLength = 0;     // reset corridorLength counter
    ERROR_COUNTER = 0;      // reset ErrorCounter
    Serial.print("Zumo has reached the end of corridor");   // Send message to UI
    Serial.print(STRING_TERMINATOR);
    return true;
  }
  else
    return false;
}

bool detectCorridor() {
  sensors.read(sensor_values);
  bool returnBool = false;
  if (sensor_values[0] >= DETECT_LINE)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    goBackwards(DURATION);
    turnRight(TURN_DURATION_2);
    ++ERROR_COUNTER;
    returnBool = true;
  }

  if (sensor_values[5] >=  DETECT_LINE)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    goBackwards(DURATION);
    turnLeft(TURN_DURATION_2);
    ++ERROR_COUNTER;
    returnBool = true;
  }
  return returnBool;
}

void automatedMovement() {
  isAutoModeOn = true;  // set the automode to true
  while (isAutoModeOn == true) {
    // Check for the inputs to stop the zumo
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      if (incomingByte == 'S') {
        zumoStop();
      }
    }
    if (!isAutoModeOn)  // Break the loop if the bool is set to false
      break;
    sensors.read(sensor_values);
    goForward(DURATION);
    if (detectWall())   // Break the loop is detectWall function returns true
      break;
    detectCorridor();
  }
}

void scanRoom() {
  isPersonFound = false;    // Reset the bool to false
  //turn right
  for (int i = 0; i < NINETY_DEGREE_TURN_RIGHT; i = i + NINETY_DEGREE_TURN_RIGHT / 10)
  {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(NINETY_DEGREE_TURN_RIGHT / 10);
    motors.setSpeeds(0, 0);
    //scan for object
    detectPerson();
  }
  // Recentres the Zumo, this is for the automated return so that the zumo comes out of room straight rather than at an angle
  motors.setSpeeds(-TURN_SPEED, +TURN_SPEED);
  delay(NINETY_DEGREE_TURN_LEFT);
  motors.setSpeeds(0, 0);
  delay(1000);
  if (isPersonFound == false) {
    for (int i = 0; i < NINETY_DEGREE_TURN_LEFT; i = i + NINETY_DEGREE_TURN_LEFT / 10)
    {
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(NINETY_DEGREE_TURN_LEFT / 10);
      motors.setSpeeds(0, 0);
      detectPerson();
    }
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(NINETY_DEGREE_TURN_RIGHT);
    motors.setSpeeds(0, 0);
  }
  if (isPersonFound == false)
  {
    // Send a message to UI singalling which rooms and corridor the person was found in
    Serial.print("No person detected in room ");
    Serial.print(roomCounter + 1);
    Serial.print(" of corridor ");
    Serial.print(corridorCounter + 1);
    Serial.print(STRING_TERMINATOR);
    delay(10);
  }
  if (isPersonFound == true)
    personFound(); // Function to send message to the UI
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

void mapCorridor (String type) {
  if (isAtTJunction == false) {   // Checks that the Zumo is at a TJunction
    ++corridorCounter;
    corridors[corridorCounter].setCorridorStatus(true); // Sets the room to true so that the room is being used in the array
    corridors[corridorCounter].setCorridorType(type); // Sets the corridor as a regular R or L corridor
  }
  if (isAtTJunction == true) {
    ++corridorCounter;
    corridors[corridorCounter].setCorridorStatus(true);
    corridors[corridorCounter].setCorridorType("T" + type); // If the Zumo is at a T-Junction, it wil set the corridor type as TR or TL in the array
  }
  roomCounter = -1;
}

void detectPerson() {
  float distance = sonar.ping_cm();
  if (distance > 2 && distance < 24) { // Limits the range of censor to the room size
    isPersonFound = true;
  }
}

void personFound() {
  if (!isReturnTrip) { // Check whether the room is on it's return trip or not
    corridors[corridorCounter].setPersonInCorridor();
    corridors[corridorCounter].rooms[roomCounter].setPersonFound(true); // Set the room as being used in the array
    // Send Message to UI
    Serial.print("Person Found in room ");
    Serial.print(corridors[corridorCounter].rooms[roomCounter].getRoomNum() + 1);
    Serial.print(" in corridor ");
    Serial.print(corridorCounter + 1);
    Serial.print(STRING_TERMINATOR);
    delay(10);
  }
  if (isReturnTrip) {
    Serial.print("Follow me"); // Send message to UI
    Serial.print(STRING_TERMINATOR);
    digitalWrite(ledPin, HIGH); // Turn on LED
  }
}

void returnRoute() {
  // Set new local variables
  int numberOfCorridors = 0; // 'True' size of the Corridor array
  int numberOfRooms = 0;  // 'True' size of the rooms array
  int currentRoom = 0;    // Int to save the current room on the T-Junction to optimize the return rout
  bool leaveTJunction = false; // Bool to tell the Zumo to leave the T-Jucntion
  int forwardDuration;  // Int to save the forward duration for

  isReturnTrip = true;        // Set global variable isReturnTrip to true
  returnToJunctionBool = false; // Set global variable returnToJunctionBool to false

  Serial.print("Optimizing return route"); //Send message to UI telling it the return trip has started
  Serial.print(STRING_TERMINATOR);
  delay(1000);

  // For loop to determine the number of corridors being used in the corridors array
  for (int i = 0; i < 6; i++) {
    if (corridors[i].getCorridorStatus() == true)
      ++numberOfCorridors;
  }

  for (int i = numberOfCorridors - 1; i >= 0; i--) { // Set the index for this loop as the size of the corridors array
    Serial.print("At Corridor ");   // Signal which corridor it is currently in
    Serial.print(i + 1);
    Serial.print(STRING_TERMINATOR);
    forwardDuration = 0; // Reset the foward duration
    if (corridors[i].getPersonInCorridor() == false) {  // Check if the current corridor even has any people found in it
      if (isAtTJunction == false) { // Check if the Zumo is at the T-Junction
        forwardDuration = corridors[i].getCorridorLength(); // Set the forwardDuration to the full corridor length
      }
      // Check if Zumo is at the T-Junction and if it's at the second half of the T-Junction in the array so that it can skip the corridor since there are no people in this corridor
      if (isAtTJunction == true && i == numberOfCorridors - 2) {
        forwardDuration = 0;
        leaveTJunction = true;
        Serial.print("No people found on the otherside of the T-Junction, skipping corridor ");
        Serial.print(STRING_TERMINATOR);
      }
      // Check if Zumo is at the T-Junction and if it's at the first half of the T-Junction in the array so that it compensate for the Zumos length and stop in centre of T-Junction
      if (isAtTJunction == true && i == numberOfCorridors - 1) {
        forwardDuration = corridors[i].getCorridorLength() - 300;
        Serial.print("No people found in this Corridor returning to T-Junction");
        Serial.print(STRING_TERMINATOR);
      }
      // Loop used so that Zumo can go the full duration but also account for wall detection
      for (int f = forwardDuration; f > 0; f = f - DURATION) {
        if (f > 0) {
          goForward(DURATION);
          if (detectWall())
            break;
          if (detectCorridor())
            f = f + DURATION;
        }
        if (f == 0) {
          goForward(1);
        }
      }
    }
    numberOfRooms = 0; // reset room size
    for (int r = 0; r < 3; r++) { // Loop used to determine the 'true' size of the rooms array depending of whether the rooms are used
      if (corridors[i].rooms[r].getRoomStatus() == true)
        ++numberOfRooms;
    }
    if (numberOfRooms > 0 && corridors[i].getPersonInCorridor() == true) { // Check if people are in the corridor
      for (int r = numberOfRooms - 1; r > -2; r--) { // Loop through all the rooms in the array
        if (r == numberOfRooms - 1) { // If the room is at the top of the array
          // Check if the Zumo needs to return to the T-Junction to compensate for the Zumos length and enter the 1st room of the corridor rather than last
          if (returnToJunctionBool == true) {
            forwardDuration = corridors[i].rooms[0].getRoomPos() - 300;
          }
          if (returnToJunctionBool == false) {  // Set the forwardDuration the corridorLength minus the rooms Position in the corridor
            forwardDuration = corridors[i].getCorridorLength() - corridors[i].rooms[r].getRoomPos();
          }
          for (int f = forwardDuration; f > 0; f = f - DURATION) {
            goForward(DURATION);
            if (detectCorridor())
              f = f + DURATION;
          }
          // If the Zumo is at the second part of the T-Junction it will the variables at the top of the array
          if (returnToJunctionBool == true && corridors[i].rooms[r].getPersonFound() == true) {
            autoEnterRoom(i, 0);
          }
          else if (returnToJunctionBool == false && corridors[i].rooms[r].getPersonFound() == true) // // Check if the room has a person in it to determine if the Zumo should search it
            autoEnterRoom(i, r);
        }
        if (r == -1) { // Check if the array has finised and there are no rooms left in the corridor
          ERROR_COUNTER = 5;
          if (isAtTJunction == true && returnToJunctionBool == true) { // Check if the Zumo is on the T-Junction and if it's on the second part of the T-Junction
            forwardDuration = corridors[i].rooms[currentRoom].getRoomPos() - 300;
            leaveTJunction = true;
            // Set which side of the T-Junction the Zumo is at so it knows which way to turn
            if (corridors[numberOfCorridors - 2].getCorridorType() == "TR")
              currentSide = 'r';
            if (corridors[numberOfCorridors - 2].getCorridorType() == "TL")
              currentSide = 'l';
          }
          if (isAtTJunction == false) { // By this point the Zumo at the last room
            forwardDuration = corridors[i].rooms[0].getRoomPos();
          }
          if (isAtTJunction == true && returnToJunctionBool == false) { //
            forwardDuration = corridors[i].rooms[0].getRoomPos() - 300;
          }
          for (int f = forwardDuration; f > 0; f = f - DURATION) {
            goForward(DURATION);
            if (detectWall())
              break;
            if (detectCorridor())
              f = f + DURATION;
          }
        }
        if (r != numberOfRooms - 1 && r > -1) { // Check if the current room is not the top in the array and that the array has not finished
          if (isAtTJunction == true && returnToJunctionBool == true) { // Check that the Zumo is at the T-Junction and that it needs to return to it
            if (corridors[i].rooms[currentRoom + 1].getPersonFound() == true) { // if the next room has person in it the Zumo will move to it
              forwardDuration = corridors[currentRoom + 1].rooms[r].getRoomPos() - corridors[i].rooms[currentRoom].getPersonFound();
              ++currentRoom;
            }
            else // if not the Zumo will stay still
              forwardDuration = 0;
          }
          else
            forwardDuration = corridors[i].rooms[r + 1].getRoomPos() - corridors[i].rooms[r].getRoomPos(); // move to the next room in the array
          for (int f = forwardDuration; f > 0; f = f - DURATION) {
            goForward(DURATION);
            if (detectWall())
              break;
            if (detectCorridor())
              f = f + DURATION;
          }
          if (returnToJunctionBool == true && corridors[i].rooms[r].getPersonFound() == true && currentRoom != 0) {
            autoEnterRoom(i, currentRoom);
          }
          else if (returnToJunctionBool == false && corridors[i].rooms[r].getPersonFound() == true)
            autoEnterRoom(i, r);
        }
      }
    }
    if (corridors[i].getCorridorType() == "R") { // If the corridor has a right turn, it will reverse it
      Serial.print("Turning left onto new Corridor");
      Serial.print(STRING_TERMINATOR);
      turnLeft(NINETY_DEGREE_TURN_LEFT);
    }
    if  (corridors[i].getCorridorType() == "L") { // // If the corridor has a left turn, it will reverse it
      turnRight(NINETY_DEGREE_TURN_RIGHT);
      Serial.print("Turning right onto new Corridor");
      Serial.print(STRING_TERMINATOR);
    }
    if (corridors[i].getCorridorType() == "TR" || corridors[i].getCorridorType() == "LR") { // Wait at T-Junction
      delay(1000);
    }
    // Checks the second half of the T-Junction and proceeds to scan if there is someone there
    if (isAtTJunction == true && i == numberOfCorridors - 1 && corridors[numberOfCorridors - 2].getPersonInCorridor() == true) {
      delay(1000);
      returnToJunctionBool = true; // Set variable to true
      Serial.print("People found on opposite side of T-Junction, proceeding to scan");
      Serial.print(STRING_TERMINATOR);
    }
    if (leaveTJunction == true) { // Check if the Zumo needs to leave the T-Junction
      Serial.print("Leaving T-Junction");
      Serial.print(STRING_TERMINATOR);
      if (currentSide == 'r') { // If on the right side, turn left
        turnLeft(NINETY_DEGREE_TURN_LEFT);
        Serial.print("Turning Right");
        Serial.print(STRING_TERMINATOR);
      }
      if (currentSide == 'l') { // If on the left side, turn right
        turnRight(NINETY_DEGREE_TURN_RIGHT);
        Serial.print("Turning Left");
        Serial.print(STRING_TERMINATOR);
      }
      leaveTJunction = false;
      isAtTJunction = false;
      returnToJunctionBool = false;
    }
  }
  digitalWrite(ledPin, LOW); // Turn off LED
  Serial.print("Zumo has finished the return journey");
  Serial.print(STRING_TERMINATOR);
}

void returnToTJunction() {
  returnToJunctionBool = true;
  Serial.print("T-Junction");
  Serial.print(STRING_TERMINATOR);
  Serial.print("Zumo is on the ");
  Serial.print(corridorCounter);
  Serial.print(STRING_TERMINATOR);
  for (int f = corridors[corridorCounter].getCorridorLength() - 300; f > 0; f = f - DURATION) { // Go the duration of the T-Junction
    goForward(DURATION);
    if (detectCorridor())
      f = f + DURATION;
  }
  if (corridors[corridorCounter].getPersonInCorridor() == false) {
    Serial.print("Nobody in this corridor"); // Alert UI that nobody is in the corridor
    Serial.print(STRING_TERMINATOR);
  }
  if (corridors[corridorCounter].getCorridorType() == "TR" && returnToJunctionBool == true) {
    // Add the next corridor in the array
    ++corridorCounter;
    corridors[corridorCounter].setCorridorStatus(true);
    // Set Corridor Type as the opposite of the previous corridor
    corridors[corridorCounter].setCorridorType("TL");
    Serial.print("Now ready to explore the left side of the T-Junction");
    Serial.print(STRING_TERMINATOR);
    returnToJunctionBool = false;
    currentSide = 'l';
  }
  if (corridors[corridorCounter].getCorridorType() == "TL" && returnToJunctionBool == true) { // Same as before but inverted logic
    ++corridorCounter;
    corridors[corridorCounter].setCorridorStatus(true);
    corridors[corridorCounter].setCorridorType("TR");
    Serial.print("Now ready to explore the right side of the T-Junction");
    Serial.print(STRING_TERMINATOR);
    returnToJunctionBool = false;
    currentSide = 'r';
  }
  roomCounter = -1;   // Reset room counter
}

void autoEnterRoom (int i, int r) {
  ERROR_COUNTER = 0;
  if (corridors[i].rooms[r].getRoomSide() == 'r') {
    if (returnToJunctionBool == true) // If the Zumo is on the second half of the T-Junction, it will do the regular turn
      turnRight(NINETY_DEGREE_TURN_RIGHT);
    if (returnToJunctionBool == false) // If the Zumo is on any other corridor, it will do the opposite turn
      turnLeft(NINETY_DEGREE_TURN_LEFT);
  }
  if (corridors[i].rooms[r].getRoomSide() == 'l') {
    if (returnToJunctionBool == true)
      turnLeft(NINETY_DEGREE_TURN_LEFT);
    if (returnToJunctionBool == false)
      turnRight(NINETY_DEGREE_TURN_RIGHT);
  }
  // Enter the room and scan it
  goForward(400);
  scanRoom();
  goBackwards(300);
  if (corridors[i].rooms[r].getRoomSide() == 'r') {
    // If on the second part of the T-Junction it will turn to the next room if it has a person in it
    if (returnToJunctionBool == true && corridors[i].rooms[r + 1].getPersonFound() == true)
      turnLeft(NINETY_DEGREE_TURN_LEFT);
    if (returnToJunctionBool == true && corridors[i].rooms[r + 1].getPersonFound() == false)
      turnRight(NINETY_DEGREE_TURN_LEFT);
    if (returnToJunctionBool == false)
      turnRight(NINETY_DEGREE_TURN_RIGHT);
  }
  if (corridors[i].rooms[r].getRoomSide() == 'l') {
    if (returnToJunctionBool == true && corridors[i].rooms[r + 1].getPersonFound() == true)
      turnRight(NINETY_DEGREE_TURN_RIGHT);
    if (returnToJunctionBool == true && corridors[i].rooms[r + 1].getPersonFound() == false)
      turnLeft(NINETY_DEGREE_TURN_LEFT);
    if (returnToJunctionBool == false)
      turnLeft(NINETY_DEGREE_TURN_LEFT);
  }
}
