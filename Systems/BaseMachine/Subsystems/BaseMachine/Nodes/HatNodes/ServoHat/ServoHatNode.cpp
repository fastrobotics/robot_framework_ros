#include "ServoHatNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <boost/bind/bind.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;
namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem::HatDriver {

    ServoHatNode::ServoHatNode() {}
    ServoHatNode::~ServoHatNode() {}

    void ServoHatNode::robot_armcommand_state_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg) {
        robot_framework_ros::arm_command msg = *t_msg;
        process.update_RobotArmCommand(fast::rf_ros::utils::TranslateUtility::convert(msg));
    }
    void ServoHatNode::drive_Callback([[maybe_unused]] const std_msgs::Float64::ConstPtr& t_msg, uint16_t channel) {
        uint16_t value = (uint16_t)t_msg->data;
        bool status = process.setServoValue(channel, value);
        if (status == false) {
            fast::rf::Logger::log_error("Unable to update Channel: " + std::to_string(channel) +
                                        " with Value: " + std::to_string(value));
        }
    }
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
        status = load_config();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to load config!");
            return false;
        }

        robot_arm_command_state_sub = n->subscribe<robot_framework_ros::arm_command>(
            get_robotnamespace() + "/arm_command", 10, &ServoHatNode::robot_armcommand_state_Callback, this);

        std::string topic_left_drive;
        std::string param_left_drive = get_nodename() + "/topic_left_drive";
        if (n->getParam(param_left_drive, topic_left_drive) == false) {
            return false;
        }

        left_drive_sub = n->subscribe<std_msgs::Float64>(get_robotnamespace() + topic_left_drive, 10,
                                                         boost::bind(&ServoHatNode::drive_Callback, this, _1, 0));

        std::string topic_right_drive;
        std::string param_right_drive = get_nodename() + "/topic_right_drive";
        if (n->getParam(param_right_drive, topic_right_drive) == false) {
            return false;
        }
        right_drive_sub = n->subscribe<std_msgs::Float64>(get_robotnamespace() + topic_right_drive, 10,
                                                          boost::bind(&ServoHatNode::drive_Callback, this, _1, 1));
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool ServoHatNode::load_config() {
        std::string system_id_str = fast::rf::BaseMachineSystem::toString(fast::rf::BaseMachineSystem::Id{});
        std::string subsystem_id_str = fast::rf::BaseMachineSystem::BaseMachineSubsystem::toString(
            fast::rf::BaseMachineSystem::BaseMachineSubsystem::Id{});
        std::string process_id_str = fast::rf::BaseMachineSystem::BaseMachineSubsystem::HatDriver::toString(
            fast::rf::BaseMachineSystem::BaseMachineSubsystem::HatDriver::Id{});
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::log_info("Loading Config from:" + config_path);

        // Add support for config during AB#1850
        /*
        status = process.set_config();
        if(status == false) {
            fast::rf::Logger::log_error("Unable to set config!");
            return false;
        }
            */

        return true;
    }
    bool ServoHatNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
    bool ServoHatNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool ServoHatNode::run_loop2() { return true; }
    bool ServoHatNode::run_loop3() { return true; }
    bool ServoHatNode::run_100hz() { return true; }
    bool ServoHatNode::run_10hz() {
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool ServoHatNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();

        set_diagnostics(diagnostics);

        return true;
    }
    bool ServoHatNode::run_01hz() {
        fast::rf::Logger::log_info(process.pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool ServoHatNode::run_001hz() { return true; }

    void ServoHatNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void ServoHatNode::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem::HatDriver

using namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem::HatDriver;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeServoHat");
    auto node = std::make_unique<ServoHatNode>();
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
    std::thread thread(&ServoHatNode::thread_loop, node.get());
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
