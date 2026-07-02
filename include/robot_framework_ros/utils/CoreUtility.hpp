/**
 * @file CoreUtility.hpp
 * @author David Gitz
 * @brief
 * @version 0.1
 * @date 2026-07-01
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include "ros/ros.h"

namespace fast::rf_ros {
namespace utils {
/**
 * @brief CoreUtility class that provides utility functions
 *
 */
class CoreUtility {
   public:
    /**
     * @brief Measure the time difference between two ros::Time objects
     *
     * @param time_a
     * @param time_b
     * @return double
     */
    static double measure_time_diff(ros::Time time_a, ros::Time time_b);
};
}  // namespace utils
}  // namespace fast::rf_ros