#include <Arduino.h>

void setup() {
    Serial.begin(9600,SERIAL_8N1);
    Serial.setTimeout(1000);
}

void loop() {;

    Serial.write("test");

    delay(250);
}

void serialEvent() {



}