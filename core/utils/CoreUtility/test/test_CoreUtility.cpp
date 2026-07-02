
#include <gtest/gtest.h>

#include <robot_framework_ros/utils/CoreUtility.hpp>

using namespace fast::rf_ros::utils;

TEST(TestConversion, TimeChecks) {
    ros::Time::init();
    ros::Time t_now = ros::Time::now();
    double t_dur = CoreUtility::measure_time_diff(t_now, t_now);
    EXPECT_NEAR(t_dur, 0.0, 1e-8);
    sleep(1.0);
    ros::Time t_future = ros::Time::now();
    t_dur = CoreUtility::measure_time_diff(t_now, t_future);
    EXPECT_NEAR(t_dur, -1.0, 0.25);  // Allow for imprecise time clocks
    t_dur = CoreUtility::measure_time_diff(t_future, t_now);
    EXPECT_NEAR(t_dur, 1.0, 0.25);  // Allow for imprecise time clocks
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}