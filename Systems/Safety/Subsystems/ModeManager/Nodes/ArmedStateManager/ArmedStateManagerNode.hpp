/**
 * @file ArmedStateManagerNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <robot_framework_ros/arm_state_change.h>

#include <ArmedStateManagerProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem::ArmedStateManager {
    /**
     * @brief ArmedStateManager Node
     *
     */
    class ArmedStateManagerNode : public BaseNode {
       public:
        ArmedStateManagerNode();
        ~ArmedStateManagerNode();

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
         * @brief Request to Change Armed State
         *
         * @param req
         * @param res
         * @return true
         * @return false
         */
        bool arm_statechange_service(robot_framework_ros::arm_state_change::Request& req,
                                     robot_framework_ros::arm_state_change::Response& res);

        /**
         * @brief Process a Ready to Arm Signal
         *
         * @param t_msg
         */
        void ready_to_arm_Callback(const robot_framework_ros::ready_to_arm::ConstPtr& t_msg);

        /**
         * @brief Stop the Node
         *
         */
        void stop();

       private:
        std::atomic<bool> is_node_running{false};  //!< If the node is running
        fast::rf::SafetySystem::ModeManagerSubsystem::ArmedStateManager::ArmedStateManagerProcess
            process;  //!< Execution Process
        std::vector<std::string> nodes_to_monitor;
        std::vector<ros::Subscriber> ready_to_arm_subs;  //!< Container for multiple Ready to Arm Subscribers
        ros::Publisher arm_command_pub;                  //!< Publish the robot's Arm State Command
        ros::ServiceServer armstate_change_srv;          //!< Provide a Service to change the Arm State
    };
}  // namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem::ArmedStateManager
