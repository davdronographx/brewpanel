#ifndef BREWPANEL_PLATFORM_API_HPP
#define BREWPANEL_PLATFORM_API_HPP

#include "brewpanel-types.hpp"

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

struct BrewpanelPlatformApi {
    func_platform_memory_allocate memory_allocate;
    func_platform_memory_free     memory_free;
    func_platform_file_open       file_open;
    func_platform_file_get_size   file_get_size;
    func_platform_file_create     file_create;
    func_platform_file_close      file_close;
    func_platform_file_read       file_read;
    func_platform_file_write      file_write;
};

global BrewpanelPlatformApi platform_api;

#define brewpanel_platform_memory_allocate(size)               platform_api.memory_allocate(size)
#define brewpanel_platform_memory_free(size,memory)            platform_api.memory_free(size,memory)
#define brewpanel_platform_file_open(path)                     platform_api.file_open(path)
#define brewpanel_platform_file_get_size(file)                 platform_api.file_get_size(file)
#define brewpanel_platform_file_create(path)                   platform_api.file_create(path)
#define brewpanel_platform_file_close(file)                    platform_api.file_close(file)
#define brewpanel_platform_file_read(file,buffer,size,offset)  platform_api.file_read(file,buffer,size,offset)
#define brewpanel_platform_file_write(file,buffer,size,offset) platform_api.file_write(file,buffer,size,offset)

#endif //BREWPANEL_PLATFORM_API_HPP
