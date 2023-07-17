#ifndef BREWPANEL_IMAGES_HPP
#define BREWPANEL_IMAGES_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"

#define BREWPANEL_IMAGES_FILE "brewpanel-images.bpi"

#define BREWPANEL_IMAGES_COUNT           128
#define BREWPANEL_IMAGES_NAME_LENGTH     16
#define BREWPANEL_IMAGES_COLOR_CHANNELS  4
#define BREWPANEL_IMAGES_ALLOCATION_SIZE BREW_PANEL_PIXEL_COUNT

//TODO: rename
struct BrewPanelImage {
    s32        width;
    s32        height;
    s32        channels;
    RGBAPixel* pixels;
    mem_data   temp_image_file_data;
};

enum BrewPanelImagesId : u16 {
    BREWPANEL_IMAGES_ID_MAIN_SCREEN = 0,
    BREWPANEL_IMAGES_ID_COUNT       = 1
};

global str brewpanel_images_file_paths[] = {
    "main-screen.png"
};

global str brewpanel_images_image_names[] = {
    "main-screen"
};

struct BrewPanelImagesFileIndex {
    u16  image_id;
    u32  image_offset;
    u32  image_width;
    u32  image_height;
    u32  image_size;    
    char image_name[BREWPANEL_IMAGES_NAME_LENGTH];
};

struct BrewPanelImagesFileHeader {
    char                     verifiction[3];
    u16                      image_count;
    u32                      image_data_size;
    BrewPanelImagesFileIndex image_indexes[BREWPANEL_IMAGES_ID_COUNT];
};

struct BrewPanelImagesFile {
    file_handle               file_handle;
    BrewPanelImagesFileHeader file_header;
    BrewPanelImage            temp_image;
    mem_data                  image_data;
};

struct BrewPanelImagesState {
    BrewPanelImagesFile images_file;
};

#define brewpanel_images_index(images_state,image_id)   images_state->images_file.file_header.image_indexes[image_id]
#define brewpanel_images_address(images_state,image_id) (RGBAPixel*)(&images_state->images_file.image_data[brewpanel_images_index(images_state,image_id).image_offset]);

#endif //BREWPANEL_IMAGES_HPP