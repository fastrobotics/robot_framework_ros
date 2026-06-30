#include "TankDriveExecutorNode.hpp"
bool kill_node = false;
namespace fast::rf::NavigationSystem::NavigationExecutorSubsystem {
TankDriveNode::TankDriveNode()
    : system_command_action_server(
          *n.get(),
          read_robotnamespace() + "SystemCommandAction",
          boost::bind(&TankDriveNode::system_commandAction_Callback, this, _1),
          false) {
    system_command_action_server.start();
}
TankDriveNode::~TankDriveNode() {
}
void TankDriveNode::system_commandAction_Callback(const eros::system_commandGoalConstPtr &goal) {
    /*
    eros::eros_diagnostic::Diagnostic diag = process->get_root_diagnostic();
    eros::system_commandResult system_commandResult_;
    system_command_action_server.setAborted(system_commandResult_);
    diag = process->update_diagnostic(
        eros::eros_diagnostic::DiagnosticType::COMMUNICATIONS,
        eros::Level::Type::WARN,
        eros::eros_diagnostic::Message::DROPPING_PACKETS,
        "Received unsupported CommandAction: " +
            eros::Command::CommandString((eros::Command::Type)goal->Command));
    logger->log_diagnostic(diag);
    */
}
void TankDriveNode::cmd_vel_Callback(const geometry_msgs::Twist::ConstPtr &t_msg) {
    geometry_msgs::Twist cmd_vel = eros::eros_utility::ConvertUtility::convert_fromptr(t_msg);
    //process->new_cmd_vel(cmd_vel);
}
void TankDriveNode::command_Callback(const eros::command::ConstPtr &t_msg) {
    eros::command cmd = eros::eros_utility::ConvertUtility::convert_fromptr(t_msg);
    /*
    auto diag_list = process->new_commandmsg(cmd);
    for (auto diag : diag_list) { logger->log_diagnostic(diag); }
    */
}
bool TankDriveNode::changenodestate_service(eros::srv_change_nodestate::Request &req,
                                            eros::srv_change_nodestate::Response &res) {
    eros::Node::State req_state = eros::Node::NodeState(req.RequestedNodeState);
    //process->request_statechange(req_state);
   // res.NodeState = eros::Node::NodeStateString(process->get_nodestate());
    return true;
}
bool TankDriveNode::start() {
    initialize_diagnostic(DIAGNOSTIC_SYSTEM, DIAGNOSTIC_SUBSYSTEM, DIAGNOSTIC_COMPONENT);
    bool status = false;
    //process = new TankDriveNodeProcess();
    set_basenodename(BASE_NODE_NAME);
    initialize_firmware(
        MAJOR_RELEASE_VERSION, MINOR_RELEASE_VERSION, BUILD_NUMBER, FIRMWARE_DESCRIPTION);
    diagnostic = preinitialize_basenode();
    if (diagnostic.level > eros::Level::Type::WARN) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return false;
        // LCOV_EXCL_STOP
    }
    diagnostic = read_launchparameters();
    if (diagnostic.level > eros::Level::Type::WARN) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return false;
        // LCOV_EXCL_STOP
    }
    /*

    process->initialize(get_basenodename(),
                        get_nodename(),
                        get_hostname(),
                        DIAGNOSTIC_SYSTEM,
                        DIAGNOSTIC_SUBSYSTEM,
                        DIAGNOSTIC_COMPONENT,
                        logger);
                        */
    std::vector<eros::eros_diagnostic::DiagnosticType> diagnostic_types;
    diagnostic_types.push_back(eros::eros_diagnostic::DiagnosticType::SOFTWARE);
    diagnostic_types.push_back(eros::eros_diagnostic::DiagnosticType::DATA_STORAGE);
    diagnostic_types.push_back(eros::eros_diagnostic::DiagnosticType::SYSTEM_RESOURCE);
    diagnostic_types.push_back(eros::eros_diagnostic::DiagnosticType::COMMUNICATIONS);
    diagnostic_types.push_back(eros::eros_diagnostic::DiagnosticType::REMOTE_CONTROL);

    //process->enable_diagnostics(diagnostic_types);
    //process->finish_initialization();
    diagnostic = finish_initialization();
    if (diagnostic.level > eros::Level::Type::WARN) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return false;
        // LCOV_EXCL_STOP
    }
    if (diagnostic.level < eros::Level::Type::WARN) {
        diagnostic.type = eros::eros_diagnostic::DiagnosticType::SOFTWARE;
        diagnostic.level = eros::Level::Type::INFO;
        diagnostic.message = eros::eros_diagnostic::Message::NOERROR;
        diagnostic.description = "Node Configured.  Initializing.";
        get_logger()->log_diagnostic(diagnostic);
    }
    /*
    if (process->request_statechange(eros::Node::State::RUNNING, true) == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        logger->log_warn("Unable to Change State to: " +
                         eros::Node::NodeStateString(eros::Node::State::RUNNING));
        // LCOV_EXCL_STOP
    }
    logger->log_notice("Node State: " + eros::Node::NodeStateString(process->get_nodestate()));
    */
    status = true;
    return status;
}
eros::eros_diagnostic::Diagnostic TankDriveNode::read_launchparameters() {
    eros::eros_diagnostic::Diagnostic diag = diagnostic;
    /*
    bool status = load_drive_parameters("left_drive");
    if (status == false) {
        diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::DATA_STORAGE,
                                          eros::Level::Type::ERROR,
                                          eros::eros_diagnostic::Message::INITIALIZING_ERROR,
                                          "Unable to load config for left_drive");
        logger->log_diagnostic(diag);
        return diag;
    }
    status = load_drive_parameters("right_drive");
    if (status == false) {
        diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::DATA_STORAGE,
                                          eros::Level::Type::ERROR,
                                          eros::eros_diagnostic::Message::INITIALIZING_ERROR,
                                          "Unable to load config for right_drive");
        logger->log_diagnostic(diag);
        return diag;
    }

    command_sub = n->subscribe<eros::command>(
        get_robotnamespace() + "SystemCommand", 10, &TankDriveNode::command_Callback, this);
    cmd_vel_sub = n->subscribe<geometry_msgs::Twist>(
        get_robotnamespace() + "cmd_vel_perc", 10, &TankDriveNode::cmd_vel_Callback, this);
    leftdrive_pub = n->advertise<std_msgs::UInt16>(robot_namespace + "/left_drive", 20);
    rightdrive_pub = n->advertise<std_msgs::UInt16>(robot_namespace + "/right_drive", 20);

    get_logger()->log_notice("Configuration Files Loaded.");
    */
    return diag;
}
eros::eros_diagnostic::Diagnostic TankDriveNode::finish_initialization() {
    eros::eros_diagnostic::Diagnostic diag = diagnostic;
    std::string srv_nodestate_topic = "srv_nodestate_change";
    nodestate_srv =
        n->advertiseService(srv_nodestate_topic, &TankDriveNode::changenodestate_service, this);
        /*
    diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::COMMUNICATIONS,
                                      eros::Level::Type::INFO,
                                      eros::eros_diagnostic::Message::NOERROR,
                                      "Comms Ready.");
    diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::SOFTWARE,
                                      eros::Level::Type::INFO,
                                      eros::eros_diagnostic::Message::NOERROR,
                                      "Running");
    diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::REMOTE_CONTROL,
                                      eros::Level::Type::INFO,
                                      eros::eros_diagnostic::Message::NOERROR,
                                      "Remote Control Initialized.");
    diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::DATA_STORAGE,
                                      eros::Level::Type::INFO,
                                      eros::eros_diagnostic::Message::NOERROR,
                                      "All Configuration Files Loaded.");
                                      */
    return diag;
}
bool TankDriveNode::run_loop1() {
    return true;
}
bool TankDriveNode::run_loop2() {
    return true;
}
bool TankDriveNode::run_loop3() {
    return true;
}
bool TankDriveNode::run_001hz() {
    return true;
}
bool TankDriveNode::run_01hz() {
    return true;
}
bool TankDriveNode::run_01hz_noisy() {
    eros::eros_diagnostic::Diagnostic diag = diagnostic;
    logger->log_debug(pretty());
    return true;
}
std::string TankDriveNode::pretty() {
    //std::string str = process->pretty();
    std::string str = "";
    return str;
}
bool TankDriveNode::run_1hz() {
    /*
    std::vector<eros::eros_diagnostic::Diagnostic> latest_diagnostics =
        process->get_latest_diagnostics();
    for (std::size_t i = 0; i < latest_diagnostics.size(); ++i) {
        logger->log_diagnostic(latest_diagnostics.at(i));
        diagnostic_pub.publish(
            eros::eros_diagnostic::DiagnosticUtility::convert(latest_diagnostics.at(i)));
    }
    eros::eros_diagnostic::Diagnostic diag = process->get_root_diagnostic();
    if (process->get_nodestate() == eros::Node::State::RESET) {
        base_reset();
        process->reset();
        logger->log_notice("Node has Reset");
        if (process->request_statechange(eros::Node::State::RUNNING) == false) {
            // No practical way to unit test
            // LCOV_EXCL_START
            diag = process->update_diagnostic(eros::eros_diagnostic::DiagnosticType::SOFTWARE,
                                              eros::Level::Type::ERROR,
                                              eros::eros_diagnostic::Message::DEVICE_NOT_AVAILABLE,
                                              "Not able to Change Node State to Running.");
            logger->log_diagnostic(diag);
            // LCOV_EXCL_STOP
        }
    }
        */
    return true;
}
bool TankDriveNode::run_10hz() {
    /*
    process->update(0.1, ros::Time::now().toSec());
    process->update_armedstate(eros::eros_utility::ConvertUtility::convert(armed_state));
    auto output = process->get_drive_command();
    leftdrive_pub.publish(output.left_drive);
    rightdrive_pub.publish(output.right_drive);
    update_diagnostics(process->get_diagnostics());
    update_ready_to_arm(process->get_ready_to_arm());
    */
    return true;
}
void TankDriveNode::thread_loop() {
    while (kill_node == false) { ros::Duration(1.0).sleep(); }
}
void TankDriveNode::cleanup() {
    //process->request_statechange(eros::Node::State::FINISHED);
    //process->cleanup();
   // delete process;
    base_cleanup();
}
bool TankDriveNode::load_drive_parameters(std::string drive_name) {
    /*
    TankDriveNodeProcess::DriveChannelConfig config;
    int min_value;
    if (!n->getParam(drive_name + "/minimum", min_value)) {
        return false;
    }
    else {
        config.min_value = (uint16_t)min_value;
    }
    int neutral_value;
    if (!n->getParam(drive_name + "/neutral", neutral_value)) {
        return false;
    }
    else {
        config.neutral_value = (uint16_t)neutral_value;
    }
    int max_value;
    if (!n->getParam(drive_name + "/maximum", max_value)) {
        return false;
    }
    else {
        config.max_value = (uint16_t)max_value;
    }
    if (drive_name == "left_drive") {
        //process->set_left_drive_config(config);
    }
    else if (drive_name == "right_drive") {
        //process->set_right_drive_config(config);
    }
    else {
        return false;
    }
        */
    return true;
}
}  // namespace fast::rf::NavigationSystem::NavigationExecutorSubsystem
// No practical way to unit test
// LCOV_EXCL_START
using namespace fast::rf::NavigationSystem::NavigationExecutorSubsystem;
void signalinterrupt_handler(int sig) {
    printf("Killing TankDriveNode with Signal: %d\n", sig);
    kill_node = true;
    exit(0);
}
// LCOV_EXCL_STOP
int main(int argc, char **argv) {
    signal(SIGINT, signalinterrupt_handler);
    signal(SIGTERM, signalinterrupt_handler);
    ros::init(argc, argv, "tankdrive_node");
    TankDriveNode *node = new TankDriveNode();
    bool status = node->start();
    if (status == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    std::thread thread(&TankDriveNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = true;//node->update(node->get_process()->get_nodestate());
    }
    node->cleanup();
    thread.detach();
    delete node;
    return 0;
}