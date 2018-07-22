/**
* @Author: Lutz Reiter [http://www.lu-re.de] <lutz>
* @Date:   2018-07-21T14:10:26+02:00
* @Project: Brain String
 * @Last modified by:   lutz
 * @Last modified time: 2018-07-22T20:23:29+02:00
*/


#include <Arduino.h>
#include <SPI.h>
#include <NRFLite.h>
#include <EEPROM.h>
#include "defines.h"
#include "RfMessage.h"

#define SOLENOID_PIN 6
#define LEDS_PIN 7

#define RADIO_CE_PIN 8
#define RADIO_CSN_PIN 10

NRFLite radio;
RfMessage *message;

void triggerDrum(int triggerTime) {
	digitalWrite(SOLENOID_PIN, HIGH);
	digitalWrite(LED_BUILTIN,HIGH);
	digitalWrite(LEDS_PIN,HIGH);
	delay(triggerTime);
	digitalWrite(SOLENOID_PIN, LOW);
	digitalWrite(LED_BUILTIN,LOW);
	digitalWrite(LEDS_PIN,LOW);
}

void setup() {

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(SOLENOID_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LEDS_PIN, OUTPUT);

	radio.init(1, RADIO_CE_PIN, RADIO_CSN_PIN);

	//play startup sequence
	for (int i=0; i<5; i++) {
		triggerDrum(50);
		delay(500);
	}

	message = 0;
}

void loop() {

	while (radio.hasData())
	{
		radio.readData(message);
	}

	if ( message != 0) {
		int triggerTime = map(message->value,0,255, MIN_TRIGGER_TIME, MAX_TRIGGER_TIME);
		triggerDrum(triggerTime);
		message = 0;
	}
}
