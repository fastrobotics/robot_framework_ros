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
         * @brief Process a Twist
         *
         * @param t_msg
         */
        void twist_Callback(const geometry_msgs::Twist::ConstPtr& t_msg);

       private:
        fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveExecutorProcess
            process;  //!< Execution Process
        ros::Subscriber twist_sub;
        ros::Publisher left_drive_pub;
        ros::Publisher right_drive_pub;
    };
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem
