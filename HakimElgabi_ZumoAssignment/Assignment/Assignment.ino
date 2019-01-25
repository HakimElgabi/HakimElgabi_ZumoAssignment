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
#define TURN_DURATION_1   100 // how long it takes to turn in ms
#define TURN_DURATION_2   90
#define DETECT_LINE       150 // threshold for detecting the borders


//declration of objects
ZumoMotors motors;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
ZumoBuzzer buzzer;
//variable declerations
int incomingByte;                             // a variable to read incoming serial data into
unsigned int sensor_values[NUM_SENSORS];      // declare number of sensors on the zumo
bool isAutoModeOn = false;                    // a bool for the automated movement
int calibratedValue[6];                       // the calibrated QTR_THRESHOLD of the black line
int corridorLength = 0;                       // counter to hold the corridor length
int corridorCounter = 0;                      // counter to hold the current corridor
int roomCounter = -1;                         // counter for the current room turn
bool personFound = false;

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
    if (incomingByte == 'a') {
      turnLeft(TURN_DURATION_2);
    }
    if (incomingByte == 'd') {
      turnRight(TURN_DURATION_1);
    }
    if (incomingByte == 'S') {
      zumoStop();
    }
    if (incomingByte == 'A') {
      automatedMovement();
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
    if (incomingByte == 's') {
      goForward(400);
    }
    if (incomingByte == 'e') {
      returnToTJunction();
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
  if ((sensor_values[0] >=  DETECT_LINE && sensor_values[5] >=  DETECT_LINE) ||
      (sensor_values[0] >=  DETECT_LINE && sensor_values[1] >=  DETECT_LINE) ||
      (sensor_values[4] >=  DETECT_LINE && sensor_values[4] >=  DETECT_LINE) ||
      sensor_values[1] >=  DETECT_LINE || sensor_values[2] >=  DETECT_LINE
      || sensor_values[4] >= DETECT_LINE || ERROR_COUNTER > 10)
  {
    // if any other sensors detect a wall, it stops the zumo
    goBackwards(DURATION);
    zumoStop();
    corridors[corridorCounter].setCorridorLength(corridorLength + 300); // The 300 is added to compensate for the Zumo's length
    corridorLength = 0;
    ERROR_COUNTER = 0;
    Serial.print("Zumo has reached the end of corridor");
    Serial.print(STRING_TERMINATOR);
    Serial.print(corridors[corridorCounter].getCorridorLength());
    Serial.print(STRING_TERMINATOR);
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

void scanRoom()
{
  personFound = false;
  motors.setSpeeds(200, 200);
  delay(450);
  //turn right
  for (int i = 0; i < 24 && objectFound == false; i++)
  {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(40);
    motors.setSpeeds(0, 0);
    //scan for object
    if (sonar.ping_cm() > 0)
    {
      //send message to GUI
      objectFound = true;
      roomScan = false;
      message = "Found a person at room ";
      Serial.print(message + (roomCounter + 1));
      Serial.print(STRING_TERMINATOR);
      break;
    }
  }
  //turn left similar to 180 degrees
  for (int i = 0; i < 48 && objectFound == false; i++)
  {
    motors.setSpeeds(-TURN_SPEED, +TURN_SPEED);
    delay(40);
    motors.setSpeeds(0, 0);
    if (sonar.ping_cm() > 0)
    {
      objectFound = true;
      roomScan = false;
      message = "Found a person at room ";
      Serial.print(message + (roomCounter + 1));
      Serial.print(STRING_TERMINATOR);
      break;
    }
  }
  if (objectFound == false)
  {
    Serial.print("No object detected!");
    delay(10);
  }
  Serial.print("Please drive me outside the room!");
  delay(10);
}

void returnToTJunction() {
  goForward(corridors[corridorCounter - 1]);
  Serial.print("Returning to TJunction");
  delay(10);
}

void mapCorridor (String type) {
  if (isAtTJunction == false) {
    ++corridorCounter;
    corridors[corridorCounter].setCorridorStatus(true);
    corridors[corridorCounter].setCorridorType(type);
  }
  if (isAtTJunction == true) {
    ++corridorCounter;
    corridors[corridorCounter].setCorridorStatus(true);
    corridors[corridorCounter].setCorridorType("T" + type);
  }
  roomCounter = -1;
}
