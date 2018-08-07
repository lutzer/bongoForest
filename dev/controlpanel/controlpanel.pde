import controlP5.*;
import processing.serial.*;
import java.util.*;
import java.io.*;

Serial serial;

int sleepMessage = 30;

// UI
int activeID = 0;
boolean listenChange = true;
final int ctrYpos = 80;
final int ctrXMargin = 30;
final int ctrXpos = (600-400-ctrXMargin*3) / 2;
final int numIDs = 12;
final int tabFontSize = 14;
final int btnUpdateId = 777; 

final String saveName = "saveConfigName";
final String saveBtn = "saveConfigBtn";
final String loadName = "loadConfigName";

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
     
  // load / save
  cp5.getTab("default")
    .setHeight(30)
    .activateEvent(true)
    .setLabel("save/load")
    .setId(numIDs+1)
    .getCaptionLabel()
    .setFont(font)
    .setSize(tabFontSize);
      
  cp5.addTextfield(saveName)
    .setPosition(ctrXpos + 50, ctrYpos)
    .setSize(160,20)
    .setText("config_version_name")
    .setCaptionLabel("Save config version");
    
  cp5.addButton(saveBtn)
    .setPosition(ctrXpos + 50, ctrYpos + 50)
    .setSize(160,19)
    .setCaptionLabel("Save");
    
  /* add a ScrollableList, by default it behaves like a DropdownList */
  cp5.addScrollableList(loadName)
    .setPosition(width - ctrXpos - 250, ctrYpos)
    .setSize(200, 100)
    .setBarHeight(20)
    .setItemHeight(20)
    .setLabel("load saved version")
    .setType(ScrollableList.DROPDOWN);
     
  cp5.getController(saveName).moveTo("default");
  cp5.getController(saveBtn).moveTo("default");
  cp5.getController(loadName).moveTo("default");

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
  
  // update file list
  updateConfigFiles(sketchPath(""), ".json");
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
    } else
      if (theControlEvent.getController().getName() == saveBtn) {
        saveSettings(cp5.get(Textfield.class,saveName).getText());
      }
  }
}

void loadConfigName(int n) {
  if (listenChange) {
    listenChange = false;
    println("click");
    println( cp5.get(ScrollableList.class,loadName).getItem(n).get("value").toString() );
    loadSettings( cp5.get(ScrollableList.class,loadName).getItem(n).get("value").toString() );  
  } else {
    listenChange = true;
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
  println("Saving config file - "+name);
  cp5.getProperties().setSnapshot(name);
  cp5.getProperties().saveSnapshot(name);
  updateConfigFiles(sketchPath(""), ".json");
}

void loadSettings(String name) {
  println("Loading config file - "+name);
  cp5.getProperties().load(name);
}

void updateConfigFiles(final String path, final String format) {
  File dir = new File(path);
  //println("Searching on "+ dir);
  File [] files = dir.listFiles(new FilenameFilter() {
    @Override
    public boolean accept(File dir, String name) {
      return name.endsWith(format);
    }
  });
  
  cp5.get(ScrollableList.class,loadName).clear();
  
  for (File filename : files) {
    println("Reading config file - "+filename.getName());
    cp5.get(ScrollableList.class,loadName).addItem(filename.getName(), filename.getName());
  }
}

void sendSerialMessage(char type, int value, int id) {
  println(type+" "+value+" "+id);
  serial.write(type+","+value+","+id+";");
  delay(sleepMessage); // wait for serial
} 
