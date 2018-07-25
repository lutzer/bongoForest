import controlP5.*;
import processing.serial.*;

Serial serial;

int sleepMessage = 30;

// UI
int activeID = 0;
final int ctrYpos = 80;
final int ctrXMargin = 30;
final int ctrXpos = (600-400-ctrXMargin*3) / 2;
final int numIDs = 12;
final int tabFontSize = 14;
final int btnUpdateId = 777; 
ControlP5 cp5;

void setup() {
  size(600, 300);
  
  //TODO: detect port or choose one on UI
  String portName = Serial.list()[0];
  
  printArray(Serial.list());
  serial = new Serial(this, portName, 9600);
  
  // UI setup
  cp5 = new ControlP5(this);
  
  PFont pfont = createFont("Arial",tabFontSize,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,tabFontSize);
     
  cp5.getTab("default")
    .setHeight(30)
    .activateEvent(true)
    .setLabel("save/load")
    .setId(numIDs+1)
    .getCaptionLabel()
    .setFont(font)
    .setSize(tabFontSize);

  cp5.addTab("test")
    .setHeight(30)
    .activateEvent(true)
    .setLabel("Test")
    .getCaptionLabel()
    .setFont(font)
    .setSize(tabFontSize);
     
  for (int i = 0; i < numIDs; i++) {
    // create tab
    int id = i+1;
    String tabName = "tab"+id;
    cp5.addTab(tabName)
      .setHeight(30)
      .setWidth(30)
      .setLabel(""+id+"")
      .activateEvent(true)
      .setId(id)
      .getCaptionLabel()
      .setFont(font)
      .setSize(tabFontSize);
      
    createControllers(id, tabName);
  
  }
}

void draw() {
  background(0);
  
}

void createControllers(int id, String tabName) {
  // d = delayTime
  // c = delayChange
  // i = changeInterval
  // t = threshold
  String ctrNameD = "nb"+id+"-d";
  String ctrNameC = "nb"+id+"-c";
  String ctrNameI = "nb"+id+"-i";
  String ctrNameT = "nb"+id+"-t";
  
  cp5.addTextfield(ctrNameD)
    .setPosition(ctrXpos,ctrYpos)
    .setSize(100,20)
    .setInputFilter(Textfield.INTEGER)
    .setMin(0)
    .setText("1000")
    .setCaptionLabel("Delay Time (ms)");
  
  cp5.addTextfield(ctrNameC)
    .setPosition(ctrXpos + (100+ctrXMargin)*1, ctrYpos)
    .setSize(100,20)
    .setInputFilter(Textfield.INTEGER)
    .setDefaultValue(id*10)
    .setMin(0)
    .setText("200")
    .setCaptionLabel("Delay Change (ms)");
    
  cp5.addTextfield(ctrNameI)
    .setPosition(ctrXpos + (100+ctrXMargin)*2, ctrYpos)
    .setSize(100,20)
    .setInputFilter(Textfield.INTEGER)
    .setDefaultValue(id*10)
    .setMin(0)
    .setText("4000")
    .setCaptionLabel("Change Interval (ms)");
    
  cp5.addTextfield(ctrNameT)
    .setPosition(ctrXpos + (100+ctrXMargin)*3, ctrYpos)
    .setSize(100,20)
    .setInputFilter(Textfield.INTEGER)
    .setDefaultValue(id*10)
    .setMin(0)
    .setMax(255)
    .setText("0")
    .setCaptionLabel("Threshold");
   
  // upload btn
  String ctrNameUpload = "upload"+id;
  cp5.addButton(ctrNameUpload)
    //.setValue(0)
    .setPosition(ctrXpos,height-ctrYpos)
    .setSize(200,19)
    .setId(btnUpdateId)
    .setCaptionLabel("upload " + id);
   
   
  // arrange controller in each tabs
  cp5.getController(ctrNameD).moveTo(tabName);
  cp5.getController(ctrNameC).moveTo(tabName);
  cp5.getController(ctrNameI).moveTo(tabName);
  cp5.getController(ctrNameT).moveTo(tabName);
  cp5.getController(ctrNameUpload).moveTo(tabName);

}

void controlEvent(ControlEvent theControlEvent) {
  if (theControlEvent.isTab()) {
    activeID = theControlEvent.getTab().getId();
    //println("got an event from tab : "+theControlEvent.getTab().getName()+
    //  " with id "+theControlEvent.getTab().getId());
  } else {
    int id = theControlEvent.getController().getId();
    if (id == btnUpdateId && activeID < 13 && activeID > 0) {
      processControllersByTabId(activeID);
    }
  } 
}

void processControllersByTabId(int id) {
  String ctrNameD = "nb"+id+"-d";
  String ctrNameC = "nb"+id+"-c";
  String ctrNameI = "nb"+id+"-i";
  String ctrNameT = "nb"+id+"-t";
  int value = 0;
  
  value = parseInt(cp5.get(Textfield.class,ctrNameD).getText());
  sendSerialMessage('d', value, id);
  
  value = parseInt(cp5.get(Textfield.class,ctrNameC).getText());
  sendSerialMessage('c', value, id);
  
  value = parseInt(cp5.get(Textfield.class,ctrNameI).getText());
  sendSerialMessage('i', value, id);
  
  value = parseInt(cp5.get(Textfield.class,ctrNameT).getText());
  sendSerialMessage('t', value, id);
}

void saveSettings(String name) {
    cp5.getProperties().saveSnapshot(name);
}
void loadSettings(String name) {
    cp5.getProperties().load(name);
}
void displaySnapshots() {
    println(cp5.getProperties().getSnapshotIndices());
}
void sendSerialMessage(char type, int value, int id) {
  println(type+" "+value+" "+id);
  serial.write(type+","+value+","+id+";");
  delay(sleepMessage); // wait for serial
} 
