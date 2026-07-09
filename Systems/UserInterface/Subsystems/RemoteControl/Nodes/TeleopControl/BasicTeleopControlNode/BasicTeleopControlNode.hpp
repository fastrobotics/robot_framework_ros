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

        bool init();

        bool start();

        bool run_100hz();

        bool run_10hz();

        bool run_1hz();

        bool run_01hz();

        bool run_001hz();

        bool run_loop1();

        bool run_loop2();

        bool run_loop3();

        void thread_loop();

        void joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg);

       private:
        fast::rf::UserInterfaceSystem::RemoteControlSubsystem::BasicTeleopControlProcess process;
        ros::Subscriber joy_sub;
        ros::Publisher twist_pub;
    };
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem
