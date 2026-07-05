/**
 * @file TranslateUtility.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <robot_framework_ros/diagnostic.h>

#include <DiagnosticMsg.hpp>
namespace fast::rf_ros::utils {
    /**
     * @brief Translation Utilities
     *
     */
    class TranslateUtility {
       public:
        /**
         * @brief Translate from a ROS message to a Core message of type Diagnostic
         *
         * @param msg
         * @return fast::rf::messages::InfrastructureMsgs::DiagnosticMsg
         */
        static fast::rf::messages::InfrastructureMsgs::DiagnosticMsg convert(robot_framework_ros::diagnostic msg);

        /**
         * @brief Translate from a Core Message to a ROS message of type Diagnostic
         *
         * @param msg
         * @return robot_framework_ros::diagnostic
         */
        static robot_framework_ros::diagnostic convert(fast::rf::messages::InfrastructureMsgs::DiagnosticMsg msg);
    };
}  // namespace fast::rf_ros::utils