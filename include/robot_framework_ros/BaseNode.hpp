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
#include <robot_framework_ros/nodestate.h>

// ROS Dependencies

#include "ros/ros.h"

namespace fast::rf_ros {
/**
 * @brief BaseNode, all other Nodes should inherit from this
 *
 */
class BaseNode {
   public:
    BaseNode() : n(new ros::NodeHandle("~")) {
        node_state.state = robot_framework_ros::nodestate::STATE_UNKNOWN;
        max_rate = ros_rate / 10.0;  // Max rate is 10Hz
    }
    virtual ~BaseNode() = default;
    // Initialization Functions

    // Node Lifecycle Functions
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

    /**
     * @brief Start Base Node.  Should be called AFTER User runs `init`.
     *
     * @return true
     * @return false
     */
    bool base_start();

    /**
     * @brief Start the Node.  Should be called AFTER User runs `init`.
     *
     * @return true
     * @return false
     */
    virtual bool start() = 0;

    /**
     * @brief Restart the Node.
     *
     * @return true
     * @return false
     */
    bool base_restart();

    /**
     * @brief Base Node 100Hz Loop.
     *
     * @return true
     * @return false
     */
    bool base_run_100hz();

    /**
     * @brief Base Node 10Hz Loop.
     *
     * @return true
     * @return false
     */
    bool base_run_10hz();

    /**
     * @brief Base Node 1Hz Loop.
     *
     * @return true
     * @return false
     */
    bool base_run_1hz();

    /**
     * @brief Base Node 0.1Hz Loop.
     *
     * @return true
     * @return false
     */
    bool base_run_01hz();

    /**
     * @brief Base Node 0.01Hz Loop.
     *
     * @return true
     * @return false
     */
    bool base_run_001hz();

    // Required User Node Implementation Functions for Loops
    /**
     * @brief Gets called automatically to run at 100 Hz.
     *
     * @return true
     * @return false
     */
    virtual bool run_100hz() = 0;

    /**
     * @brief Gets called automatically to run at 10 Hz.
     *
     * @return true
     * @return false
     */
    virtual bool run_10hz() = 0;

    /**
     * @brief Gets called automatically to run at 1 Hz.
     *
     * @return true
     * @return false
     */
    virtual bool run_1hz() = 0;

    /**
     * @brief Gets called automatically to run at 0.1 Hz.
     *
     * @return true
     * @return false
     */
    virtual bool run_01hz() = 0;

    /**
     * @brief Gets called automatically to run at 0.01 Hz.
     *
     * @return true
     * @return false
     */
    virtual bool run_001hz() = 0;

    /**
     * @brief User defined Loop, runs at user-defined rate.  This is called automatially at the set rate.
     *
     * @return true
     * @return false
     */

    virtual bool run_loop1() = 0;

    /**
     * @brief User defined Loop, runs at user-defined rate.  This is called automatially at the set rate.
     *
     * @return true
     * @return false
     */
    virtual bool run_loop2() = 0;

    /**
     * @brief User defined Loop, runs at user-defined rate.  This is called automatially at the set rate.
     *
     * @return true
     * @return false
     */
    virtual bool run_loop3() = 0;

    /**
     * @brief Update function that main function should call.
     * @details triggers all other timing loops
     *
     * @return true
     * @return false
     */
    bool update();

    /**
     * @brief Get the node state object
     *
     * @return robot_framework_ros::nodestate
     */
    robot_framework_ros::nodestate get_node_state() { return node_state; }

    /**
     * @brief Get a string representation of the node
     *
     * @return std::string
     */
    std::string pretty();

    /**
     * @brief Convert a node state to a string representation
     *
     * @param state
     * @return std::string
     */
    std::string convert(robot_framework_ros::nodestate state);

    // Standard Publishers

   protected:
    boost::shared_ptr<ros::NodeHandle> n;  //!< Node Handle

   private:
    robot_framework_ros::nodestate node_state;
    std::string node_namespace{""};
    std::string node_name{""};
    double loop1_rate{-1.0};
    bool loop1_enabled{true};

    double loop2_rate{-1.0};
    bool loop2_enabled{true};

    double loop3_rate{-1.0};
    bool loop3_enabled{true};

    bool request_node_statechange(uint8_t new_state, bool override = false);
    ros::Publisher heartbeat_pub;

    double ros_rate{400.0};
    double max_rate;

    ros::Time last_100hz_timer;
    ros::Time last_10hz_timer;
    ros::Time last_1hz_timer;
    ros::Time last_01hz_timer;
    ros::Time last_001hz_timer;

    ros::Time last_loop1_timer;
    ros::Time last_loop2_timer;
    ros::Time last_loop3_timer;
};
}  // namespace fast::rf_ros
