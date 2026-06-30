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

namespace fast::rf_ros {
class ExampleNode : public BaseNode {
   public:
    ExampleNode();
    ~ExampleNode();
    bool init();

    bool start();

    bool run_10hz();

    void thread_loop();
};
}  // namespace fast::rf_ros
