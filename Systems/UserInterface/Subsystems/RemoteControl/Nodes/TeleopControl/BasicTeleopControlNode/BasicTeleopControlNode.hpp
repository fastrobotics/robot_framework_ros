/**
 * @file BasicTeleopControlNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-08
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

#include <BasicTeleopControlProcess/BasicTeleopControlProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem {
    /**
     * @brief Basic Teleop Control Node
     *
     */
    class BasicTeleopControlNode : public BaseNode {
       public:
        BasicTeleopControlNode();
        ~BasicTeleopControlNode();

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
         * @brief 0.001Hz Loop Function
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
         * @brief Process a Joystick Message
         *
         * @param t_msg
         */
        void joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg);

       private:
        fast::rf::UserInterfaceSystem::RemoteControlSubsystem::BasicTeleopControlProcess process;
        ros::Subscriber joy_sub;
        ros::Publisher twist_pub;
    };
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem
