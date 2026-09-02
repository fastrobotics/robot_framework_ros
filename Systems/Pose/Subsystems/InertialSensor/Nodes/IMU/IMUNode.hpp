/**
 * @file IMUNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 * @compare_tag Node-Header  v0.1
 */
#pragma once

#include <IMUProcess.hpp>
#include <robot_framework_ros/BaseNode.hpp>

namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem::IMU {
    /**
     * @brief IMU Node
     *
     */
    class IMUNode : public BaseNode {
       public:
        IMUNode();
        ~IMUNode();

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
         * @brief Stop the Node
         *
         */
        void stop();

       private:
        std::atomic<bool> is_node_running{false};                                //!< If the node is running
        fast::rf::PoseSystem::InertialSensorSubsystem::IMU::IMUProcess process;  //!< Execution Process
        std::string imu_sensor_frame{""};
        ros::Publisher imu_pub;
        ros::Publisher magnetometer_pub;
        ros::Publisher imu_accel_pub;
    };
}  // namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem::IMU
