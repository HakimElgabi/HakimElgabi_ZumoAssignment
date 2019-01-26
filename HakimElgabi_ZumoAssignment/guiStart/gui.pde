/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.

 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
     // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void leftClick(GButton source, GEvent event) { //_CODE_:leftBtn:308221:
  myPort.write('a');
  showCommands.setText("Zumo moved left");
} //_CODE_:leftBtn:308221:

public void rightClick(GButton source, GEvent event) { //_CODE_:rightBtn:604378:
  myPort.write('d');
  showCommands.setText("Zumo moved right");
} //_CODE_:rightBtn:604378:

public void forwardClick(GButton source, GEvent event) { //_CODE_:forwardBtn:348760:
  myPort.write('F');
  showCommands.setText("Zumo moved forward");
} //_CODE_:forwardBtn:348760:

public void backClick(GButton source, GEvent event) { //_CODE_:backBtn:984108:
   myPort.write('B');
   showCommands.setText("Zumo is reversing");
} //_CODE_:backBtn:984108:

public void stopClick(GButton source, GEvent event) { //_CODE_:stopBtn:713639:
  myPort.write('S');
  showCommands.setText("Stop Zumo");
  roomLeftBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  roomRightbtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  roomLeftBtn.setEnabled(true);
  roomRightbtn.setEnabled(true);
} //_CODE_:stopBtn:713639:

public void autoAction(GButton source, GEvent event) { //_CODE_:autoBtn:225239:
  myPort.write('A');
  showCommands.setText("Automated Mode Activated");
  roomLeftBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  roomRightbtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  roomLeftBtn.setEnabled(false);
  roomRightbtn.setEnabled(false);

} //_CODE_:autoBtn:225239:

public void showCommandsAction(GTextField source, GEvent event) { //_CODE_:showCommands:358095:
} //_CODE_:showCommands:358095:

public void calibrateAction(GButton source, GEvent event) { //_CODE_:calibrateBtn:772610:
 myPort.write('z');
 showCommands.setText("Calibrating zumo!");
} //_CODE_:calibrateBtn:772610:

public void roomLeftAction(GButton source, GEvent event) { //_CODE_:roomLeftBtn:308807:
 myPort.write('l');
 showCommands.setText("Room to the left found");
 roomLeftBtn.setEnabled(false);
 roomRightbtn.setEnabled(false);
 roomLeftBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
 roomRightbtn.setLocalColorScheme(GCScheme.RED_SCHEME);
} //_CODE_:roomLeftBtn:308807:

public void showTextAction(GTextArea source, GEvent event) { //_CODE_:showText:987080:
} //_CODE_:showText:987080:

public void roomRightAction(GButton source, GEvent event) { //_CODE_:roomRightbtn:427325:
 myPort.write('r');
 showCommands.setText("Room to the right found");
 roomLeftBtn.setEnabled(false);
 roomRightbtn.setEnabled(false);
 roomLeftBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
 roomRightbtn.setLocalColorScheme(GCScheme.RED_SCHEME);
} //_CODE_:roomRightbtn:427325:

public void scanAction(GButton source, GEvent event) { //_CODE_:scanRoomBtn:963136:
 myPort.write('s');
 showCommands.setText("Scanning Room");
} //_CODE_:scanRoomBtn:963136:

public void endButtonAction(GButton source, GEvent event) { //_CODE_:end:200236:
    myPort.write('e');
    showCommands.setText("Starting return Journey");
 } //_CODE_:end:200236:

public void junctionAction(GButton source, GEvent event) { //_CODE_:junctionBtn:827796:
  myPort.write('t');
  showCommands.setText("Begining return journey Room");
} //_CODE_:junctionBtn:827796:

public void ReturnTJunctionAction(GButton source, GEvent event) { //_CODE_:ReturnTJunction:709279:
    myPort.write('E');
    showCommands.setText("Returning to T-Junction");
} //_CODE_:ReturnTJunction:709279:

public void RightCorridorAction(GButton source, GEvent event) { //_CODE_:RightCorridorBtn:999615:
  myPort.write('R');
  showCommands.setText("Mapping corridor to the right");
} //_CODE_:RightCorridorBtn:999615:

public void LeftCorridorAction(GButton source, GEvent event) { //_CODE_:LeftCorridorBtn:928367:
  myPort.write('L');
  showCommands.setText("Mapping corridor to the left");
} //_CODE_:LeftCorridorBtn:928367:

