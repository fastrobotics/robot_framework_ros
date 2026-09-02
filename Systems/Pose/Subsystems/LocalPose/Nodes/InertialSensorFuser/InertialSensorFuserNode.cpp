/**
 * @compare_tag Node-Source  v0.1
 *
 */
#include "InertialSensorFuserNode.hpp"

#include <BasicInertialSensorFuserProcess/BasicInertialSensorFuserProcess.hpp>
#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;
namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::InertialSensorFuser {

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
            fast::rf::Logger::logError("Unable to initialize Base Node!");
            return false;
        }

        process = new fast::rf::PoseSystem::LocalPoseSubsystem::InertialSensorFuser::BasicInertialSensorFuserProcess();
        status = process->init(1);  // Basic Inertial Sensor Fuser requires 1 and only 1 IMU
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Process!");
            return false;
        }
        status = load_config();
        if (status == false) {
            fast::rf::Logger::logError("Unable to load config!");
            return false;
        }

        std::string topic_imu1_input;  // Make this support multiple IMU's during AB#1814
        std::string param_imu1_input = get_nodename() + "/topic_imu1_input";
        if (n->getParam(param_imu1_input, topic_imu1_input) == false) {
            fast::rf::Logger::logError("Parameter topic_imu1_input Not Defined!  Exiting.");
            return false;
        }

        imu_sub = n->subscribe<sensor_msgs::Imu>(get_robotnamespace() + topic_imu1_input, 10,
                                                 &InertialSensorFuserNode::IMU_Callback, this);

        std::string topic_machine_inertial_output;
        if (n->getParam(get_nodename() + "/topic_machine_inertial_output", topic_machine_inertial_output) == false) {
            fast::rf::Logger::logError("Parameter topic_machine_inertial_output Not Defined!  Exiting.");
            return false;
        }
        machine_inertial_pub = n->advertise<sensor_msgs::Imu>(get_robotnamespace() + topic_machine_inertial_output, 1);

        status = initBaseNodeDiagnostics(process->getSystemId(), process->getSubSystemId(), process->getProcessId());
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node Diagnostics!");
            return false;
        }
        set_ready_to_arm(process->get_ready_to_arm());
        return true;
    }
    bool InertialSensorFuserNode::load_config() {
        std::string system_id_str = fast::rf::PoseSystem::toString(fast::rf::PoseSystem::Id{});
        std::string subsystem_id_str =
            fast::rf::PoseSystem::LocalPoseSubsystem::toString(fast::rf::PoseSystem::LocalPoseSubsystem::Id{});
        std::string process_id_str = fast::rf::PoseSystem::LocalPoseSubsystem::InertialSensorFuser::toString(
            fast::rf::PoseSystem::LocalPoseSubsystem::InertialSensorFuser::Id{});
        std::string config_path = get_config_path(system_id_str, subsystem_id_str, process_id_str);

        fast::rf::Logger::logInfo("Loading Config from:" + config_path);

        // Get user space config during AB#1852
        return true;
    }
    bool InertialSensorFuserNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
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
        auto diagnostics = process->getDiagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool InertialSensorFuserNode::run_01hz() {
        auto baseNodeDiagnostics = getBaseNodeDiagnostics();
        for (auto it : baseNodeDiagnostics) {
            process->updateDiagnostic(it.second.diagnosticType, it.second.level, it.second.diagnosticMessage,
                                      it.second.description);
        }
        fast::rf::Logger::logInfo(process->pretty());
        fast::rf::Logger::logInfo(pretty());
        return true;
    }
    bool InertialSensorFuserNode::run_001hz() { return true; }

    void InertialSensorFuserNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void InertialSensorFuserNode::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::InertialSensorFuser

using namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::InertialSensorFuser;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeInertialSensorFuser");
    auto node = std::make_unique<InertialSensorFuserNode>();
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
    std::thread thread(&InertialSensorFuserNode::thread_loop, node.get());
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
