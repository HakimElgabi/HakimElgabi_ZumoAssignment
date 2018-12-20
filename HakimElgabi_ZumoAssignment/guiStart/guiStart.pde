import processing.serial.*;
import java.util.ArrayList;
import g4p_controls.*;

int bgcol = 15;
int tempNumb;
Serial myPort;
String val;
GCustomSlider sdr4;

public void setup() {
  size(500, 320);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  createGUI();
}

public void draw() {
  background(15);
  fill(227, 230, 255);
  modify();
  while (myPort.available() > 0) 
  {
    val = myPort.readStringUntil('!');
    if (val != null) {
      if (val.startsWith("R"))
      {
        showRoom.setText(val);
      } else if (val.startsWith("Z"))
      {
        showText.appendText(val);
      } else
      {
        showText.appendText(val);
        //int tempNumb = Integer.parseInt(val);
        //showText.appendText("Corridor length " + Integer.toString(tempNumb));
      }
    }
  }
}

public void modify()
{
  showCommands.setTextBold();
  showText.setTextBold();
  showRoom.setTextBold();
}

public class Map extends ArrayList<Corridor> {
  public Map() {
    super();
  }

  public void addCorridor(Corridor aCorridor) {
    super.add(aCorridor);
  }
}

public class Corridor extends ArrayList<Room> {

  private int corridorLength;

  public Corridor() {
    super();
  }

  public void addCorridor(Room aRoom) {
    super.add(aRoom);
  }

  public void setCorridorLength(int measurement) {
    corridorLength = measurement;
  }
}

public class Room {
  private char RoomSide;
  private int position;
  private int roomOrder;

  public Room(char side, int pos, int order) {
    RoomSide = side;
    position = pos;
    roomOrder = order;
  }

  public char getSide() {
    return RoomSide;
  }

  public int getPos() {
    return position;
  }
  
  public int getOrder() {
    return roomOrder;
  }
}
