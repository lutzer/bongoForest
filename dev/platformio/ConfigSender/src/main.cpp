#include <Arduino.h>

#include <SPI.h>
#include <NRFLite.h>

#include "RfMessage.h"

// RF chip
/*
  Radio MISO -> Arduino 12 MISO
  Radio MOSI -> Arduino 11 MOSI
  Radio SCK  -> Arduino 13 SCK
  Radio CE   -> Any GPIO Pin (can be same as CSN)
  Radio CSN  -> Any GPIO Pin (pin 10 recommended)
  Radio IRQ  -> Any GPIO Pin (optional)Radio MISO
*/
#define RADIO_CE_PIN 8
#define RADIO_CSN_PIN 10

// #define RADIO_MASTER_ID 0
#define RADIO_SLAVE_ID 1
#define RADIO_CONFIG_ID 2

NRFLite _radio;

RfMessage *message;

const char serialTerminator = ';';
// const char *SerialMessageFormat = "%c%d,%c%d,%c%d";
const char *SerialMessageFormat = "%c,%d,%d";

String serialResponse = "";

void setup() {
  // pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  _radio.init(RADIO_CONFIG_ID, RADIO_CE_PIN, RADIO_CSN_PIN);
}

void loop() {
  if (Serial.available() > 0) {
    serialResponse = Serial.readStringUntil(serialTerminator);
    int   value, id; // values
    char  type; // parameters

    // Serial.println(" Getting | ");
    // Serial.println(serialResponse);

    // Convert from String Object to String.
    char buf[serialResponse.length()+1];
    serialResponse.toCharArray(buf, sizeof(buf));
    char *p = buf;

    // check the format
    if (sscanf(p, SerialMessageFormat, &type, &value, &id) == 3) {
      Serial.println(serialResponse);

      // send config message
      message = new RfMessage();
      message->type = type;
      message->value = value;
      message->id = id;
      _radio.send(RADIO_SLAVE_ID, message, sizeof(*message));

      delete message;
    }

  }

}
