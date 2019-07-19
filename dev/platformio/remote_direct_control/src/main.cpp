#include <Arduino.h>
#include <SPI.h>
#include <NRFLite.h>
#include <Queue.h>
#include "defines.h"
#include "RfMessage.h"
#include "Trigger.h"

NRFLite radio;
RfMessage message;

struct Settings {
	uint16_t delayTime;
	uint16_t delayChange;
	uint16_t changeInterval;
	uint8_t threshold;
};

Settings settings;

//Queue<Trigger> queue = Queue<Trigger>(64);

void triggerB(int duration) {
	digitalWrite(PIN_B, HIGH);
	digitalWrite(LED_BUILTIN,HIGH);
	delay(duration);
	digitalWrite(PIN_B, LOW);
	digitalWrite(LED_BUILTIN,LOW);
}

void triggerA(int duration) {
    digitalWrite(PIN_A,HIGH);
	digitalWrite(LED_BUILTIN,HIGH);
	delay(duration);
	digitalWrite(PIN_A, LOW);
	digitalWrite(LED_BUILTIN,LOW);
}

void setup() {

	Serial.begin(9600);

	Serial.print("Starting ");
	Serial.println(BOARD_ID);

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(PIN_A, OUTPUT);
	pinMode(PIN_B, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	radio.init(1, RADIO_CE_PIN, RADIO_CSN_PIN);

	//play startup sequence
	for (int i=0; i<5; i++) {
		triggerA(50);
		delay(50);
		triggerB(50);
		delay(100);
		triggerB(50);
		delay(100);
	}
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
			case 'a':
				//Trigger trigger;
                //trigger.type = 'a';
				//trigger.duration = 60;
                //trigger(trigger);
                triggerA(message.duration);
				break;
            case 'b':
                //Trigger trigger;
                //trigger.type = 'b';
                //trigger.duration = 60;
                //trigger(trigger);
                triggerB(message.duration);
				break;
		}
        hasNewMessage = false;
	}

	// check start of Queue if drum should be triggered
	//if ( queue.count() > 0 && queue.peek().triggerTime < millis() ) {
		//Trigger trigger = queue.pop();
		//int triggerTime = map(trigger.velocity , 0, 255, MIN_TRIGGER_TIME, MAX_TRIGGER_TIME);
		//triggerA(triggerTime);

	//}
}
