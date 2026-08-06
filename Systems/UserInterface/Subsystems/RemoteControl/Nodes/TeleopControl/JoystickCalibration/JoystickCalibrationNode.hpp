/**
 * @file JoystickCalibrationNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <sensor_msgs/Joy.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem {
    /**
     * @brief JoystickCalibration Node
     *
     */
    class JoystickCalibrationNode : public BaseNode {
       public:
        struct CalibrationData {
            double x_min;
            double x_max;
            double x_deadband;
            double y_min;
            double y_max;
            double y_deadband;
            double throttle_min;
            double throttle_max;
            double throttle_deadband;
            CalibrationData() {
                x_deadband = 0.0;
                x_min = INFINITY;
                x_max = -INFINITY;
                y_deadband = 0.0;
                y_min = INFINITY;
                y_max = -INFINITY;
                throttle_min = INFINITY;
                throttle_max = -INFINITY;
                throttle_deadband = 0.0;
            }
            std::string pretty() {
                std::string str = "\nX:\n\t DB: " + std::to_string(x_deadband) + " min: " + std::to_string(x_min) +
                                  " max: " + std::to_string(x_max) + "\n";
                str += "Y:\n\t DB: " + std::to_string(y_deadband) + " min: " + std::to_string(y_min) +
                       " max: " + std::to_string(y_max) + "\n";
                str += "Thr:\n\t DB: " + std::to_string(throttle_deadband) + " min: " + std::to_string(throttle_min) +
                       " max: " + std::to_string(throttle_max);
                return str;
            }
        };

        JoystickCalibrationNode();
        ~JoystickCalibrationNode();

        /**
         * @brief Initialize the Node
         *
         * @return true
         * @return false
         */
        bool init();

        /**
         * @brief Start the Node
         *
         * @return true
         * @return false
         */
        bool start();

        /**
         * @brief 100Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_100hz();

        /**
         * @brief 10Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_10hz();

        /**
         * @brief 1Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_1hz();

        /**
         * @brief 0.1Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_01hz();

        /**
         * @brief 0.01Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_001hz();

        /**
         * @brief User Function 1
         *
         * @return true
         * @return false
         */
        bool run_loop1();

        /**
         * @brief User Function 2
         *
         * @return true
         * @return false
         */
        bool run_loop2();

        /**
         * @brief User Function 3
         *
         * @return true
         * @return false
         */
        bool run_loop3();

        /**
         * @brief User Thread Loop
         *
         */
        void thread_loop();

        void joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg);

       private:
        ros::Subscriber joy_sub;
    };
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem
