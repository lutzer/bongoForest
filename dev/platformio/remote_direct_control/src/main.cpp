#include <Arduino.h>
#include <SPI.h>
#include <NRFLite.h>
#include "defines.h"
#include "RfMessage.h"
#include "Trigger.h"

NRFLite radio;
RfMessage message;

Trigger tA = {'a'}, tB = {'b'};

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

void triggerAsync(Trigger &t) {
    t.on = true;
    switch (t.type) {
        case 'a':
            digitalWrite(PIN_A, HIGH);
            break;
        case 'b':
            digitalWrite(PIN_B, HIGH);
            break;
    }
}

void cutAsync(Trigger &t) {
    t.on = false;
    switch (t.type) {
        case 'a':
            digitalWrite(PIN_A, LOW);
            break;
        case 'b':
            digitalWrite(PIN_B, LOW);
            break;
    }
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

	if ( hasNewMessage && message.id == _ID ) {
        switch (message.type) {
            case 'a':
                tA.duration = message.duration;
                tA.start = millis();
                triggerAsync(tA);
                break;
            case 'b':
                tB.duration = message.duration;
                tB.start = millis();
                triggerAsync(tB);
                break;
        }

        hasNewMessage = false;
	}

    // check open tasks
    if (tA.on && tA.start + tA.duration <= millis()) cutAsync(tA);
    if (tB.on && tB.start + tB.duration <= millis()) cutAsync(tB);
}
