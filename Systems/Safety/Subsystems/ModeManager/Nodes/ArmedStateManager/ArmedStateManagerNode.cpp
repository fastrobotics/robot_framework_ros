#include "ArmedStateManagerNode.hpp"

#include <robot_framework_ros/arm_command.h>

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem {

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
            fast::rf::Logger::log_error("Unable to process Ready To Arm Msg");
        }
    }
    bool ArmedStateManagerNode::init() {
        disable_ready_to_arm_publish();  // Don't publish a Ready to Arm Topic
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

        /**
         * @todo Configure this during AB#1767

         *
         */
        std::string arm_command_topic = get_robotnamespace() + "/arm_command";
        arm_command_pub = n->advertise<robot_framework_ros::arm_command>(arm_command_topic, 1);

        // Read Ready To Arm Topic Parameters and Subscribe
        uint8_t counter = 0;
        bool found = true;
        while (found) {
            char param_topic[512];
            sprintf(param_topic, "%s/%03d_ReadyToArm_Topic", get_nodename().c_str(), counter);
            std::string new_ready_to_arm_topic;
            if (n->getParam(param_topic, new_ready_to_arm_topic) == true) {
                ros::Subscriber sub = n->subscribe<robot_framework_ros::ready_to_arm>(
                    new_ready_to_arm_topic, 10, &ArmedStateManagerNode::ready_to_arm_Callback, this);
                ready_to_arm_subs.push_back(sub);
            } else {
                found = false;
            }
            counter++;
        }
        if (ready_to_arm_subs.size() == 0) {
            fast::rf::Logger::log_error("Need at least 1 Ready To Arm Topic.  Exiting.");
            return false;
        }
        std::string armstate_change_topic = get_robotnamespace() + "/arm_state_change";
        armstate_change_srv =
            n->advertiseService(armstate_change_topic, &ArmedStateManagerNode::arm_statechange_service, this);
        return true;
    }

    bool ArmedStateManagerNode::start() { return BaseNode::base_start(); }
    bool ArmedStateManagerNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool ArmedStateManagerNode::run_loop2() { return true; }
    bool ArmedStateManagerNode::run_loop3() { return true; }
    bool ArmedStateManagerNode::run_100hz() { return true; }
    bool ArmedStateManagerNode::run_10hz() {
        arm_command_pub.publish(fast::rf_ros::utils::TranslateUtility::convert(process.get_ArmCommandMsg()));
        return true;
    }
    bool ArmedStateManagerNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool ArmedStateManagerNode::run_01hz() {
        fast::rf::Logger::log_info(process.pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool ArmedStateManagerNode::run_001hz() { return true; }

    void ArmedStateManagerNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing ArmedStateManagerNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::SafetySystem::ModeManagerSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeArmedStateManager");
    ArmedStateManagerNode* node = new ArmedStateManagerNode();
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
    std::thread thread(&ArmedStateManagerNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}