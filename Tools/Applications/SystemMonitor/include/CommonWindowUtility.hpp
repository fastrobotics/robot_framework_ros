/**
 * @file CommonWindowUtility.hpp
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

#include <Definitions.hpp>

namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    WINDOW* create_newwin(int height, int width, int starty, int startx);
    ScreenCoordinatePixel convertCoordinate(ScreenCoordinatePerc coord_perc, uint16_t width_pix, uint16_t height_pix);
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor
