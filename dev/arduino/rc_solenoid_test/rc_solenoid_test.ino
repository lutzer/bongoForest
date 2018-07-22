#include <SPI.h>
#include <NRFLite.h>

#define SOLENOID_PIN 6
#define LEDS_PIN 7

#define RADIO_CE_PIN 8
#define RADIO_CSN_PIN 10

NRFLite _radio;
uint8_t _data;

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDS_PIN, OUTPUT);

  _radio.init(1, RADIO_CE_PIN, RADIO_CSN_PIN);

  //test leds
  for (int i=0; i<5; i++) {
    triggerDrum(50);
    delay(500);
  }
  
}

// the loop function runs over and over again forever
void loop() {

  while (_radio.hasData())
  {
    _radio.readData(&_data);
  }

   if ( _data != 0) {
      int triggerTime = map(_data,0,255,0,50);
      triggerDrum(triggerTime);
      _data = 0;
   }
}

void triggerDrum(int triggerTime) {
  digitalWrite(SOLENOID_PIN, HIGH);
  digitalWrite(LED_BUILTIN,HIGH);
  digitalWrite(LEDS_PIN,HIGH);
  delay(triggerTime);                       
  digitalWrite(SOLENOID_PIN, LOW);
  digitalWrite(LED_BUILTIN,LOW);
  digitalWrite(LEDS_PIN,LOW);
}

