#include <gtest/gtest.h>
#include <ros/ros.h>

#include <robot_framework_ros/BaseNode.hpp>

namespace fast::rf_ros {
class BaseNodeTester : public BaseNode {
   public:
    bool init() { return base_init(); }
    bool start() { return base_start(); }
    bool run_10hz() {
        ROS_WARN("Running 10Hz Loop");
        bool status = base_run_10hz();
        if (status == false) {
            return false;
        }
        return true;
    }
};
TEST(BaseNode, BasicFunctionality) {
    BaseNodeTester tester;
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_UNKNOWN);
    ROS_WARN("%s", tester.pretty().c_str());
    ASSERT_TRUE(tester.init());
    ROS_WARN("%s", tester.pretty().c_str());
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_INITIALIZING);
    ROS_WARN("%s", tester.pretty().c_str());
    ASSERT_TRUE(tester.start());
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_STARTING);
    ASSERT_TRUE(tester.update());
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_RUNNING);
    ASSERT_TRUE(tester.update());
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_RUNNING);

    ASSERT_TRUE(tester.base_restart());
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_STARTING);
    ASSERT_TRUE(tester.update());
    ASSERT_EQ(tester.get_node_state().state, robot_framework_ros::nodestate::STATE_RUNNING);
}
}  // namespace fast::rf_ros
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_baseNode");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}