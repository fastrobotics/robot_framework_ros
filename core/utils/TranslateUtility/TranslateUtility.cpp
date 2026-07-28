#include <geometry_msgs/Quaternion.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/transform_datatypes.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <robot_framework_ros/utils/TranslateUtility.hpp>
namespace fast::rf_ros::utils {
    fast::rf::messages::InfrastructureMsgs::DiagnosticMsg TranslateUtility::convert(
        robot_framework_ros::diagnostic msg) {
        fast::rf::messages::InfrastructureMsgs::DiagnosticMsg data;
        data.systemID = msg.SystemID;
        data.subsystemID = msg.SubsystemID;
        data.processID = msg.ProcessID;
        data.diagnosticType = (fast::rf::DiagnosticDefinition::DiagnosticType)msg.DiagnosticType;
        data.level = (fast::rf::Level)msg.Level;
        data.diagnosticMessage = (fast::rf::DiagnosticDefinition::DiagnosticMessage)msg.DiagnosticMessage;
        data.description = msg.Description;
        return data;
    }

    robot_framework_ros::diagnostic TranslateUtility::convert(
        fast::rf::messages::InfrastructureMsgs::DiagnosticMsg data) {
        robot_framework_ros::diagnostic msg;
        msg.SystemID = data.systemID;
        msg.SubsystemID = data.subsystemID;
        msg.ProcessID = data.processID;
        msg.DiagnosticType = (uint8_t)data.diagnosticType;
        msg.Level = (uint8_t)data.level;
        msg.DiagnosticMessage = (uint8_t)data.diagnosticMessage;
        msg.Description = data.description;
        return msg;
    }
    fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg TranslateUtility::convert(
        robot_framework_ros::ready_to_arm msg) {
        fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg data;
        data.systemID = msg.SystemID;
        data.subsystemID = msg.SubsystemID;
        data.processID = msg.ProcessID;
        data.ready_to_arm = msg.ready_to_arm;
        return data;
    }
    robot_framework_ros::ready_to_arm TranslateUtility::convert(
        fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg msg) {
        robot_framework_ros::ready_to_arm data;
        data.SystemID = msg.systemID;
        data.SubsystemID = msg.subsystemID;
        data.ProcessID = msg.processID;
        data.ready_to_arm = msg.ready_to_arm;
        return data;
    }
    fast::rf::messages::InfrastructureMsgs::ArmCommandMsg TranslateUtility::convert(
        robot_framework_ros::arm_command msg) {
        fast::rf::messages::InfrastructureMsgs::ArmCommandMsg data;
        data.armed_state = (fast::rf::ArmedState)msg.armed_state.state;
        return data;
    }
    robot_framework_ros::arm_command TranslateUtility::convert(
        fast::rf::messages::InfrastructureMsgs::ArmCommandMsg msg) {
        robot_framework_ros::arm_command data;
        data.armed_state.state = (uint8_t)msg.armed_state;
        return data;
    }
    fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest TranslateUtility::convert(
        robot_framework_ros::arm_state_change::Request req) {
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest request;
        request.requested_armed_state = (fast::rf::ArmedState)req.requested_armed_state.state;

        return request;
    }
    robot_framework_ros::arm_state_change::Request TranslateUtility::convert(
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest req) {
        robot_framework_ros::arm_state_change::Request request;
        request.requested_armed_state.state = (uint8_t)req.requested_armed_state;
        return request;
    }

    fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse TranslateUtility::convert(
        robot_framework_ros::arm_state_change::Response resp) {
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse response;
        response.current_armed_state = (fast::rf::ArmedState)resp.current_armed_state.state;
        response.request_approved = resp.request_approved;
        return response;
    }
    robot_framework_ros::arm_state_change::Response TranslateUtility::convert(
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse resp) {
        robot_framework_ros::arm_state_change::Response response;
        response.current_armed_state.state = (uint8_t)resp.current_armed_state;
        response.request_approved = resp.request_approved;
        return response;
    }
    fast::rf::messages::GeometryMsgs::TwistMsg TranslateUtility::convert(geometry_msgs::Twist msg) {
        fast::rf::messages::GeometryMsgs::TwistMsg data;
        data.linear = convert(msg.linear);
        data.angular = convert(msg.angular);
        return data;
    }
    fast::rf::messages::GeometryMsgs::OrientationMsg TranslateUtility::convert(const geometry_msgs::Quaternion& msg) {
        fast::rf::messages::GeometryMsgs::OrientationMsg orientation;
        // 1. Convert geometry_msgs to tf Quaternion
        tf::Quaternion tf_quat;
        tf::quaternionMsgToTF(msg, tf_quat);

        // 2. Convert tf Quaternion to Matrix3x3
        tf::Matrix3x3 matrix(tf_quat);

        // 3. Extract Roll, Pitch, and Yaw (in radians)
        double roll, pitch, yaw;
        matrix.getRPY(roll, pitch, yaw);
        orientation.roll = roll;
        orientation.pitch = pitch;
        orientation.yaw = yaw;
        return orientation;
    }
    geometry_msgs::Quaternion TranslateUtility::convert(fast::rf::messages::GeometryMsgs::OrientationMsg msg) {
        tf2::Quaternion tf_quaternion;
        tf_quaternion.setRPY(msg.roll, msg.pitch, msg.yaw);
        geometry_msgs::Quaternion data = tf2::toMsg(tf_quaternion);
        return data;
    }
    geometry_msgs::Twist TranslateUtility::convert(fast::rf::messages::GeometryMsgs::TwistMsg msg) {
        geometry_msgs::Twist data;
        data.linear = convert(msg.linear);
        data.angular = convert(msg.angular);
        return data;
    }

