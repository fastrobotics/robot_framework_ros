/**
 * @file ServoHatNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <std_msgs/Float64.h>

#include <ServoHatDriverProcess/ServoHatDriverProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem {
    /**
     * @brief ServoHat Node
     *
     */
    class ServoHatNode : public BaseNode {
       public:
        ServoHatNode();
        ~ServoHatNode();

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

        void drive_Callback(const std_msgs::Float64::ConstPtr& t_msg, uint16_t channel);

       private:
        fast::rf::BaseMachineSystem::BaseMachineSubsystem::ServoHatDriverProcess process;  //!< Execution Process
        ros::Subscriber left_drive_sub;
        ros::Subscriber right_drive_sub;
    };
}  // namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem
