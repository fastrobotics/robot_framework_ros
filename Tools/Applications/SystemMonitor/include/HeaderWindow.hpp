/**
 * @file HeaderWindow.hpp
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
namespace fast::rf_ros::Tools::Applications::SystemMonitor {

    class HeaderWindow : public BaseWindow {
       public:
        static constexpr double START_X_PERC =
            0.0; /*!< What percentage of the screen to put top left corner (X) of window. */
        static constexpr double START_Y_PERC =
            0.0; /*!< What percentage of the screen to put top left corner (Y) of window. */
        static constexpr double WIDTH_PERC = 100.0; /*!< What percentage of the screen (Width) to draw the window. */
        static constexpr double HEIGHT_PERC = 15.0; /*!< What percentage of the screen (Height) to draw the window. */
        HeaderWindow(uint16_t mainwindow_height, uint16_t mainwindow_width)
            : BaseWindow("header_window", START_X_PERC, START_Y_PERC, WIDTH_PERC, HEIGHT_PERC, mainwindow_height,
                         mainwindow_width) {
            ScreenCoordinatePixel coord_pix =
                convertCoordinate(get_screen_coordinates_perc(), mainwindow_width, mainwindow_height);
            WINDOW* win =
                create_newwin(coord_pix.height_pix, coord_pix.width_pix, coord_pix.start_y_pix, coord_pix.start_x_pix);
            set_screen_coordinates_pix(coord_pix);
            set_window(win);
            wrefresh(win);
        }
        std::string pretty() override;

        bool update(double current_time_sec) override;

       private:
        bool update_window();
        double current_time_sec_;
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor