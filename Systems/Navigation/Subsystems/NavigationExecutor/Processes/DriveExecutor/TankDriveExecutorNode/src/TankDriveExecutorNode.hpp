/*! \file TankDriveNode.h
 */
#pragma once
// C System Files
// C++ System Files
// ROS Base Functionality
// ROS Messages
// Project
#include <eros/BaseNode.h>
#include <eros/BaseNodeProcess.h>
namespace fast::rf::NavigationSystem::NavigationExecutorSubsystem {
/*! \class TankDriveNode TankDriveNode.h "TankDriveNode.h"
 *  \brief */
class TankDriveNode : public eros::BaseNode
{
   public:
    TankDriveNode();
    ~TankDriveNode();
    // Constants
    /*! \brief The base name of the Node.*/
    const std::string BASE_NODE_NAME = "tankdrive_node";

    /*! \brief The Major Release Version of the Node.*/
    const uint16_t MAJOR_RELEASE_VERSION = 0;

    /*! \brief The Minor Release Version of the Node.*/
    const uint16_t MINOR_RELEASE_VERSION = 1;

    /*! \brief The Build Number of the Node.*/
    const uint16_t BUILD_NUMBER = 0;

    /*! \brief A Description of the Firmware.*/
    const std::string FIRMWARE_DESCRIPTION = "Latest Rev: 29-June-2026";

    /*! \brief What System this Node falls under.*/
    const eros::System::MainSystem DIAGNOSTIC_SYSTEM = eros::System::MainSystem::ROVER;

    /*! \brief What Subsystem this Node falls under.*/
    const eros::System::SubSystem DIAGNOSTIC_SUBSYSTEM = eros::System::SubSystem::ENTIRE_SYSTEM;

    /*! \brief What Component this Node falls under.*/
    const eros::System::Component DIAGNOSTIC_COMPONENT = eros::System::Component::NAVIGATION;

    // Enums

    // Structs

    // Initialization Functions
    bool start();
    eros::eros_diagnostic::Diagnostic finish_initialization();

    // Update Functions
    bool run_loop1();
    bool run_loop2();
    bool run_loop3();
    bool run_001hz();
    bool run_01hz();
    bool run_01hz_noisy();
    bool run_1hz();
    bool run_10hz();
    void thread_loop();

    // Attribute Functions

    // Utility Functions

    // Support Functions

    // Message Functions
    bool changenodestate_service(eros::srv_change_nodestate::Request& req,
                                 eros::srv_change_nodestate::Response& res);
    void system_commandAction_Callback(const eros::system_commandGoalConstPtr& goal);
    void command_Callback(const eros::command::ConstPtr& t_msg);
    void cmd_vel_Callback(const geometry_msgs::Twist::ConstPtr& t_msg);

    // Destructors
    void cleanup();

    // Printing Functions
    std::string pretty() override;

   private:
    bool load_drive_parameters(std::string drive_name);
    eros::eros_diagnostic::Diagnostic read_launchparameters();
    actionlib::SimpleActionServer<eros::system_commandAction> system_command_action_server;
    ros::Subscriber cmd_vel_sub;

    ros::Publisher leftdrive_pub;
    ros::Publisher rightdrive_pub;
};

}  // namespace fast::rf::NavigationSystem::NavigationExecutorSubsystem {