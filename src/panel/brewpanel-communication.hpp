#ifndef BREWPANEL_COMMUNICATION_HPP
#define BREWPANEL_COMMUNICATION_HPP

#include "brewpanel-types.hpp"

#define BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES 8
#define BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE   256
#define BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE        368
#define BREWPANEL_COMMUNICATION_MESSAGE_HEARTBEAT_ACK_SIZE (sizeof(BrewPanelCommunicationMessageHeader) + sizeof(BrewPanelCommunicationMessagePayloadHeartBeatAck) + 1)

enum BrewPanelCommunicationMessageSender : u8 {
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC = 0x01,
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_HMI = 0x02
};

enum BrewPanelCommunicationMessageType : u8 {
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
};

struct BrewPanelCommunicationMessageHeader {
    u8  sender;
    u8  message_type;
    u32 message_size;
    u64 timestamp;
};

struct BrewPanelCommunicationMessage {
    BrewPanelCommunicationMessageHeader header;
    u32                                 payload_size_bytes;
    mem_byte                            payload_data[BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE];
};

struct BrewPanelCommunicationMessageBuffer {
    u32      buffer_size;
    mem_byte buffer[BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE];
};

enum BrewPanelCommunicationMode : u8 {
    BREWPANEL_COMMUNICATION_MODE_MASH = 0x01,
    BREWPANEL_COMMUNICATION_MODE_BOIL = 0x02,
};

enum BrewPanelCommunicationPumpStatus : u8 {
    BREWPANEL_COMMUNICATION_PUMP_STATUS_ON  = 0x01,
    BREWPANEL_COMMUNICATION_PUMP_STATUS_OFF = 0x02,
};

enum BrewPanelCommunicationTimerStatus : u8 {
    BREWPANEL_COMMUNICATION_TIMER_STATUS_NOT_SET = 0x01,
    BREWPANEL_COMMUNICATION_TIMER_STATUS_RUNNING = 0x02,
    BREWPANEL_COMMUNICATION_TIMER_STATUS_PAUSED  = 0x03,
    BREWPANEL_COMMUNICATION_TIMER_STATUS_EXPIRED = 0x04
};

struct BrewPanelCommunicationMessagePayloadHeartBeatAck {
    u8  hlt_element_temp;
    u8  mlt_element_temp;
    u8  boil_element_temp;
    u8  panel_mode;
    u8  water_pump_status;
    u8  wort_pump_status;
    u8  timer_status;
    u64 timer_set_point_ms;
    u64 timer_elapsed_ms;
};

struct BrewPanelCommunicationMessageQueue {
    u8                            message_count;
    BrewPanelCommunicationMessage messages[BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES]; 
};

struct BrewPanelCommunicationHandler {
    BrewPanelCommunicationMessageQueue               outgoing_message_queue;
    BrewPanelCommunicationMessageQueue               incoming_message_queue;
    BrewPanelControllerInfo                          controller_info;
    BrewPanelCommunicationMessagePayloadHeartBeatAck latest_heartbeat;
    thread_handle                                    read_thread;
    thread_handle                                    write_thread;
    BrewPanelControlCommData                         comm_data;
};

typedef BrewPanelCommunicationMessageType   comm_message_type;
typedef BrewPanelCommunicationMessageSender comm_message_sender;
typedef BrewPanelCommunicationMessageHeader comm_message_header; 
typedef BrewPanelCommunicationMessage       comm_message;
typedef BrewPanelCommunicationMode          comm_panel_mode;
typedef BrewPanelCommunicationPumpStatus    comm_pump_status;
typedef BrewPanelCommunicationTimerStatus   comm_timer_status;
typedef BrewPanelCommunicationMessageQueue  comm_message_queue;
typedef BrewPanelCommunicationHandler       comm_handler;

#endif //BREWPANEL_COMMUNICATION_HPP