    fast::rf::messages::StandardMsgs::Vector3DMsg TranslateUtility::convert(geometry_msgs::Vector3 msg) {
        fast::rf::messages::StandardMsgs::Vector3DMsg data;
        data.x = msg.x;
        data.y = msg.y;
        data.z = msg.z;
        return data;
    }

    geometry_msgs::Vector3 TranslateUtility::convert(fast::rf::messages::StandardMsgs::Vector3DMsg msg) {
        geometry_msgs::Vector3 data;
        data.x = msg.x;
        data.y = msg.y;
        data.z = msg.z;
        return data;
    }
    fast::rf::messages::StandardMsgs::Covariance3DMsg TranslateUtility::convert_covariance3D(
        boost::array<double, Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION> msg) {
        fast::rf::messages::StandardMsgs::Covariance3DMsg data;
        data.covariance.assign(msg.begin(), msg.end());
        return data;
    }
    boost::array<double, Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION>
    TranslateUtility::convert_covariance3D(fast::rf::messages::StandardMsgs::Covariance3DMsg msg) {
        boost::array<double, Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION> data;
        std::copy(msg.covariance.begin(), msg.covariance.end(), data.begin());
        return data;
    }
    fast::rf::messages::SensorMsgs::JoyMsg TranslateUtility::convert(sensor_msgs::Joy msg) {
        fast::rf::messages::SensorMsgs::JoyMsg joy;
        joy.time_stamp = msg.header.stamp.toSec();
        joy.axes.assign(msg.axes.begin(), msg.axes.end());
        joy.buttons.assign(msg.buttons.begin(), msg.buttons.end());
        return joy;
    }

    sensor_msgs::Joy TranslateUtility::convert(fast::rf::messages::SensorMsgs::JoyMsg msg) {
        sensor_msgs::Joy joy;
        joy.header.stamp = ros::Time(msg.time_stamp);
        joy.axes.assign(msg.axes.begin(), msg.axes.end());
        joy.buttons.assign(msg.buttons.begin(), msg.buttons.end());
        return joy;
    }
    fast::rf::messages::SensorMsgs::ImuMsg TranslateUtility::convert(sensor_msgs::Imu data) {
        fast::rf::messages::SensorMsgs::ImuMsg msg;
        msg.time_stamp = data.header.stamp.toSec();
        msg.orientation = convert(data.orientation);
        msg.orientation = convert(data.orientation);
        msg.orientation_covariance = convert_covariance3D(data.orientation_covariance);
        msg.angular_velocity = convert(data.angular_velocity);
        msg.angular_velocity_covariance = convert_covariance3D(data.angular_velocity_covariance);
        msg.linear_acceleration = convert(data.linear_acceleration);
        msg.linear_acceleration_covariance = convert_covariance3D(data.linear_acceleration_covariance);
        return msg;
    }
    sensor_msgs::Imu TranslateUtility::convert(fast::rf::messages::SensorMsgs::ImuMsg data) {
        sensor_msgs::Imu msg;
        /**
         * @todo NOT WORKING
         *
         */
        // msg.header.stamp = ros::Time(data.time_stamp);
        msg.orientation = convert(data.orientation);
        msg.orientation_covariance = convert_covariance3D(data.orientation_covariance);
        msg.angular_velocity = convert(data.angular_velocity);
        msg.angular_velocity_covariance = convert_covariance3D(data.angular_velocity_covariance);
        msg.linear_acceleration = convert(data.linear_acceleration);
        msg.linear_acceleration_covariance = convert_covariance3D(data.linear_acceleration_covariance);
        return msg;
    }
    fast::rf::messages::SensorMsgs::MagneticFieldMsg TranslateUtility::convert(sensor_msgs::MagneticField data) {
        fast::rf::messages::SensorMsgs::MagneticFieldMsg msg;
        msg.time_stamp = data.header.stamp.toSec();
        msg.magnetic_field = convert(data.magnetic_field);
        msg.magnetic_field_covariance = convert_covariance3D(data.magnetic_field_covariance);
        return msg;
    }
    sensor_msgs::MagneticField TranslateUtility::convert(fast::rf::messages::SensorMsgs::MagneticFieldMsg data) {
        sensor_msgs::MagneticField msg;
        msg.header.stamp = ros::Time(data.time_stamp);
        msg.magnetic_field = convert(data.magnetic_field);
        msg.magnetic_field_covariance = convert_covariance3D(data.magnetic_field_covariance);
        return msg;
    }

}  // namespace fast::rf_ros::utils