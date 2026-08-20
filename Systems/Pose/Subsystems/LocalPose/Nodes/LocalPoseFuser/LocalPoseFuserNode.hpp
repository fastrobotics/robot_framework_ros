/**
 * @file LocalPoseFuserNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <geometry_msgs/AccelWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>

#include <ILocalPoseFuserProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::LocalPoseFuser {
    /**
     * @brief LocalPoseFuser Node
     *
     */
    class LocalPoseFuserNode : public BaseNode {
       public:
        LocalPoseFuserNode();
        ~LocalPoseFuserNode();

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

        /**
         * @brief Stop the Node
         *
         */
        void stop();

       private:
        std::atomic<bool> is_node_running{false};  //!< If the node is running
        void machine_inertial_Callback(const sensor_msgs::Imu::ConstPtr& t_msg);
        fast::rf::PoseSystem::LocalPoseSubsystem::LocalPoseFuser::ILocalPoseFuserProcess* process{
            nullptr};  //!< Execution Process
        ros::Subscriber machine_inertial_sub;
        ros::Publisher local_pose_pub;
        ros::Publisher local_pose_angular_accel_pub;
    };
}  // namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::LocalPoseFuser
