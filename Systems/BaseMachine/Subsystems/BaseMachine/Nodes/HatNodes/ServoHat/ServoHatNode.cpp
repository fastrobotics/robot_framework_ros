#include "ServoHatNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem {

    ServoHatNode::ServoHatNode() {}
    ServoHatNode::~ServoHatNode() {}
    bool ServoHatNode::init() {
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
        return true;
    }

    bool ServoHatNode::start() { return BaseNode::base_start(); }
    bool ServoHatNode::run_loop1() {
        process.update(ros::Time::now().toSec(), 0.0);

        return true;
    }
    bool ServoHatNode::run_loop2() {
        bool diagnostic_check_ok = false;
        if (diagnostic_check_ok == true) {
        } else {
            fast::rf::Logger::log_warn("Diagnostic Check Failed!  Disabling Outputs.");
        }
        return true;
    }
    bool ServoHatNode::run_loop3() { return true; }
    bool ServoHatNode::run_100hz() { return true; }
    bool ServoHatNode::run_10hz() { return true; }
    bool ServoHatNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool ServoHatNode::run_01hz() {
        fast::rf::Logger::log_notice(pretty());
        return true;
    }
    bool ServoHatNode::run_001hz() { return true; }

    void ServoHatNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing ServoHatNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeServoHat");
    ServoHatNode* node = new ServoHatNode();
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
    std::thread thread(&ServoHatNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}