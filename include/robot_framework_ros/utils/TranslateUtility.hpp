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
#pragma once
#include <geometry_msgs/Accel.h>
#include <geometry_msgs/AccelWithCovariance.h>
#include <geometry_msgs/AccelWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <nav_msgs/Odometry.h>
#include <robot_framework_ros/arm_command.h>
#include <robot_framework_ros/arm_state_change.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/ready_to_arm.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/MagneticField.h>

#include <AccelMsg.hpp>
#include <AccelWithCovarianceMsg.hpp>
#include <ArmCommandMsg.hpp>
#include <ArmStateChangeSrv.hpp>
#include <Covariance3DMsg.hpp>
#include <Covariance6DMsg.hpp>
#include <DiagnosticMsg.hpp>
#include <ImuMsg.hpp>
#include <JoyMsg.hpp>
#include <MagneticFieldMsg.hpp>
#include <OdomMsg.hpp>
#include <OrientationMsg.hpp>
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
        // Standard Messages
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
         * @brief Convert from a ROS Message to a Core Message of type Covariance3D
         *
         * @param msg
         * @return fast::rf::messages::StandardMsgs::Covariance3DMsg
         */
        static fast::rf::messages::StandardMsgs::Covariance3DMsg convert_covariance3D(
            boost::array<double, fast::rf::messages::StandardMsgs::Covariance3DMsg::DIMENSION *
                                     fast::rf::messages::StandardMsgs::Covariance3DMsg::DIMENSION>
                msg);

        /**
         * @brief Convert from a Core Message to a ROS Message of type Covariance3D
         *
         * @param msg
         * @return boost::array<double, Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION>
         */
        static boost::array<double, fast::rf::messages::StandardMsgs::Covariance3DMsg::DIMENSION *
                                        fast::rf::messages::StandardMsgs::Covariance3DMsg::DIMENSION>
        convert_covariance3D(fast::rf::messages::StandardMsgs::Covariance3DMsg msg);

        static fast::rf::messages::StandardMsgs::Covariance6DMsg convert_covariance6D(
            boost::array<double, fast::rf::messages::StandardMsgs::Covariance6DMsg::DIMENSION *
                                     fast::rf::messages::StandardMsgs::Covariance6DMsg::DIMENSION>
                msg);

        static boost::array<double, fast::rf::messages::StandardMsgs::Covariance6DMsg::DIMENSION *
                                        fast::rf::messages::StandardMsgs::Covariance6DMsg::DIMENSION>
        convert_covariance6D(fast::rf::messages::StandardMsgs::Covariance6DMsg msg);

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

        static fast::rf::messages::GeometryMsgs::AccelMsg convert(const geometry_msgs::Accel& msg);
        static geometry_msgs::Accel convert(fast::rf::messages::GeometryMsgs::AccelMsg msg);
        static fast::rf::messages::GeometryMsgs::AccelWithCovarianceMsg convert(
            geometry_msgs::AccelWithCovarianceStamped msg);
        static geometry_msgs::AccelWithCovarianceStamped convert(
            fast::rf::messages::GeometryMsgs::AccelWithCovarianceMsg msg);

        /**
         * @brief Convert from a ROS message to a Core message of type Quaternion->Orientation
         *
         * @param msg
         * @return fast::rf::messages::GeometryMsgs::OrientationMsg
         */
        static fast::rf::messages::GeometryMsgs::OrientationMsg convert(const geometry_msgs::Quaternion& msg);

        /**
         * @brief Convert from a Core Message to a ROS message of type Orientation->Quaternion
         *
         * @param msg
         * @return geometry_msgs::Quaternion
         */
        static geometry_msgs::Quaternion convert(fast::rf::messages::GeometryMsgs::OrientationMsg msg);

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
         * @brief Translate from a ROS mesage to a Core message of type Odometry
         *
         * @param msg
         * @return fast::rf::messages::GeometryMsgs::OdomMsg
         */
        static fast::rf::messages::GeometryMsgs::OdomMsg convert(nav_msgs::Odometry msg);

        /**
         * @brief Convert from a Core message to a ROS message of type Odometry
         *
         * @param msg
         * @return nav_msgs::Odometry
         */
        static nav_msgs::Odometry convert(fast::rf::messages::GeometryMsgs::OdomMsg msg);

        // Sensor Messages

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

        /**
         * @brief Convert from a ROS message to a Core message of type Imu
         *
         * @param data
         * @return fast::rf::messages::SensorMsgs::ImuMsg
         */
        static fast::rf::messages::SensorMsgs::ImuMsg convert(sensor_msgs::Imu data);

        /**
         * @brief Convert from a Core message to a ROS message of type IMU
         *
         * @param data
         * @return sensor_msgs::Imu
         */
        static sensor_msgs::Imu convert(fast::rf::messages::SensorMsgs::ImuMsg data);

        /**
         * @brief Convert from a ROS message to a Core message of type MagneticField
         *
         * @param data
         * @return fast::rf::messages::SensorMsgs::MagneticFieldMsg
         */
        static fast::rf::messages::SensorMsgs::MagneticFieldMsg convert(sensor_msgs::MagneticField data);

        /**
         * @brief Convert from a Core message to a ROS message of type MagneticField
         *
         * @param data
         * @return sensor_msgs::MagneticField
         */
        static sensor_msgs::MagneticField convert(fast::rf::messages::SensorMsgs::MagneticFieldMsg data);
    };
}  // namespace fast::rf_ros::utils