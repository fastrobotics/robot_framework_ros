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
    void BaseWindow::decrement_selected_record() {
        auto _current_record = record_selected;
        _current_record--;
        if (_current_record < 0) {
            _current_record = 0;
        }
        record_selected = _current_record;
    }
    void BaseWindow::increment_selected_record() {
        auto _current_record = record_selected;
        _current_record++;
        if (_current_record >= (record_count - 1)) {
            _current_record = (record_count - 1);
        }
        record_selected = _current_record;
    }
    void BaseWindow::update_record_count(uint16_t count) { record_count = count; }
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