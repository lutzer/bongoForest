import processing.serial.*;
import java.util.Collections;

Serial serial;  // The serial port

int bufferSize = 300;
ArrayList<Integer> buffer = new ArrayList<Integer>(Collections.nCopies(bufferSize, 0));
ArrayList<Integer> hits = new ArrayList<Integer>(Collections.nCopies(bufferSize, 0));

// visuals
int margin = 40;
int hitRadius = 14;

// filtering
int threshold = 40;
long lastHitTime = 0;
long lastDebounceTime = 0;
int hitDelay = 70;
int hitListenDebounce = 30;
int maxHit = 0;

void setup() {
  size(800, 300);
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  serial = new Serial(this, Serial.list()[4], 9600);
  
  buffer.trimToSize();
}

void filterValue(int value) {
  int lastValue = buffer.get(buffer.size()-2);
  if (millis() > lastDebounceTime + hitListenDebounce && maxHit != 0) {
        hits.add(1); //<>//
        maxHit = 0;
        lastDebounceTime = 0;
        lastHitTime = millis();
  } else {
    if (value > threshold) {
      if (value >= lastValue && millis() > lastHitTime + hitDelay) { //<>//
        maxHit = value;
        lastDebounceTime = millis();
        hits.add(-2);
      } else {
        hits.add(-1);
      }
    } else
      hits.add(0);
  }
}

void readSerial() {
  int value = 0;
  while (serial.available() > 0) {
    //int inByte = serial.read();
    String read = serial.readStringUntil(10); 
    if (read != null) { 
      value = int(float(read)); 
    }
  }
  
  buffer.add(value);
  filterValue(value);
  
  if (buffer.size() > bufferSize) {
    buffer.remove(0);
    hits.remove(0);
  }
    
  buffer.trimToSize();
  hits.trimToSize();
}

void renderBuffer() {
  background(0);
  //println(buffer);
  
  float limit = map(threshold, 0, 190, 0, height - margin);
  stroke(0,250,180);
  line(0, height-limit - margin, width, height-limit - margin);
  
  int i = 0;
  for (int value: buffer) {
    stroke(255);
    float x = map(i, 0, bufferSize, 0, width);
    float y = map(value, 0, 255, 0, height - margin);
      
    if (value > threshold)
      stroke(255, 0, 0);
      
    line(x, height-margin, x, height-margin - y);
    
    pushStyle();
    noFill();
    switch( hits.get(i) ) {
      case -1: // ignored
        stroke(0,0,255);
        line(x - hitRadius/4,height - margin/4, x + hitRadius/4, height - margin/4);
        break;
      case -2: // max
        stroke(255,0,100);
        line(x - hitRadius/4,height - margin/4, x + hitRadius/4, height - margin/4);
        break;
      case 1: // hit
        stroke(255);
        ellipse(x, height - margin/2, hitRadius, hitRadius);
        break;
    }
    popStyle();
    i++;
  }
}

void draw() {
  readSerial();
  renderBuffer();
}
