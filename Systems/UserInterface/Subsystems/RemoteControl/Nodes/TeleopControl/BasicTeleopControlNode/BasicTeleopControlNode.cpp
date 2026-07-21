#include "BasicTeleopControlNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;

using namespace fast::rf_ros;
namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem {
    void BasicTeleopControlNode::joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg) {
        sensor_msgs::Joy joy_msg = *t_msg;
        process.new_joy(fast::rf_ros::utils::TranslateUtility::convert(joy_msg));
    }
    BasicTeleopControlNode::BasicTeleopControlNode() {}
    BasicTeleopControlNode::~BasicTeleopControlNode() {}
    bool BasicTeleopControlNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Base Node!");
            return false;
        }

        status =
            process.init(fast::rf::UserInterfaceSystem::RemoteControlSubsystem::ControlDevice::THRUSTMASTER_JOYSTICK);
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Process!");
            return false;
        }
        std::string operation_mode;
        std::string param_op_mode = get_nodename() + "/operation_mode";
        if (n->getParam(param_op_mode, operation_mode) == false) {
            status =
                process.set_operation_mode(fast::rf::UserInterfaceSystem::RemoteControlSubsystem::OperationMode::RUN);
        }
        if (operation_mode == "RUN") {
            status =
                process.set_operation_mode(fast::rf::UserInterfaceSystem::RemoteControlSubsystem::OperationMode::RUN);
        } else if (operation_mode == "TEST") {
            status = process.set_operation_mode(
                fast::rf::UserInterfaceSystem::RemoteControlSubsystem::OperationMode::JOY_TEST);
        }
        if (status == false) {
            fast::rf::Logger::log_error("Unable to set Operation Mode");
            return false;
        }
        std::string topic_joy_command;
        std::string param_topic_joy_command = get_nodename() + "/topic_joystick_command";
        if (n->getParam(param_topic_joy_command, topic_joy_command) == false) {
            return false;
        }

        std::string topic_throttle_command;
        std::string param_throttle_command = get_nodename() + "/topic_command_throttle";
        if (n->getParam(param_throttle_command, topic_throttle_command) == false) {
            return false;
        }

        joy_sub = n->subscribe<sensor_msgs::Joy>(get_robotnamespace() + topic_joy_command, 10,
                                                 &BasicTeleopControlNode::joy_Callback, this);
        twist_pub = n->advertise<geometry_msgs::Twist>(get_robotnamespace() + topic_throttle_command, 1);
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }

    bool BasicTeleopControlNode::start() { return BaseNode::base_start(); }
    bool BasicTeleopControlNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool BasicTeleopControlNode::run_loop2() {
        if (process.get_ready_to_arm().ready_to_arm == true) {
            auto twist = process.get_twist_output();
            twist_pub.publish(fast::rf_ros::utils::TranslateUtility::convert(twist));
        }

        return true;
    }
    bool BasicTeleopControlNode::run_loop3() { return true; }
    bool BasicTeleopControlNode::run_100hz() { return true; }
    bool BasicTeleopControlNode::run_10hz() {
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool BasicTeleopControlNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);
        return true;
    }
    bool BasicTeleopControlNode::run_01hz() {
        fast::rf::Logger::log_debug(process.pretty());
        fast::rf::Logger::log_debug(pretty());
        return true;
    }
    bool BasicTeleopControlNode::run_001hz() { return true; }

    void BasicTeleopControlNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing BasicTeleopControlNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeBasicTeleopControl");
    BasicTeleopControlNode* node = new BasicTeleopControlNode();
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
    std::thread thread(&BasicTeleopControlNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}