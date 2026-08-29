#include <Windows/StatusWindow.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    std::string StatusWindow::pretty() {
        std::string str = "---Status Window---\n";
        str += BaseWindow::pretty();
        return str;
    }
    bool StatusWindow::update(double currentTimeSec) {
        bool status = BaseWindow::update(currentTimeSec);
        if (status == false) {
            return false;
        }
        status = update_window();
        return status;
    }
    bool StatusWindow::update_window() {
        if (get_window() == nullptr) {
            return false;
        }
        // GCOVR_EXCL_START
        box(get_window(), 0, 0);
        wrefresh(get_window());
        return true;
        // GCOVR_EXCL_STOP
    }
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor