#ifndef BREWPANEL_CLOCK_HPP
#define BREWPANEL_CLOCK_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-platform-api.hpp"

#define BREWPANEL_CLOCK_OFFSET_X 60

const image_id brewpanel_clock_glyph_table[] = {
    BREWPANEL_IMAGES_ID_CLOCK_0,
    BREWPANEL_IMAGES_ID_CLOCK_1,
    BREWPANEL_IMAGES_ID_CLOCK_2,
    BREWPANEL_IMAGES_ID_CLOCK_3,
    BREWPANEL_IMAGES_ID_CLOCK_4,
    BREWPANEL_IMAGES_ID_CLOCK_5,
    BREWPANEL_IMAGES_ID_CLOCK_6,
    BREWPANEL_IMAGES_ID_CLOCK_7,
    BREWPANEL_IMAGES_ID_CLOCK_8,
    BREWPANEL_IMAGES_ID_CLOCK_9
};

struct BrewPanelClockFaceDigits {
    image_id tens_face;
    image_id ones_face;
};

struct BrewPanelClockFaces {
    BrewPanelClockFaceDigits hours;
    BrewPanelClockFaceDigits minutes;
    BrewPanelClockFaceDigits seconds;
};

struct BrewPanelClock {
    BrewPanelClockFaces      faces;
    image_id                 am_pm_face;    
    BrewPanelSystemTime      system_time;
};

#endif //BREWPANEL_CLOCK_HPP