public void configureBtnAction(GButton source, GEvent event) { //_CODE_:configureBtn:936868:
   window1.setVisible(true);
} //_CODE_:configureBtn:936868:

synchronized public void win_draw1(PApplet appc, GWinData data) { //_CODE_:window1:427167:
  appc.background(230);
} //_CODE_:window1:427167:

public void UpTurnSpeedRightAction(GButton source, GEvent event) { //_CODE_:UpTurnSpeedRight:910588:
  myPort.write('U');
} //_CODE_:UpTurnSpeedRight:910588:

public void lowerTurnSpeedRightAction(GButton source, GEvent event) { //_CODE_:lowerTurnSpeedRight:217258:
  myPort.write('M');
} //_CODE_:lowerTurnSpeedRight:217258:

public void textfield1_change1(GTextField source, GEvent event) { //_CODE_:showTurnSpeedRight:630943:
} //_CODE_:showTurnSpeedRight:630943:

public void testButtonAction(GButton source, GEvent event) { //_CODE_:testButton:660332:
  myPort.write('T');
} //_CODE_:testButton:660332:

public void testLButtonAction(GButton source, GEvent event) { //_CODE_:testLButton:654967:
  myPort.write('K');
} //_CODE_:testLButton:654967:

public void upTurnSpeedLeftAction(GButton source, GEvent event) { //_CODE_:upTurnSpeedLeft:846599:
  myPort.write('u');
} //_CODE_:upTurnSpeedLeft:846599:

public void lowerTurnSpeedLeftAction(GButton source, GEvent event) { //_CODE_:lowerTurnSpeedLeft:500092:
  myPort.write('m');
} //_CODE_:lowerTurnSpeedLeft:500092:

public void textfield1_change2(GTextField source, GEvent event) { //_CODE_:TurnSpeedLeft:580209:
} //_CODE_:TurnSpeedLeft:580209:

public void slowButtonAction(GButton source, GEvent event) { //_CODE_:slowButton:695311:
  myPort.write('1');
  speedField.setText("Slow");
} //_CODE_:slowButton:695311:

public void normalBtnAction(GButton source, GEvent event) { //_CODE_:normalBtn:663480:
  myPort.write('2');
  speedField.setText("Normal");
} //_CODE_:normalBtn:663480:

public void fastBTnAction(GButton source, GEvent event) { //_CODE_:fastBtn:279605:
  myPort.write('3');
  speedField.setText("Fast");
} //_CODE_:fastBtn:279605:

public void textfield1_change3(GTextField source, GEvent event) { //_CODE_:speedField:403540:
  println("textfield1 - GTextField >> GEvent." + event + " @ " + millis());
} //_CODE_:speedField:403540:



// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI(){
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.ORANGE_SCHEME);
  G4P.setCursor(ARROW);
  surface.setTitle("Sketch Window");
  leftBtn = new GButton(this, 10, 73, 80, 30);
  leftBtn.setText("Left");
  leftBtn.setTextBold();
  leftBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  leftBtn.addEventHandler(this, "leftClick");
  rightBtn = new GButton(this, 191, 75, 80, 30);
  rightBtn.setText("Right");
  rightBtn.setTextBold();
  rightBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  rightBtn.addEventHandler(this, "rightClick");
  forwardBtn = new GButton(this, 99, 39, 80, 30);
  forwardBtn.setText("Forward");
  forwardBtn.setTextBold();
  forwardBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  forwardBtn.addEventHandler(this, "forwardClick");
  backBtn = new GButton(this, 100, 113, 80, 30);
  backBtn.setText("Back");
  backBtn.setTextBold();
  backBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  backBtn.addEventHandler(this, "backClick");
  stopBtn = new GButton(this, 100, 75, 80, 30);
  stopBtn.setText("Stop");
  stopBtn.setTextBold();
  stopBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  stopBtn.addEventHandler(this, "stopClick");
  label1 = new GLabel(this, 20, 152, 140, 20);
  label1.setText("Messages from Zumo:");
  label1.setTextBold();
  label1.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  label1.setOpaque(false);
  autoBtn = new GButton(this, 410, 38, 80, 30);
  autoBtn.setText("Auto Move");
  autoBtn.setTextBold();
  autoBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  autoBtn.addEventHandler(this, "autoAction");
  label2 = new GLabel(this, 283, 168, 160, 20);
  label2.setText("Commands sent to Zumo:");
  label2.setTextBold();
  label2.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  label2.setOpaque(false);
  showCommands = new GTextField(this, 282, 187, 180, 30, G4P.SCROLLBARS_NONE);
  showCommands.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  showCommands.setOpaque(true);
  showCommands.addEventHandler(this, "showCommandsAction");
  calibrateBtn = new GButton(this, 314, 38, 80, 30);
  calibrateBtn.setText("Calibrate");
  calibrateBtn.setTextBold();
  calibrateBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  calibrateBtn.addEventHandler(this, "calibrateAction");
  roomLeftBtn = new GButton(this, 511, 123, 80, 30);
  roomLeftBtn.setText("Room to Left");
  roomLeftBtn.setTextBold();
  roomLeftBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  roomLeftBtn.addEventHandler(this, "roomLeftAction");
  showText = new GTextArea(this, 20, 170, 250, 141, G4P.SCROLLBARS_VERTICAL_ONLY | G4P.SCROLLBARS_AUTOHIDE);
  showText.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  showText.setOpaque(true);
  showText.addEventHandler(this, "showTextAction");
  roomRightbtn = new GButton(this, 606, 123, 80, 30);
  roomRightbtn.setText("Room to Right");
  roomRightbtn.setTextBold();
  roomRightbtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  roomRightbtn.addEventHandler(this, "roomRightAction");
  scanRoomBtn = new GButton(this, 409, 78, 80, 30);
  scanRoomBtn.setText("Scan Room");
  scanRoomBtn.setTextBold();
  scanRoomBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  scanRoomBtn.addEventHandler(this, "scanAction");
  sketchPad1 = new GSketchPad(this, 210, 130, 80, 60);
  end = new GButton(this, 409, 119, 80, 30);
  end.setText("End");
  end.setTextBold();
  end.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  end.addEventHandler(this, "endButtonAction");
  junctionBtn = new GButton(this, 559, 74, 80, 30);
  junctionBtn.setText("T-Junction");
  junctionBtn.setTextBold();
  junctionBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  junctionBtn.addEventHandler(this, "junctionAction");
  ReturnTJunction = new GButton(this, 315, 77, 80, 40);
  ReturnTJunction.setText("Return to the T Junction");
  ReturnTJunction.setTextBold();
  ReturnTJunction.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  ReturnTJunction.addEventHandler(this, "ReturnTJunctionAction");
  label4 = new GLabel(this, 557, 12, 80, 19);
  label4.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label4.setText("Mapping");
  label4.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  label4.setOpaque(false);
  RightCorridorBtn = new GButton(this, 605, 37, 80, 30);
  RightCorridorBtn.setText("Corridor to Right");
  RightCorridorBtn.setTextBold();
  RightCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  RightCorridorBtn.addEventHandler(this, "RightCorridorAction");
  LeftCorridorBtn = new GButton(this, 517, 37, 80, 30);
  LeftCorridorBtn.setText("Corridor to Left");
  LeftCorridorBtn.setTextBold();
  LeftCorridorBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  LeftCorridorBtn.addEventHandler(this, "LeftCorridorAction");
  label5 = new GLabel(this, 364, 12, 80, 20);
  label5.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label5.setText("Commands");
  label5.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  label5.setOpaque(false);
  label6 = new GLabel(this, 99, 12, 80, 20);
  label6.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label6.setText("Movement");
  label6.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  label6.setOpaque(false);
  configureBtn = new GButton(this, 610, 281, 80, 30);
  configureBtn.setText("Configure");
  configureBtn.setTextBold();
  configureBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  configureBtn.addEventHandler(this, "configureBtnAction");
  window1 = GWindow.getWindow(this, "Window title", 0, 0, 300, 300, JAVA2D);
  window1.noLoop();
  window1.setActionOnClose(G4P.CLOSE_WINDOW);
  window1.addDrawHandler(this, "win_draw1");
  UpTurnSpeedRight = new GButton(window1, 5, 8, 42, 32);
  UpTurnSpeedRight.setText("UP");
  UpTurnSpeedRight.setTextBold();
  UpTurnSpeedRight.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  UpTurnSpeedRight.addEventHandler(this, "UpTurnSpeedRightAction");
  lowerTurnSpeedRight = new GButton(window1, 57, 8, 45, 32);
  lowerTurnSpeedRight.setText("DOWN");
  lowerTurnSpeedRight.setTextBold();
  lowerTurnSpeedRight.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  lowerTurnSpeedRight.addEventHandler(this, "lowerTurnSpeedRightAction");
  label7 = new GLabel(window1, 105, 9, 120, 30);
  label7.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label7.setText("Current Turn Duration:");
  label7.setTextBold();
  label7.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  label7.setOpaque(false);
  showTurnSpeedRight = new GTextField(window1, 224, 9, 69, 30, G4P.SCROLLBARS_NONE);
  showTurnSpeedRight.setOpaque(true);
  showTurnSpeedRight.addEventHandler(this, "textfield1_change1");
  testButton = new GButton(window1, 153, 95, 50, 30);
  testButton.setText("Test R");
  testButton.setTextBold();
  testButton.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  testButton.addEventHandler(this, "testButtonAction");
  testLButton = new GButton(window1, 89, 94, 50, 30);
  testLButton.setText("Test L");
  testLButton.setTextBold();
  testLButton.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  testLButton.addEventHandler(this, "testLButtonAction");
  upTurnSpeedLeft = new GButton(window1, 4, 48, 42, 30);
  upTurnSpeedLeft.setText("UP");
  upTurnSpeedLeft.setTextBold();
  upTurnSpeedLeft.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  upTurnSpeedLeft.addEventHandler(this, "upTurnSpeedLeftAction");
  lowerTurnSpeedLeft = new GButton(window1, 55, 50, 50, 30);
  lowerTurnSpeedLeft.setText("DOWN");
  lowerTurnSpeedLeft.setTextBold();
  lowerTurnSpeedLeft.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  lowerTurnSpeedLeft.addEventHandler(this, "lowerTurnSpeedLeftAction");
  label8 = new GLabel(window1, 105, 51, 120, 27);
  label8.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label8.setText("Current Turn Duration: ");
  label8.setTextBold();
  label8.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  label8.setOpaque(false);
  TurnSpeedLeft = new GTextField(window1, 225, 50, 69, 30, G4P.SCROLLBARS_NONE);
  TurnSpeedLeft.setOpaque(true);
  TurnSpeedLeft.addEventHandler(this, "textfield1_change2");
  sketchPad2 = new GSketchPad(window1, 208, 199, 80, 60);
  slowButton = new GButton(window1, 3, 147, 80, 30);
  slowButton.setText("Slow");
  slowButton.setTextBold();
  slowButton.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  slowButton.addEventHandler(this, "slowButtonAction");
  normalBtn = new GButton(window1, 110, 148, 80, 30);
  normalBtn.setText("Normal");
  normalBtn.setTextBold();
  normalBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  normalBtn.addEventHandler(this, "normalBtnAction");
  fastBtn = new GButton(window1, 213, 148, 80, 30);
  fastBtn.setText("Fast");
  fastBtn.setTextBold();
  fastBtn.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  fastBtn.addEventHandler(this, "fastBTnAction");
  speedField = new GTextField(window1, 111, 189, 73, 20, G4P.SCROLLBARS_NONE);
  speedField.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  speedField.setOpaque(true);
  speedField.addEventHandler(this, "textfield1_change3");
  label3 = new GLabel(window1, 50, 189, 80, 20);
  label3.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label3.setText("Speed: ");
  label3.setOpaque(false);
  window1.loop();
}

