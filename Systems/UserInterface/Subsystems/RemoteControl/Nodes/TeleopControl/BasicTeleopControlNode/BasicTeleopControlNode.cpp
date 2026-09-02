/**
 * @compare_tag Node-Source  v0.1
 *
 */
#include "BasicTeleopControlNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>

using namespace fast::rf_ros;
namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl {
    void BasicTeleopControlNode::robot_armcommand_state_Callback(
        const robot_framework_ros::arm_command::ConstPtr& t_msg) {
        robot_framework_ros::arm_command msg = *t_msg;
        process.update_RobotArmCommand(fast::rf_ros::utils::TranslateUtility::convert(msg));
    }
    void BasicTeleopControlNode::joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg) {
        sensor_msgs::Joy joy_msg = *t_msg;
        process.new_joy(fast::rf_ros::utils::TranslateUtility::convert(joy_msg));
    }

    BasicTeleopControlNode::BasicTeleopControlNode() {}
    BasicTeleopControlNode::~BasicTeleopControlNode() {}
    bool BasicTeleopControlNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node!");
            return false;
        }

        status = load_config();
        if (status == false) {
            fast::rf::Logger::logError("Unable to load config!");
            return false;
        }

        std::string operation_mode;
        std::string param_op_mode = get_nodename() + "/operation_mode";
        if (n->getParam(param_op_mode, operation_mode) == false) {
            status = process.set_operation_mode(
                fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::OperationMode::RUN);
        }
        if (operation_mode == "RUN") {
            status = process.set_operation_mode(
                fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::OperationMode::RUN);
        } else if (operation_mode == "TEST") {
            status = process.set_operation_mode(
                fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::OperationMode::JOY_TEST);
        }
        if (status == false) {
            fast::rf::Logger::logError("Unable to set Operation Mode");
            return false;
        }
        status = initBaseNodeDiagnostics(process.getSystemId(), process.getSubSystemId(), process.getProcessId());
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node Diagnostics!");
            return false;
        }
        robot_arm_command_state_sub = n->subscribe<robot_framework_ros::arm_command>(
            get_robotnamespace() + "/arm_command", 10, &BasicTeleopControlNode::robot_armcommand_state_Callback, this);

        armstate_change_client =
            n->serviceClient<robot_framework_ros::arm_state_change>(get_robotnamespace() + "/arm_state_change");

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
    bool BasicTeleopControlNode::load_config() {
        std::string system_id_str = fast::rf::UserInterfaceSystem::toString(fast::rf::UserInterfaceSystem::Id{});
        std::string subsystem_id_str = fast::rf::UserInterfaceSystem::RemoteControlSubsystem::toString(
            fast::rf::UserInterfaceSystem::RemoteControlSubsystem::Id{});
        std::string process_id_str = fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::toString(
            fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::Id{});
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::logInfo("Loading Config from:" + config_path);

        /**
         * @todo Make this config during AB#1853
         *
         */
        fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::JoystickCalibrationData
            joy_calibration_data;
        XmlRpc::XmlRpcValue calibration_config;
        if (n->getParam(get_nodename() + "/calibration", calibration_config)) {
            if (calibration_config.getType() == XmlRpc::XmlRpcValue::TypeStruct) {
                joy_calibration_data.x_max = calibration_config["x_max"];
                joy_calibration_data.x_deadband = calibration_config["x_deadband"];
                joy_calibration_data.x_min = calibration_config["x_min"];
                joy_calibration_data.y_max = calibration_config["y_max"];
                joy_calibration_data.y_deadband = calibration_config["y_deadband"];
                joy_calibration_data.y_min = calibration_config["y_min"];
                joy_calibration_data.throttle_max = calibration_config["throttle_max"];
                joy_calibration_data.throttle_deadband = calibration_config["throttle_deadband"];
                joy_calibration_data.throttle_min = calibration_config["throttle_min"];

            } else {
                fast::rf::Logger::logError("Error parsing Joystick Calibration.  Exiting.");
                return false;
            }
        } else {
            fast::rf::Logger::logWarn("No Joystick Calibration Found.  Using default!");
            joy_calibration_data.optional_init();
        }
        bool status = process.init(
            fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::ControlDevice::THRUSTMASTER_JOYSTICK,
            joy_calibration_data);
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Process!");
            return false;
        }
        return status;
    }
    bool BasicTeleopControlNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
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
        auto request = process.get_armstate_change_request();
        if (request.requested_armed_state != fast::rf::ArmedState::UNKNOWN) {
            robot_framework_ros::arm_state_change req;
            req.request = fast::rf_ros::utils::TranslateUtility::convert(request);
            if (armstate_change_client.call(req) == true) {
                fast::rf::Logger::logNotice("Requested Arm State Changed!");
            } else {
                fast::rf::Logger::logError("Requested Arm State Rejected!");
            }
        }
        return true;
    }
    bool BasicTeleopControlNode::run_1hz() {
        auto diagnostics = process.getDiagnostics();
        set_diagnostics(diagnostics);
        return true;
    }
    bool BasicTeleopControlNode::run_01hz() {
        auto baseNodeDiagnostics = getBaseNodeDiagnostics();
        for (auto it : baseNodeDiagnostics) {
            process.updateDiagnostic(it.second.diagnosticType, it.second.level, it.second.diagnosticMessage,
                                     it.second.description);
        }
        fast::rf::Logger::logInfo(process.pretty());
        fast::rf::Logger::logInfo(pretty());
        return true;
    }
    bool BasicTeleopControlNode::run_001hz() { return true; }

    void BasicTeleopControlNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void BasicTeleopControlNode::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl

using namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeBasicTeleopControl");
    auto node = std::make_unique<BasicTeleopControlNode>();
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
    std::thread thread(&BasicTeleopControlNode::thread_loop, node.get());
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
