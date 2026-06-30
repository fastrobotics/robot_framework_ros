/**
 * @file BaseNode.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-06-30
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

// STL Dependencies
#include <csignal>
#include <thread>

// Standard Messages
#include <robot_framework_ros/heartbeat.h>

// ROS Dependencies

#include "ros/ros.h"

namespace fast::rf_ros {
/**
 * @brief BaseNode, all other Nodes should inherit from this
 *
 */
class BaseNode {
   public:
    BaseNode() : n(new ros::NodeHandle("~")) {}
    // Initialization Functions

    /**
     * @brief Initialize Base Node.
     *
     * @return true
     * @return false
     */
    bool base_init();

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    virtual bool init() = 0;
    // Node Lifecycle Functions
    /**
     * @brief Start the Node.  Should be called AFTER User runs `init`.
     *
     * @return true
     * @return false
     */
    virtual bool start() = 0;

    // Required User Node Implementation Functions for Loops

    /**
     * @brief // User function should call base_<> function of the same name.  Gets called automatically to run at 10
     * Hz.
     *
     * @return true
     * @return false
     */
    virtual bool run_10hz() = 0;

    // Node Base Function Loops.  User function should call these

    /**
     * @brief Base Node 10Hz Loop.
     *
     * @return true
     * @return false
     */
    bool base_run_10hz();

    /**
     * @brief Update function that main function should call.
     * @details triggers all other timing loops
     *
     * @return true
     * @return false
     */
    bool update();

    // Standard Publishers

   protected:
    boost::shared_ptr<ros::NodeHandle> n;  //!< Node Handle

   private:
    double measure_time_diff(ros::Time time_a, ros::Time time_b);
    ros::Publisher heartbeat_pub;

    double ros_rate;

    ros::Time last_10hz_timer;
};
}  // namespace fast::rf_ros
