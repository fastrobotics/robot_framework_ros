#include <gtest/gtest.h>
#include <robot_framework_ros/heartbeat.h>
#include <ros/ros.h>

#include "../ExampleNode.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "example_node";
uint64_t heartbeat_count = 0;
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_count++; }
TEST(ExampleNode, TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = "/heartbeat";
    ros::Subscriber sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);
    sleep(5.0);
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    EXPECT_EQ(1, sub.getNumPublishers());
    sleep(1.0);  // Wait for DiagnosticNode to Start.
    EXPECT_TRUE(heartbeat_count > 0);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_exampleNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}