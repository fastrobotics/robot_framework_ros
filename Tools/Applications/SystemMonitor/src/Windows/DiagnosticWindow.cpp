#include <Windows/DiagnosticWindow.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    std::string DiagnosticWindow::pretty() {
        std::string str = "---Diagnostic Window---\n";
        str += BaseWindow::pretty();
        return str;
    }
    void DiagnosticWindow::new_DiagnosticMsg(robot_framework_ros::diagnostic msg) {
        node_diagnostic_monitors[msg.NodeName][msg.DiagnosticType].node_diagnostic = msg;
        node_diagnostic_monitors[msg.NodeName][msg.DiagnosticType].last_update_sec = get_current_time_sec();
    }
    bool DiagnosticWindow::update(double current_time_sec) {
        bool status = BaseWindow::update(current_time_sec);
        if (status == false) {
            return false;
        }
        for (auto& node_diagnostics : node_diagnostic_monitors) {
            for (auto& diagnostic : node_diagnostics.second) {
                diagnostic.second.last_update_delta_sec = (get_current_time_sec() - diagnostic.second.last_update_sec);
                if (diagnostic.second.last_update_delta_sec > COMMTIMEOUT_THRESHOLD) {
                    diagnostic.second.node_diagnostic.Level = (uint8_t)fast::rf::Level::UNKNOWN;
                }
            }
        }
        status = update_window();
        return status;
    }
    std::string DiagnosticWindow::get_window_header() {
        if (window_mode == WindowMode::NODE) {
            return "  NODE DIAGNOSTICS";
        }
        return "";
    }
    bool DiagnosticWindow::update_window() {
        if (get_window() == nullptr) {
            return false;
        }
        if (window_mode == WindowMode::NODE) {
            auto node_diagnostic_it = node_diagnostic_monitors.find(node_to_monitor);
            if (node_diagnostic_it == node_diagnostic_monitors.end()) {
                // Node not present yet.  Don't do anything.
                return true;
            }
            const uint16_t DIAGSTART_COORD_Y = 1;
            const uint16_t DIAGSTART_COORD_X = 1;
            uint16_t index = 0;
            for (auto i = (uint8_t)fast::rf::DiagnosticDefinition::DiagnosticType::UNKNOWN;
                 i < (uint8_t)fast::rf::DiagnosticDefinition::DiagnosticType::END_OF_LIST; ++i) {
                fast::rf::DiagnosticDefinition::DiagnosticType diagnostic_type =
                    (fast::rf::DiagnosticDefinition::DiagnosticType)i;
                if ((diagnostic_type == fast::rf::DiagnosticDefinition::DiagnosticType::UNKNOWN) ||
                    (diagnostic_type == fast::rf::DiagnosticDefinition::DiagnosticType::UNKNOWN_TYPE) ||
                    (diagnostic_type == fast::rf::DiagnosticDefinition::DiagnosticType::END_OF_LIST)) {
                    continue;
                }
                auto node_diagnostic_map = node_diagnostic_it->second;
                auto diagnostic_type_it = node_diagnostic_map.find(i);

                if (diagnostic_type_it != node_diagnostic_map.end()) {
                    fast::rf::Level level = (fast::rf::Level)diagnostic_type_it->second.node_diagnostic.Level;
                    fast::rf::DiagnosticDefinition::DiagnosticMessage diag_message =
                        (fast::rf::DiagnosticDefinition::DiagnosticMessage)
                            diagnostic_type_it->second.node_diagnostic.DiagnosticMessage;
                    Color color = Color::UNKNOWN;
                    switch (level) {
                        case fast::rf::Level::UNKNOWN:
                            color = Color::RED_COLOR;
                            break;
                        case fast::rf::Level::NOERROR:
                            color = Color::BLUE_COLOR;
                            break;
                        case fast::rf::Level::DEBUG:
                            color = Color::BLUE_COLOR;
                            break;
                        case fast::rf::Level::INFO:
                            color = Color::BLUE_COLOR;
                            break;
                        case fast::rf::Level::NOTICE:
                            color = Color::BLUE_COLOR;
                            break;
                        case fast::rf::Level::WARN:
                            color = Color::YELLOW_COLOR;
                            break;
                        case fast::rf::Level::ERROR:
                            color = Color::RED_COLOR;
                            break;
                        case fast::rf::Level::FATAL:
                            color = Color::RED_COLOR;
                            break;
                        default:
                            color = Color::RED_COLOR;
                            break;
                    }

                    wattron(get_window(), COLOR_PAIR(color));
                    std::string str = fast::rf::DiagnosticDefinition::pretty(diagnostic_type) + ":" +
                                      fast::rf::DiagnosticDefinition::pretty(diag_message) + "(" +
                                      std::to_string(diagnostic_type_it->second.node_diagnostic.stamp.toSec()) +
                                      ") - " + diagnostic_type_it->second.node_diagnostic.Description;
                    int window_width = getmaxx(get_window());
                    if (str.size() >= (std::size_t)window_width) {
                        str.erase(window_width - 3);
                    }
                    mvwprintw(get_window(), DIAGSTART_COORD_Y + 2 + (int)index, DIAGSTART_COORD_X + 1, str.c_str());
                    wclrtoeol(get_window());
                    wattroff(get_window(), COLOR_PAIR(color));

                } else {
                    Color color = Color::NO_COLOR;
                    wattron(get_window(), COLOR_PAIR(color));
                    std::string str = fast::rf::DiagnosticDefinition::pretty(diagnostic_type);
                    mvwprintw(get_window(), DIAGSTART_COORD_Y + 2 + (int)index, DIAGSTART_COORD_X + 1, str.c_str());
                    wclrtoeol(get_window());
                    wattroff(get_window(), COLOR_PAIR(color));
                }

                index++;
            }
        }

        // GCOVR_EXCL_START
        box(get_window(), 0, 0);
        wrefresh(get_window());
        return true;
        // GCOVR_EXCL_STOP
    }
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor