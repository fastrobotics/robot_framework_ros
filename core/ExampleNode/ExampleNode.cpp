#include "ExampleNode.hpp"
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::example_package {

    ExampleNode::ExampleNode() {}
    ExampleNode::~ExampleNode() {}
    bool ExampleNode::init() { return BaseNode::base_init(); }

    bool ExampleNode::start() { return BaseNode::base_start(); }
    bool ExampleNode::run_loop1() {
        ROS_INFO("Loop1");
        return true;
    }
    bool ExampleNode::run_loop2() {
        ROS_INFO("Loop2");
        return true;
    }
    bool ExampleNode::run_loop3() {
        ROS_INFO("Loop3");
        return true;
    }
    bool ExampleNode::run_100hz() { return true; }
    bool ExampleNode::run_10hz() {
        robot_framework_ros::diagnostic diagnostic;
        diagnostic.SystemID = 0;
        diagnostic.SubsystemID = 0;
        diagnostic.ProcessID = 0;
        diagnostic.DiagnosticType = 0;
        diagnostic.Level = 0;
        diagnostic.DiagnosticMessage = 0;
        diagnostic.Description = "Test";
        std::vector<robot_framework_ros::diagnostic> diagnostics;
        diagnostics.push_back(diagnostic);
        set_diagnostics(diagnostics);
        return true;
    }
    bool ExampleNode::run_1hz() { return true; }
    bool ExampleNode::run_01hz() { return true; }
    bool ExampleNode::run_001hz() { return true; }

    void ExampleNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::example_package

void signalinterrupt_handler(int sig) {
    ROS_WARN("Killing ExampleNode with Signal: %d\n", sig);
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::example_package;
int main(int argc, char** argv) {
    ros::init(argc, argv, "example_node");
    ExampleNode* node = new ExampleNode();
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
    std::thread thread(&ExampleNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
        // status = node->update(node->get_process()->get_nodestate());
    }
    thread.detach();
    delete node;
    return 0;
}