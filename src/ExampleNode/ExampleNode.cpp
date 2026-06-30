#include "ExampleNode.hpp"
bool kill_node = false;
namespace fast::rf_ros {

ExampleNode::ExampleNode() : *n.get() {}
ExampleNode::~ExampleNode() {}
}  // namespace fast::rf_ros
void signalinterrupt_handler(int sig) {
    printf("Killing ExampleNode with Signal: %d\n", sig);
    kill_node = true;
    exit(0);
}
using fast::rf_ros;
int main(int argc, char **argv) {
    signal(SIGINT, signalinterrupt_handler);
    signal(SIGTERM, signalinterrupt_handler);
    ros::init(argc, argv, "example_node");
    ExampleNode *node = new ExampleNode();
    bool status = node->start();
    if (status == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    std::thread thread(&ExampleNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        // status = node->update(node->get_process()->get_nodestate());
    }
    node->cleanup();
    thread.detach();
    delete node;
    return 0;
}