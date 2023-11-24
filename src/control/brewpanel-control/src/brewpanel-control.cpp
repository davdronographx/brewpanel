#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <PID_v1.h>
#include "brewpanel-control.hpp"

BrewPanelControlState control_state;

Adafruit_MAX31865 hlt_thermo = 
    Adafruit_MAX31865(
        BREWPANEL_CONTROL_THERMO_PIN_CS_HLT, 
        BREWPANEL_CONTROL_THERMO_PIN_SDI, 
        BREWPANEL_CONTROL_THERMO_PIN_SDO, 
        BREWPANEL_CONTROL_THERMO_PIN_CLK
);
Adafruit_MAX31865 mlt_thermo = 
    Adafruit_MAX31865(
        BREWPANEL_CONTROL_THERMO_PIN_CS_MLT, 
        BREWPANEL_CONTROL_THERMO_PIN_SDI, 
        BREWPANEL_CONTROL_THERMO_PIN_SDO, 
        BREWPANEL_CONTROL_THERMO_PIN_CLK
);
Adafruit_MAX31865 boil_thermo = 
    Adafruit_MAX31865(
        BREWPANEL_CONTROL_THERMO_PIN_CS_BOIL, 
        BREWPANEL_CONTROL_THERMO_PIN_SDI, 
        BREWPANEL_CONTROL_THERMO_PIN_SDO, 
        BREWPANEL_CONTROL_THERMO_PIN_CLK
);

PID hlt_pid = 
    PID(
        &control_state.hlt_temp,
        &control_state.hlt_element.set_value,
        &control_state.hlt_element.output_value,
        BREWPANEL_CONTROL_PID_CONSERVATIVE_P,
        BREWPANEL_CONTROL_PID_CONSERVATIVE_I,
        BREWPANEL_CONTROL_PID_CONSERVATIVE_D
    );

void setup() {

    control_state = {0};

    Serial.begin(115200,SERIAL_8N1);
    Serial.setTimeout(1000);

    pinMode(BREWPANEL_CONTROL_PIN_WATER_PUMP,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_WORT_PUMP,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR,OUTPUT);
    pinMode(BREWPANEL_CONTROL_HLT_SSR,OUTPUT);
    pinMode(BREWPANEL_CONTROL_BOIL_SSR,OUTPUT);

    hlt_thermo.begin(MAX31865_3WIRE);
    mlt_thermo.begin(MAX31865_3WIRE);
    boil_thermo.begin(MAX31865_3WIRE);

}

void
brewpanel_control_message_heartbeat_build_and_send() {

    comm_message_buffer message_buffer = {0};
    
    char msg_start[9] ="<<<<<<<<";
    char msg_end[9] = ">>>>>>>>";

    comm_message heartbeat = {0};
    heartbeat.header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    heartbeat.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;
    heartbeat.header.message_size = sizeof(BrewPanelCommunicationMessagePayloadHeartbeat);
    
    heartbeat.payload.heartbeat.hlt_element_temp  = control_state.hlt_temp;
    heartbeat.payload.heartbeat.mlt_element_temp  = control_state.mlt_temp;
    heartbeat.payload.heartbeat.boil_element_temp = control_state.boil_temp;
    heartbeat.payload.heartbeat.timer_set_point_ms = 0xAAAA;
    heartbeat.payload.heartbeat.timer_elapsed_ms = 0xBBBB;
    message_buffer.buffer_size = sizeof(comm_message_header) + sizeof(comm_payload_heartbeat_ack) + 16; 

    memmove(
        message_buffer.buffer,
        msg_start,
        8
    );

    memmove(
        &message_buffer.buffer[8],
        &heartbeat,
        
        message_buffer.buffer_size
    );

    memmove(
        &message_buffer.buffer[8 + message_buffer.buffer_size],
        msg_end,
        8
    );

    Serial.write(message_buffer.buffer,message_buffer.buffer_size);
}


