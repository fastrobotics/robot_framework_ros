#include <robot_framework_ros/BaseNode.hpp>
#include <robot_framework_ros/utils/CoreUtility.hpp>
namespace fast::rf_ros {
    std::string BaseNode::pretty() {
        std::string str = "Node State: " + convert(node_state);
        return str;
    }
    std::string BaseNode::convert(robot_framework_ros::nodestate state) {
        std::string str;
        switch (state.state) {
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
        node_namespace = ros::this_node::getNamespace();
        node_name = ros::this_node::getName();
        std::string heartbeat_topic = node_name + "/heartbeat";
        heartbeat_pub = n->advertise<robot_framework_ros::heartbeat>(heartbeat_topic, 1);

        std::string param_loop1_rate = node_name + "/loop1_rate";
        if (n->getParam(param_loop1_rate, loop1_rate) == false) {
            ROS_WARN("Missing parameter: loop1_rate.  Not running loop1 code.");
            loop1_enabled = false;
        } else {
            loop1_enabled = true;
            if (loop1_rate > max_rate) {
                ROS_WARN("loop1_rate is greater than max_rate.  Setting loop1_rate to max_rate.");
                loop1_rate = max_rate;
            }
        }

        std::string param_loop2_rate = node_name + "/loop2_rate";
        if (n->getParam(param_loop2_rate, loop2_rate) == false) {
            ROS_WARN("Missing parameter: loop2_rate.  Not running loop2 code.");
            loop2_enabled = false;
        } else {
            loop2_enabled = true;
            if (loop2_rate > max_rate) {
                ROS_WARN("loop2_rate is greater than max_rate.  Setting loop2_rate to max_rate.");
                loop2_rate = max_rate;
            }
        }

        std::string param_loop3_rate = node_name + "/loop3_rate";
        if (n->getParam(param_loop3_rate, loop3_rate) == false) {
            ROS_WARN("Missing parameter: loop3_rate.  Not running loop3 code.");
            loop3_enabled = false;
        } else {
            loop3_enabled = true;
            if (loop3_rate > max_rate) {
                ROS_WARN("loop3_rate is greater than max_rate.  Setting loop3_rate to max_rate.");
                loop3_rate = max_rate;
            }
        }

        return true;
    }
    bool BaseNode::base_start() {
        bool status = request_node_statechange(robot_framework_ros::nodestate::STATE_STARTING, false);
        last_100hz_timer = ros::Time::now();
        last_10hz_timer = ros::Time::now();
        last_1hz_timer = ros::Time::now();
        last_01hz_timer = ros::Time::now();
        last_001hz_timer = ros::Time::now();

        last_loop1_timer = ros::Time::now();
        last_loop2_timer = ros::Time::now();
        last_loop3_timer = ros::Time::now();
        if (status == false) {
            return false;
        }
        return true;
    }
    bool BaseNode::base_restart() {
        bool status = start();
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
        double mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_100hz_timer);
        if (mtime >= 0.01) {  // 0.01 Seconds
            base_run_100hz();
            last_100hz_timer = ros::Time::now();
        }

        mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_10hz_timer);
        if (mtime >= 0.1) {  // 0.1 Seconds
            base_run_10hz();
            last_10hz_timer = ros::Time::now();
        }

        mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_1hz_timer);
        if (mtime >= 1.0) {  // 1.0 Seconds
            base_run_1hz();
            last_1hz_timer = ros::Time::now();
        }

        mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_01hz_timer);
        if (mtime >= 10.0) {  // 10.0 Seconds
            base_run_01hz();
            last_01hz_timer = ros::Time::now();
        }

        mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_001hz_timer);
        if (mtime >= 100.0) {  // 100.0 Seconds
            base_run_001hz();
            last_001hz_timer = ros::Time::now();
        }

        if (loop1_enabled == true) {
            mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_loop1_timer);
            if (mtime >= (1.0 / loop1_rate)) {
                run_loop1();
                last_loop1_timer = ros::Time::now();
            }
        }
        if (loop2_enabled == true) {
            mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_loop2_timer);
            if (mtime >= (1.0 / loop2_rate)) {
                run_loop2();
                last_loop2_timer = ros::Time::now();
            }
        }
        if (loop3_enabled == true) {
            mtime = utils::CoreUtility::measure_time_diff(ros::Time::now(), last_loop3_timer);
            if (mtime >= (1.0 / loop3_rate)) {
                run_loop3();
                last_loop3_timer = ros::Time::now();
            }
        }
        return true;
    }
    bool BaseNode::base_run_100hz() { return run_100hz(); }
    bool BaseNode::base_run_10hz() {
        robot_framework_ros::heartbeat beat;
        beat.stamp = ros::Time::now();
        beat.NodeName = node_name;
        beat.NodeState = node_state;
        heartbeat_pub.publish(beat);
        return run_10hz();
    }
    bool BaseNode::base_run_1hz() { return run_1hz(); }

    bool BaseNode::base_run_01hz() { return run_01hz(); }
    bool BaseNode::base_run_001hz() { return run_001hz(); }
    bool BaseNode::request_node_statechange(uint8_t new_state, bool override) {
        uint8_t current_state = node_state.state;
        if (current_state == new_state) {
            return true;
        }
        bool state_change_allowed = false;
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
            } else if (current_state == robot_framework_ros::nodestate::STATE_RUNNING) {
                if (new_state == robot_framework_ros::nodestate::STATE_STARTING) {
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