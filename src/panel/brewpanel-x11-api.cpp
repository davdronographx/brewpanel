#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "brewpanel-types.hpp"

internal mem_data
brewpanel_x11_api_memory_allocate(
    u64 size) {
    
    //TODO: we're using malloc here, but we may be able to do something for linux
    mem_data memory = (mem_data)malloc(size);
    return(memory);
}


internal void
brewpanel_x11_api_memory_free(
    u64      size,
    mem_data memory) {

    free(memory);
}

internal brewpanel_file_handle
brewpanel_x11_api_file_open(
    str file_path) {

    //on linux, the file handle is an int
    //TODO: I HATE THIS, it would be better if we passed the handle into the arguments
    //but I'm lazy and want my damn beer
    s32* file_descriptor = (s32*)malloc(sizeof(s32));

    *file_descriptor = 
        open(
            file_path,
            O_RDWR | O_EXCL,
            ALLPERMS
    );

    if (*file_descriptor == -1) {
        free(file_descriptor);
        return(NULL);
    }

    return((brewpanel_file_handle)file_descriptor);
}

internal u64
brewpanel_x11_api_file_get_size(
    brewpanel_file_handle file) {

    s32 file_descriptor = *((s32*)file);

    struct stat file_stat = {0};

    s32 fstat_result = fstat(file_descriptor,&file_stat); 

    u32 size = fstat_result == -1 ? 0 : file_stat.st_size;

    return(size);
}

internal brewpanel_file_handle
brewpanel_x11_api_file_create(
    str file_path) {

    //on linux, the file handle is an int
    //TODO: I HATE THIS, it would be better if we passed the handle into the arguments
    //but I'm lazy and want my damn beerO_TRUNC
    mode_t mode = ALLPERMS;

    s32* file_descriptor = (s32*)malloc(sizeof(s32));

    *file_descriptor = open(file_path,O_RDWR | O_CREAT,mode);

    return((brewpanel_file_handle)file_descriptor);
}

internal void
brewpanel_x11_api_file_close(
    brewpanel_file_handle file) {
    
    s32 file_descriptor = *((s32*)file);
    
    close(file_descriptor);

    free((s32*)file);
}

internal void
brewpanel_x11_api_file_read(
    brewpanel_file_handle file,
    mem_data              read_buffer,
    u64                   read_buffer_size,
    u64                   file_offset) {

    s32 file_descriptor = *(s32*)file;

    pread(
        file_descriptor,
        read_buffer,
        read_buffer_size,
        file_offset
    );
}

internal void
brewpanel_x11_api_file_write(
    brewpanel_file_handle file,
    mem_data              write_buffer,
    u64                   write_buffer_size,
    u64                   file_offset) {

    s32 file_descriptor = *(s32*)file;

    pwrite(
        file_descriptor,
        write_buffer,
        write_buffer_size,
        file_offset        
    );
}

internal BrewPanelSystemTime
brewpanel_x11_api_system_time(){
    
    BrewPanelSystemTime bp_system_time = {0};

    auto system_clock_now = std::chrono::system_clock::now();
    auto system_time_now  = std::chrono::system_clock::to_time_t(system_clock_now);
    auto local_time       = *localtime(&system_time_now);
    
    bp_system_time.hours   = local_time.tm_hour;
    bp_system_time.minutes = local_time.tm_min;
    bp_system_time.seconds = local_time.tm_sec;

    return(bp_system_time);
}

/**
 * USB COMM
 * https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 * 
 * 
*/

internal controller_handle
brewpanel_x11_api_controller_handle(
    BrewPanelControllerInfo controller_info) {

    s32 port_num = open(controller_info.port,O_RDWR);    


    struct termios tty = {0};
    if (tcgetattr(port_num,&tty) != 0) {
        return(NULL);
    }

    //control modes
    tty.c_cflag &= ~PARENB; 
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CSTOPB; 
    tty.c_cflag &= ~CRTSCTS; 
    tty.c_cflag |= CREAD | CLOCAL; 
    
    //local modes
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG; 

    //input modes
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); 
    
    //output modes
    tty.c_oflag &= ~OPOST; 
    tty.c_oflag &= ~ONLCR;    if (port_num < 0) {
        return(NULL);
    }


    //speed
    tty.c_cc[VTIME] = 10;
    tty.c_cc[VMIN] = 0;
    cfsetspeed(&tty, B115200);

    if (tcsetattr(port_num, TCSANOW, &tty) != 0) {
        return(NULL);
    }

    controller_handle controller = malloc(sizeof(s32));
    *(s32*)controller = port_num;

    return(controller);
}

internal void
brewpanel_x11_api_controller_close(
    controller_handle handle) {
    
    close(*(s32*)handle);

    free((s32*)handle);
}

internal void
brewpanel_x11_api_controller_write_buffer(
    controller_handle controller_handle,
    mem_data          write_buffer,
    u64               write_buffer_size) {

    //make sure we have a valid controller handle
    if (!controller_handle) {
        return;
    }

    //wait until we can geLOCK_EX | LOCK_NBt an exclusive lock on the controller
    s32 controller_fd = *(s32*)controller_handle;
    while(flock(controller_fd, LOCK_EX | LOCK_NB) == -1) {            
        brewpanel_nop();
    }

    u32 bytes_written =
        write(
            controller_fd,
            write_buffer,
            write_buffer_size
    );

    //release the lock on the controller
    flock(controller_fd, LOCK_UN | LOCK_NB);
}

/**
 * THREADS
 * 
 * https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
*/

internal void
brewpanel_x11_api_controller_read_thread(
    BrewPanelControlCommData* controller_comm_data) {

    s32 bytes_available = 0;

    while (true) {

        //make sure we have a valid controller handle
        if (controller_comm_data->controller == NULL) {
            continue;
        }

        //get an exclusive lock on the controller
        s32 controller_fd = *(s32*)controller_comm_data->controller;
        if(flock(controller_fd, LOCK_EX | LOCK_NB) == -1) {            
            continue;
        }

        //check if we have anything available
        ioctl(controller_fd,FIONREAD,&bytes_available);
        if (bytes_available == 0) {
            continue;
        }

        //read from the controller until we can't read anymore
        s32 bytes_read = 0;
        
        char buf[1] = {0};
        
        do {

            bytes_read = read(
                controller_fd,
                buf,
                1
            );

            if (bytes_read > 0) {
                    
                controller_comm_data->read_buffer[controller_comm_data->bytes_read] = buf[0],
                ++controller_comm_data->bytes_read;
                if (controller_comm_data->bytes_read == BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE) {
                    break;
                }
            }


        } while (bytes_read > 0);

    
        //let the app know dat was read
        if (controller_comm_data->bytes_read > 0) {
            controller_comm_data->read_callback(controller_comm_data->panel_comm_handler);
        }

        sleep(2);
        tcflush(controller_fd,TCIOFLUSH);

        //unlock the controller
        flock(controller_fd, LOCK_UN | LOCK_NB);
    }

}

internal thread_handle
brewpanel_x11_api_start_controller_comm_thread(
    BrewPanelControlCommData* controller_comm_data) {

    u64* thread_id = (u64*)malloc(sizeof(u64));

    pthread_create(
        thread_id,
        NULL,
        brewpanel_x11_api_controller_read_thread,
        controller_comm_data
    );

    return(thread_id);
}
