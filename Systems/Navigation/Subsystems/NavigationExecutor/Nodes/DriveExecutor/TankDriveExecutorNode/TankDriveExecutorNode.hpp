/**
 * @file TankDriveExecutorNode.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <geometry_msgs/Twist.h>

#include <TankDriveExecutorProcess/TankDriveExecutorProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem {
    /**
     * @brief Tank Drive Executor Node
     *
     */
    class TankDriveExecutorNode : public BaseNode {
       public:
        TankDriveExecutorNode();
        ~TankDriveExecutorNode();

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

        void twist_Callback(const geometry_msgs::Twist::ConstPtr& t_msg);

       private:
        ros::Subscriber twist_sub;
        ros::Publisher left_drive_pub;
        ros::Publisher right_drive_pub;
        fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveExecutorProcess process;
    };
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem
