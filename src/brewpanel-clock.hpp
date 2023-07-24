#ifndef BREWPANEL_CLOCK_HPP
#define BREWPANEL_CLOCK_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-platform-api.hpp"

struct BrewPanelClockFaceDigits {
    image_id tens_face;
    image_id ones_face;
};

struct BrewPanelClock {
    BrewPanelClockFaceDigits hours_faces;
    BrewPanelClockFaceDigits minutes_faces;
    BrewPanelClockFaceDigits seconds_faces;
    image_id                 am_pm_face;    
    BrewPanelSystemTime      system_time;
};

#endif //BREWPANEL_CLOCK_HPP