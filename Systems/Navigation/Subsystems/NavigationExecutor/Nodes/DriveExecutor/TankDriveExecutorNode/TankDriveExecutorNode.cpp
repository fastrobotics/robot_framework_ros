#include "TankDriveExecutorNode.hpp"

#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem {

    TankDriveExecutorNode::TankDriveExecutorNode() {}
    TankDriveExecutorNode::~TankDriveExecutorNode() {}
    void TankDriveExecutorNode::twist_Callback(const geometry_msgs::Twist::ConstPtr& t_msg) {
        geometry_msgs::Twist twist_msg = *t_msg;
        process.new_cmd(fast::rf_ros::utils::TranslateUtility::convert(twist_msg));
        ROS_WARN("Got Twist");
    }
    bool TankDriveExecutorNode::init() {
        bool status = process.init();
        if (status == false) {
            return false;
        }
        twist_sub = n->subscribe<geometry_msgs::Twist>("/cmd_vel", 10, &TankDriveExecutorNode::twist_Callback, this);
        return BaseNode::base_init();
    }

    bool TankDriveExecutorNode::start() { return BaseNode::base_start(); }
    bool TankDriveExecutorNode::run_loop1() { return true; }
    bool TankDriveExecutorNode::run_loop2() { return true; }
    bool TankDriveExecutorNode::run_loop3() { return true; }
    bool TankDriveExecutorNode::run_100hz() { return true; }
    bool TankDriveExecutorNode::run_10hz() { return true; }
    bool TankDriveExecutorNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);
        ROS_WARN("%s", process.pretty().c_str());
        return true;
    }
    bool TankDriveExecutorNode::run_01hz() { return true; }
    bool TankDriveExecutorNode::run_001hz() { return true; }

    void TankDriveExecutorNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::NavigationSystem::NavigationExecutorSubsystem

void signalinterrupt_handler(int sig) {
    ROS_WARN("Killing TankDriveExecutorNode with Signal: %d\n", sig);
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