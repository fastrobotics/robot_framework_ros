/**
 * @compare_tag Node-Source  v0.1
 *
 */
#include "LocalPoseFuserNode.hpp"

#include <BasicLocalPoseFuserProcess/BasicLocalPoseFuserProcess.hpp>
#include <Infrastructure/Logger.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;
namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::LocalPoseFuser {

    LocalPoseFuserNode::LocalPoseFuserNode() {}
    LocalPoseFuserNode::~LocalPoseFuserNode() {}
    void LocalPoseFuserNode::machine_inertial_Callback([[maybe_unused]] const sensor_msgs::Imu::ConstPtr& t_msg) {
        sensor_msgs::Imu msg = *t_msg;

        process->new_machine_inertial_data(fast::rf_ros::utils::TranslateUtility::convert(msg));
        fast::rf::messages::GeometryMsgs::OdomMsg local_pose_data;
        fast::rf::messages::GeometryMsgs::AccelWithCovarianceMsg local_pose_angular_accel_data;
        if (process->get_local_pose(local_pose_data, local_pose_angular_accel_data) == true) {
            auto local_pose = fast::rf_ros::utils::TranslateUtility::convert(local_pose_data);
            local_pose.header.frame_id = msg.header.frame_id;
            local_pose_pub.publish(local_pose);
            auto local_pose_angular_accel =
                fast::rf_ros::utils::TranslateUtility::convert(local_pose_angular_accel_data);
            local_pose_angular_accel.header.frame_id = msg.header.frame_id;
            local_pose_angular_accel_pub.publish(local_pose_angular_accel);
        }
    }
    bool LocalPoseFuserNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node!");
            return false;
        }

        process = new fast::rf::PoseSystem::LocalPoseSubsystem::LocalPoseFuser::BasicLocalPoseFuserProcess();
        status = process->init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Process!");
            return false;
        }

        status = load_config();
        if (status == false) {
            fast::rf::Logger::logError("Unable to load config!");
            return false;
        }

        std::string topic_machine_inertial_input;
        if (n->getParam(get_nodename() + "/topic_machine_inertial_input", topic_machine_inertial_input) == false) {
            fast::rf::Logger::logError("Parameter topic_machine_inertial_input Not Defined!  Exiting.");
            return false;
        }
        machine_inertial_sub = n->subscribe<sensor_msgs::Imu>(get_robotnamespace() + topic_machine_inertial_input, 10,
                                                              &LocalPoseFuserNode::machine_inertial_Callback, this);

        std::string topic_local_pose_output;
        if (n->getParam(get_nodename() + "/topic_local_pose_output", topic_local_pose_output) == false) {
            fast::rf::Logger::logError("Parameter topic_local_pose_output Not Defined!  Exiting.");
            return false;
        }
        local_pose_pub = n->advertise<nav_msgs::Odometry>(get_robotnamespace() + topic_local_pose_output, 1);

        std::string topic_local_pose_angular_accel_output;
        if (n->getParam(get_nodename() + "/topic_local_pose_angular_accel_output",
                        topic_local_pose_angular_accel_output) == false) {
            fast::rf::Logger::logError("Parameter topic_local_pose_angular_accel_output Not Defined!  Exiting.");
            return false;
        }
        local_pose_angular_accel_pub = n->advertise<geometry_msgs::AccelWithCovarianceStamped>(
            get_robotnamespace() + topic_local_pose_angular_accel_output, 1);

        status = initBaseNodeDiagnostics(process->getSystemId(), process->getSubSystemId(), process->getProcessId());
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node Diagnostics!");
            return false;
        }
        set_ready_to_arm(process->get_ready_to_arm());
        return true;
    }
    bool LocalPoseFuserNode::load_config() {
        // Nothing to configure
        return true;
    }
    bool LocalPoseFuserNode::start() {
        is_node_running = true;
        return BaseNode::base_start();
    }
    bool LocalPoseFuserNode::run_loop1() {
        process->update(ros::Time::now().toSec());

        return true;
    }
    bool LocalPoseFuserNode::run_loop2() { return true; }
    bool LocalPoseFuserNode::run_loop3() { return true; }
    bool LocalPoseFuserNode::run_100hz() { return true; }
    bool LocalPoseFuserNode::run_10hz() {
        set_ready_to_arm(process->get_ready_to_arm());
        return true;
    }
    bool LocalPoseFuserNode::run_1hz() {
        fast::rf::Logger::logDebug(process->pretty());
        auto diagnostics = process->getDiagnostics();
        set_diagnostics(diagnostics);

        return true;
    }
    bool LocalPoseFuserNode::run_01hz() {
        auto baseNodeDiagnostics = getBaseNodeDiagnostics();
        for (auto it : baseNodeDiagnostics) {
            process->updateDiagnostic(it.second.diagnosticType, it.second.level, it.second.diagnosticMessage,
                                      it.second.description);
        }
        fast::rf::Logger::logInfo(process->pretty());
        fast::rf::Logger::logInfo(pretty());
        return true;
    }
    bool LocalPoseFuserNode::run_001hz() { return true; }

    void LocalPoseFuserNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void LocalPoseFuserNode::stop() { is_node_running = false; }
}  // namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::LocalPoseFuser

using namespace fast::rf_ros::PoseSystem::LocalPoseSubsystem::LocalPoseFuser;
int main(int argc, char** argv) {
    ros::init(argc, argv, "nodeLocalPoseFuser");
    auto node = std::make_unique<LocalPoseFuserNode>();
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
    std::thread thread(&LocalPoseFuserNode::thread_loop, node.get());
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
