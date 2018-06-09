// TX example

#include <SPI.h>
#include <NRFLite.h>

#define RADIO_CE_PIN 8
#define RADIO_CSN_PIN 10

#define BUTTON_PIN 3

NRFLite _radio;
uint8_t _data = 111;
byte previousButtonState = HIGH;

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
   _radio.init(0, RADIO_CE_PIN, RADIO_CSN_PIN);; // Set radio Id = 0, along with the CE and CSN pins
}

void loop()
{
    byte currentButtonState = digitalRead(BUTTON_PIN);

    if (currentButtonState == LOW && previousButtonState == HIGH) {
       _radio.send(1, &_data, sizeof(_data)); // Send to the radio with Id = 1
       Serial.write("pressed");
    }
    
    previousButtonState = currentButtonState;
}
