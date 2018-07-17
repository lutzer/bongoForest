#include <Arduino.h>

#include <SPI.h>
#include <NRFLite.h>

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

#define RADIO_MASTER_ID 0
#define RADIO_SLAVE_ID 1

NRFLite _radio;

// LED
#define LED_PIN 7
const uint16_t LEDDuration = 150;
unsigned long lastLEDTime = 0;

// piezoelectric
#define SENSOR_PIN A0

// filter

#define FILTERED_SIGNAL
const uint8_t sensorThreshold = 51;
const uint8_t hitDelay = 72;
const uint8_t hitListenDebounce = 20;
uint64_t lastHitTime = 0;
uint64_t lastDebounceTime = 0;
uint16_t maxHit = 0;


// TODO: remove?
uint8_t sensorReading = 0;
uint8_t lastSensorData = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  // _radio.init(RADIO_MASTER_ID, RADIO_CE_PIN, RADIO_CSN_PIN);
}

void flashLED() {
  digitalWrite(LED_PIN, HIGH);
  lastLEDTime = millis();
}

void updateLED() {
  if (millis() > lastLEDTime + LEDDuration) {
    digitalWrite(LED_PIN, LOW);
  }
}

void trigger(uint8_t value) {
  Serial.println(value, DEC);
  flashLED();

  uint8_t _data = (uint8_t) value;
  // _radio.send(RADIO_SLAVE_ID, &_data, sizeof(_data));
}

#ifdef FILTERED_SIGNAL
void filterSignal(uint8_t value) {

  // when read no more higher signal for a period of time, triggers the max signal
  if (millis() > lastDebounceTime + hitListenDebounce && maxHit != 0) {
        trigger(maxHit);
        maxHit = 0;
        lastDebounceTime = 0;
        lastHitTime = millis();
  } else {
    // If still inside hit debounce time, and time away from last triggered hit
    if (value > sensorThreshold && millis() > lastHitTime + hitDelay) {
      if (value >= lastSensorData) {
        maxHit = value;
        lastDebounceTime = millis();
      }
    }
  }

  lastSensorData = value;
}
#endif

void loop() {
  updateLED();

  sensorReading = analogRead(SENSOR_PIN);

  #ifndef FILTERED_SIGNAL
    trigger(sensorReading);
  #else
    filterSignal(sensorReading);
  #endif

  delay(10);
}
