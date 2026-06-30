#include <ros/ros.h> // Core ROS header
#include <eros/BaseNode.h>
#include <Vector3DMsg.hpp>
#include <TankDriveExecutorProcess/TankDriveExecutorProcess.hpp>
using namespace fast::rf::NavigationSystem::NavigationExecutorSubsystem;
int main(int argc, char** argv) {
    // Initialize the ROS node with a unique name
    ros::init(argc, argv, "hello_ros_node");
    TankDriveExecutorProcess process;

    // Create a NodeHandle to manage communications
    ros::NodeHandle nh;

    // Set a loop execution rate (1 Hz = once per second)
    ros::Rate loop_rate(1);

    // Keep running until ROS shuts down (e.g., Ctrl+C)
    while (ros::ok()) {
        printf("ROS message\n");
        ROS_INFO("Hello from your new C++ ROS node!"); // Log to console
        bool state = process.update(1.0,0.1);
        printf("state: %d\n",state);
        ros::spinOnce(); // Handle any incoming callbacks
        loop_rate.sleep(); // Pause to maintain the 1 Hz rate
    }

    return 0;
}