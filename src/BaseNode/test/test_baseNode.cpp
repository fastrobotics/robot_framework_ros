#include <gtest/gtest.h>
#include <ros/ros.h>

#include "../BaseNode.hpp"

namespace fast::rf_ros {
class BaseNodeTester : public BaseNode {
   public:
    bool start() { return false; }
};
TEST(BaseNode, BasicFunctionality) {
    BaseNodeTester tester;
    bool status = tester.start();
    ASSERT_TRUE(status);
}
}  // namespace fast::rf_ros
int main(int argc, char** argv) {
    ros::init(argc, argv, "test_baseNode");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}