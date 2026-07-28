
#include <gtest/gtest.h>

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
TEST(TestConversion, InfrastructureMessageConvertReadyToArm) {
    {  // Convert to ROS Message
        fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg data;
        data.systemID = 1;
        data.subsystemID = 2;
        data.processID = 3;
        data.ready_to_arm = true;
        robot_framework_ros::ready_to_arm msg = TranslateUtility::convert(data);
        ASSERT_EQ(data.systemID, msg.SystemID);
        ASSERT_EQ(data.subsystemID, msg.SubsystemID);
        ASSERT_EQ(data.processID, msg.ProcessID);
        ASSERT_EQ(data.ready_to_arm, msg.ready_to_arm);
    }
    {  // Convert from ROS Message
        robot_framework_ros::ready_to_arm msg;
        msg.SystemID = 1;
        msg.SubsystemID = 2;
        msg.ProcessID = 3;
        msg.ready_to_arm = true;
        fast::rf::messages::InfrastructureMsgs::ReadyToArmStatusMsg data = TranslateUtility::convert(msg);
        ASSERT_EQ(data.systemID, msg.SystemID);
        ASSERT_EQ(data.subsystemID, msg.SubsystemID);
        ASSERT_EQ(data.processID, msg.ProcessID);
        ASSERT_EQ(data.ready_to_arm, msg.ready_to_arm);
    }
}
TEST(TestConversion, InfrastructureMessageConvertArmCommand) {
    {  // Convert to ROS Message
        fast::rf::messages::InfrastructureMsgs::ArmCommandMsg data;
        robot_framework_ros::arm_command msg = TranslateUtility::convert(data);
        ASSERT_EQ(msg.armed_state.state, (uint8_t)data.armed_state);
    }
    {  // Convert from ROS Message
        robot_framework_ros::arm_command msg;
        fast::rf::messages::InfrastructureMsgs::ArmCommandMsg data = TranslateUtility::convert(msg);
        ASSERT_EQ(msg.armed_state.state, (uint8_t)data.armed_state);
    }
}
TEST(TestConversion, InfrastructureMessageConvertRequestArmStateChange) {
    {  // Convert to ROS Service Request
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest req;
        req.requested_armed_state = fast::rf::ArmedState::DISARMED;
        robot_framework_ros::arm_state_change::Request request = TranslateUtility::convert(req);
        ASSERT_EQ(request.requested_armed_state.state, (uint8_t)req.requested_armed_state);
    }
    {  // Convert from ROS Service Request
        robot_framework_ros::arm_state_change::Request req;
        req.requested_armed_state.state = robot_framework_ros::armedstate::DISARMED;
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest request =
            TranslateUtility::convert(req);
        ASSERT_EQ(req.requested_armed_state.state, (uint8_t)request.requested_armed_state);
    }
    {  // Convert to ROS Service Response
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse resp;
        resp.current_armed_state = fast::rf::ArmedState::DISARMED;
        resp.request_approved = true;
        robot_framework_ros::arm_state_change::Response response = TranslateUtility::convert(resp);
        ASSERT_EQ(response.current_armed_state.state, (uint8_t)resp.current_armed_state);
        ASSERT_EQ(response.request_approved, resp.request_approved);
    }
    {  // Convert from ROS Service Response
        robot_framework_ros::arm_state_change::Response resp;
        resp.current_armed_state.state = robot_framework_ros::armedstate::DISARMED;
        resp.request_approved = true;
        fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvResponse response =
            TranslateUtility::convert(resp);
        ASSERT_EQ(resp.current_armed_state.state, (uint8_t)response.current_armed_state);
        ASSERT_EQ(resp.request_approved, response.request_approved);
    }
}

TEST(TestConversion, GeometryMessageConvertQuaternion) {
    {// Convert from ROS Message
     {geometry_msgs::Quaternion data;
    data.w = 1.0;
    data.x = 0.0;
    data.y = 0.0;
    data.z = 0.0;
    auto msg = TranslateUtility::convert(data);
    ASSERT_FLOAT_EQ(msg.roll, 0.0);
    ASSERT_FLOAT_EQ(msg.pitch, 0.0);
    ASSERT_FLOAT_EQ(msg.yaw, 0.0);
}
{
    geometry_msgs::Quaternion data;
    data.w = 0.844623198620733;
    data.x = 0.191341716182545;
    data.y = 0.461939766255643;
    data.z = 0.191341716182545;
    auto msg = TranslateUtility::convert(data);
    ASSERT_FLOAT_EQ(msg.roll, 45.0 * M_PI / 180.0);
    ASSERT_FLOAT_EQ(msg.pitch, 45.0 * M_PI / 180.0);
    ASSERT_FLOAT_EQ(msg.yaw, 45.0 * M_PI / 180.0);
}
}
{  // Convert to ROS Message
    {
        fast::rf::messages::GeometryMsgs::OrientationMsg data;
        data.roll = 0.0;
        data.pitch = 0.0;
        data.yaw = 0.0;
        auto msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(msg.w, 1.0);
        ASSERT_FLOAT_EQ(msg.x, 0.0);
        ASSERT_FLOAT_EQ(msg.y, 0.0);
        ASSERT_FLOAT_EQ(msg.z, 0.0);
    }
    {
        fast::rf::messages::GeometryMsgs::OrientationMsg data;
        data.roll = 45.0 * M_PI / 180.0;
        data.pitch = 45.0 * M_PI / 180.0;
        data.yaw = 45.0 * M_PI / 180.0;
        auto msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(msg.w, 0.844623198620733);
        ASSERT_FLOAT_EQ(msg.x, 0.191341716182545);
        ASSERT_FLOAT_EQ(msg.y, 0.461939766255643);
        ASSERT_FLOAT_EQ(msg.z, 0.191341716182545);
    }
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
TEST(TestConversion, StandardMessageConvertCovariance) {
    {  // Convert to ROS Message
        fast::rf::messages::StandardMsgs::Covariance3DMsg data;
        ASSERT_EQ(data.covariance.size(), Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION);
        for (std::size_t i = 0; i < data.covariance.size(); ++i) {
            data.covariance[i] = (double)i + 1;
        }
        auto msg = TranslateUtility::convert_covariance3D(data);
        ASSERT_EQ(msg.size(), Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION);
        for (std::size_t i = 0; i < msg.size(); ++i) {
            ASSERT_EQ(data.covariance[i], msg[i]);
        }
    }
    {  // Convert from ROS Message
        boost::array<double, Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION> data;
        ASSERT_EQ(data.size(), Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION);
        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i] = (double)i + 1;
        }
        auto msg = TranslateUtility::convert_covariance3D(data);
        ASSERT_EQ(msg.covariance.size(), Covariance3DMsg::DIMENSION * Covariance3DMsg::DIMENSION);
        for (std::size_t i = 0; i < msg.covariance.size(); ++i) {
            ASSERT_EQ(data[i], msg.covariance[i]);
        }
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
TEST(TestConversion, SensorMsgsConvertImu) {
    {  // Convert to ROS Message
        fast::rf::messages::SensorMsgs::ImuMsg data;
        data.time_stamp = 1.0;
        data.seq = 2;
        data.orientation.roll = 0.0;
        data.orientation.pitch = 0.0;
        data.orientation.yaw = 0.0;
        auto msg = TranslateUtility::convert(data);
        auto msg_orientation_euler = TranslateUtility::convert(msg.orientation);
        ASSERT_FLOAT_EQ(msg_orientation_euler.roll, data.orientation.roll);
        ASSERT_FLOAT_EQ(msg_orientation_euler.pitch, data.orientation.pitch);
        ASSERT_FLOAT_EQ(msg_orientation_euler.yaw, data.orientation.yaw);
    }
    {  // Convert from ROS Message
        sensor_msgs::Imu data;
        data.orientation.w = 1.0;
        data.orientation.x = 0.0;
        data.orientation.y = 0.0;
        data.orientation.z = 0.0;
        auto msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(msg.orientation.roll, 0.0);
        ASSERT_FLOAT_EQ(msg.orientation.pitch, 0.0);
        ASSERT_FLOAT_EQ(msg.orientation.yaw, 0.0);
    }
}
TEST(TestConversion, SensorMsgsConvertMagneticField) {
    {  // Convert to ROS Message
        fast::rf::messages::SensorMsgs::MagneticFieldMsg data;
        data.time_stamp = 1.0;
        auto msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(msg.magnetic_field.x, data.magnetic_field.x);
        ASSERT_FLOAT_EQ(msg.magnetic_field.y, data.magnetic_field.y);
        ASSERT_FLOAT_EQ(msg.magnetic_field.z, data.magnetic_field.z);
    }
    {  // Convert from ROS Message
        sensor_msgs::MagneticField data;
        auto msg = TranslateUtility::convert(data);
        ASSERT_FLOAT_EQ(msg.magnetic_field.x, data.magnetic_field.x);
        ASSERT_FLOAT_EQ(msg.magnetic_field.y, data.magnetic_field.y);
        ASSERT_FLOAT_EQ(msg.magnetic_field.z, data.magnetic_field.z);
    }
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}