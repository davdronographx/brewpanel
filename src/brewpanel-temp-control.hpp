#ifndef BREWPANEL_TEMP_CONTROL_HPP
#define BREWPANEL_TEMP_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE 0
#define BREWPANEL_TEMP_READ_PANEL_X_OFFSET      0
#define BREWPANEL_TEMP_READ_PANEL_Y_SPACING     10

struct BrewPanelTempRead {
    bool     redraw;
    image_id panel_id;
    u32      y_offset;
    u8       temp_fahrenheit;
};
typedef BrewPanelTempRead temp_read;

struct BrewPanelTempControl {
    bool      redraw;
    temp_read hlt_temp_panel;
    temp_read mlt_temp_panel;
    temp_read boil_temp_panel;
};
typedef BrewPanelTempControl temp_control;


#endif //BREWPANEL_TEMP_CONTROL_HPP