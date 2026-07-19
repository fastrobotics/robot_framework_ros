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
#include <robot_framework_ros/arm_command.h>
#include <robot_framework_ros/arm_state_change.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/ready_to_arm.h>
#include <sensor_msgs/Joy.h>

#include <ArmCommandMsg.hpp>
#include <ArmStateChangeSrv.hpp>
#include <DiagnosticMsg.hpp>
#include <JoyMsg.hpp>
#include <ReadyToArmStatusMsg.hpp>
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

        /**
         * @brief Translate to a Core Message to a ROS message of type ReadyToArm
         *
         * @param msg
         * @return fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg
         */
        static fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg convert(
            robot_framework_ros::ready_to_arm msg);

        /**
         * @brief Translate from a Core Message to a ROS message of type ReadyToArm
         *
         * @param msg
         * @return robot_framework_ros::ready_to_arm
         */
        static robot_framework_ros::ready_to_arm convert(
            fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg msg);

        /**
         * @brief Translate from a ROS message to a Core message of type ArmCommand
         *
         * @param msg
         * @return fast::rf::messages::InfrastructureMsgs::ArmCommandMsg
         */
        static fast::rf::messages::InfrastructureMsgs::ArmCommandMsg convert(robot_framework_ros::arm_command msg);

        /**
         * @brief Translate from a Coremessage to a ROS message of type ArmCommand
         *
         * @param msg
         * @return robot_framework_ros::arm_command
         */
        static robot_framework_ros::arm_command convert(fast::rf::messages::InfrastructureMsgs::ArmCommandMsg msg);

        /**
         * @brief Translate from a ROS Service Request to a Core Service Request of type ArmStateChange
         *
         * @param req
         * @return fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest
         */
        static fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest convert(
            robot_framework_ros::arm_state_change::Request req);

        /**
         * @brief Translate from a Core Service Request to a ROS Service Request of type ArmStateChange
         *
         * @param req
         * @return robot_framework_ros::arm_state_change::Request
         */
        static robot_framework_ros::arm_state_change::Request convert(
            fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest req);

        /**
         * @brief Translate from a ROS Service Response to a Core Service Response of type ArmStateChange
         *
         * @param resp
         * @return fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse
         */
        static fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse convert(
            robot_framework_ros::arm_state_change::Response resp);

        /**
         * @brief Translate from a Core Service Request to a ROS Service Request of type ArmStateChange
         *
         * @param resp
         * @return robot_framework_ros::arm_state_change::Response
         */
        static robot_framework_ros::arm_state_change::Response convert(
            fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse resp);

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