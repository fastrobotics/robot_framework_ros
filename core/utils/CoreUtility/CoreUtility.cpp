#include <robot_framework_ros/utils/CoreUtility.hpp>
namespace fast::rf_ros::utils {
double CoreUtility::measure_time_diff(ros::Time time_a, ros::Time time_b) { return time_a.toSec() - time_b.toSec(); }

}  // namespace fast::rf_ros::utils