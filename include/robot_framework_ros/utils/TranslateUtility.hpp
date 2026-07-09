/**
 * @file TranslateUtility.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <robot_framework_ros/diagnostic.h>
#include <sensor_msgs/Joy.h>

#include <DiagnosticMsg.hpp>
#include <JoyMsg.hpp>
#include <TwistMsg.hpp>
#include <Vector3DMsg.hpp>

namespace fast::rf_ros::utils {
    /**
     * @brief Translation Utilities
     *
     */
    class TranslateUtility {
       public:
        // Infrastructure Messages

        /**
         * @brief Translate from a ROS message to a Core message of type Diagnostic
         *
         * @param msg
         * @return fast::rf::messages::InfrastructureMsgs::DiagnosticMsg
         */
        static fast::rf::messages::InfrastructureMsgs::DiagnosticMsg convert(robot_framework_ros::diagnostic msg);

        /**
         * @brief Translate from a Core Message to a ROS message of type Diagnostic
         *
         * @param msg
         * @return robot_framework_ros::diagnostic
         */
        static robot_framework_ros::diagnostic convert(fast::rf::messages::InfrastructureMsgs::DiagnosticMsg msg);

        // Geometry Messages

        /**
         * @brief Translate from a ROS mesage to a Core message of type Twist
         *
         * @param msg
         * @return fast::rf::messages::GeometryMsgs::TwistMsg
         */
        static fast::rf::messages::GeometryMsgs::TwistMsg convert(geometry_msgs::Twist msg);

        /**
         * @brief Convert from a Core message to a ROS message of type Twist
         *
         * @param msg
         * @return geometry_msgs::Twist
         */
        static geometry_msgs::Twist convert(fast::rf::messages::GeometryMsgs::TwistMsg msg);

        /**
         * @brief Convert from a ROS message to a Core message of type Vector3
         *
         * @param msg
         * @return fast::rf::messages::StandardMsgs::Vector3DMsg
         */
        static fast::rf::messages::StandardMsgs::Vector3DMsg convert(geometry_msgs::Vector3 msg);

        /**
         * @brief Convert from a Core message to a ROS message of type Vector3
         *
         * @param msg
         * @return geometry_msgs::Vector3
         */
        static geometry_msgs::Vector3 convert(fast::rf::messages::StandardMsgs::Vector3DMsg msg);

        /**
         * @brief Convert from a ROS message to a Core message of type Joy
         *
         * @param msg
         * @return fast::rf::messages::SensorMsgs::JoyMsg
         */
        static fast::rf::messages::SensorMsgs::JoyMsg convert(sensor_msgs::Joy msg);

        /**
         * @brief Convert from a Core message to a ROS message of type Joy
         *
         * @param msg
         * @return sensor_msgs::Joy
         */
        static sensor_msgs::Joy convert(fast::rf::messages::SensorMsgs::JoyMsg msg);
    };
}  // namespace fast::rf_ros::utils