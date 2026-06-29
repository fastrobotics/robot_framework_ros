#include <ros/ros.h> // Core ROS header

#include <Vector3DMsg.hpp>
#include <BaseDriveExecutorProcess.hpp>

int main(int argc, char** argv) {
    // Initialize the ROS node with a unique name
    ros::init(argc, argv, "hello_ros_node");

    // Create a NodeHandle to manage communications
    ros::NodeHandle nh;

    // Set a loop execution rate (1 Hz = once per second)
    ros::Rate loop_rate(1);

    // Keep running until ROS shuts down (e.g., Ctrl+C)
    while (ros::ok()) {
        ROS_INFO("Hello from your new C++ ROS node!"); // Log to console
        
        ros::spinOnce(); // Handle any incoming callbacks
        loop_rate.sleep(); // Pause to maintain the 1 Hz rate
    }

    return 0;
}