void brewpanel_control_handle_incoming_message() {

    if (!control_state.incoming_message.message_ready) {
        return;
    }

    BrewPanelCommunicationMessage incoming_message = control_state.incoming_message.parsed_message;
    
    switch (incoming_message.header.message_type) {

        case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PUMP_CONTROL: {

            auto pump_payload = &incoming_message.payload.pump_control;

            switch(pump_payload->pump_id) {
                
                case BREWPANEL_COMMUNICATION_PUMP_ID_WATER: {
                    control_state.water_pump_state = pump_payload->pump_status == BREWPANEL_COMMUNICATION_PUMP_STATUS_ON;
                } break;
                
                case BREWPANEL_COMMUNICATION_PUMP_ID_WORT: {
                    control_state.wort_pump_state = pump_payload->pump_status == BREWPANEL_COMMUNICATION_PUMP_STATUS_ON;
                } break;

                default: break;
            }
        } break;

        case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_MODE_CHANGE: {
            
            control_state.mode = incoming_message.payload.mode_change.mode;
        } break;

        case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_ELEMENT_OUTPUT_SET: {
            
            switch (control_state.mode) {
                
                case BREWPANEL_COMMUNICATION_MODE_MASH: {

                    if (incoming_message.payload.element_output_set.element == BREWPANEL_COMMUNICATION_ELEMENT_HLT) {
                        control_state.hlt_element.state        = BREWPANEL_CONTROL_ELEMENT_STATE_ON;
                        control_state.hlt_element.set_value = incoming_message.payload.element_output_set.set_value;
                    }

                    control_state.boil_element.state     = BREWPANEL_CONTROL_ELEMENT_STATE_OFF;
                    control_state.boil_element.set_value = 0;

                } break;
                
                case BREWPANEL_COMMUNICATION_MODE_BOIL: {

                    if (incoming_message.payload.element_output_set.element == BREWPANEL_COMMUNICATION_ELEMENT_BOIL) {
                        control_state.boil_element.state        = BREWPANEL_CONTROL_ELEMENT_STATE_ON;
                        control_state.boil_element.set_value = incoming_message.payload.element_output_set.set_value;
                    }

                    control_state.hlt_element.state        = BREWPANEL_CONTROL_ELEMENT_STATE_OFF;
                    control_state.hlt_element.set_value = 0;

                } break;
                
                case BREWPANEL_COMMUNICATION_MODE_OFF:
                default: {

                    //in this case, we are turning everything off to be safe
                    control_state.hlt_element.state         = BREWPANEL_CONTROL_ELEMENT_STATE_OFF;
                    control_state.hlt_element.set_value = 0;

                    control_state.boil_element.state        = BREWPANEL_CONTROL_ELEMENT_STATE_OFF;
                    control_state.boil_element.set_value = 0;
                
                } break;
            }
        }
    }

    control_state.incoming_message.message_ready = false;
}

void brewpanel_control_update_ssr_hlt() {

}

void brewpanel_control_update_ssr_boil() {

    control_state.boil_element.output_value = 
        map(
            control_state.boil_element.set_value, //value
            0,                                       //fromLow
            100,                                     //fromHigh
            0,                                       //toLow
            255                                      //toHigh
    );

    analogWrite(BREWPANEL_CONTROL_BOIL_SSR,control_state.boil_element.output_value);
}

void brewpanel_control_update_outputs() {

    //pumps
    brewpanel_control_water_pump(control_state.water_pump_state ? ON : OFF);
    brewpanel_control_wort_pump(control_state.wort_pump_state ? ON : OFF);

    //mode/element contactors and ssrs
    switch (control_state.mode)
    {
        case BREWPANEL_COMMUNICATION_MODE_MASH: {
            brewpanel_control_hlt_contactor_on();
            brewpanel_control_boil_contactor_off();
            if (control_state.hlt_element.state == BREWPANEL_CONTROL_ELEMENT_STATE_ON) {
                brewpanel_control_update_ssr_hlt();
            }
        } break;
        
        case BREWPANEL_COMMUNICATION_MODE_BOIL: {
            brewpanel_control_hlt_contactor_off();
            brewpanel_control_boil_contactor_on();
            if (control_state.boil_element.state == BREWPANEL_CONTROL_ELEMENT_STATE_ON) {
                brewpanel_control_update_ssr_boil();
            }
        } break;

        //default is off
        case BREWPANEL_COMMUNICATION_MODE_OFF: {
            brewpanel_control_hlt_contactor_off();
            brewpanel_control_boil_contactor_off();
        } 
        default: break;
    }

    delay(10);
}

void brewpanel_control_read_and_parse_incoming_data() {

    if (Serial.available() > 0) {

        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size] = Serial.read();
        ++control_state.incoming_message.message_size;
    }    

    control_state.incoming_message.message_start = 
        control_state.incoming_message.message_size >= 8 &&
        control_state.incoming_message.message_buffer[0] == '<' &&
        control_state.incoming_message.message_buffer[1] == '<' &&
        control_state.incoming_message.message_buffer[2] == '<' &&
        control_state.incoming_message.message_buffer[3] == '<' &&
        control_state.incoming_message.message_buffer[4] == '<' &&
        control_state.incoming_message.message_buffer[5] == '<' &&
        control_state.incoming_message.message_buffer[6] == '<' &&
        control_state.incoming_message.message_buffer[7] == '<';

    control_state.incoming_message.message_end = 
        control_state.incoming_message.message_size > 8 &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 8] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 7] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 6] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 5] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 4] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 3] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 2] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 1] == '>';

    if (control_state.incoming_message.message_end   && 
        control_state.incoming_message.message_start &&
        control_state.incoming_message.message_size  >= 16) {
            
        //we have a complete message
        control_state.incoming_message.message_ready = true;

        memmove(
            &control_state.incoming_message.parsed_message,
            &control_state.incoming_message.message_buffer[8],
            control_state.incoming_message.message_size - 16
        );

        //clear the message buffer
        memset(
            control_state.incoming_message.message_buffer,
            0,
            control_state.incoming_message.message_size
        );
        control_state.incoming_message.message_size  = 0;
        control_state.incoming_message.message_start = false;
        control_state.incoming_message.message_end   = false;
    }
}


u16 thermo_index = 0;

void brewpanel_control_update_temperatures() {

    static unsigned long chrono = millis();
    unsigned long elapsed       = millis();

    static u16 hlt_temp  = 0;
    static u16 mlt_temp  = 0;
    static u16 boil_temp = 0;

    control_state.mlt_temp  = (u8)mlt_temp;
    control_state.hlt_temp  = (u8)hlt_temp;
    control_state.boil_temp = (u8)boil_temp;

    if (elapsed - chrono < 1000) {
        return;
    }

    chrono = millis();

    switch (thermo_index) {

        case 0: {
            hlt_temp = (u16)((hlt_thermo.temperature(RNOMINAL,  RREF) * 1.8) + 32);
            if (hlt_temp > 212) {
               hlt_temp = 212;
            }
        } break;

        case 1: {
            mlt_temp = (u16)((mlt_thermo.temperature(RNOMINAL,  RREF) * 1.8) + 32);
            if (mlt_temp > 212) {
                mlt_temp = 212;
            }
        } break;

        case 2: {
            boil_temp = (u16)((boil_thermo.temperature(RNOMINAL, RREF) * 1.8) + 32);
            if (boil_temp > 212) {
                boil_temp = 212;
            }
        } break;
    }

    thermo_index++;
    if (thermo_index > 2) {
        thermo_index = 0;
    }
}

void loop() {
    brewpanel_control_update_temperatures();

    brewpanel_control_read_and_parse_incoming_data();

    brewpanel_control_handle_incoming_message();

    brewpanel_control_update_outputs();

    brewpanel_control_message_heartbeat_build_and_send();

}