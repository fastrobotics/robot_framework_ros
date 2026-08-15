/**
 * @file SystemMonitorNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <robot_framework_ros/arm_command.h>

#include <Definitions.hpp>
#include <IWindow.hpp>
#include <map>
#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {
    /**
     * @brief SystemMonitor Node
     *
     */
    class SystemMonitorNode : public BaseNode {
       public:
        SystemMonitorNode();
        ~SystemMonitorNode();

        /**
         * @brief Initialize the Node
         *
         * @return true
         * @return false
         */
        bool init();

        bool init_windows();
        bool init_screen();

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
        bool rescan_rosnetwork();

        void arm_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg);

        void heartbeat_Callback(const robot_framework_ros::heartbeat::ConstPtr& msg);

        bool update_monitorlist(std::vector<std::string> heartbeat_list,
                                std::vector<std::string>& new_heartbeat_topics_to_subscribe);
        std::map<std::string, std::shared_ptr<IWindow>> windows;

        std::map<std::string, bool> filter_list;
        ros::Subscriber arm_command_sub;
        std::vector<std::string> monitored_heartbeat_topics;
        std::vector<ros::Subscriber> heartbeat_subs;
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor
