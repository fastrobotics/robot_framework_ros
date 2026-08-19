#include "TrajectoryControllerNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController {

    TrajectoryControllerNode::TrajectoryControllerNode() {}
    TrajectoryControllerNode::~TrajectoryControllerNode() {}
    void TrajectoryControllerNode::desired_command_Callback(const geometry_msgs::Twist::ConstPtr& t_msg) {
        geometry_msgs::Twist desired_command = *t_msg;
        process.new_desired_command(fast::rf_ros::utils::TranslateUtility::convert(desired_command));
    }

    void TrajectoryControllerNode::pose_Callback(const nav_msgs::Odometry::ConstPtr& t_msg) {
        nav_msgs::Odometry pose = *t_msg;
        process.new_pose(fast::rf_ros::utils::TranslateUtility::convert(pose));
    }
    bool TrajectoryControllerNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Base Node!");
            return false;
        }
        status = process.init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Process!");
            return false;
        }
        process.set_parameters(10.0, -10.0, 1.0, 1.0, 0.0, 0.0);  // Tune this dynamically during AB#1815
        std::string topic_pose;
        std::string param_pose = get_nodename() + "/topic_pose_input";
        if (n->getParam(param_pose, topic_pose) == false) {
            fast::rf::Logger::log_error("Parameter topic_pose_input Not Defined!  Exiting.");
            return false;
        }

        pose_sub = n->subscribe<nav_msgs::Odometry>(get_robotnamespace() + topic_pose, 10,
                                                    &TrajectoryControllerNode::pose_Callback, this);

        std::string topic_desired_command;
        std::string param_command_throttle = get_nodename() + "/topic_command_throttle_input";
        if (n->getParam(param_command_throttle, topic_desired_command) == false) {
            fast::rf::Logger::log_error("Parameter topic_command_throttle_input Not Defined!  Exiting.");
            return false;
        }

        desired_command_sub =
            n->subscribe<geometry_msgs::Twist>(get_robotnamespace() + topic_desired_command, 10,
                                               &TrajectoryControllerNode::desired_command_Callback, this);

        std::string topic_command;
        std::string param_command = get_nodename() + "/topic_command_robot_output";
        if (n->getParam(param_command, topic_command) == false) {
            fast::rf::Logger::log_error("Parameter topic_command_robot_output Not Defined!  Exiting.");
            return false;
        }
        command_pub = n->advertise<geometry_msgs::Twist>(get_robotnamespace() + topic_command, 1);
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }

    bool TrajectoryControllerNode::start() { return BaseNode::base_start(); }
    bool TrajectoryControllerNode::run_loop1() {
        process.update(ros::Time::now().toSec());
        fast::rf::messages::GeometryMsgs::TwistMsg new_command;
        if (process.get_command(new_command)) {
            auto cmd = fast::rf_ros::utils::TranslateUtility::convert(new_command);
            command_pub.publish(cmd);
        }
        return true;
    }
    bool TrajectoryControllerNode::run_loop2() { return true; }
    bool TrajectoryControllerNode::run_loop3() { return true; }
    bool TrajectoryControllerNode::run_100hz() { return true; }
    bool TrajectoryControllerNode::run_10hz() {
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool TrajectoryControllerNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool TrajectoryControllerNode::run_01hz() {
        fast::rf::Logger::log_info(process.pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool TrajectoryControllerNode::run_001hz() { return true; }

    void TrajectoryControllerNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing TrajectoryControllerNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeTrajectoryController");
    TrajectoryControllerNode* node = new TrajectoryControllerNode();
    signal(SIGINT, signalinterrupt_handler);
    signal(SIGTERM, signalinterrupt_handler);
    bool status = node->init();
    if (status == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    status = node->start();
    if (status == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    std::thread thread(&TrajectoryControllerNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}