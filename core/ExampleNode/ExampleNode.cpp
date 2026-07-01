#include "ExampleNode.hpp"
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::example_package {

ExampleNode::ExampleNode() {}
ExampleNode::~ExampleNode() {}
bool ExampleNode::init() { return BaseNode::base_init(); }

bool ExampleNode::start() { return BaseNode::base_start(); }

bool ExampleNode::run_10hz() {
    return BaseNode::base_run_10hz();
    ROS_WARN(pretty().c_str());
}
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