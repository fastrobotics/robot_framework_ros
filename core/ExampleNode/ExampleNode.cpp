#include "ExampleNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <RobotFrameworkDefinitions.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::example_package {

    ExampleNode::ExampleNode() {}
    ExampleNode::~ExampleNode() {}
    bool ExampleNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node!");
            return false;
        }
        status = load_config();
        if (status == false) {
            fast::rf::Logger::logError("Unable to load Config!");
            return false;
        }
        return true;
    }
    bool ExampleNode::load_config() {
        std::string system_id_str = "example_system";
        std::string subsystem_id_str = "example_subsystem";
        std::string process_id_str = "example_process";
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::logInfo("Loading Config from:" + config_path);
        return true;
    }
    bool ExampleNode::start() { return BaseNode::base_start(); }
    bool ExampleNode::run_loop1() {
        fast::rf::Logger::logDebug("Loop1");
        return true;
    }
    bool ExampleNode::run_loop2() {
        fast::rf::Logger::logDebug("Loop2");
        return true;
    }
    bool ExampleNode::run_loop3() {
        fast::rf::Logger::logDebug("Loop3");
        return true;
    }
    bool ExampleNode::run_100hz() { return true; }
    bool ExampleNode::run_10hz() {
        fast::rf::messages::InfrastructureMsgs::DiagnosticMsg diag;
        diag.systemID = 0;
        diag.subsystemID = 0;
        diag.processID = 0;
        diag.diagnosticType = fast::rf::DiagnosticDefinition::DiagnosticType::SOFTWARE;
        diag.level = fast::rf::Level::INFO;
        diag.diagnosticMessage = fast::rf::DiagnosticDefinition::DiagnosticMessage::INITIALIZING;
        diag.description = "Test";
        std::vector<fast::rf::messages::InfrastructureMsgs::DiagnosticMsg> diagnostics;
        diagnostics.push_back(diag);
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
    fast::rf::Logger::logWarn("Killing ExampleNode with Signal: " + std::to_string(sig));
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