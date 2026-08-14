
#include <gtest/gtest.h>

#include <Infrastructure/Logger.hpp>
#include <Windows/HeaderWindow.hpp>

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
TEST(HeaderWindow, BasicTests) {
    ros::NodeHandle* n = new ros::NodeHandle("~");
    HeaderWindow SUT(-1, n, "", 100, 100);
    ASSERT_NE(SUT.get_name(), "");
    ASSERT_FALSE(SUT.is_initialized());
    ASSERT_FALSE(SUT.update(0.0));
    fast::rf::Logger::log_debug(SUT.pretty());
    delete n;
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "test_Windows");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int ret = RUN_ALL_TESTS();
    spinner.stop();
    ros::shutdown();
    return ret;
}
