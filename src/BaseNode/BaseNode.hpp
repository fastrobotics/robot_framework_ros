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
namespace fast::rf_ros {
class BaseNode {
    // Node Lifecycle Functions
    /**
     * @brief Start the Node
     *
     * @return true
     * @return false
     */
    virtual bool start() = 0;
};
}  // namespace fast::rf_ros
