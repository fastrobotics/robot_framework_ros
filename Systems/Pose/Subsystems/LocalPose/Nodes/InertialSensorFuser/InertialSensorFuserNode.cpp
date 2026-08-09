#include "InertialSensorFuserNode.hpp"

#include <BasicInertialSensorFuserProcess/BasicInertialSensorFuserProcess.hpp>
#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem {

    InertialSensorFuserNode::InertialSensorFuserNode() {}
    InertialSensorFuserNode::~InertialSensorFuserNode() {}
    void InertialSensorFuserNode::IMU_Callback(const sensor_msgs::Imu::ConstPtr& t_msg) {
        sensor_msgs::Imu msg = *t_msg;
        process->new_imu_data(0, fast::rf_ros::utils::TranslateUtility::convert(msg));
        fast::rf::messages::SensorMsgs::ImuMsg data;
        if (process->get_machine_inertial_data(data) == true) {
            auto machine_inertial_data = fast::rf_ros::utils::TranslateUtility::convert(data);
            machine_inertial_data.header.frame_id = msg.header.frame_id;
            machine_inertial_pub.publish(machine_inertial_data);
        }
    }
    bool InertialSensorFuserNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Base Node!");
            return false;
        }
        process = new fast::rf::PoseSystem::LocalPoseSubsystem::BasicInertialSensorFuserProcess();
        status = process->init(1);  // Basic Inertial Sensor Fuser requires 1 and only 1 IMU
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Process!");
            return false;
        }
        std::string topic_imu1_input;  // Make this support multiple IMU's during AB#1814
        std::string param_imu1_input = get_nodename() + "/topic_imu1_input";
        if (n->getParam(param_imu1_input, topic_imu1_input) == false) {
            fast::rf::Logger::log_error("Parameter topic_imu1_input Not Defined!  Exiting.");
            return false;
        }

        imu_sub = n->subscribe<sensor_msgs::Imu>(get_robotnamespace() + topic_imu1_input, 10,
                                                 &InertialSensorFuserNode::IMU_Callback, this);

        std::string topic_machine_inertial_output;
        if (n->getParam(get_nodename() + "/topic_machine_inertial_output", topic_machine_inertial_output) == false) {
            fast::rf::Logger::log_error("Parameter topic_machine_inertial_output Not Defined!  Exiting.");
            return false;
        }
        machine_inertial_pub = n->advertise<sensor_msgs::Imu>(get_robotnamespace() + topic_machine_inertial_output, 1);

        set_ready_to_arm(process->get_ready_to_arm());
        return true;
    }

    bool InertialSensorFuserNode::start() { return BaseNode::base_start(); }
    bool InertialSensorFuserNode::run_loop1() {
        process->update(ros::Time::now().toSec());

        return true;
    }
    bool InertialSensorFuserNode::run_loop2() { return true; }
    bool InertialSensorFuserNode::run_loop3() { return true; }
    bool InertialSensorFuserNode::run_100hz() { return true; }
    bool InertialSensorFuserNode::run_10hz() {
        set_ready_to_arm(process->get_ready_to_arm());
        return true;
    }
    bool InertialSensorFuserNode::run_1hz() {
        auto diagnostics = process->get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool InertialSensorFuserNode::run_01hz() {
        fast::rf::Logger::log_info(process->pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool InertialSensorFuserNode::run_001hz() { return true; }

    void InertialSensorFuserNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing InertialSensorFuserNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeInertialSensorFuser");
    InertialSensorFuserNode* node = new InertialSensorFuserNode();
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
    std::thread thread(&InertialSensorFuserNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}