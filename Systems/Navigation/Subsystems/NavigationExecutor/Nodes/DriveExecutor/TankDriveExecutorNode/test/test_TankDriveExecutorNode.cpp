#include <gtest/gtest.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/heartbeat.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>

#include "../TankDriveExecutorNode.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "nodeTankDriveExecutor";
uint64_t heartbeat_rx_count = 0;
uint64_t diagnostic_rx_count = 0;
uint64_t leftdrive_rx_count = 0;
uint64_t rightdrive_rx_count = 0;
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_rx_count++; }
void diagnostic_Callback([[maybe_unused]] const robot_framework_ros::diagnostic& msg) { diagnostic_rx_count++; }
void leftdrive_Callback([[maybe_unused]] const std_msgs::Float64& msg) { leftdrive_rx_count++; }
void rightdrive_Callback([[maybe_unused]] const std_msgs::Float64& msg) { rightdrive_rx_count++; }
TEST(TankDriveExecutorNode, TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = robot_namespace + unittest_nodename + "/heartbeat";
    ros::Subscriber heartbeat_sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);

    std::string diagnostic_topic = robot_namespace + unittest_nodename + "/diagnostic";
    ros::Subscriber diagnostic_sub = nh.subscribe(diagnostic_topic, 100, &diagnostic_Callback);

    std::string leftdrive_topic = robot_namespace + "/left_drive";
    ros::Subscriber leftdrive_sub = nh.subscribe(leftdrive_topic, 100, &leftdrive_Callback);

    std::string rightdrive_topic = robot_namespace + "/right_drive";
    ros::Subscriber rightdrive_sub = nh.subscribe(rightdrive_topic, 100, &rightdrive_Callback);

    ros::Publisher throttle_cmd_pub = nh.advertise<geometry_msgs::Twist>(robot_namespace + "/cmd_throttle", 1);

    sleep(5.0);
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::diagnostic>(diagnostic_topic, ros::Duration(10)),
              nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_EQ(1, diagnostic_sub.getNumPublishers());
    EXPECT_EQ(1, leftdrive_sub.getNumPublishers());
    EXPECT_EQ(1, rightdrive_sub.getNumPublishers());

    sleep(1.0);  // Wait for DiagnosticNode to Start.
    EXPECT_TRUE(heartbeat_rx_count > 0);
    EXPECT_TRUE(diagnostic_rx_count > 0);

    // Give it a Twist Command
    geometry_msgs::Twist twist;
    throttle_cmd_pub.publish(twist);

    sleep(1.0);  // Wait for it to process the twist

    // Should receive left and right drive commands
    EXPECT_TRUE(leftdrive_rx_count > 0);
    EXPECT_TRUE(rightdrive_rx_count > 0);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_tankDriveExecutorNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}