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

}  // namespace fast::rf_ros::utils