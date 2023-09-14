#ifndef BREWPANEL_PLATFORM_API_HPP
#define BREWPANEL_PLATFORM_API_HPP

#include "brewpanel-types.hpp"

#define COMPORTS_LINUX_COUNT   38
#define COMPORTS_WINDOWS_COUNT 48
#define COMPORTS_BAUD_RATE 9600

const str comports_linux[COMPORTS_LINUX_COUNT] = {
    "/dev/ttyS0",
    "/dev/ttyS1",
    "/dev/ttyS2",
    "/dev/ttyS3",
    "/dev/ttyS4",
    "/dev/ttyS5",
    "/dev/ttyS6",
    "/dev/ttyS7",
    "/dev/ttyS8",
    "/dev/ttyS9",
    "/dev/ttyS10",
    "/dev/ttyS11",
    "/dev/ttyS12",
    "/dev/ttyS13",
    "/dev/ttyS14",
    "/dev/ttyS15",
    "/dev/ttyUSB0",
    "/dev/ttyUSB1",
    "/dev/ttyUSB2",
    "/dev/ttyUSB3",
    "/dev/ttyUSB4",
    "/dev/ttyUSB5",
    "/dev/ttyAMA0",
    "/dev/ttyAMA1",
    "/dev/ttyACM0",
    "/dev/ttyACM1",
    "/dev/rfcomm0",
    "/dev/rfcomm1",
    "/dev/ircomm0",
    "/dev/ircomm1",
    "/dev/cuau0",
    "/dev/cuau1",
    "/dev/cuau2",
    "/dev/cuau3",
    "/dev/cuaU0",
    "/dev/cuaU1",
    "/dev/cuaU2",
    "/dev/cuaU3"
};

const str comports_windows[COMPORTS_WINDOWS_COUNT] = {
    "\\\\.\\COM1",
    "\\\\.\\COM2",
    "\\\\.\\COM3",
    "\\\\.\\COM4",
    "\\\\.\\COM5",
    "\\\\.\\COM6",
    "\\\\.\\COM7",
    "\\\\.\\COM8",
    "\\\\.\\COM9",
    "\\\\.\\COM10",
    "\\\\.\\COM11",
    "\\\\.\\COM12",
    "\\\\.\\COM13",
    "\\\\.\\COM14",
    "\\\\.\\COM15",
    "\\\\.\\COM16",
    "\\\\.\\COM17",
    "\\\\.\\COM18",
    "\\\\.\\COM19",
    "\\\\.\\COM20",
    "\\\\.\\COM21",
    "\\\\.\\COM22",
    "\\\\.\\COM23",
    "\\\\.\\COM24",
    "\\\\.\\COM25",
    "\\\\.\\COM26",
    "\\\\.\\COM27",
    "\\\\.\\COM28",
    "\\\\.\\COM29",
    "\\\\.\\COM30",
    "\\\\.\\COM31",
    "\\\\.\\COM32",
    "\\\\.\\COM33",
    "\\\\.\\COM34",
    "\\\\.\\COM35",
    "\\\\.\\COM36",
    "\\\\.\\COM37",
    "\\\\.\\COM38",
    "\\\\.\\COM39",
    "\\\\.\\COM40",
    "\\\\.\\COM41",
    "\\\\.\\COM42",
    "\\\\.\\COM43",
    "\\\\.\\COM44",
    "\\\\.\\COM45",
    "\\\\.\\COM46",
    "\\\\.\\COM47",
    "\\\\.\\COM48"
};

typedef mem_data
(*func_platform_memory_allocate)
(
    u64 size
);

typedef void
(*func_platform_memory_free)
(
    u64      size,
    mem_data memory
);

typedef file_handle
(*func_platform_file_open)
(
    str file_path
);

typedef u64
(*func_platform_file_get_size)
(
    file_handle file
);

typedef file_handle
(*func_platform_file_create)
(
    str file_path
);

typedef void
(*func_platform_file_close)
(
    file_handle file
);

typedef void
(*func_platform_file_read)
(
    file_handle file_handle,
    mem_data    read_buffer,
    u64         read_buffer_size,
    u64         file_offset
);

typedef void
(*func_platform_file_write)
(
    file_handle file_handle,
    mem_data    write_buffer,
    u64         write_buffer_size,
    u64         file_offset
);

typedef BrewPanelSystemTime
(*func_platform_system_time)
(
    void
);

typedef controller_handle
(*func_platform_controller_handle)
(
    BrewPanelControllerInfo controller_info
);

typedef void
(*func_platform_controller_close)
(
    controller_handle handle
);

typedef bool
(*func_platform_controller_write_buffer)
(
    controller_handle controller_handle,
    mem_data          write_buffer,
    u64               write_buffer_size
);

typedef thread_handle
(*func_platform_start_controller_comm_thread)
(
    BrewPanelControlCommData* controller_comm_data
);

struct BrewpanelPlatformApi {
    func_platform_memory_allocate               memory_allocate;
    func_platform_memory_free                   memory_free;
    func_platform_file_open                     file_open;
    func_platform_file_get_size                 file_get_size;
    func_platform_file_create                   file_create;
    func_platform_file_close                    file_close;
    func_platform_file_read                     file_read;
    func_platform_file_write                    file_write;
    func_platform_system_time                   system_time_get;
    func_platform_controller_handle             controller_handle;
    func_platform_controller_close              controller_close;
    func_platform_start_controller_comm_thread  controller_thread_start_read;
    func_platform_start_controller_comm_thread  controller_thread_start_write;
    func_platform_controller_write_buffer       controller_write;
};

global BrewpanelPlatformApi platform_api;

#define brewpanel_platform_memory_allocate(size)                          platform_api.memory_allocate(size)
#define brewpanel_platform_memory_free(size,memory)                       platform_api.memory_free(size,memory)
#define brewpanel_platform_file_open(path)                                platform_api.file_open(path)
#define brewpanel_platform_file_get_size(file)                            platform_api.file_get_size(file)
#define brewpanel_platform_file_create(path)                              platform_api.file_create(path)
#define brewpanel_platform_file_close(file)                               platform_api.file_close(file)
#define brewpanel_platform_file_read(file,buffer,size,offset)             platform_api.file_read(file,buffer,size,offset)
#define brewpanel_platform_file_write(file,buffer,size,offset)            platform_api.file_write(file,buffer,size,offset)
#define brewpanel_platform_system_time_get()                              platform_api.system_time_get()
#define brewpanel_platform_controller_handle(info)                        platform_api.controller_handle(info)
#define brewpanel_platform_controller_close(handle)                       platform_api.controller_close(handle)
#define brewpanel_platform_controller_write(handle,buffer,size)           platform_api.controller_write(handle,buffer,size)
#define brewpanel_platform_controller_thread_start_read(data)             platform_api.controller_thread_start_read(data)
#define brewpanel_platform_controller_thread_start_write(data)            platform_api.controller_thread_start_write(data)

#endif //BREWPANEL_PLATFORM_API_HPP
