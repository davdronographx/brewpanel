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

    Serial.write("JUST FUCKING WORK",18);
    delay(1000);    

}