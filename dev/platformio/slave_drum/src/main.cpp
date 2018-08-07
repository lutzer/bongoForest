/**
* @Author: Lutz Reiter [http://www.lu-re.de] <lutz>
* @Date:   2018-07-21T14:10:26+02:00
 * @Last modified by:   lutz
 * @Last modified time: 2018-08-07T23:18:40+02:00
*/


#include <Arduino.h>
#include <SPI.h>
#include <NRFLite.h>
#include <EEPROM.h>
#include <Queue.h>
#include "defines.h"
#include "RfMessage.h"
#include "Trigger.h"

#define SOLENOID_PIN 6
#define LEDS_PIN 7

#define RADIO_CE_PIN 8
#define RADIO_CSN_PIN 10

NRFLite radio;
RfMessage message;

struct Settings {
	uint16_t delayTime;
	uint16_t delayChange;
	uint16_t changeInterval;
	uint8_t threshold;
};

Settings settings;

Queue<Trigger> queue = Queue<Trigger>(64);

void triggerDrum(int triggerTime) {
	digitalWrite(SOLENOID_PIN, HIGH);
	digitalWrite(LED_BUILTIN,HIGH);
	digitalWrite(LEDS_PIN,HIGH);
	delay(triggerTime);
	digitalWrite(SOLENOID_PIN, LOW);
	digitalWrite(LED_BUILTIN,LOW);
	digitalWrite(LEDS_PIN,LOW);
}

// reads settings from eeprom
void readSettings() {
	EEPROM.get(EEPROM_DELAY_TIME, settings.delayTime);
	EEPROM.get(EEPROM_THRESHOLD, settings.threshold);
	EEPROM.get(EEPROM_DELAY_CHANGE, settings.delayChange);
	EEPROM.get(EEPROM_CHANGE_INTERVAL, settings.changeInterval);

	Serial.println("# SETTINGS");
	Serial.print("delayTime: ");
	Serial.println(settings.delayTime);
	Serial.print("delayChange: ");
	Serial.println(settings.delayChange);
	Serial.print("changeInterval: ");
	Serial.println(settings.changeInterval);
	Serial.print("threshold: ");
	Serial.println(settings.threshold);
}

template <typename T>
void updateEEPROM(int address, T value) {
	T currentValue;
	EEPROM.get(address, currentValue);

	// only update when changed
	if (currentValue != value)
		EEPROM.put(address, value);
}

void setDefaultSettings() {
	Serial.println("set default values");
	updateEEPROM(EEPROM_DELAY_TIME, DEFAULT_DELAY_TIME);
	updateEEPROM(EEPROM_DELAY_CHANGE, DEFAULT_DELAY_CHANGE);
	updateEEPROM(EEPROM_CHANGE_INTERVAL, DEFAULT_CHANGE_INTERVAL);
	updateEEPROM(EEPROM_THRESHOLD, DEFAULT_THRESHOLD);
}

void clearEEPROM() {
	uint8_t first;
	EEPROM.get(0, first);
	if (first == 0) {
		first = 0;
		Serial.println("Clearing eeprom.");
		for (int i = 0 ; i < EEPROM.length() ; i++) {
			EEPROM.put(i, 0);
		}
	}
}

float getDelayTime(long ms) {
	return settings.delayTime + sin(M_PI / (float)settings.changeInterval * ms) * (float)settings.delayChange;
}

void setup() {

	// reset eeprom
	clearEEPROM();

	Serial.begin(9600);

	Serial.print("Starting ");
	Serial.print(BOARD_TYPE);
	Serial.print(" with id: ");
	Serial.println(BOARD_ID);

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(SOLENOID_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LEDS_PIN, OUTPUT);

	radio.init(1, RADIO_CE_PIN, RADIO_CSN_PIN);

	// read settings from EEPROM
	readSettings();

	//play startup sequence
	for (int i=0; i<5; i++) {
		triggerDrum(50);
		delay(500);
	}
}

void loop() {

	bool hasNewMessage = false;

	while (radio.hasData())
	{
		radio.readData(&message);
		hasNewMessage = true;
	}

	if (Serial.available() > 0 && Serial.read() == 'r') {
		setDefaultSettings();
		readSettings();
	}

	if ( hasNewMessage ) {

		switch ( message.type ) {
			// trigger drum
			case 'h':
				Trigger trigger;
				trigger.velocity = message.value;
				trigger.triggerTime = millis() + getDelayTime(millis());
				queue.push(trigger);
				break;
			// set delay and update eeprom
			case 'd':
				if (message.id != BOARD_ID)
					break;
				settings.delayTime = message.value;
				updateEEPROM(EEPROM_DELAY_TIME, settings.delayTime);
				Serial.print("Updated delay time to ");
				Serial.println(settings.delayTime);
				break;
			case 'c':
				if (message.id != BOARD_ID)
					break;
				settings.delayChange = message.value;
				updateEEPROM(EEPROM_DELAY_CHANGE, settings.delayChange);
				Serial.print("Updated delayChange to ");
				Serial.println(settings.delayChange);
				break;
			case 'i':
				if (message.id != BOARD_ID)
					break;
				settings.changeInterval = message.value;
				updateEEPROM(EEPROM_THRESHOLD, settings.changeInterval);
				Serial.print("Updated changeInterval to ");
				Serial.println(settings.changeInterval);
				break;
			case 't':
				if (message.id != BOARD_ID)
					break;
				settings.threshold = message.value;
				updateEEPROM(EEPROM_THRESHOLD, settings.threshold);
				Serial.print("Updated delayChange to ");
				Serial.println(settings.threshold);
				break;
		}
		hasNewMessage = false;
	}

	// check start of Queue if drum should be triggered
	if ( queue.count() > 0 && queue.peek().triggerTime < millis() ) {
		Trigger trigger = queue.pop();
		int triggerTime = map(trigger.velocity , 0, 255, MIN_TRIGGER_TIME, MAX_TRIGGER_TIME);
		triggerDrum(triggerTime);

	}
}
