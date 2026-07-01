/**
 * @file ExampleNode.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-06-30
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <robot_framework_ros/BaseNode.hpp>

namespace fast::rf_ros::example_package {
/**
 * @brief Example Node
 *
 */
class ExampleNode : public BaseNode {
   public:
    ExampleNode();
    ~ExampleNode();
    /**
     * @brief Concrete Init Function
     *
     * @return true
     * @return false
     */
    bool init();

    /**
     * @brief Concrete Start Function
     *
     * @return true
     * @return false
     */
    bool start();

    /**
     * @brief Concrete 10Hz Loop
     *
     * @return true
     * @return false
     */
    bool run_10hz();

    /**
     * @brief Concrete Separate Thread
     *
     */
    void thread_loop();
};
}  // namespace fast::rf_ros::example_package
