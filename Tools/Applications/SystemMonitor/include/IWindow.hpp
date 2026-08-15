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
#include <robot_framework_ros/arm_command.h>
#include <robot_framework_ros/heartbeat.h>
#include <robot_framework_ros/ready_to_arm.h>

#include <string>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    /**
     * @brief A generic Window interface
     *
     */
    class IWindow {
       public:
        virtual ~IWindow() {}
        /**
         * @brief Get the name
         *
         * @return std::string
         */
        virtual std::string get_name() = 0;
        /**
         * @brief Check if initialized
         *
         * @return true
         * @return false
         */
        virtual bool is_initialized() = 0;
        /**
         * @brief Update the object
         *
         * @param current_time_sec
         * @return true
         * @return false
         */
        virtual bool update(double current_time_sec) = 0;
        /**
         * @brief Pretty print human readable data
         *
         * @return std::string
         */
        virtual std::string pretty() = 0;

        /**
         * @brief Check if the window is currently focused
         *
         * @return true
         * @return false
         */
        virtual bool has_focus() = 0;

        /**
         * @brief Set the focus
         *
         * @param cmd_focus
         * @return true
         * @return false
         */
        virtual bool set_focus(bool cmd_focus) = 0;

        virtual KeyEventContainer new_keyevent(int key) = 0;

        virtual void new_ArmCommandMsg(robot_framework_ros::arm_command msg) = 0;

        virtual void new_HeartbeatMsg(robot_framework_ros::heartbeat msg) = 0;

        virtual void new_ReadyToArmMsg(robot_framework_ros::ready_to_arm msg) = 0;
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor