#ifndef BREWPANEL_COMMUNICATION_HPP
#define BREWPANEL_COMMUNICATION_HPP

#include "brewpanel-types.hpp"

#define BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES 8
#define BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE  256

enum BrewPanelCommunicationMessageSender : u8 {
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC = 0x01,
    BREWPANEL_COMMUNICATION_MESSAGE_SENDER_HMI = 0x02
};
typedef BrewPanelCommunicationMessageSender comm_message_sender;

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
typedef BrewPanelCommunicationMessageType comm_message_type;

struct BrewPanelCommunicationMessageHeader {
    u8  sender;
    u8  message_type;
    u32 message_size;
    u64 timestamp;
};
typedef BrewPanelCommunicationMessageHeader comm_message_header; 

struct BrewPanelCommunicationMessage {
    comm_message_header header;
    u32                 payload_size_bytes;
    mem_byte            payload_data[BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE];
};
typedef BrewPanelCommunicationMessage comm_message;

enum BrewPanelCommunicationMode : u8 {
    BREWPANEL_COMMUNICATION_MODE_MASH = 0x01,
    BREWPANEL_COMMUNICATION_MODE_BOIL = 0x02,
};
typedef BrewPanelCommunicationMode comm_panel_mode;

enum BrewPanelCommunicationPumpStatus : u8 {
    BREWPANEL_COMMUNICATION_PUMP_STATUS_ON  = 0x01,
    BREWPANEL_COMMUNICATION_PUMP_STATUS_OFF = 0x02,
};
typedef BrewPanelCommunicationPumpStatus comm_pump_status;

enum BrewPanelCommunicationTimerStatus : u8 {
    BREWPANEL_COMMUNICATION_TIMER_STATUS_NOT_SET = 0x01,
    BREWPANEL_COMMUNICATION_TIMER_STATUS_RUNNING = 0x02,
    BREWPANEL_COMMUNICATION_TIMER_STATUS_PAUSED  = 0x03,
    BREWPANEL_COMMUNICATION_TIMER_STATUS_EXPIRED = 0x04
};
typedef BrewPanelCommunicationTimerStatus comm_timer_status;

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
    u8           sent_message_count;
    comm_message sent_messages[BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES]; 
};
typedef BrewPanelCommunicationMessageQueue comm_message_queue;

struct BrewPanelCommunicationHandler {
    comm_message            incoming_message;
    comm_message_queue      sent_message_queue;
    BrewPanelControllerInfo controller_info;
    controller_handle       controller_handle;
};
typedef BrewPanelCommunicationHandler comm_handler;

#endif //BREWPANEL_COMMUNICATION_HPP