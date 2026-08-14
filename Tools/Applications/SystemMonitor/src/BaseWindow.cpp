#include <BaseWindow.hpp>
#include <Infrastructure/Logger.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    bool BaseWindow::is_initialized() {
        if (name == "") {
            fast::rf::Logger::log_error("Window Name is Not Set!");
            return false;
        }
        if (win_ == nullptr) {
            fast::rf::Logger::log_error("Window: " + name + " Pointer is Null!");
            return false;
        }
        return true;
    }
    bool BaseWindow::update(double current_time_sec) {
        current_time_sec_ = current_time_sec;
        return true;
    }
    std::string BaseWindow::pretty() {
        if (is_initialized() == false) {
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