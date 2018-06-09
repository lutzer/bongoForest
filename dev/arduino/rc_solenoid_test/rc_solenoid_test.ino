#include <SPI.h>
#include <NRFLite.h>

#define SOLENOID_PIN 6

#define RADIO_CE_PIN 8
#define RADIO_CSN_PIN 10

NRFLite _radio;
uint8_t _data;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  _radio.init(1, RADIO_CE_PIN, RADIO_CSN_PIN);
}

// the loop function runs over and over again forever
void loop() {

  while (_radio.hasData())
   {
       _radio.readData(&_data);
   }

   if ( _data == 111) {
      digitalWrite(SOLENOID_PIN, HIGH);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(30);                       
      digitalWrite(SOLENOID_PIN, LOW);
      digitalWrite(LED_BUILTIN,LOW);
      _data = 0;
   }
}
