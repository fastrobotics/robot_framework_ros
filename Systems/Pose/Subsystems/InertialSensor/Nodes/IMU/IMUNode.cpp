#include "IMUNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem {

    IMUNode::IMUNode() {}
    IMUNode::~IMUNode() {}
    bool IMUNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Base Node!");
            return false;
        }
        status = process.init(fast::rf::PoseSystem::InertialSensorSubsystem::IIMUDriver::IMUDevice::RAZOR9DOF_IMU);
        // status = process.init(fast::rf::PoseSystem::InertialSensorSubsystem::IIMUDriver::IMUDevice::SYDTM151_IMU);
        //  status = process.init(fast::rf::PoseSystem::InertialSensorSubsystem::IIMUDriver::IMUDevice::MOCK_IMU);
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Process!");
            return false;
        }
        imu_pub = n->advertise<sensor_msgs::Imu>("/imu", 1);
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }

    bool IMUNode::start() { return BaseNode::base_start(); }
    bool IMUNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool IMUNode::run_loop2() {
        auto imu_data = fast::rf_ros::utils::TranslateUtility::convert(process.get_imu_data());
        imu_data.header.frame_id = "base_link";
        imu_pub.publish(imu_data);
        return true;
    }
    bool IMUNode::run_loop3() { return true; }
    bool IMUNode::run_100hz() { return true; }
    bool IMUNode::run_10hz() {
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }
    bool IMUNode::run_1hz() {
        auto diagnostics = process.get_diagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool IMUNode::run_01hz() {
        fast::rf::Logger::log_info(process.pretty());
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool IMUNode::run_001hz() { return true; }

    void IMUNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing IMUNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeIMU");
    IMUNode* node = new IMUNode();
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
    std::thread thread(&IMUNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}