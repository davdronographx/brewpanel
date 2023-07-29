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
    comm_message_sender sender;       //u8
    comm_message_type   message_type; //u8
    u32                 message_size; //u32
    u64                 timestamp;    //u64
};
typedef BrewPanelCommunicationMessageHeader comm_message_header; 

struct BrewPanelCommunicationMessage {
    comm_message_header header;
    u32                 payload_size_bytes;
    mem_data            payload_data[BREWPANEL_COMMUNICATION_MESSAGE_PAYLOAD_MAX_SIZE];
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
    u8                hlt_element_temp;
    u8                mlt_element_temp;
    u8                boil_element_temp;
    comm_panel_mode   panel_mode;
    comm_pump_status  water_pump_status;
    comm_pump_status  wort_pump_status;
    comm_timer_status timer_status;
    u64               timer_set_point_ms;
    u64               timer_elapsed_ms;
};

struct BrewPanelCommunicationMessageQueue {
    u8           sent_message_count;
    mem_data     sent_messages[5]; 
};
typedef BrewPanelCommunicationMessageQueue comm_message_queue;

struct BrewPanelCommunicationHandler {
    mem_data       incoming_message;
    comm_message_queue sent_message_queue;
};
typedef BrewPanelCommunicationHandler comm_handler;

#endif //BREWPANEL_COMMUNICATION_HPP