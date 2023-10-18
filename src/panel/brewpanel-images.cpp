#pragma once

#include "brewpanel-images.hpp"
#include "brewpanel-memory.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb/stb_image.h"

internal void
brewpanel_images_build_file(
    BrewPanelImagesFile* images_file) {

    u32 header_size = sizeof(BrewPanelImagesFileHeader);
    images_file->file_header.verifiction[0] = 'B'; 
    images_file->file_header.verifiction[1] = 'P'; 
    images_file->file_header.verifiction[2] = 'I'; 
    images_file->file_header.image_count    = BREWPANEL_IMAGES_ID_COUNT;

    u32 offset = 0;

    for (
        u32 image_index = 0;
        image_index < BREWPANEL_IMAGES_ID_COUNT;
        ++image_index
    ) {

        //get the file handle and make sure its valid
        str image_path = brewpanel_images_file_paths[image_index];
        brewpanel_file_handle image_file_handle = brewpanel_platform_file_open(image_path);
        brewpanel_assert(image_file_handle != NULL);
        
        u32 file_size = brewpanel_platform_file_get_size(image_file_handle);

        //read the file contents
        brewpanel_platform_file_read(
            image_file_handle,
            images_file->temp_image.temp_image_file_data,
            file_size,
            0
        );
        
        //get the pixel data for the image
        images_file->temp_image.pixels = (RGBAPixel*)stbi_load_from_memory(
            images_file->temp_image.temp_image_file_data,
            file_size,
            &images_file->temp_image.width_pixels,
            &images_file->temp_image.height_pixels,
            &images_file->temp_image.channels,
            BREWPANEL_IMAGES_COLOR_CHANNELS
        );

        u32 image_data_size = 
            sizeof(RGBAPixel) * 
            images_file->temp_image.width_pixels * 
            images_file->temp_image.height_pixels;

        RGBAPixel* pixel_buffer = (RGBAPixel*)images_file->temp_image.pixels;
        for (
            u32 pixel_index = 0;
            pixel_index < image_data_size / 4;
            ++pixel_index
        ) {
            RGBAPixel* pixel = &pixel_buffer[pixel_index];

            u8 tmp_red = pixel->red;
            pixel->red = pixel->blue;
            pixel->blue = tmp_red;
        }

        //write the data to the file
        brewpanel_platform_file_write(
            images_file->file_handle,
            (mem_data)images_file->temp_image.pixels,
            image_data_size,
            offset + header_size
        );
    
        //update the index for this image
        BrewPanelImagesFileIndex image_file_index = {0};
        image_file_index.image_id             = image_index;
        image_file_index.image_offset         = offset;
        image_file_index.image_width_pixels   = images_file->temp_image.width_pixels;
        image_file_index.image_height_pixels  = images_file->temp_image.height_pixels;
        image_file_index.image_size_pixels    = images_file->temp_image.width_pixels * images_file->temp_image.height_pixels; 
        image_file_index.image_row_size_bytes = images_file->temp_image.width_pixels * sizeof(RGBAPixel);
        image_file_index.image_size_bytes     = image_data_size;
        strcpy(image_file_index.image_name,brewpanel_images_image_names[image_index]);

        images_file->file_header.image_indexes[image_index] = image_file_index;

        offset += image_data_size; 
        images_file->file_header.image_data_size += image_data_size;

        brewpanel_platform_file_close(image_file_handle);
    }

    //now we can write the header
    brewpanel_platform_file_write(
        images_file->file_handle,
        (mem_data)(&images_file->file_header),
        sizeof(BrewPanelImagesFileHeader),
        0
    );
}

internal void
brewpanel_images_state_create(
    images_store*    images_state,
    BrewPanelMemory* memory,
    mem_data         draw_buffer) {

    *images_state = {0};
    images_state->image_instance_count = -1;
    images_state->draw_buffer = draw_buffer;

    //allocate space for temporarily storing image data
    //there will never be an image bigger than the resolution of 1024x600
    images_state->images_file.temp_image.pixels = (RGBAPixel*)brewpanel_memory_allocate(memory,BREWPANEL_IMAGES_ALLOCATION_SIZE);
    
    //this allocation is for the actual file
    //to be safe, we will allocate twice that size to give us more than enough memory
    images_state->images_file.temp_image.temp_image_file_data = brewpanel_memory_allocate(memory,BREWPANEL_IMAGES_ALLOCATION_SIZE * 2);

    //open the file or create it if it doesn't exist
    images_state->images_file.file_handle = 
        brewpanel_platform_file_open(
            BREWPANEL_IMAGES_FILE
    );

    if (images_state->images_file.file_handle == NULL) {    //TODO

    
        //the file doesn't exist, so we need to create it
        images_state->images_file.file_handle = 
            brewpanel_platform_file_create(
                BREWPANEL_IMAGES_FILE
        );
        brewpanel_assert(images_state->images_file.file_handle != NULL);
    
        //now, build the file contents from the individual images
        brewpanel_images_build_file(&images_state->images_file);
    }
    else {

        brewpanel_platform_file_read(
            images_state->images_file.file_handle,
            (mem_data)(&images_state->images_file.file_header),
            sizeof(BrewPanelImagesFileHeader),
            0
        );

        images_state->images_file.image_data = 
            brewpanel_memory_allocate(
                memory,
                images_state->images_file.file_header.image_data_size
        );
    }

    //with the file header parsed, we can read the image data
    images_state->images_file.image_data = 
        brewpanel_memory_allocate(
            memory,
            images_state->images_file.file_header.image_data_size
    );

    brewpanel_platform_file_read(
        images_state->images_file.file_handle,
        images_state->images_file.image_data,
        images_state->images_file.file_header.image_data_size,
        sizeof(BrewPanelImagesFileHeader)
    );
}

internal void
brewpanel_images_draw_image_instance(
    images_store*     images_state,
    image_instance_id image_instance_id) {

    //get image data
    image_instance instance = images_state->image_instances[image_instance_id];
    
    BrewPanelImagesFileIndex image_index = brewpanel_images_index(images_state,instance.image_id);
    RGBAPixel* image_address = brewpanel_images_address(images_state,instance.image_id);

    //set the pixel offset
    u32 draw_buffer_row_offset = (instance.x_offset * sizeof(RGBAPixel)) + (instance.y_offset * BREW_PANEL_ROW_BYTES);

    for (
        u32 image_pixel_row = 0;
        image_pixel_row < image_index.image_height_pixels;
        ++image_pixel_row
    ) {
        
        //calculate the offsets
        u32 image_byte_offset = image_pixel_row * image_index.image_row_size_bytes;
        u32 draw_byte_offset  = (image_pixel_row * BREW_PANEL_ROW_BYTES) + draw_buffer_row_offset;

        //get the source and destination rows
        mem_data image_pixels = ((mem_data)image_address) + image_byte_offset; 
        mem_data draw_pixels  = images_state->draw_buffer + draw_byte_offset;

        memmove(
            draw_pixels,
            image_pixels,
            image_index.image_row_size_bytes
        );
    }
}

internal void
brewpanel_images_draw_image_immediate(
    BrewPanelImagesStore* images_state,
    u16                   image_id,
    u32                   x_offset,
    u32                   y_offset) {

    //get image data
    BrewPanelImagesFileIndex image_index = brewpanel_images_index(images_state,image_id);
    RGBAPixel* image_address = brewpanel_images_address(images_state,image_id);

    //set the pixel offset
    u32 draw_buffer_row_offset = (x_offset * sizeof(RGBAPixel)) + (y_offset * BREW_PANEL_ROW_BYTES);

    for (
        u32 image_pixel_row = 0;
        image_pixel_row < image_index.image_height_pixels;
        ++image_pixel_row
    ) {
        
        //calculate the offsets
        u32 image_byte_offset = image_pixel_row * image_index.image_row_size_bytes;
        u32 draw_byte_offset  = (image_pixel_row * BREW_PANEL_ROW_BYTES) + draw_buffer_row_offset;

        //get the source and destination rows
        mem_data image_pixels = ((mem_data)image_address) + image_byte_offset; 
        mem_data draw_pixels  = images_state->draw_buffer + draw_byte_offset;

        memmove(
            draw_pixels,
            image_pixels,
            image_index.image_row_size_bytes
        );
    }
}

internal image_instance_id
brewpanel_images_create_image_instance(
    images_store* images,
    image_id      image_id,
    u32           x_offset,
    u32           y_offset) {

    //make sure we can create an image
    brewpanel_assert(images->image_instance_count != BREWPANEL_IMAGES_COUNT);
    ++images->image_instance_count;

    image_instance instance = {0};
    instance.image_id = image_id;
    instance.x_offset = x_offset;
    instance.y_offset = y_offset;

    images->image_instances[images->image_instance_count] = instance;

    return(images->image_instance_count);
}

internal void
brewpanel_images_update_instance_image(
    images_store*     images,
    image_instance_id instance_id,
    image_id          image_id) {

    images->image_instances[instance_id].image_id = image_id;
}