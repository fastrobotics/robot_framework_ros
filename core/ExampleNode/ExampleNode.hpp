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
     * @brief Concrete Init Function.
     * @details It is where the user should implement their node's initialization code.  The user should call the
     * base_init() function of the BaseNode class at the beginning of this function.  Suggested implementations include
     * setting up publishers, subscribers, and services.
     *
     * @return true
     * @return false
     */
    bool init();

    /**
     * @brief Concrete Start Function
     * @details It is where the user should implement their node's start code.  The user should call the base_start()
     * function of the BaseNode class at the beginning of this function.  Suggested implementations include
     * starting threads, initializing hardware, or any other setup required for the node to begin operation.  These
     * should be things that can be reset (this will be called during the Base Node restart() command).
     *
     * @return true
     * @return false
     */
    bool start();

    /**
     * @brief Concrete 10Hz Loop
     * @details It is where the user should implement their node's 10Hz loop code.  Care should be taken to ensure that
     * this function can run at 10Hz.  This function is called automatically by the BaseNode class at 10Hz.
     * @return true
     * @return false
     */
    bool run_10hz();

    /**
     * @brief Example Node Loop1
     *
     * @return true
     * @return false
     */
    bool run_loop1();

    /**
     * @brief Example Node Loop2
     *
     * @return true
     * @return false
     */
    bool run_loop2();

    /**
     * @brief Example Node Loop3
     *
     * @return true
     * @return false
     */
    bool run_loop3();

    /**
     * @brief Concrete Separate Thread
     *
     */
    void thread_loop();
};
}  // namespace fast::rf_ros::example_package
