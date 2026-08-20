#include "{{cookiecutter.Node}}Node.hpp"


#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;
namespace fast::rf_ros::{{cookiecutter.System}}System::{{cookiecutter.Subsystem}}Subsystem::{{cookiecutter.Node}} {

    {{cookiecutter.Node}}Node::{{cookiecutter.Node}}Node() {}
    {{cookiecutter.Node}}Node::~{{cookiecutter.Node}}Node() {}
    bool {{cookiecutter.Node}}Node::init() {
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
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }

    bool {{cookiecutter.Node}}Node::start() { 
        is_node_running = true;
        return BaseNode::base_start(); }
    bool {{cookiecutter.Node}}Node::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool {{cookiecutter.Node}}Node::run_loop2() {
        return true;
    }
    bool {{cookiecutter.Node}}Node::run_loop3() { return true; }
    bool {{cookiecutter.Node}}Node::run_100hz() { return true; }
    bool {{cookiecutter.Node}}Node::run_10hz() { 
        set_ready_to_arm(process.get_ready_to_arm());
    return true; }
    bool {{cookiecutter.Node}}Node::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool {{cookiecutter.Node}}Node::run_01hz() {
        fast::rf::Logger::log_info(process.pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool {{cookiecutter.Node}}Node::run_001hz() { return true; }

    void {{cookiecutter.Node}}Node::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void {{cookiecutter.Node}}Node::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::{{cookiecutter.System}}System::{{cookiecutter.Subsystem}}Subsystem
using namespace fast::rf_ros::{{cookiecutter.System}}System::{{cookiecutter.Subsystem}}Subsystem::{{cookiecutter.Node}};
int main(int argc, char** argv) {
    ros::init(argc, argv, "node{{cookiecutter.Node}}");
    auto node = std::make_unique<{{cookiecutter.Node}}Node>();
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
    std::thread thread(&{{cookiecutter.Node}}Node::thread_loop, node.get());
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