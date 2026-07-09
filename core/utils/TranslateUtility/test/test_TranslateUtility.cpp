
#include <gtest/gtest.h>
#include <robot_framework_ros/diagnostic.h>

#include <DiagnosticMsg.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros::utils;

TEST(TestConversion, InfrastructureMessageConvertDiagnostic) {
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

TEST(TestConversion, GeometryMessageConvertVector) {
    {  // Convert to ROS Message
        fast::rf::messages::StandardMsgs::Vector3DMsg data;
        data.x = 1.0;
        data.y = 2.0;
        data.z = 3.0;
        geometry_msgs::Vector3 msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(msg.x, data.x);
        ASSERT_FLOAT_EQ(msg.y, data.y);
        ASSERT_FLOAT_EQ(msg.z, data.z);
    }

    {  // Convert from ROS Message
        geometry_msgs::Vector3 data;
        data.x = 1.0;
        data.y = 2.0;
        data.z = 3.0;
        fast::rf::messages::StandardMsgs::Vector3DMsg msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(data.x, msg.x);
        ASSERT_FLOAT_EQ(data.y, msg.y);
        ASSERT_FLOAT_EQ(data.z, msg.z);
    }
}

TEST(TestConversion, GeometryMessageConvertTwist) {
    {  // Convert to ROS Message
        fast::rf::messages::GeometryMsgs::TwistMsg twist;
        twist.linear.x = 1.0;
        twist.linear.y = 2.0;
        twist.linear.z = 3.0;
        twist.angular.x = 4.0;
        twist.angular.y = 5.0;
        twist.angular.z = 6.0;
        geometry_msgs::Twist msg = TranslateUtility::convert(twist);
        ASSERT_FLOAT_EQ(msg.linear.x, twist.linear.x);
        ASSERT_FLOAT_EQ(msg.linear.y, twist.linear.y);
        ASSERT_FLOAT_EQ(msg.linear.z, twist.linear.z);
        ASSERT_FLOAT_EQ(msg.angular.x, twist.angular.x);
        ASSERT_FLOAT_EQ(msg.angular.y, twist.angular.y);
        ASSERT_FLOAT_EQ(msg.angular.z, twist.angular.z);
    }

    {  // Convert from ROS Message
        geometry_msgs::Twist twist;
        twist.linear.x = 1.0;
        twist.linear.y = 2.0;
        twist.linear.z = 3.0;
        twist.angular.x = 4.0;
        twist.angular.y = 5.0;
        twist.angular.z = 6.0;
        fast::rf::messages::GeometryMsgs::TwistMsg msg = TranslateUtility::convert(twist);
        ASSERT_FLOAT_EQ(msg.linear.x, twist.linear.x);
        ASSERT_FLOAT_EQ(msg.linear.y, twist.linear.y);
        ASSERT_FLOAT_EQ(msg.linear.z, twist.linear.z);
        ASSERT_FLOAT_EQ(msg.angular.x, twist.angular.x);
        ASSERT_FLOAT_EQ(msg.angular.y, twist.angular.y);
        ASSERT_FLOAT_EQ(msg.angular.z, twist.angular.z);
    }
}
TEST(TestConversion, SensorMsgsConvertJoy) {
    {  // Convert to ROS Message
        fast::rf::messages::SensorMsgs::JoyMsg joy;
        joy.axes.push_back(1.0);
        joy.buttons.push_back(2);
        auto msg = TranslateUtility::convert(joy);
        ASSERT_EQ(msg.axes.size(), joy.axes.size());
        ASSERT_EQ(msg.buttons.size(), joy.buttons.size());
    }
    {  // Convert from ROS Message
        sensor_msgs::Joy joy;
        joy.axes.push_back(1.0);
        joy.buttons.push_back(2);
        auto msg = TranslateUtility::convert(joy);
        ASSERT_EQ(msg.axes.size(), joy.axes.size());
        ASSERT_EQ(msg.buttons.size(), joy.buttons.size());
    }
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}