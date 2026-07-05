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
}  // namespace fast::rf_ros::utils