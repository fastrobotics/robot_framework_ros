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

#include <algorithm>
#include <boost/array.hpp>
#include <vector>

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

            static boost::array<double, 9> convert_boostarray_9(std::vector<double> vector);
        };
    }  // namespace utils
}  // namespace fast::rf_ros