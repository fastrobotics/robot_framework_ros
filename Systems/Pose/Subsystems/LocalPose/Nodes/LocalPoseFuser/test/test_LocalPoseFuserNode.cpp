#include <gtest/gtest.h>
#include <robot_framework_ros/diagnostic.h>
#include <robot_framework_ros/heartbeat.h>
#include <robot_framework_ros/ready_to_arm.h>
#include <ros/ros.h>

#include "../LocalPoseFuserNode.hpp"

using namespace fast::rf_ros;

std::string robot_namespace = "/test/";
std::string unittest_nodename = "nodeLocalPoseFuser";
uint64_t heartbeat_rx_count = 0;
uint64_t diagnostic_rx_count = 0;
uint64_t ready_to_arm_rx_count = 0;
uint64_t local_pose_rx_count = 0;
void heartbeat_Callback([[maybe_unused]] const robot_framework_ros::heartbeat& msg) { heartbeat_rx_count++; }
void diagnostic_Callback([[maybe_unused]] const robot_framework_ros::diagnostic& msg) { diagnostic_rx_count++; }
void ready_to_arm_Callback([[maybe_unused]] const robot_framework_ros::ready_to_arm& msg) { ready_to_arm_rx_count++; }
void local_pose_Callback([[maybe_unused]] const nav_msgs::Odometry& msg) { local_pose_rx_count++; }
TEST(LocalPoseFuserNode, TestBasics) {
    ros::NodeHandle nh("~");
    std::string heartbeat_topic = robot_namespace + unittest_nodename + "/heartbeat";
    ros::Subscriber heartbeat_sub = nh.subscribe(heartbeat_topic, 100, &heartbeat_Callback);

    std::string diagnostic_topic = robot_namespace + unittest_nodename + "/diagnostic";
    ros::Subscriber diagnostic_sub = nh.subscribe(diagnostic_topic, 100, &diagnostic_Callback);

    std::string ready_to_arm_topic = robot_namespace + unittest_nodename + "/ready_to_arm";
    ros::Subscriber ready_to_arm_sub = nh.subscribe(ready_to_arm_topic, 100, &ready_to_arm_Callback);

    std::string local_pose_topic = robot_namespace + "/local_pose";
    ros::Subscriber local_pose_sub = nh.subscribe(local_pose_topic, 100, &local_pose_Callback);

    ros::Publisher imu_pub = nh.advertise<sensor_msgs::Imu>(robot_namespace + "/imu", 1);
    sleep(5.0);
    ASSERT_NE(ros::topic::waitForMessage<robot_framework_ros::heartbeat>(heartbeat_topic, ros::Duration(10)), nullptr);
    ASSERT_EQ(1, heartbeat_sub.getNumPublishers());
    ASSERT_NE(ros::topic::waitForMessage<robot_framework_ros::diagnostic>(diagnostic_topic, ros::Duration(10)),
              nullptr);
    ASSERT_EQ(1, heartbeat_sub.getNumPublishers());
    ASSERT_EQ(1, diagnostic_sub.getNumPublishers());
    ASSERT_EQ(1, ready_to_arm_sub.getNumPublishers());
    ASSERT_EQ(1, local_pose_sub.getNumPublishers());
    ASSERT_EQ(1, imu_pub.getNumSubscribers());

    sleep(1.0);  // Wait for LocalPoseFuserNode to Start.
    ASSERT_TRUE(heartbeat_rx_count > 0);
    ASSERT_TRUE(diagnostic_rx_count > 0);
    ASSERT_TRUE(ready_to_arm_rx_count > 0);

    // Feed it some IMU Data
    double current_time = 0.0;
    double delta_t = 0.1;
    while (current_time < 3.0) {
        sensor_msgs::Imu imu_data;
        imu_pub.publish(imu_data);

        current_time += delta_t;
        usleep(delta_t * 1000000.0);
    }
    ASSERT_TRUE(local_pose_rx_count > 0);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_LocalPoseFuserNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}