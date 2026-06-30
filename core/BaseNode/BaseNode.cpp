#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros {
bool BaseNode::base_init() {
    ros_rate = 200.0;
    std::string heartbeat_topic = "/heartbeat";
    heartbeat_pub = n->advertise<robot_framework_ros::heartbeat>(heartbeat_topic, 1);
    return true;
}
bool BaseNode::update() {
    ros::Rate r(ros_rate);
    r.sleep();
    ros::spinOnce();
    double mtime = measure_time_diff(ros::Time::now(), last_10hz_timer);
    if (mtime >= 0.1) {
        run_10hz();
        last_10hz_timer = ros::Time::now();
    }
    return true;
}
double BaseNode::measure_time_diff(ros::Time time_a, ros::Time time_b) { return time_a.toSec() - time_b.toSec(); }
bool BaseNode::base_run_10hz() {
    robot_framework_ros::heartbeat beat;
    heartbeat_pub.publish(beat);
    return true;
}
}  // namespace fast::rf_ros