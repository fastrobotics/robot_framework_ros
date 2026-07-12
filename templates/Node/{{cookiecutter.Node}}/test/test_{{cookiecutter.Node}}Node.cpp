#include <gtest/gtest.h>

#include "../{{cookiecutter.Node}}Node.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "node{{cookiecutter.Node}}";
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_rx_count++; }
void diagnostic_Callback([[maybe_unused]] const robot_framework_ros::diagnostic& msg) { diagnostic_rx_count++; }
TEST(
    {
        { cookiecutter.Node }
    } Node,
    TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = robot_namespace + unittest_nodename + "/heartbeat";
    ros::Subscriber heartbeat_sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);

    std::string diagnostic_topic = robot_namespace + unittest_nodename + "/diagnostic";
    ros::Subscriber diagnostic_sub = nh.subscribe(diagnostic_topic, 100, &diagnostic_Callback);

    sleep(5.0);
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_NE(ros::topic::waitForMessage<robot_framework_ros::diagnostic>(diagnostic_topic, ros::Duration(10)),
              nullptr);
    EXPECT_EQ(1, heartbeat_sub.getNumPublishers());
    EXPECT_EQ(1, diagnostic_sub.getNumPublishers());

    sleep(1.0);  // Wait for DiagnosticNode to Start.
    EXPECT_TRUE(heartbeat_rx_count > 0);
    EXPECT_TRUE(diagnostic_rx_count > 0);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_{{cookiecutter.Node}}Node");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}