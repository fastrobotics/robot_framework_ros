/**
 * @file DiagnosticWindow.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-08-13
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <BaseWindow.hpp>
#include <map>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    /**
     * @brief Diagnostic Window
     *
     */
    class DiagnosticWindow : public BaseWindow {
       public:
        class NodeDiagnosticMonitor {
           public:
            robot_framework_ros::diagnostic node_diagnostic;
            double last_update_sec;
            double last_update_delta_sec;
        };
        enum class DiagnosticWindowMode { UNKNOWN = 0, NODE = 1, DIAGNOSTIC_TYPE = 2, SYSTEM = 3 };
        static constexpr double START_X_PERC =
            66.0; /*!< What percentage of the screen to put top left corner (X) of window. */
        static constexpr double START_Y_PERC =
            15.0; /*!< What percentage of the screen to put top left corner (Y) of window. */
        static constexpr double WIDTH_PERC = 34.5;  /*!< What percentage of the screen (Width) to draw the window. */
        static constexpr double HEIGHT_PERC = 60.0; /*!< What percentage of the screen (Height) to draw the window. */

        DiagnosticWindow(int16_t tab_order, int16_t mainwindow_height, uint16_t mainwindow_width)
            : BaseWindow("diagnostic_window", tab_order, START_X_PERC, START_Y_PERC, WIDTH_PERC, HEIGHT_PERC,
                         mainwindow_height, mainwindow_width) {
            ScreenCoordinatePixel coord_pix =
                convertCoordinate(get_screen_coordinates_perc(), mainwindow_width, mainwindow_height);
            WINDOW* win =
                create_newwin(coord_pix.height_pix, coord_pix.width_pix, coord_pix.start_y_pix, coord_pix.start_x_pix);
            set_screen_coordinates_pix(coord_pix);
            set_window(win);
            std::string header = get_window_header();
            mvwprintw(win, 1, 1, header.c_str());
            std::string dashed(get_screen_coordinates_pixel().width_pix - 2, '-');
            mvwprintw(win, 2, 1, dashed.c_str());
            wrefresh(win);
            wrefresh(win);
        }
        KeyEventContainer newKeyEvent([[maybe_unused]] int key) override {
            KeyEventContainer container;
            return container;
        }
        void newArmCommandMsg([[maybe_unused]] robot_framework_ros::arm_command msg) override {}

        void newHeartbeatMsg([[maybe_unused]] robot_framework_ros::heartbeat msg) override {}

        void newReadyToArmMsg([[maybe_unused]] robot_framework_ros::ready_to_arm msg) override {}

        void newDiagnosticMsg(robot_framework_ros::diagnostic msg) override;
        /**
         * @brief Human readable string
         *
         * @return std::string
         */
        std::string pretty() override;

        /**
         * @brief Update window at a regular rate
         *
         * @param currentTimeSec
         * @return true
         * @return false
         */
        bool update(double currentTimeSec) override;

        std::map<std::string, std::map<uint8_t, NodeDiagnosticMonitor>> get_node_diagnostic_monitors() {
            return node_diagnostic_monitors;
        }
        void set_node_to_monitor(std::string node_name) { node_to_monitor = node_name; }

       protected:
        bool update_window();

       private:
        DiagnosticWindowMode window_mode{DiagnosticWindowMode::NODE};
        std::string get_window_header();
        std::map<std::string, std::map<uint8_t, NodeDiagnosticMonitor>> node_diagnostic_monitors;
        std::string node_to_monitor{""};
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor