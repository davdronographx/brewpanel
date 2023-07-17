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
        file_handle image_file_handle = brewpanel_platform_file_open(image_path);
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
            &images_file->temp_image.width,
            &images_file->temp_image.height,
            &images_file->temp_image.channels,
            BREWPANEL_IMAGES_COLOR_CHANNELS
        );

        u32 image_data_size = 
            sizeof(RGBAPixel) * 
            images_file->temp_image.width * 
            images_file->temp_image.height;

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
        image_file_index.image_id     = image_index;
        image_file_index.image_offset = offset;
        image_file_index.image_width  = images_file->temp_image.width;
        image_file_index.image_height = images_file->temp_image.height;
        image_file_index.image_size   = image_data_size;
        strcpy(image_file_index.image_name,brewpanel_images_image_names[image_index]);

        images_file->file_header.image_indexes[image_index] = image_file_index;

        offset += image_data_size; 
        images_file->file_header.image_data_size += image_data_size;
    }

    //now we can write the header
    brewpanel_platform_file_write(
        images_file->file_handle,
        (mem_data)(&images_file->file_header),
        sizeof(BrewPanelImagesFileHeader),
        0
    );
}

internal BrewPanelImagesState
brewpanel_images_state_create(
    BrewPanelMemory* memory) {

    BrewPanelImagesState images_state = {0};

    //allocate space for temporarily storing image data
    //there will never be an image bigger than the resolution of 1024x600
    images_state.images_file.temp_image.pixels = (RGBAPixel*)brewpanel_memory_allocate(memory,BREWPANEL_IMAGES_ALLOCATION_SIZE);
    
    //this allocation is for the actual file
    //to be safe, we will allocate twice that size to give us more than enough memory
    images_state.images_file.temp_image.temp_image_file_data = brewpanel_memory_allocate(memory,BREWPANEL_IMAGES_ALLOCATION_SIZE * 2);

    //open the file or create it if it doesn't exist
    images_state.images_file.file_handle = 
        brewpanel_platform_file_open(
            BREWPANEL_IMAGES_FILE
    );

    if (images_state.images_file.file_handle == NULL) {
    
        //the file doesn't exist, so we need to create it
        images_state.images_file.file_handle = 
            brewpanel_platform_file_create(
                BREWPANEL_IMAGES_FILE
        );
        brewpanel_assert(images_state.images_file.file_handle != NULL);
    
        //now, build the file contents from the individual images
        brewpanel_images_build_file(&images_state.images_file);
    }
    else {

        brewpanel_platform_file_read(
            images_state.images_file.file_handle,
            (mem_data)(&images_state.images_file.file_header),
            sizeof(BrewPanelImagesFileHeader),
            0
        );

        images_state.images_file.image_data = 
            brewpanel_memory_allocate(
                memory,
                images_state.images_file.file_header.image_data_size
        );
    }

    //with the file header parsed, we can read the image data
    images_state.images_file.image_data = 
        brewpanel_memory_allocate(
            memory,
            images_state.images_file.file_header.image_data_size
    );

    brewpanel_platform_file_read(
        images_state.images_file.file_handle,
        images_state.images_file.image_data,
        images_state.images_file.file_header.image_data_size,
        sizeof(BrewPanelImagesFileHeader)
    );


    return(images_state);
}

internal void
brewpanel_images_draw_image(
    BrewPanelImagesState* images_state,
    BrewPanelImagesId     image_id,
    u32                   x_offset,
    u32                   y_offset,
    mem_data              draw_buffer) {

    //get image data
    BrewPanelImagesFileIndex image_index = brewpanel_images_index(images_state,image_id);
    RGBAPixel* image_address = brewpanel_images_address(images_state,image_id)

    //write the image to the buffer
    memmove(
        draw_buffer,
        image_address,
        image_index.image_size
    );    
}