#include <Windows/HeaderWindow.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    std::string HeaderWindow::pretty() {
        std::string str = "---Header Window---\n";
        str += BaseWindow::pretty();
        return str;
    }
    bool HeaderWindow::update(double current_time_sec) {
        bool status = BaseWindow::update(current_time_sec);
        if (status == false) {
            return false;
        }
        status = update_window();
        return status;
    }
    bool HeaderWindow::update_window() {
        if (get_window() == nullptr) {
            return false;
        }
        //  GCOVR_EXCL_START
        {  // Time
            std::string str = "Time: " + std::to_string(current_time_sec_);
            str.insert(str.end(), 40 - str.size(), ' ');
            Color color;
            color = Color::RED_COLOR;

            wattron(get_window(), COLOR_PAIR(color));
            mvwprintw(get_window(), 1, 1, str.c_str());
            wclrtoeol(get_window());
            wattroff(get_window(), COLOR_PAIR(color));
        }
        {  // Armed State

            Color color;
            std::string str = "Armed State: Unknown";
            str.insert(str.end(), 40 - str.size(), ' ');

            color = Color::RED_COLOR;

            wattron(get_window(), COLOR_PAIR(color));
            mvwprintw(get_window(), 2, 1, str.c_str());
            wclrtoeol(get_window());
            wattroff(get_window(), COLOR_PAIR(color));
        }

        box(get_window(), 0, 0);
        wrefresh(get_window());
        return true;
    }
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor