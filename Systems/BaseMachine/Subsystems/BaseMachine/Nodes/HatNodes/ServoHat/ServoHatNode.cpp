#include "ServoHatNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <boost/bind/bind.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem {

    ServoHatNode::ServoHatNode() {}
    ServoHatNode::~ServoHatNode() {}
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

    bool ServoHatNode::start() { return BaseNode::base_start(); }
    bool ServoHatNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool ServoHatNode::run_loop2() {
        /**
         * @todo Implement this during AB#1779
         *
         */
        bool robot_armed = true;
        if ((process.get_ready_to_arm().ready_to_arm == true) && (robot_armed == true)) {
        }
        return true;
    }
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
        fast::rf::Logger::log_debug(process.pretty());
        fast::rf::Logger::log_debug(pretty());
        return true;
    }
    bool ServoHatNode::run_001hz() { return true; }

    void ServoHatNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing ServoHatNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::BaseMachineSystem::BaseMachineSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeServoHat");
    ServoHatNode* node = new ServoHatNode();
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
    std::thread thread(&ServoHatNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}