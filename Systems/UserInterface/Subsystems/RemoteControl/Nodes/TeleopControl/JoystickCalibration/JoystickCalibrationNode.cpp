#include "JoystickCalibrationNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <chrono>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;

namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl {

    JoystickCalibrationNode::JoystickCalibrationNode() {}
    JoystickCalibrationNode::~JoystickCalibrationNode() {}
    void JoystickCalibrationNode::joy_Callback(const sensor_msgs::Joy::ConstPtr& t_msg) {
        sensor_msgs::Joy msg = *t_msg;
        if (msg.axes.size() < 3) {
            fast::rf::Logger::logError("Got unexpected Joy Message!");
            return;
        }
        double x_value = msg.axes[0];
        if (x_value > calibration_data.x_max) {
            calibration_data.x_max = x_value;
        }
        if (x_value < calibration_data.x_min) {
            calibration_data.x_min = x_value;
        }
        double y_value = msg.axes[1];
        if (y_value > calibration_data.y_max) {
            calibration_data.y_max = y_value;
        }
        if (y_value < calibration_data.y_min) {
            calibration_data.y_min = y_value;
        }
        double throttle_value = msg.axes[2];
        if (throttle_value > calibration_data.throttle_max) {
            calibration_data.throttle_max = throttle_value;
        }
        if (throttle_value < calibration_data.throttle_min) {
            calibration_data.throttle_min = throttle_value;
        }
    }
    bool JoystickCalibrationNode::init() {
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

        std::string topic_joy_command = "/robot/joy";
        if (n->getParam(get_nodename() + "/topic_joy_command", topic_joy_command) == false) {
            fast::rf::Logger::logWarn("topic_joy_command Not specified.  Using default: " + topic_joy_command);
        }
        joy_sub = n->subscribe<sensor_msgs::Joy>(get_robotnamespace() + topic_joy_command, 10,
                                                 &JoystickCalibrationNode::joy_Callback, this);
        return true;
    }
    bool JoystickCalibrationNode::load_config() {
        std::string system_id_str = fast::rf::UserInterfaceSystem::toString(fast::rf::UserInterfaceSystem::Id{});
        std::string subsystem_id_str = fast::rf::UserInterfaceSystem::RemoteControlSubsystem::toString(
            fast::rf::UserInterfaceSystem::RemoteControlSubsystem::Id{});
        std::string process_id_str = fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::toString(
            fast::rf::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl::Id{});
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::logInfo("Loading Config from:" + config_path);

        // Provide user space config during AB#1854
        std::string output_file_path;
        if (n->getParam(get_nodename() + "/output_file_path", output_file_path) == false) {
            fast::rf::Logger::logError("output_file_path Not specified!  Exiting.");
            return false;
        }
        double x_deadband = 0.0;
        if (n->getParam(get_nodename() + "/x_deadband", x_deadband) == false) {
            fast::rf::Logger::logWarn("x_deadband Not specified.  Using Default=0.0.");
        }
        double y_deadband = 0.0;
        if (n->getParam(get_nodename() + "/y_deadband", y_deadband) == false) {
            fast::rf::Logger::logWarn("y_deadband Not specified.  Using Default=0.0.");
        }

        double throttle_deadband = 0.0;
        if (n->getParam(get_nodename() + "/throttle_deadband", throttle_deadband) == false) {
            fast::rf::Logger::logWarn("throttle_deadband Not specified.  Using Default=0.0.");
        }
        calibration_data.x_deadband = x_deadband;
        calibration_data.y_deadband = y_deadband;
        calibration_data.throttle_deadband = throttle_deadband;
        output_config_fd.open(output_file_path);
        if (output_config_fd.is_open() == false) {
            fast::rf::Logger::logError("Unable to create file at path: " + output_file_path + "! Exiting.");
            return false;
        }
        return true;
    }
    bool JoystickCalibrationNode::start() { return BaseNode::base_start(); }
    bool JoystickCalibrationNode::run_loop1() { return true; }
    bool JoystickCalibrationNode::run_loop2() { return true; }
    bool JoystickCalibrationNode::run_loop3() { return true; }
    bool JoystickCalibrationNode::run_100hz() { return true; }
    bool JoystickCalibrationNode::run_10hz() {
        printf("%s", calibration_data.pretty().c_str());
        return true;
    }
    bool JoystickCalibrationNode::run_1hz() { return true; }
    bool JoystickCalibrationNode::run_01hz() {
        fast::rf::Logger::logInfo(pretty());
        return true;
    }
    bool JoystickCalibrationNode::run_001hz() { return true; }

    void JoystickCalibrationNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void JoystickCalibrationNode::stop() {
        is_node_running = false;
        fast::rf::Logger::logWarn("Writing Joystick configuration.");
        output_config_fd << "config_file_type: Joystick Calibration Configuration" << std::endl;
        // 1. Get current time point from system clock
        auto now = std::chrono::system_clock::now();

        // 2. Convert to time_t (seconds since epoch)
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);

        // 3. Convert to local time structure
        std::tm tm_now = *std::localtime(&time_now);

        // 4. Use stringstream and put_time to format
        output_config_fd << "date_generated: " << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << std::endl;

        output_config_fd << "calibration:" << std::endl;
        output_config_fd << "  x_deadband: " << std::to_string(calibration_data.x_deadband) << std::endl;
        output_config_fd << "  x_min: " << std::to_string(calibration_data.x_min) << std::endl;
        output_config_fd << "  x_max: " << std::to_string(calibration_data.x_max) << std::endl;
        output_config_fd << "  y_deadband: " << std::to_string(calibration_data.y_deadband) << std::endl;
        output_config_fd << "  y_min: " << std::to_string(calibration_data.y_min) << std::endl;
        output_config_fd << "  y_max: " << std::to_string(calibration_data.y_max) << std::endl;
        output_config_fd << "  throttle_deadband: " << std::to_string(calibration_data.throttle_deadband) << std::endl;
        output_config_fd << "  throttle_min: " << std::to_string(calibration_data.throttle_min) << std::endl;
        output_config_fd << "  throttle_max: " << std::to_string(calibration_data.throttle_max) << std::endl;
        output_config_fd.close();
    }
}  // namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl

using namespace fast::rf_ros::UserInterfaceSystem::RemoteControlSubsystem::TeleopControl;

int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeJoystickCalibration");
    auto node = std::make_unique<JoystickCalibrationNode>();
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
    std::thread thread(&JoystickCalibrationNode::thread_loop, node.get());
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
