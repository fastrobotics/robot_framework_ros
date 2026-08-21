
#include <gtest/gtest.h>

#include <Infrastructure/Logger.hpp>
#include <Windows/NodeInfoWindow.hpp>

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
TEST(NodeInfoWindow, BasicTests) {
    NodeInfoWindow SUT(-1, 100, 100);
    ASSERT_NE(SUT.get_name(), "");
    ASSERT_FALSE(SUT.is_initialized());
    ASSERT_FALSE(SUT.update(0.0));
    fast::rf::Logger::log_debug(SUT.pretty());
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
