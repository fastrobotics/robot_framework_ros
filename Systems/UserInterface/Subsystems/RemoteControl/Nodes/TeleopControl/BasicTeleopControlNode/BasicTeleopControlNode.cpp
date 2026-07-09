#include "BasicTeleopControlNode.hpp"

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
            ROS_ERROR("Unable to initialize Base Node!");
            return false;
        }

        status =
            process.init(fast::rf::UserInterfaceSystem::RemoteControlSubsystem::ControlDevice::THRUSTMASTER_JOYSTICK);
        if (status == false) {
            ROS_ERROR("Unable to initialize Process!");
            return false;
        }
        std::string operation_mode;
        std::string param_op_mode = get_nodename() + "/operation_mode";
        if (n->getParam(param_op_mode, operation_mode) == false) {
            status =
                process.set_operation_mode(fast::rf::UserInterfaceSystem::RemoteControlSubsystem::OperationMode::RUN);
        }
        if (operation_mode == "run") {
            status =
                process.set_operation_mode(fast::rf::UserInterfaceSystem::RemoteControlSubsystem::OperationMode::RUN);
        } else if (operation_mode == "test") {
            status = process.set_operation_mode(
                fast::rf::UserInterfaceSystem::RemoteControlSubsystem::OperationMode::JOY_TEST);
        }
        if (status == false) {
            ROS_ERROR("Unable to set Operation Mode");
            return false;
        }

        joy_sub = n->subscribe<sensor_msgs::Joy>("/joy", 10, &BasicTeleopControlNode::joy_Callback, this);
        twist_pub = n->advertise<geometry_msgs::Twist>(get_robotnamespace() + "/cmd_throttle", 1);
        return true;
    }

    bool BasicTeleopControlNode::start() { return BaseNode::base_start(); }
    bool BasicTeleopControlNode::run_loop1() {
        process.update(ros::Time::now().toSec(), 0.0);

        return true;
    }
    bool BasicTeleopControlNode::run_loop2() {
        auto twist = process.get_twist_output();
        twist_pub.publish(fast::rf_ros::utils::TranslateUtility::convert(twist));
        // bool diagnostic_check_ok = false;
        /*
        auto diagnostics = process.get_diagnostics();
        for (auto diagnostic : diagnostics) {
            if ((diagnostic.diagnosticType == fast::rf::DiagnosticDefinition::DiagnosticType::REMOTE_CONTROL) and
                (diagnostic.diagnosticMessage == fast::rf::DiagnosticDefinition::DiagnosticMessage::NOERROR)) {
                diagnostic_check_ok = true;
            }
        }
        if (diagnostic_check_ok == true) {
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::IDriveExecutorOutput* general_output =
                process.get_output();
            fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveExecutorOutput* output =
                dynamic_cast<fast::rf::NavigationSystem::NavigationExecutorSubsystem::TankDriveExecutorOutput*>(
                    general_output);
            std_msgs::Float64 left_drive;
            left_drive.data = output->left_drive;
            std_msgs::Float64 right_drive;
            right_drive.data = output->right_drive;
            left_drive_pub.publish(left_drive);
            right_drive_pub.publish(right_drive);
        } else {
            ROS_WARN("Diagnostic Check Failed!  Disabling Outputs.");
        }
            */
        return true;
    }
    bool BasicTeleopControlNode::run_loop3() { return true; }
    bool BasicTeleopControlNode::run_100hz() { return true; }
    bool BasicTeleopControlNode::run_10hz() { return true; }
    bool BasicTeleopControlNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool BasicTeleopControlNode::run_01hz() {
        ROS_WARN("%s", process.pretty().c_str());
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
    ROS_WARN("Killing BasicTeleopControlNode with Signal: %d\n", sig);
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