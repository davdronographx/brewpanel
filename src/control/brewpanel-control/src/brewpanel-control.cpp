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


    brewpanel_control_temperature_update(&control.temp);

    brewpanel_control_communication_update(
        &control.comm,
        control.temp
    );

    // mem_byte read_buffer[BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE];
    // u32 bytes_read = 0;

    // while (Serial.available()) {
        
    //     if (Serial.available() > 0) {
            
    //         char c = Serial.read();
    //         read_buffer[bytes_read] = c;
    //         ++bytes_read;

    //         if (c == '\0') {
    //             break;
    //         }
    //     }
    // }

    // if (bytes_read > 0) {
    //     Serial.write("ASDF");
    //     // delay(100);
    // }

    // // Serial.write("ASDF");
    // // delay(1000);


}