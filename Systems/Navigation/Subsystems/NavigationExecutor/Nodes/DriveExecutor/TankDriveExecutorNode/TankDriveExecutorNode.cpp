#include "TankDriveExecutorNode.hpp"

#include <std_msgs/Float64.h>

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;
namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor {

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

        fast::rf::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor::TankDriveChannelConfig
            left_channel_config(1000.0, 1500.0, 2000.0);
        fast::rf::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor::TankDriveChannelConfig
            right_channel_config(1000.0, 1500.0, 2000.0);
        if (process.set_config(left_channel_config, right_channel_config) == false) {
            return false;
        }
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }

    bool TankDriveExecutorNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
    bool TankDriveExecutorNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool TankDriveExecutorNode::run_loop2() {
        if (process.get_ready_to_arm().ready_to_arm == true) {
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor::IDriveExecutorOutput*
                general_output = process.get_output();
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor::TankDriveExecutorOutput* output =
                dynamic_cast<
                    fast::rf::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor::TankDriveExecutorOutput*>(
                    general_output);
            std_msgs::Float64 left_drive;
            left_drive.data = output->left_drive;
            std_msgs::Float64 right_drive;
            right_drive.data = output->right_drive;
            left_drive_pub.publish(left_drive);
            right_drive_pub.publish(right_drive);
        }
        return true;
    }
    bool TankDriveExecutorNode::run_loop3() { return true; }
    bool TankDriveExecutorNode::run_100hz() { return true; }
    bool TankDriveExecutorNode::run_10hz() {
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool TankDriveExecutorNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool TankDriveExecutorNode::run_01hz() {
        fast::rf::Logger::log_info(process.pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool TankDriveExecutorNode::run_001hz() { return true; }
    void TankDriveExecutorNode::stop() { is_node_running = false; }
    void TankDriveExecutorNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor

using namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem::DriveExecutor;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeTankDriveExecutor");
    auto node = std::make_unique<TankDriveExecutorNode>();
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
    std::thread thread(&TankDriveExecutorNode::thread_loop, node.get());
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
