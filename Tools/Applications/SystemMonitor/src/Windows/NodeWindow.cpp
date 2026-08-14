#include <Windows/NodeWindow.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    std::string NodeWindow::pretty() {
        std::string str = "---Node Window---\n";
        str += BaseWindow::pretty();
        return str;
    }
    bool NodeWindow::update(double current_time_sec) {
        bool status = BaseWindow::update(current_time_sec);
        if (status == false) {
            return false;
        }
        status = update_window();
        return status;
    }
    bool NodeWindow::update_window() {
        if (get_window() == nullptr) {
            return false;
        }

        box(get_window(), 0, 0);
        wrefresh(get_window());
        return true;
    }
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor