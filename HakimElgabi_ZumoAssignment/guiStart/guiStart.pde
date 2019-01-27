// Complete GUI
import processing.serial.*;
import g4p_controls.*;

int bgcol = 15;
Serial myPort;
String val;
GCustomSlider sdr4;

public void setup() {
  // Initialise the UI
  size(700, 320);
  String portName = Serial.list()[0];
  myPort = new Serial(this,portName,9600);
  createGUI();
  window1.setVisible(false);
  speedField.setText("Normal");
  roomLeftBtn.setEnabled(false);
  roomRightbtn.setEnabled(false);
  roomLeftBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  roomRightbtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  RightCorridorBtn.setEnabled(false);
  LeftCorridorBtn.setEnabled(false);
  junctionBtn.setEnabled(false);
  RightCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  LeftCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  junctionBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
}

public void draw() {
  background(230);
  fill(1, 1, 1);
  init();

  while (myPort.available() > 0) 
   {
     val = myPort.readStringUntil('.');
     if (val != null) {
       if (val.startsWith("Start.")){
         // Initialise the GUI when it recieves the Start message 
         showText.setText("Start");
         roomLeftBtn.setEnabled(false);
         roomRightbtn.setEnabled(false);
         roomLeftBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         roomRightbtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         RightCorridorBtn.setEnabled(false);
         LeftCorridorBtn.setEnabled(false);
         junctionBtn.setEnabled(false);
         RightCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         LeftCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         junctionBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
       }
       else if (val.startsWith("Ri:")){
         // Set the textfield to the new value
         showTurnSpeedRight.setText(val);
       }
       else if (val.startsWith("Le:")){
         // Set the textfield to the new value
         TurnSpeedLeft.setText(val);
       }
       else if (val.startsWith("Cor")){ 
         // Set the buttons to red and stop them being used
         TurnSpeedLeft.appendText(val);
         RightCorridorBtn.setEnabled(false); 
         LeftCorridorBtn.setEnabled(false);
         junctionBtn.setEnabled(false);
         RightCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         LeftCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         junctionBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
         showText.appendText(val);
       }
       else
       {
         if (val.contains("Zumo has reached the end of corridor")){ 
           // Set the buttons to cyan and make them available
              RightCorridorBtn.setEnabled(true);
              LeftCorridorBtn.setEnabled(true);
              junctionBtn.setEnabled(true);
              RightCorridorBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
              LeftCorridorBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
              junctionBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
         }
         showText.appendText(val);
       }
     }
   }
}

public void init()
{
  // Initialise the textfields as bold
   showCommands.setTextBold();
   showText.setTextBold();
   showTurnSpeedRight.setTextBold();
   TurnSpeedLeft.setTextBold();
}
