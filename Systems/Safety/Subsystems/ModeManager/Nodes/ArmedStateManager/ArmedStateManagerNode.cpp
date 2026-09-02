/**
 * @compare_tag Node-Source  v0.1
 *
 */
#include "ArmedStateManagerNode.hpp"

#include <robot_framework_ros/arm_command.h>

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>

using namespace fast::rf_ros;
namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem::ArmedStateManager {

    ArmedStateManagerNode::ArmedStateManagerNode() {}
    ArmedStateManagerNode::~ArmedStateManagerNode() {}
    bool ArmedStateManagerNode::arm_statechange_service(robot_framework_ros::arm_state_change::Request& req,
                                                        robot_framework_ros::arm_state_change::Response& res) {
        auto request = fast::rf_ros::utils::TranslateUtility::convert(req);
        auto response = process.request_armstate_change(request);
        res = fast::rf_ros::utils::TranslateUtility::convert(response);
        return true;
    }
    void ArmedStateManagerNode::ready_to_arm_Callback(const robot_framework_ros::ready_to_arm::ConstPtr& t_msg) {
        robot_framework_ros::ready_to_arm msg = *t_msg;
        if (process.new_ReadyToArmStatus(fast::rf_ros::utils::TranslateUtility::convert(msg)) == false) {
            fast::rf::Logger::logError("Node: " + msg.NodeName + " Unable to process Ready To Arm Msg");
        }
    }
    bool ArmedStateManagerNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node!");
            return false;
        }
        status = process.init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Process!");
            return false;
        }
        status = load_config();
        if (status == false) {
            fast::rf::Logger::logError("Unable to load config!");
            return false;
        }

        std::string arm_command_topic = get_robotnamespace() + "/arm_command";
        arm_command_pub = n->advertise<robot_framework_ros::arm_command>(arm_command_topic, 1);

        // Read Ready To Arm Topics and Subscribe
        for (auto node_name : nodes_to_monitor) {
            std::string ready_to_arm_topic = get_robotnamespace() + node_name + "/ready_to_arm";
            fast::rf::Logger::logInfo("Subscribing to: " + ready_to_arm_topic);
            ros::Subscriber sub = n->subscribe<robot_framework_ros::ready_to_arm>(
                ready_to_arm_topic, 10, &ArmedStateManagerNode::ready_to_arm_Callback, this);
            ready_to_arm_subs.push_back(sub);
        }
        if (ready_to_arm_subs.size() == 0) {
            fast::rf::Logger::logError("Need at least 1 Ready To Arm Topic.  Exiting.");
            return false;
        }
        std::string armstate_change_topic = get_robotnamespace() + "/arm_state_change";
        armstate_change_srv =
            n->advertiseService(armstate_change_topic, &ArmedStateManagerNode::arm_statechange_service, this);
        status = initBaseNodeDiagnostics(process.getSystemId(), process.getSubSystemId(), process.getProcessId());
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node Diagnostics!");
            return false;
        }
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool ArmedStateManagerNode::load_config() {
        std::string system_id_str = fast::rf::SafetySystem::toString(fast::rf::SafetySystem::Id{});
        std::string subsystem_id_str =
            fast::rf::SafetySystem::ModeManagerSubsystem::toString(fast::rf::SafetySystem::ModeManagerSubsystem::Id{});
        std::string process_id_str = fast::rf::SafetySystem::ModeManagerSubsystem::ArmedStateManager::toString(
            fast::rf::SafetySystem::ModeManagerSubsystem::ArmedStateManager::Id{});
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::logInfo("Loading Config from:" + config_path);

        if (n->getParam(config_path + "/nodes_to_monitor", nodes_to_monitor) == false) {
            fast::rf::Logger::logError("Parameter: " + config_path + "/nodes_to_monitor Not Defined!  Exiting.");
            return false;
        }
        fast::rf::SafetySystem::ModeManagerSubsystem::ArmedStateManager::ArmedStateManagerProcessConfig config;
        config.expected_arm_signals = (uint8_t)nodes_to_monitor.size();
        if (process.set_config(config) == false) {
            fast::rf::Logger::logError("Config not Valid! " + config.pretty());
            return false;
        }
        return true;
    }
    bool ArmedStateManagerNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
    bool ArmedStateManagerNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool ArmedStateManagerNode::run_loop2() { return true; }
    bool ArmedStateManagerNode::run_loop3() { return true; }
    bool ArmedStateManagerNode::run_100hz() { return true; }
    bool ArmedStateManagerNode::run_10hz() {
        set_ready_to_arm(process.get_ready_to_arm());
        arm_command_pub.publish(fast::rf_ros::utils::TranslateUtility::convert(process.get_ArmCommandMsg()));
        return true;
    }
    bool ArmedStateManagerNode::run_1hz() {
        auto diagnostics = process.getDiagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool ArmedStateManagerNode::run_01hz() {
        auto baseNodeDiagnostics = getBaseNodeDiagnostics();
        for (auto it : baseNodeDiagnostics) {
            process.updateDiagnostic(it.second.diagnosticType, it.second.level, it.second.diagnosticMessage,
                                     it.second.description);
        }
        fast::rf::Logger::logInfo(process.pretty());
        fast::rf::Logger::logInfo(pretty());
        return true;
    }
    bool ArmedStateManagerNode::run_001hz() { return true; }

    void ArmedStateManagerNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void ArmedStateManagerNode::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem::ArmedStateManager

using namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem::ArmedStateManager;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeArmedStateManager");
    auto node = std::make_unique<ArmedStateManagerNode>();
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
    std::thread thread(&ArmedStateManagerNode::thread_loop, node.get());
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
