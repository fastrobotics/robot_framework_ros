/**
 * @file IWindow.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-08-13
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <string>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    class IWindow {
       public:
        virtual ~IWindow() {}
        virtual std::string get_name() = 0;

        virtual std::string pretty() = 0;
        virtual bool update(double current_time_sec) = 0;
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor