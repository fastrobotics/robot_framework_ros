#include <BaseWindow.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    std::string BaseWindow::pretty() {
        if (win_ == nullptr) {
            return name + " Is Uninitialized.";
        } else {
            char tempstr[128];
            sprintf(tempstr, "%s (X:%d%%Y:%d%%W:%d%%H:%d%%)", name.c_str(), (uint16_t)screen_coord_perc.start_x_perc,
                    (uint16_t)screen_coord_perc.start_y_perc, (uint16_t)screen_coord_perc.width_perc,
                    (uint16_t)screen_coord_perc.height_perc);
            return std::string(tempstr);
        }
    }
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor