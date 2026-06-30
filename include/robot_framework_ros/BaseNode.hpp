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
#include <robot_framework_ros/heartbeat.h>

#include "ros/ros.h"

namespace fast::rf_ros {
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
     * @brief // User function should call base_<> function of the same name.
     *
     * @return true
     * @return false
     */
    virtual bool run_01hz() = 0;

    // Node Base Function Loops.  User function should call these
    bool base_run_01hz();

    // Standard Publishers

   protected:
    boost::shared_ptr<ros::NodeHandle> n;

   private:
    ros::Publisher heartbeat_pub;
};
}  // namespace fast::rf_ros
