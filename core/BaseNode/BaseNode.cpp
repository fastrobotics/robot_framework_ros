#include <robot_framework_ros/BaseNode.hpp>
namespace fast::rf_ros {
std::string BaseNode::pretty() {
    std::string str = "Node State: " + convert(node_state);
    return str;
}
std::string BaseNode::convert(robot_framework_ros::nodestate state) {
    std::string str;
    switch (node_state.state) {
        case robot_framework_ros::nodestate::STATE_UNKNOWN:
            str = "STATE_UNKNOWN";
            break;
        case robot_framework_ros::nodestate::STATE_INITIALIZING:
            str = "STATE_INITIALIZING";
            break;
        case robot_framework_ros::nodestate::STATE_STARTING:
            str = "STATE_STARTING";
            break;
        case robot_framework_ros::nodestate::STATE_RUNNING:
            str = "STATE_RUNNING";
            break;
        default:
            str = "STATE_UNKNOWN";
            break;
    }
    return str;
}
bool BaseNode::base_init() {
    bool status = request_node_statechange(robot_framework_ros::nodestate::STATE_INITIALIZING, false);
    if (status == false) {
        return false;
    }
    ros_rate = 200.0;
    std::string heartbeat_topic = "/heartbeat";
    heartbeat_pub = n->advertise<robot_framework_ros::heartbeat>(heartbeat_topic, 1);

    return true;
}
bool BaseNode::base_start() {
    bool status = request_node_statechange(robot_framework_ros::nodestate::STATE_STARTING, false);
    if (status == false) {
        return false;
    }
    return true;
}
bool BaseNode::update() {
    bool status = request_node_statechange(robot_framework_ros::nodestate::STATE_RUNNING, false);
    if (status == false) {
        return false;
    }
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
bool BaseNode::request_node_statechange(uint8_t new_state, bool override) {
    uint8_t current_state = node_state.state;
    if (current_state == new_state) {
        return true;
    }
    bool state_change_allowed = false;
    bool state_changed = false;
    if (override == false) {
        if (current_state == robot_framework_ros::nodestate::STATE_UNKNOWN) {
            if (new_state == robot_framework_ros::nodestate::STATE_INITIALIZING) {
                state_change_allowed = true;
            }
        } else if (current_state == robot_framework_ros::nodestate::STATE_INITIALIZING) {
            if (new_state == robot_framework_ros::nodestate::STATE_STARTING) {
                state_change_allowed = true;
            }
        } else if (current_state == robot_framework_ros::nodestate::STATE_STARTING) {
            if (new_state == robot_framework_ros::nodestate::STATE_RUNNING) {
                state_change_allowed = true;
            }
        }
    }
    if (state_change_allowed == true) {
        node_state.state = new_state;
        return true;
    } else {
        ROS_ERROR("Node State Change Not Allowed: %d -> %d", current_state, new_state);
        return false;
    }
}
}  // namespace fast::rf_ros