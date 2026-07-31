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
        std::string imu_type;
        if (n->getParam("imu_type", imu_type) == false) {
            fast::rf::Logger::log_error("Can't find parameter: imu_type");
        }
        status = process.init(fast::rf::PoseSystem::InertialSensorSubsystem::IIMUDriver::convert_name(imu_type));
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Process with IMU: " + imu_type);
            return false;
        }
        if (n->getParam("frame", imu_frame) == false) {
            fast::rf::Logger::log_error("Can't find parameter: frame");
            return false;
        }
        std::string imu_topic;
        if (n->getParam("topic_imu", imu_topic) == false) {
            fast::rf::Logger::log_error("Can't find parameter: topic_imu");
            return false;
        }
        imu_pub = n->advertise<sensor_msgs::Imu>(get_robotnamespace() + imu_topic, 1);

        std::string magnetometer_topic;
        if (n->getParam("topic_magnetometer", magnetometer_topic) == false) {
            fast::rf::Logger::log_error("Can't find parameter: topic_magnetometer");
            return false;
        }
        magnetometer_pub = n->advertise<sensor_msgs::MagneticField>(get_robotnamespace() + magnetometer_topic, 1);
        process.update(ros::Time::now().toSec());  // Kick off the Process
        set_ready_to_arm(process.get_ready_to_arm());
        return true;
    }

    bool IMUNode::start() { return BaseNode::base_start(); }
    bool IMUNode::run_loop1() {
        process.update(ros::Time::now().toSec());

        return true;
    }
    bool IMUNode::run_loop2() {
        {
            fast::rf::messages::SensorMsgs::ImuMsg data;
            if (process.get_imu_data(data)) {
                auto imu_data = fast::rf_ros::utils::TranslateUtility::convert(data);
                imu_data.header.frame_id = imu_frame;
                imu_pub.publish(imu_data);
            }
        }
        {
            fast::rf::messages::SensorMsgs::MagneticFieldMsg data;
            if (process.get_magnetic_data(data)) {
                auto magnetic_data = fast::rf_ros::utils::TranslateUtility::convert(data);
                magnetic_data.header.frame_id = imu_frame;
                magnetometer_pub.publish(magnetic_data);
            }
        }
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