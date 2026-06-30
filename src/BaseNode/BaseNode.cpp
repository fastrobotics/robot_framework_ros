#include "BaseNode.hpp"
namespace fast::rf_ros {
bool BaseNode::base_init() {
    std::string heartbeat_topic = "/a";
    heartbeat_pub = n->advertise<robot_framework_ros::heartbeat>(heartbeat_topic, 1);
    return true;
}
bool BaseNode::base_run_01hz() {
    printf("Running 01 Hz\n");
    robot_framework_ros::heartbeat beat;
    heartbeat_pub.publish(beat);
    return true;
}
}  // namespace fast::rf_ros