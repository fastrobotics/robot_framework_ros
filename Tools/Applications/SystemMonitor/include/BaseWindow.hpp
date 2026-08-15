/**
 * @file BaseWindow.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-08-13
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once
#include <curses.h>

#include <CommonWindowUtility.hpp>
#include <Definitions.hpp>
#include <IWindow.hpp>
#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/CoreUtility.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>

#include "ros/ros.h"
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    /**
     * @brief Abstract Base Window
     *
     */
    class BaseWindow : public IWindow {
       public:
        /**
         * @brief Construct a new Base Window object
         *
         * @param name
         * @param tab_order
         * @param start_x_perc
         * @param start_y_perc
         * @param width_perc
         * @param height_perc
         * @param mainwindow_height
         * @param mainwindow_width
         */
        BaseWindow(const std::string name, int16_t tab_order, double start_x_perc, double start_y_perc,
                   double width_perc, double height_perc, uint16_t mainwindow_height, uint16_t mainwindow_width)
            : name(name),
              tab_order(tab_order),
              screen_coord_perc(start_x_perc, start_y_perc, width_perc, height_perc),
              screen_coord_pixel(0, 0, 0, 0),
              mainwindow_height(mainwindow_height),
              mainwindow_width(mainwindow_width) {}
        virtual ~BaseWindow() {}
        /**
         * @brief Check if initialized
         *
         * @return true
         * @return false
         */
        bool is_initialized() override;
        /**
         * @brief Get the window name
         *
         * @return std::string
         */
        std::string get_name() override { return name; }

        /**
         * @brief Check if the window has focus
         *
         * @return true
         * @return false
         */
        bool has_focus() override { return focused; }

        /**
         * @brief Set the Window's focus
         *
         * @param cmd_focus
         * @return true
         * @return false
         */
        bool set_focus(bool cmd_focus) override {
            focused = cmd_focus;
            return true;
        }

       protected:
        /**
         * @brief Abstract Periodic Update
         *
         * @param current_time_sec
         * @return true
         * @return false
         */
        bool update(double current_time_sec);

        double get_current_time_sec() { return current_time_sec_; }
        /**
         * @brief Update the window details
         *
         * @return true
         * @return false
         */
        virtual bool update_window() = 0;
        /**
         * @brief Get base object pretty data
         *
         * @return std::string
         */
        std::string pretty() override = 0;
        /**
         * @brief Set the window
         *
         * @param win
         */
        void set_window(WINDOW* win) { win_ = win; }
        /**
         * @brief Get the window object
         *
         * @return WINDOW*
         */
        WINDOW* get_window() { return win_; }
        /**
         * @brief Set the screen coordinates pix object
         *
         * @param coord
         */
        void set_screen_coordinates_pix(ScreenCoordinatePixel coord) { screen_coord_pixel = coord; }
        /**
         * @brief Get the screen coordinates perc object
         *
         * @return ScreenCoordinatePerc
         */
        ScreenCoordinatePerc get_screen_coordinates_perc() { return screen_coord_perc; }
        /**
         * @brief Get the screen coordinates pixel object
         *
         * @return ScreenCoordinatePixel
         */
        ScreenCoordinatePixel get_screen_coordinates_pixel() { return screen_coord_pixel; }
        /**
         * @brief Get the tab order
         *
         * @return int16_t
         */
        int16_t get_tab_order() { return tab_order; }

        int16_t get_selected_record() { return record_selected; }

        void update_record_count(uint16_t count);
        uint16_t get_mainwindow_height() { return mainwindow_height; }
        uint16_t get_mainwindow_width() { return mainwindow_width; }
        void decrement_selected_record();
        void increment_selected_record();
        bool get_focused() { return focused; }
        std::vector<int> supported_keys;

       private:
        WINDOW* win_{nullptr};
        std::string name{""};
        int16_t tab_order;
        ScreenCoordinatePerc screen_coord_perc;
        ScreenCoordinatePixel screen_coord_pixel;
        uint16_t mainwindow_height;
        uint16_t mainwindow_width;
        bool focused{false};

        double current_time_sec_;
        int16_t record_selected{0};
        uint16_t record_count{0};
    };

}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor