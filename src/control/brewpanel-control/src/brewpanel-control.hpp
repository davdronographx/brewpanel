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

#define BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE      368
#define BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE 256
#define BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE      sizeof(BrewPanelCommunicationMessageHeaderData)

struct BrewPanelControlState {
    u8 hlt_temp;
    u8 mlt_temp;
    u8 boil_temp;
};

struct BrewPanelMessageBuffer {
    u16      buffer_size;    
    mem_byte buffer[BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE];
};

enum BrewPanelCommunicationMessageSender : u8 {
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC = 0x01,
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_HMI = 0x02
};

enum BrewPanelCommunicationMessageType : u8 {
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_INVALID           = 0x00,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT         = 0x01,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK     = 0x02,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_MODE_CHANGE       = 0x03,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_MODE_ACK          = 0x03,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PUMP_CONTROL      = 0x04,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PUMP_CONTROL_ACK  = 0x05,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_TIMER_CONTROL     = 0x06,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_TIMER_CONTROL_ACK = 0x07,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_TEMP_CONTROL      = 0x08,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_TEMP_CONTROL_ACK  = 0x09,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PID_TUNE          = 0x0A,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PID_TUNE_ACK      = 0x0B,
    BREWPANEL_COMMUNICATION_MESSAGE_TYPE_COUNT             = 0x0C,
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


union BrewPanelCommunicationMessagePayload {
    BrewPanelCommunicationMessagePayloadHeartbeat heartbeat;
};

struct BrewPanelCommunicationMessage {
    BrewPanelCommunicationMessageHeader  header;
    BrewPanelCommunicationMessagePayload payload;
};



typedef BrewPanelCommunicationMessageHeader           comm_message_header;
typedef BrewPanelCommunicationMessagePayloadHeartbeat comm_payload_heartbeat_ack;
typedef BrewPanelCommunicationMessage                 comm_message;
typedef BrewPanelMessageBuffer                        comm_message_buffer;

u16 comm_message_sizes[BREWPANEL_COMMUNICATION_MESSAGE_TYPE_COUNT] = {
    sizeof(comm_message_header) + 1,
    0,
    sizeof(comm_message_header) + sizeof(comm_payload_heartbeat_ack) + 1
};

#define comm_message_size(type) ((BREWPANEL_COMMUNICATION_MESSAGE_TYPE_INVALID < 0 || type > BREWPANEL_COMMUNICATION_MESSAGE_TYPE_COUNT) ? 0 : comm_message_sizes[type])

#endif //BREWPANEL_CONTROL _HPP