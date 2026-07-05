
#include <gtest/gtest.h>
#include <robot_framework_ros/diagnostic.h>

#include <DiagnosticMsg.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros::utils;

TEST(TestConversion, DiagnosticMessageConvert) {
    {  // Convert to ROS Message
        fast::rf::messages::InfrastructureMsgs::DiagnosticMsg data;
        data.systemID = 1;
        data.subsystemID = 2;
        data.processID = 3;
        data.diagnosticType = fast::rf::DiagnosticDefinition::DiagnosticType::SOFTWARE;
        data.level = fast::rf::Level::INFO;
        data.diagnosticMessage = fast::rf::DiagnosticDefinition::DiagnosticMessage::INITIALIZING;
        data.description = "Testing...";
        robot_framework_ros::diagnostic msg = TranslateUtility::convert(data);
        ASSERT_EQ(msg.SystemID, data.systemID);
        ASSERT_EQ(msg.SubsystemID, data.subsystemID);
        ASSERT_EQ(msg.ProcessID, data.processID);
        ASSERT_EQ(msg.DiagnosticType, (uint8_t)data.diagnosticType);
        ASSERT_EQ(msg.Level, (uint8_t)data.level);
        ASSERT_EQ(msg.DiagnosticMessage, (uint8_t)data.diagnosticMessage);
        ASSERT_EQ(msg.Description, data.description);
    }

    {  // Convert from ROS Message
        robot_framework_ros::diagnostic msg;
        msg.SystemID = 1;
        msg.SubsystemID = 2;
        msg.ProcessID = 3;
        msg.DiagnosticType = (uint8_t)fast::rf::DiagnosticDefinition::DiagnosticType::SOFTWARE;
        msg.Level = (uint8_t)fast::rf::Level::INFO;
        msg.DiagnosticMessage = (uint8_t)fast::rf::DiagnosticDefinition::DiagnosticMessage::INITIALIZING;
        msg.Description = "Testing...";
        fast::rf::messages::InfrastructureMsgs::DiagnosticMsg data = TranslateUtility::convert(msg);
        ASSERT_EQ(data.systemID, msg.SystemID);
        ASSERT_EQ(data.subsystemID, msg.SubsystemID);
        ASSERT_EQ(data.processID, msg.ProcessID);
        ASSERT_EQ((uint8_t)data.diagnosticType, msg.DiagnosticType);
        ASSERT_EQ((uint8_t)data.level, msg.Level);
        ASSERT_EQ((uint8_t)data.diagnosticMessage, msg.DiagnosticMessage);
        ASSERT_EQ(data.description, msg.Description);
    }
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}