#include <gtest/gtest.h>
#include <robot_framework_ros/arm_command.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/heartbeat.h>
#include <robot_framework_ros/ready_to_arm.h>
#include <ros/ros.h>

#include <ArmStateChangeSrv.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>

#include "../ArmedStateManagerNode.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "nodeArmedStateManager";
uint64_t heartbeat_rx_count = 0;
uint64_t diagnostic_rx_count = 0;
uint64_t arm_command_rx_count = 0;
robot_framework_ros::arm_command arm_command;
std::string arm_command_topic = "/test/arm_command";
/**
 * @todo Revisit during AB#1767
 *
 */
std::string ready_to_arm_process1_topic = "/test/nodeBasicTeleopControl/ready_to_arm";
std::string ready_to_arm_process2_topic = "/test/nodeServoHat/ready_to_arm";
std::string ready_to_arm_process3_topic = "/test/nodeTankDriveExecutor/ready_to_arm";
std::string armstate_change_service_topic = "/test/arm_state_change";
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_rx_count++; }
void diagnostic_Callback([[maybe_unused]] const robot_framework_ros::diagnostic& msg) { diagnostic_rx_count++; }
void arm_command_Callback(const robot_framework_ros::arm_command& msg) {
    arm_command_rx_count++;
    arm_command = msg;
}
TEST(ArmedStateManagerNode, TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = robot_namespace + unittest_nodename + "/heartbeat";
    ros::Subscriber heartbeat_sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);

    std::string diagnostic_topic = robot_namespace + unittest_nodename + "/diagnostic";
    ros::Subscriber diagnostic_sub = nh.subscribe(diagnostic_topic, 100, &diagnostic_Callback);

    ros::Publisher ready_to_arm_process1_pub =
        nh.advertise<robot_framework_ros::ready_to_arm>(ready_to_arm_process1_topic, 1);

    ros::Publisher ready_to_arm_process2_pub =
        nh.advertise<robot_framework_ros::ready_to_arm>(ready_to_arm_process2_topic, 1);

    ros::Publisher ready_to_arm_process3_pub =
        nh.advertise<robot_framework_ros::ready_to_arm>(ready_to_arm_process3_topic, 1);

    ros::Subscriber arm_command_sub = nh.subscribe(arm_command_topic, 100, &arm_command_Callback);

    sleep(5.0);
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::diagnostic>(diagnostic_topic, ros::Duration(10)),
              nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_EQ(1, diagnostic_sub.getNumPublishers());
    EXPECT_EQ(1, arm_command_sub.getNumPublishers());
    EXPECT_EQ(1, ready_to_arm_process1_pub.getNumSubscribers());
    EXPECT_EQ(1, ready_to_arm_process2_pub.getNumSubscribers());
    EXPECT_EQ(1, ready_to_arm_process3_pub.getNumSubscribers());

    sleep(1.0);  // Wait for ArmedStateManagerNode to Start.
    EXPECT_TRUE(heartbeat_rx_count > 0);
    EXPECT_TRUE(diagnostic_rx_count > 0);
    EXPECT_TRUE(arm_command_rx_count > 0);

    robot_framework_ros::ready_to_arm process1_data;
    process1_data.SystemID = fast::rf::BaseMachineSystem::SYSTEM_ID;
    process1_data.SubsystemID = fast::rf::BaseMachineSystem::BaseMachineSubsystem::SUBSYSTEM_ID;
    process1_data.ProcessID = fast::rf::BaseMachineSystem::BaseMachineSubsystem::PROCESS_HATDRIVER_ID;
    process1_data.ready_to_arm = true;

    robot_framework_ros::ready_to_arm process2_data;
    process2_data.SystemID = fast::rf::NavigationSystem::SYSTEM_ID;
    process2_data.SubsystemID = fast::rf::NavigationSystem::NavigationExecutorSubsystem::SUBSYSTEM_ID;
    process2_data.ProcessID = fast::rf::NavigationSystem::NavigationExecutorSubsystem::PROCESS_DRIVE_EXECUTOR_ID;
    process2_data.ready_to_arm = true;

    robot_framework_ros::ready_to_arm process3_data;
    process3_data.SystemID = fast::rf::UserInterfaceSystem::SYSTEM_ID;
    process3_data.SubsystemID = fast::rf::UserInterfaceSystem::RemoteControlSubsystem::SUBSYSTEM_ID;
    process3_data.ProcessID = fast::rf::UserInterfaceSystem::RemoteControlSubsystem::PROCESS_TELEOPCONTROL_ID;
    process3_data.ready_to_arm = true;

    // Feed it Ready To Arm Topics
    double current_time = 0.0;
    double delta_t = 0.1;
    while (current_time < 20.0) {
        ready_to_arm_process1_pub.publish(process1_data);
        ready_to_arm_process2_pub.publish(process2_data);
        ready_to_arm_process3_pub.publish(process3_data);
        current_time += delta_t;
        usleep(delta_t * 1000000.0);
        if (arm_command.armed_state.state == (uint8_t)fast::rf::ArmedState::DISARMED) {
            break;
        }
    }
    EXPECT_EQ(arm_command.armed_state.state, (uint8_t)fast::rf::ArmedState::DISARMED);

    ros::ServiceClient armstate_change_client =
        nh.serviceClient<robot_framework_ros::arm_state_change>(armstate_change_service_topic);

    fast::rf::messages::InfrastructureMsgs::ArmStateChangeSrv::ArmStateChangeSrvRequest request;
    request.requested_armed_state = fast::rf::ArmedState::ARMED;
    robot_framework_ros::arm_state_change req;
    req.request = fast::rf_ros::utils::TranslateUtility::convert(request);
    EXPECT_TRUE(armstate_change_client.call(req));

    while (current_time < 40.0) {
        ready_to_arm_process1_pub.publish(process1_data);
        ready_to_arm_process2_pub.publish(process2_data);
        ready_to_arm_process3_pub.publish(process3_data);
        current_time += delta_t;
        usleep(delta_t * 1000000.0);
        if (arm_command.armed_state.state == (uint8_t)fast::rf::ArmedState::ARMED) {
            break;
        }
    }
    EXPECT_EQ(arm_command.armed_state.state, (uint8_t)fast::rf::ArmedState::ARMED);

    request.requested_armed_state = fast::rf::ArmedState::DISARMED;
    req.request = fast::rf_ros::utils::TranslateUtility::convert(request);
    EXPECT_TRUE(armstate_change_client.call(req));

    while (current_time < 60.0) {
        ready_to_arm_process1_pub.publish(process1_data);
        ready_to_arm_process2_pub.publish(process2_data);
        ready_to_arm_process3_pub.publish(process3_data);
        current_time += delta_t;
        usleep(delta_t * 1000000.0);
        if (arm_command.armed_state.state == (uint8_t)fast::rf::ArmedState::DISARMED) {
            break;
        }
    }
    EXPECT_EQ(arm_command.armed_state.state, (uint8_t)fast::rf::ArmedState::DISARMED);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_ArmedStateManagerNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}