/**
 * @file JoystickCalibrationNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 * @compare_tag Node-Header  v0.1
 */
#pragma once

#include <sensor_msgs/Joy.h>

#include <ITeleopControlProcess.hpp>
#include <fstream>
#include <iostream>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl {
    /**
     * @brief JoystickCalibration Node
     *
     */
    class JoystickCalibrationNode : public BaseNode {
       public:
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
         * @brief Load configuration from config and sets data
         *
         * @return true
         * @return false
         */
        bool load_config() override;

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

        /**
         * @brief Process a Joy Message
         *
         * @param t_msg
         */
        void joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg);

        /**
         * @brief Stop the Node
         *
         */
        void stop();

       private:
        std::atomic<bool> is_node_running{false};  //!< If the node is running
        std::ofstream output_config_fd;
        fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::JoystickCalibrationData calibration_data;
        ros::Subscriber joy_sub;
    };
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl
