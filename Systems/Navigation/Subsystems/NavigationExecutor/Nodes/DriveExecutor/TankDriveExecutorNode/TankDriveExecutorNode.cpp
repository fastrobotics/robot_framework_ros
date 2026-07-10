#include "TankDriveExecutorNode.hpp"

#include <std_msgs/Float64.h>

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem {

    TankDriveExecutorNode::TankDriveExecutorNode() {}
    TankDriveExecutorNode::~TankDriveExecutorNode() {}
    void TankDriveExecutorNode::twist_Callback(const geometry_msgs::Twist::ConstPtr& t_msg) {
        geometry_msgs::Twist twist_msg = *t_msg;
        process.new_cmd(fast::rf_ros::utils::TranslateUtility::convert(twist_msg));
    }
    bool TankDriveExecutorNode::init() {
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
        std::string topic_left_drive;
        std::string param_left_drive = get_nodename() + "/topic_left_drive";
        if (n->getParam(param_left_drive, topic_left_drive) == false) {
            return false;
        }

        left_drive_pub = n->advertise<std_msgs::Float64>(get_robotnamespace() + topic_left_drive, 1);

        std::string topic_right_drive;
        std::string param_right_drive = get_nodename() + "/topic_right_drive";
        if (n->getParam(param_right_drive, topic_right_drive) == false) {
            return false;
        }
        right_drive_pub = n->advertise<std_msgs::Float64>(get_robotnamespace() + topic_right_drive, 1);

        std::string topic_throttle_command;
        std::string param_throttle_command = get_nodename() + "/topic_command_throttle";
        if (n->getParam(param_throttle_command, topic_throttle_command) == false) {
            return false;
        }

        twist_sub = n->subscribe<geometry_msgs::Twist>(get_robotnamespace() + topic_throttle_command, 10,
                                                       &TankDriveExecutorNode::twist_Callback, this);

        fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveChannelConfig left_channel_config(
            1000.0, 1500.0, 2000.0);
        fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveChannelConfig right_channel_config(
            1000.0, 1500.0, 2000.0);
        process.set_config(left_channel_config, right_channel_config);

        return true;
    }

    bool TankDriveExecutorNode::start() { return BaseNode::base_start(); }
    bool TankDriveExecutorNode::run_loop1() {
        process.update(ros::Time::now().toSec(), 0.0);

        return true;
    }
    bool TankDriveExecutorNode::run_loop2() {
        bool diagnostic_check_ok = false;
        auto diagnostics = process.get_diagnostics();
        for (auto diagnostic : diagnostics) {
            if ((diagnostic.diagnosticType == fast::rf::DiagnosticDefinition::DiagnosticType::REMOTE_CONTROL) and
                (diagnostic.diagnosticMessage == fast::rf::DiagnosticDefinition::DiagnosticMessage::NOERROR)) {
                diagnostic_check_ok = true;
            }
        }
        if (diagnostic_check_ok == true) {
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::IDriveExecutorOutput* general_output =
                process.get_output();
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveExecutorOutput* output =
                dynamic_cast<fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveExecutorOutput*>(
                    general_output);
            std_msgs::Float64 left_drive;
            left_drive.data = output->left_drive;
            std_msgs::Float64 right_drive;
            right_drive.data = output->right_drive;
            left_drive_pub.publish(left_drive);
            right_drive_pub.publish(right_drive);
        } else {
            fast::rf::Logger::log_warn("Diagnostic Check Failed!  Disabling Outputs.");
        }
        return true;
    }
    bool TankDriveExecutorNode::run_loop3() { return true; }
    bool TankDriveExecutorNode::run_100hz() { return true; }
    bool TankDriveExecutorNode::run_10hz() { return true; }
    bool TankDriveExecutorNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool TankDriveExecutorNode::run_01hz() {
        fast::rf::Logger::log_notice(pretty());
        return true;
    }
    bool TankDriveExecutorNode::run_001hz() { return true; }

    void TankDriveExecutorNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing TankDriveExecutorNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeTankDriveExecutor");
    TankDriveExecutorNode* node = new TankDriveExecutorNode();
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
    std::thread thread(&TankDriveExecutorNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}