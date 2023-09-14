#include <Arduino.h>
#include <avr8-stub.h>
#include <app_api.h>
#include "brewpanel-control.hpp"
#include "brewpanel-control-communication.cpp"
#include "brewpanel-control-temperature.cpp"

global BrewpanelControlState control  = {0};

void setup() {
    control.comm = brewpanel_control_communication_init();
    control.temp = brewpanel_control_temperature_init();
}

void loop() {

    String msg = "";

    // brewpanel_control_temperature_update(&control.temp);

    // brewpanel_control_communication_update(
    //     &control.comm,
    //     control.temp
    // );

    // delay(1000);
    // Serial.write("HELLO");

    String read_string = "";

    while (Serial.available()) {
        delay(10);
        
        if (Serial.available() > 0) {

            char c = Serial.read();
            if (c == '\0') {
                break;
            }

            read_string += c;
        }
    }

    if (read_string.length() > 0) {
        Serial.write("ASDF");
    }

    delay(100);
}