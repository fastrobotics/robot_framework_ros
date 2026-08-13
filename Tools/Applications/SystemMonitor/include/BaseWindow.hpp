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
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    class BaseWindow : public IWindow {
       public:
        BaseWindow(const std::string name, double start_x_perc, double start_y_perc, double width_perc,
                   double height_perc, uint16_t mainwindow_height, uint16_t mainwindow_width)
            : name(name),
              screen_coord_perc(start_x_perc, start_y_perc, width_perc, height_perc),
              screen_coord_pixel(0, 0, 0, 0),
              mainwindow_height(mainwindow_height),
              mainwindow_width(mainwindow_width) {}
        virtual ~BaseWindow() {}
        std::string get_name() override { return name; }
        void set_window(WINDOW* win) { win_ = win; }
        WINDOW* get_window() { return win_; }
        void set_screen_coordinates_pix(ScreenCoordinatePixel coord) { screen_coord_pixel = coord; }
        ScreenCoordinatePerc get_screen_coordinates_perc() { return screen_coord_perc; }
        ScreenCoordinatePixel get_screen_coordinates_pixel() { return screen_coord_pixel; }

       protected:
        std::string pretty() override = 0;
        std::string name{""};
        ScreenCoordinatePerc screen_coord_perc;
        ScreenCoordinatePixel screen_coord_pixel;
        uint16_t mainwindow_height;
        uint16_t mainwindow_width;

       private:
        WINDOW* win_{nullptr};
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor