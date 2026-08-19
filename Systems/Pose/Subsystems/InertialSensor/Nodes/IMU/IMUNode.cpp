#include "IMUNode.hpp"

#include <geometry_msgs/AccelStamped.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>

#include <Covariance3DMsg.hpp>
#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/CoreUtility.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem::IMU {

    IMUNode::IMUNode() {}
    IMUNode::~IMUNode() {}
    bool IMUNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Base Node!");
            return false;
        }
        std::string imu_type;
        if (n->getParam("info/type", imu_type) == false) {
            fast::rf::Logger::log_error("Can't find parameter: info/imu_type");
            return false;
        }
        std::string imu_device_name;
        if (n->getParam("info/device_name", imu_device_name) == false) {
            fast::rf::Logger::log_error("Can't find parameter: info/device_name");
            return false;
        }
        // Load Covariance Matrix's
        fast::rf::messages::StandardMsgs::Covariance3DMsg orientation_covariance_matrix;
        fast::rf::messages::StandardMsgs::Covariance3DMsg gyro_covariance_matrix;
        fast::rf::messages::StandardMsgs::Covariance3DMsg linear_acc_covariance_matrix;
        fast::rf::messages::StandardMsgs::Covariance3DMsg magnetometer_covariance_matrix;
        {  // Orientation Covariance Matrix
            std::vector<double> values;
            if (n->getParam("orientation_covariance_matrix", values)) {
                if (values.size() == 9) {
                    auto array = fast::rf_ros::utils::CoreUtility::convert_boostarray_9(values);
                    orientation_covariance_matrix = fast::rf_ros::utils::TranslateUtility::convert_covariance3D(array);
                } else {
                    fast::rf::Logger::log_error("Orientation Covariance Matrix not defined properly!");
                    return false;
                }
            } else {
                fast::rf::Logger::log_warn("Unable to load orientation_covariance_matrix.  Using Default.");
            }
        }
        {  // Gyro Covariance Matrix
            std::vector<double> values;
            if (n->getParam("gyro_covariance_matrix", values)) {
                if (values.size() == 9) {
                    auto array = fast::rf_ros::utils::CoreUtility::convert_boostarray_9(values);
                    gyro_covariance_matrix = fast::rf_ros::utils::TranslateUtility::convert_covariance3D(array);
                } else {
                    fast::rf::Logger::log_error("Gyro Covariance Matrix not defined properly!");
                    return false;
                }
            } else {
                fast::rf::Logger::log_warn("Unable to load gyro_covariance_matrix.  Using Default.");
            }
        }
        {  // Linear Acc Covariance Matrix
            std::vector<double> values;
            if (n->getParam("linear_accel_covariance_matrix", values)) {
                if (values.size() == 9) {
                    auto array = fast::rf_ros::utils::CoreUtility::convert_boostarray_9(values);
                    linear_acc_covariance_matrix = fast::rf_ros::utils::TranslateUtility::convert_covariance3D(array);
                } else {
                    fast::rf::Logger::log_error("Linear Acc Covariance Matrix not defined properly!");
                    return false;
                }
            } else {
                fast::rf::Logger::log_warn("Unable to load linear_accel_covariance_matrix.  Using Default.");
            }
        }
        {  // Magnetometer Covariance Matrix
            std::vector<double> values;
            if (n->getParam("magnetic_covariance_matrix", values)) {
                if (values.size() == 9) {
                    auto array = fast::rf_ros::utils::CoreUtility::convert_boostarray_9(values);
                    magnetometer_covariance_matrix = fast::rf_ros::utils::TranslateUtility::convert_covariance3D(array);
                } else {
                    fast::rf::Logger::log_error("Magnetometer Covariance Matrix not defined properly!");
                    return false;
                }
            } else {
                fast::rf::Logger::log_warn("Unable to load magnetic_covariance_matrix.  Using Default.");
            }
        }
        fast::rf::PoseSystem::InertialSensorSubsystem::IMU::IIMUProcess::IMUConfig imu_config;
        imu_config.imu_type = fast::rf::PoseSystem::InertialSensorSubsystem::IMU::IIMUDriver::convert_name(imu_type);
        imu_config.imu_device_name = imu_device_name;
        imu_config.linear_accelerometer_covariance = linear_acc_covariance_matrix;
        imu_config.gyro_covariance = gyro_covariance_matrix;
        imu_config.magnetometer_covariance = magnetometer_covariance_matrix;
        imu_config.orientation_covariance = orientation_covariance_matrix;
        status = process.init(imu_config);
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Process with IMU: " + imu_type);
            return false;
        }
        if (n->getParam("imu_sensor_frame", imu_sensor_frame) == false) {
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

        std::string imu_acc_topic;
        if (n->getParam("topic_imu_acc", imu_acc_topic) == false) {
            fast::rf::Logger::log_error("Can't find parameter: topic_imu_acc");
            return false;
        }
        imu_accel_pub = n->advertise<geometry_msgs::AccelStamped>(get_robotnamespace() + imu_acc_topic, 1);
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
                imu_data.header.frame_id = imu_sensor_frame;
                imu_pub.publish(imu_data);

                geometry_msgs::AccelStamped accel;
                accel.header = imu_data.header;
                accel.accel.linear = imu_data.linear_acceleration;
                imu_accel_pub.publish(accel);
            }
        }
        {
            fast::rf::messages::SensorMsgs::MagneticFieldMsg data;
            if (process.get_magnetic_data(data)) {
                auto magnetic_data = fast::rf_ros::utils::TranslateUtility::convert(data);
                magnetic_data.header.frame_id = imu_sensor_frame;
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
}  // namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem::IMU

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing IMUNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::PoseSystem::InertialSensorSubsystem::IMU;
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