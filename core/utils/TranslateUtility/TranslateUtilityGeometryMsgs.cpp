
#include <geometry_msgs/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/transform_datatypes.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <robot_framework_ros/utils/TranslateUtility.hpp>
namespace fast::rf_ros::utils {

    fast::rf::messages::GeometryMsgs::TwistMsg TranslateUtility::convert(geometry_msgs::Twist msg) {
        fast::rf::messages::GeometryMsgs::TwistMsg data;
        data.linear = convert(msg.linear);
        data.angular = convert(msg.angular);
        return data;
    }
    fast::rf::messages::GeometryMsgs::OrientationMsg TranslateUtility::convert(const geometry_msgs::Quaternion& msg) {
        fast::rf::messages::GeometryMsgs::OrientationMsg orientation;
        // 1. Convert geometry_msgs to tf Quaternion
        tf2::Quaternion tf_quat;
        tf2::convert(msg, tf_quat);

        // 2. Convert tf Quaternion to Matrix3x3
        tf2::Matrix3x3 matrix(tf_quat);

        // 3. Extract Roll, Pitch, and Yaw (in radians)
        double roll, pitch, yaw;
        matrix.getRPY(roll, pitch, yaw);
        orientation.roll = roll;
        orientation.pitch = pitch;
        orientation.yaw = yaw;
        return orientation;
    }
    geometry_msgs::Quaternion TranslateUtility::convert(fast::rf::messages::GeometryMsgs::OrientationMsg msg) {
        tf2::Quaternion tf_quaternion;
        tf_quaternion.setRPY(msg.roll, msg.pitch, msg.yaw);
        geometry_msgs::Quaternion data = tf2::toMsg(tf_quaternion);
        return data;
    }
    geometry_msgs::Twist TranslateUtility::convert(fast::rf::messages::GeometryMsgs::TwistMsg msg) {
        geometry_msgs::Twist data;
        data.linear = convert(msg.linear);
        data.angular = convert(msg.angular);
        return data;
    }

    fast::rf::messages::GeometryMsgs::OdomMsg TranslateUtility::convert(nav_msgs::Odometry msg) {
        fast::rf::messages::GeometryMsgs::OdomMsg data;
        data.time_stamp = msg.header.stamp.toSec();
        // data.pose = convert(msg.pose);  // Translate this and any other attribute during AB#1813
        data.twist.twist = convert(msg.twist.twist);
        return data;
    }
    nav_msgs::Odometry TranslateUtility::convert(fast::rf::messages::GeometryMsgs::OdomMsg msg) {
        nav_msgs::Odometry data;
        data.header.stamp = ros::Time(msg.time_stamp);
        // data.pose = convert(msg.pose);  // Translate this and any other attribute during AB#1813
        data.twist.twist = convert(msg.twist.twist);
        return data;
    }
}  // namespace fast::rf_ros::utils