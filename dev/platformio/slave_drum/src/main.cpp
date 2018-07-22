/**
* @Author: Lutz Reiter [http://www.lu-re.de] <lutz>
* @Date:   2018-07-21T14:10:26+02:00
* @Project: Brain String
 * @Last modified by:   lutz
 * @Last modified time: 2018-07-22T22:23:45+02:00
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

uint16_t delayTime = 500;

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
	EEPROM.get(EEPROM_DELAY, delayTime);

	if (delayTime == 0)
		delayTime = DEFAULT_DELAY_TIME;
}

void setup() {

	Serial.begin(9600);

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

	Serial.print("started, delay:");
	Serial.println(delayTime);
}

void loop() {

	bool hasNewMessage = false;

	while (radio.hasData())
	{
		radio.readData(&message);
		hasNewMessage = true;
	}

	if ( hasNewMessage ) {

		switch ( message.type ) {
			// trigger drum
			case 't':
				Trigger trigger;
				trigger.velocity = message.value;
				trigger.triggerTime = millis() + delayTime;
				queue.push(trigger);
				break;
			// set delay
			case 'd':
				delayTime = message.value;
				EEPROM.update(EEPROM_DELAY, delayTime);
				Serial.println(delayTime);
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
