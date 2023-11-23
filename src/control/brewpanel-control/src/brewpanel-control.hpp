#ifndef BREWPANEL_CONTROL_HPP
#define BREWPANEL_CONTROL_HPP


typedef char  s8;
typedef short s16;
typedef int   s32;
typedef long  s64;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u64;

typedef float  f32;
typedef double f64;

typedef u8        mem_byte;
typedef mem_byte* mem_data;

#define BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE         128
#define BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE    256
#define BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE         sizeof(BrewPanelCommunicationMessageHeaderData)
#define BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES  8

struct BrewPanelMessageBuffer {
    u16      buffer_size;    
    mem_byte buffer[BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE];
};

enum BrewPanelCommunicationMessageSender : u8 {
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC = 0x01,
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_HMI = 0x02
};

enum BrewPanelCommunicationMessageType : u8 {
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_INVALID                = 0x00,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT              = 0x01,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK          = 0x02,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_MODE_CHANGE            = 0x03,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PUMP_CONTROL           = 0x04,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_TIMER_CONTROL          = 0x06,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_ELEMENT_OUTPUT_SET     = 0x07,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_ELEMENT_OFF            = 0x08,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_COUNT                  = 0x09,
};

struct BrewPanelCommunicationMessageHeader {
    u8  sender;
    u8  message_type;
    u16 message_size;
};

struct BrewPanelCommunicationMessagePayloadHeartbeat {
    u8  hlt_element_temp;
    u8  mlt_element_temp;
    u8  boil_element_temp;
    u8  panel_mode;
    u8  water_pump_status;
    u8  wort_pump_status;
    u8  timer_status;
    u16 timer_set_point_ms;
    u16 timer_elapsed_ms;
};

enum BREWPANEL_COMMUNICATION_PUMP_ID : u8 {
    BREWPANEL_COMMUNICATION_PUMP_ID_WATER = 0x00,
    BREWPANEL_COMMUNICATION_PUMP_ID_WORT  = 0x01,
};

enum BREWPANEL_COMMUNICATION_PUMP_STATUS : u8 {
    BREWPANEL_COMMUNICATION_PUMP_STATUS_ON  = 0x01,
    BREWPANEL_COMMUNICATION_PUMP_STATUS_OFF = 0x02
};

struct BrewPanelCommunicationMessagePayloadPumpControl {
    u8 pump_id;
    u8 pump_status;
};


enum BrewPanelCommunicationMode : u8 {
    BREWPANEL_COMMUNICATION_MODE_OFF  = 0x01,
    BREWPANEL_COMMUNICATION_MODE_MASH = 0x02,
    BREWPANEL_COMMUNICATION_MODE_BOIL = 0x03,
};

struct BrewPanelCommunicationMessagePayloadModeChange {
    BrewPanelCommunicationMode mode;
};


enum BrewPanelCommunicationElement : u8 {
    BREWPANEL_COMMUNICATION_ELEMENT_HLT  = 0x01,
    BREWPANEL_COMMUNICATION_ELEMENT_BOIL = 0x02
};

struct BrewPanelCommunicationMessagePayloadElementOutputSet {
    BrewPanelCommunicationElement element;
    u8                            output_value;
};

struct BrewPanelCommunicationMessagePayloadElementOff {
    BrewPanelCommunicationElement element;
};

union BrewPanelCommunicationMessagePayload {
    BrewPanelCommunicationMessagePayloadHeartbeat        heartbeat;
    BrewPanelCommunicationMessagePayloadPumpControl      pump_control;
    BrewPanelCommunicationMessagePayloadModeChange       mode_change;
    BrewPanelCommunicationMessagePayloadElementOutputSet element_output_set;
    BrewPanelCommunicationMessagePayloadElementOff       element_off;
};

struct BrewPanelCommunicationMessage {
    BrewPanelCommunicationMessageHeader  header;
    BrewPanelCommunicationMessagePayload payload;
};

struct BrewPanelMessageQueue {
    u8                            message_count;
    BrewPanelCommunicationMessage messages[BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES];
};

struct BrewPanelControlIncomingMessage{
    bool                          message_start;
    bool                          message_end;
    bool                          message_ready;
    u8                            message_size;
    u8                            message_buffer[BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE];
    BrewPanelCommunicationMessage parsed_message;
};

enum BrewPanelControlElementState : u8 {
    BREWPANEL_CONTROL_ELEMENT_STATE_OFF = 0x00,
    BREWPANEL_CONTROL_ELEMENT_STATE_ON  = 0x01
};

struct BrewPanelControlElement {
    BrewPanelControlElementState state;
    u8                           output_value;
};

struct BrewPanelControlState {
    u8                              hlt_temp;
    u8                              mlt_temp;
    u8                              boil_temp;
    bool                            water_pump_state;
    bool                            wort_pump_state;
    bool                            read_buffer_lock;
    BrewPanelMessageBuffer          incoming_data_buffer;
    BrewPanelMessageQueue           message_queue;
    BrewPanelControlIncomingMessage incoming_message;
    BrewPanelCommunicationMode      mode;
    BrewPanelControlElement         hlt_element;
    BrewPanelControlElement         boil_element;

};

typedef BrewPanelCommunicationMessageHeader           comm_message_header;
typedef BrewPanelCommunicationMessagePayloadHeartbeat comm_payload_heartbeat_ack;
typedef BrewPanelCommunicationMessage                 comm_message;
typedef BrewPanelMessageBuffer                        comm_message_buffer;

#define BREWPANEL_CONTROL_PIN_WATER_PUMP     2
#define BREWPANEL_CONTROL_PIN_WORT_PUMP      3
#define BREWPANEL_CONTROL_PIN_HLT_CONTACTOR  4
#define BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR 5

#define brewpanel_control_water_pump_on()      if (digitalRead(BREWPANEL_CONTROL_PIN_WATER_PUMP)     == LOW)  {digitalWrite(BREWPANEL_CONTROL_PIN_WATER_PUMP,HIGH);}
#define brewpanel_control_water_pump_off()     if (digitalRead(BREWPANEL_CONTROL_PIN_WATER_PUMP)     == HIGH) {digitalWrite(BREWPANEL_CONTROL_PIN_WATER_PUMP,LOW);}
#define brewpanel_control_wort_pump_on()       if (digitalRead(BREWPANEL_CONTROL_PIN_WORT_PUMP)      == LOW)  {digitalWrite(BREWPANEL_CONTROL_PIN_WORT_PUMP,HIGH);}
#define brewpanel_control_wort_pump_off()      if (digitalRead(BREWPANEL_CONTROL_PIN_WORT_PUMP)      == HIGH) {digitalWrite(BREWPANEL_CONTROL_PIN_WORT_PUMP,LOW);}
#define brewpanel_control_hlt_contactor_on()   if (digitalRead(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR)  == LOW)  {digitalWrite(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR,HIGH);}
#define brewpanel_control_hlt_contactor_off()  if (digitalRead(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR)  == HIGH) {digitalWrite(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR,LOW);}
#define brewpanel_control_boil_contactor_on()  if (digitalRead(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR) == LOW)  {digitalWrite(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR,HIGH);}
#define brewpanel_control_boil_contactor_off() if (digitalRead(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR) == HIGH) {digitalWrite(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR,LOW);}

#define ON  HIGH
#define OFF LOW

#define brewpanel_control_water_pump(state)     digitalWrite(BREWPANEL_CONTROL_PIN_WATER_PUMP,state)
#define brewpanel_control_wort_pump(state)      digitalWrite(BREWPANEL_CONTROL_PIN_WORT_PUMP,state)
#define brewpanel_control_hlt_contactor(state)  digitalWrite(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR,state)
#define brewpanel_control_boil_contactor(state) digitalWrite(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR,state)

#define BREWPANEL_CONTROL_THERMO_PIN_CLK     8
#define BREWPANEL_CONTROL_THERMO_PIN_SDO     9
#define BREWPANEL_CONTROL_THERMO_PIN_SDI     10
#define BREWPANEL_CONTROL_THERMO_PIN_CS_HLT  A0
#define BREWPANEL_CONTROL_THERMO_PIN_CS_MLT  A1
#define BREWPANEL_CONTROL_THERMO_PIN_CS_BOIL A2

#define BREWPANEL_CONTROL_HLT_SSR  A3
#define BREWPANEL_CONTROL_BOIL_SSR A4

#define RREF      430.0
#define RNOMINAL  100.0

#endif //BREWPANEL_CONTROL _HPP