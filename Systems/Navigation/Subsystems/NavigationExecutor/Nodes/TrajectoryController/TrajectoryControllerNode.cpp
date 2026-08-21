#include "TrajectoryControllerNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
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
        status = load_config();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to load config!");
            return false;
        }

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
    bool TrajectoryControllerNode::load_config() {
        std::string system_id_str = fast::rf::NavigationSystem::toString(fast::rf::NavigationSystem::Id{});

        std::string subsystem_id_str = fast::rf::NavigationSystem::NavigationExecutorSubsystem::toString(
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::Id{});
        std::string process_id_str =
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController::toString(
                fast::rf::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController::Id{});
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::log_info("Loading Config from:" + config_path);

        double max_output;
        if (n->getParam(config_path + "/max_output", max_output) == false) {
            fast::rf::Logger::log_error("Parameter: " + config_path + "/max_output Not Defined!  Exiting.");
            return false;
        }
        double min_output;
        if (n->getParam(config_path + "/min_output", min_output) == false) {
            fast::rf::Logger::log_error("Parameter: " + config_path + "/min_output Not Defined!  Exiting.");
            return false;
        }
        double K_P;
        if (n->getParam(config_path + "/K_P", K_P) == false) {
            fast::rf::Logger::log_error("Parameter: " + config_path + "/K_P Not Defined!  Exiting.");
            return false;
        }
        double K_I;
        if (n->getParam(config_path + "/K_I", K_I) == false) {
            fast::rf::Logger::log_error("Parameter: " + config_path + "/K_I Not Defined!  Exiting.");
            return false;
        }
        double K_D;
        if (n->getParam(config_path + "/K_D", K_D) == false) {
            fast::rf::Logger::log_error("Parameter: " + config_path + "/K_D Not Defined!  Exiting.");
            return false;
        }
        double sensor_scale;
        if (n->getParam(config_path + "/sensor_scale", sensor_scale) == false) {
            fast::rf::Logger::log_error("Parameter: " + config_path + "/sensor_scale Not Defined!  Exiting.");
            return false;
        }

        fast::rf::NavigationSystem::Controller::PIDControllerConfig config;
        config.set_parameters(max_output, min_output, K_P, K_I, K_D, sensor_scale);
        if (process.set_config(config) == false) {
            return false;
        }
        return true;
    }
    bool TrajectoryControllerNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
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
        while (ros::ok() && is_node_running) {
        }
    }
    void TrajectoryControllerNode::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController

using namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::TrajectoryController;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeTrajectoryController");
    auto node = std::make_unique<TrajectoryControllerNode>();
    if (!node->init()) {
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }

    if (!node->start()) {
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    std::thread thread(&TrajectoryControllerNode::thread_loop, node.get());
    bool status = true;
    while (ros::ok() && status) {
        status = node->update();
        ros::spinOnce();
    }
    node->stop();
    if (thread.joinable()) {
        thread.join();
    }
    return 0;
}