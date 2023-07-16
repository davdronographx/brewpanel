#pragma once

#include "brewpanel-images.hpp"
#include "brewpanel-memory.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb/stb_image.h"

internal void
brewpanel_images_build_file(
    BrewPanelImagesFile* images_file) {

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
        u32 bytes_read = 
            platform_api.file_read(
                image_file_handle,
                images_file->image_data.temp_image_file_data,
                file_size,
                0
        );

        //TODO: overlapped something something blah blah blah
        // brewpanel_assert(bytes_read > 0);

        images_file->image_data.pixels = (RGBAPixel*)stbi_load_from_memory(
            images_file->image_data.temp_image_file_data,
            file_size,
            &images_file->image_data.width,
            &images_file->image_data.height,
            &images_file->image_data.channels,
            BREWPANEL_IMAGES_COLOR_CHANNELS
        );

    }

}

internal BrewPanelImagesState
brewpanel_images_state_create(
    BrewPanelMemory* memory) {

    BrewPanelImagesState images_state = {0};

    //allocate space for temporarily storing image data
    //there will never be an image bigger than the resolution of 1024x600
    images_state.images_file.image_data.pixels = (RGBAPixel*)brewpanel_memory_allocate(memory,BREWPANEL_IMAGES_ALLOCATION_SIZE);
    
    //this allocation is for the actual file
    //to be safe, we will allocate twice that size to give us more than enough memory
    images_state.images_file.image_data.temp_image_file_data = brewpanel_memory_allocate(memory,BREWPANEL_IMAGES_ALLOCATION_SIZE * 2);

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

    return(images_state);
}