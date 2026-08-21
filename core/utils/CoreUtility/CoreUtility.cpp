#include <robot_framework_ros/utils/CoreUtility.hpp>
namespace fast::rf_ros::utils {
    std::string CoreUtility::pretty(robot_framework_ros::nodestate state) {
        std::string str;
        switch (state.state) {
            case robot_framework_ros::nodestate::STATE_UNKNOWN:
                str = "UNKNOWN";
                break;
            case robot_framework_ros::nodestate::STATE_INITIALIZING:
                str = "INITIALIZING";
                break;
            case robot_framework_ros::nodestate::STATE_STARTING:
                str = "STARTING";
                break;
            case robot_framework_ros::nodestate::STATE_RUNNING:
                str = "RUNNING";
                break;
            case robot_framework_ros::nodestate::STATE_PAUSED:
                str = "PAUSED";
                break;
            case robot_framework_ros::nodestate::STATE_RESTART:
                str = "RESTART";
                break;
            case robot_framework_ros::nodestate::STATE_FINISHED:
                str = "FINISHED";
                break;
            default:
                str = "UNKNOWN";
                break;
        }
        return str;
    }
    double CoreUtility::measure_time_diff(ros::Time time_a, ros::Time time_b) {
        return time_a.toSec() - time_b.toSec();
    }

    boost::array<double, 9> CoreUtility::convert_boostarray_9(std::vector<double> vector) {
        boost::array<double, 9> boost_array;
        boost_array.fill(0.0);
        std::copy_n(vector.begin(), 9, boost_array.begin());
        return boost_array;
    }

}  // namespace fast::rf_ros::utils