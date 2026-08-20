/**
 * @file TrajectoryControllerNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

#include <BasicTrajectoryControllerProcess/BasicTrajectoryControllerProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController {
    /**
     * @brief TrajectoryController Node
     *
     */
    class TrajectoryControllerNode : public BaseNode {
       public:
        TrajectoryControllerNode();
        ~TrajectoryControllerNode();

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

       private:
        /**
         *
         * @brief Process a Command
         *
         * @param t_msg
         */
        void desired_command_Callback(const geometry_msgs::Twist::ConstPtr& t_msg);

        /**
         * @brief Process a Pose
         *
         * @param t_msg
         */
        void pose_Callback(const nav_msgs::Odometry::ConstPtr& t_msg);
        ros::Subscriber pose_sub;
        ros::Subscriber desired_command_sub;
        ros::Publisher command_pub;
        fast::rf::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController::BasicTrajectoryControllerProcess
            process;  //!< Execution Process
    };
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController
