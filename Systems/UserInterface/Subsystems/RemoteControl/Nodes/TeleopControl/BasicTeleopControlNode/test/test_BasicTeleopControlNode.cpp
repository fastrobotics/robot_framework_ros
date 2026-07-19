#include <geometry_msgs/Twist.h>
#include <gtest/gtest.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/heartbeat.h>
#include <robot_framework_ros/ready_to_arm.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

#include "../BasicTeleopControlNode.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "nodeBasicTeleopControl";
uint64_t heartbeat_rx_count = 0;
uint64_t diagnostic_rx_count = 0;
uint64_t ready_to_arm_rx_count = 0;
uint64_t twist_rx_count = 0;
void twist_Callback([[maybe_unused]] const geometry_msgs::Twist& msg) { twist_rx_count++; }
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_rx_count++; }
void diagnostic_Callback([[maybe_unused]] const robot_framework_ros::diagnostic& msg) { diagnostic_rx_count++; }
void ready_to_arm_Callback([[maybe_unused]] const robot_framework_ros::ready_to_arm& msg) { ready_to_arm_rx_count++; }
TEST(BasicTeleopControllerNode, TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = robot_namespace + unittest_nodename + "/heartbeat";
    ros::Subscriber heartbeat_sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);

    std::string diagnostic_topic = robot_namespace + unittest_nodename + "/diagnostic";
    ros::Subscriber diagnostic_sub = nh.subscribe(diagnostic_topic, 100, &diagnostic_Callback);

    std::string ready_to_arm_topic = robot_namespace + unittest_nodename + "/ready_to_arm";
    ros::Subscriber ready_to_arm_sub = nh.subscribe(ready_to_arm_topic, 100, &ready_to_arm_Callback);

    ros::Subscriber twist_sub = nh.subscribe(robot_namespace + "/cmd_throttle", 100, &twist_Callback);

    ros::Publisher joy_cmd_pub = nh.advertise<sensor_msgs::Joy>(robot_namespace + "/joy", 1);

    sleep(5.0);
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::diagnostic>(diagnostic_topic, ros::Duration(10)),
              nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_EQ(1, diagnostic_sub.getNumPublishers());
    EXPECT_EQ(1, ready_to_arm_sub.getNumPublishers());

    sleep(1.0);  // Wait for BasicTeleopControlNode to Start.
    EXPECT_TRUE(heartbeat_rx_count > 0);
    EXPECT_TRUE(diagnostic_rx_count > 0);
    EXPECT_TRUE(ready_to_arm_rx_count > 0);

    // Publish a Joy Message
    sensor_msgs::Joy joy;
    joy.axes.resize(5);
    joy_cmd_pub.publish(joy);
    sleep(1.0);

    // Check that we received a Throttle Command
    EXPECT_TRUE(twist_rx_count > 0);
    // sleep(10000.0);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_BasicTeleopControlNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}