// Variable declarations 
// autogenerated do not edit
GButton leftBtn; 
GButton rightBtn; 
GButton forwardBtn; 
GButton backBtn; 
GButton stopBtn; 
GLabel label1; 
GButton autoBtn; 
GLabel label2; 
GTextField showCommands; 
GButton calibrateBtn; 
GButton roomLeftBtn; 
GTextArea showText; 
GButton roomRightbtn; 
GButton scanRoomBtn; 
GSketchPad sketchPad1; 
GButton end; 
GButton junctionBtn; 
GButton ReturnTJunction; 
GLabel label4; 
GButton RightCorridorBtn; 
GButton LeftCorridorBtn; 
GLabel label5; 
GLabel label6; 
GButton configureBtn; 
GWindow window1;
GButton UpTurnSpeedRight; 
GButton lowerTurnSpeedRight; 
GLabel label7; 
GTextField showTurnSpeedRight; 
GButton testButton; 
GButton testLButton; 
GButton upTurnSpeedLeft; 
GButton lowerTurnSpeedLeft; 
GLabel label8; 
GTextField TurnSpeedLeft; 
GSketchPad sketchPad2; 
GButton slowButton; 
GButton normalBtn; 
GButton fastBtn; 
GTextField speedField; 
GLabel label3; 
