#include <gtest/gtest.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/heartbeat.h>
#include <robot_framework_ros/ready_to_arm.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>

#include "../ServoHatNode.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "nodeServoHat";
uint64_t heartbeat_rx_count = 0;
uint64_t diagnostic_rx_count = 0;
uint64_t ready_to_arm_rx_count = 0;
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_rx_count++; }
void diagnostic_Callback([[maybe_unused]] const robot_framework_ros::diagnostic& msg) { diagnostic_rx_count++; }
void ready_to_arm_Callback([[maybe_unused]] const robot_framework_ros::ready_to_arm& msg) { ready_to_arm_rx_count++; }
TEST(ServoHatNode, TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = robot_namespace + unittest_nodename + "/heartbeat";
    ros::Subscriber heartbeat_sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);

    std::string diagnostic_topic = robot_namespace + unittest_nodename + "/diagnostic";
    ros::Subscriber diagnostic_sub = nh.subscribe(diagnostic_topic, 100, &diagnostic_Callback);

    std::string ready_to_arm_topic = robot_namespace + unittest_nodename + "/ready_to_arm";
    ros::Subscriber ready_to_arm_sub = nh.subscribe(ready_to_arm_topic, 100, &ready_to_arm_Callback);

    std::string leftdrive_topic = robot_namespace + "/left_drive";
    ros::Publisher leftdrive_pub = nh.advertise<std_msgs::Float64>(leftdrive_topic, 1);

    std::string rightdrive_topic = robot_namespace + "/right_drive";
    ros::Publisher rightdrive_pub = nh.advertise<std_msgs::Float64>(rightdrive_topic, 1);

    sleep(5.0);
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::diagnostic>(diagnostic_topic, ros::Duration(10)),
              nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_EQ(1, diagnostic_sub.getNumPublishers());
    EXPECT_EQ(1, ready_to_arm_sub.getNumPublishers());
    EXPECT_EQ(1, leftdrive_pub.getNumSubscribers());
    EXPECT_EQ(1, rightdrive_pub.getNumSubscribers());

    sleep(1.0);  // Wait for ServoHatNode to Start.
    EXPECT_TRUE(heartbeat_rx_count > 0);
    EXPECT_TRUE(diagnostic_rx_count > 0);
    EXPECT_TRUE(ready_to_arm_rx_count > 0);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_ServoHatNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}